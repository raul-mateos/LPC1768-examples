#include <LPC17xx.h>
#include <stdio.h>
#include <stdint.h>

#include "reg_masks.h"

#define F_CPU       (SystemCoreClock)   // From system_LPC17xx.h
#define F_PCLK      (F_CPU/4)

// PWM generation API:
void pwm_cfg(float Tpwm);
void pwm_set_duty_cycle(float duty_cycle);

// QEI API:
void QEI_cfg(float t_glitch, float T_obs);
int QEI_get_speed(float *speed_rpm);

float dbg_motor_speed;

int main() {
  float motor_speed = 0.0;

  pwm_cfg(1.0/20e3);
  pwm_set_duty_cycle(-1.0);

  QEI_cfg(10e-6, 10e-3);

  while(1) {
    if(QEI_get_speed(&motor_speed)) {
      // Process motor speed:
      dbg_motor_speed = motor_speed;
    }
    
  }

  return 0;
}

//------------------------------------

//  P0.0 => direction for the wheel

#define DIR_PIN_MASK   0x01

#define DIR_POS   0
#define DIR_NEG   1

void pwm_set_period(float Tpwm) {
  uint32_t m;
  m = (uint32_t)(F_PCLK*Tpwm-1);
  LPC_PWM1->MR0 = m;              // Match Register 0: set PWM period
  LPC_PWM1->LER |= M0LEN;         // PWM Latch Enable Register: Match 0 latch enabled
}

void pwm_cfg(float Tpwm) {
  LPC_GPIO0->FIODIR |= (DIR_PIN_MASK);  // wheel dir pins as output
  LPC_GPIO0->FIOCLR  = (DIR_PIN_MASK);

  LPC_PINCON->PINSEL7 &= ~(0x3U << 18); // Clear P3.25 (PWM1.2) function
  LPC_PINCON->PINSEL7 |=  (0x3U << 18); // Set P3.25 as PWM1.2 output

  LPC_SC->PCONP |= PCPWM1;            // PWM1 power On

  LPC_PWM1->MR2 = 0;                  // Match Register 2: set duty_cycle = 0
  LPC_PWM1->LER |= M2LEN;             // PWM Latch Enable Register: PWM2 latch enabled

  pwm_set_period(Tpwm);

  LPC_PWM1->MCR |= MR0R;              // Match Control Register 0: reset timer on Match0
  LPC_PWM1->PCR |= PWMENA2;           // PWM Control Register: PWM output enabled (1.2)
  LPC_PWM1->TCR = CNT_EN | PWM_EN;    // Timer Control Register: enable timer, start timer
}

static uint32_t get_dc_code(float dc, int *dir) {
  float abs_dc, abs_dc_sat;
  uint32_t m, n_act;

  m = LPC_PWM1->MR0;
  if (dc < 0) {  // Active level = L
    abs_dc = -dc;
    *dir = DIR_NEG;
    abs_dc_sat = (abs_dc > 1.0f)? 1.0f : abs_dc;
    n_act = (uint32_t) ((1-abs_dc_sat)*m);
  }
  else {        // Active level = H
    abs_dc = +dc;
    *dir = DIR_POS;
    abs_dc_sat = (abs_dc > 1.0f)? 1.0f : abs_dc;
    n_act = (uint32_t) (abs_dc_sat*(m+1)-1);
  }
  return n_act;
}

void pwm_set_duty_cycle(float duty_cycle) {
  uint32_t duty_cycle_code;
  int dir;

  duty_cycle_code = get_dc_code(duty_cycle, &dir);
  LPC_PWM1->MR2 = duty_cycle_code;

  if (!dir) // positive dir
    LPC_GPIO0->FIOCLR = DIR_PIN_MASK;
  else
    LPC_GPIO0->FIOSET = DIR_PIN_MASK;

  LPC_PWM1->LER |= M2LEN;
}

//------------------------------------


#define F_PCLK_QEI  (F_CPU/4)
#define QEI_FUNCTION  0x1

// Encoder & gearbox features:
#define ENCODER_PPR   11          // encoder pulses per revolution
#define EDGES         4           // QEI resolution (inc/dec per cycle)
#define R_GEARBOX     34          // gearbox reduction ratio

static float k_speed;

void QEI_cfg(float t_glitch, float T_obs) {
  uint32_t filter_code, n_obs;

  LPC_SC->PCLKSEL1  &= ~PCLK_QEI_MASK;
  LPC_SC->PCLKSEL1  |= PCLK_QEI_DIV_4;  // PCLK_QEI = CCLK/4 (25 MHz)

  LPC_SC->PCONP     |= PCQEI;           // Quadrature Encoder Interface power on

  LPC_PINCON->PINSEL3 |= (QEI_FUNCTION <<  8);  // P1.20 MCI0
  LPC_PINCON->PINSEL3 |= (QEI_FUNCTION << 14);  // P1.23 MCI1

  filter_code = (uint32_t)(F_PCLK_QEI * t_glitch);

  LPC_QEI->QEICON = QEICON_RESP | QEICON_RESV;  // reset position & velocity counters
  LPC_QEI->QEICONF = QEICONF_CAPMODE; // Quadrature mode & resolution (edges) = x4
  LPC_QEI->QEIMAXPOS = 0xFFFFFFFF;    // maximum value of the encoder position
  LPC_QEI->FILTER = filter_code;

  n_obs = (uint32_t) (T_obs*F_PCLK_QEI); // Observation time
  LPC_QEI->QEILOAD = n_obs;

  //k_speed = (60.0 * F_PCLK_QEI)/(EDGES * ENCODER_PPR * n_obs);              // for motor speed
  k_speed = (60.0 * F_PCLK_QEI)/(EDGES * ENCODER_PPR * n_obs * R_GEARBOX);  // for wheel speed

  // Interrupt configuration. Because QEIIE register is read only:
  LPC_QEI->QEIIEC = ~0x0U;                  // first clear the register...
  LPC_QEI->QEIIES = QEI_TIM_INT;            // ...and then enable the ints

  NVIC_EnableIRQ(QEI_IRQn);
  NVIC_SetPriority(QEI_IRQn, 4);
}

static volatile float _speed_rpm;
static volatile int _speed_token = 0;

void QEI_IRQHandler(void) {
  uint32_t flags;
  int dir;

  flags = LPC_QEI->QEIINTSTAT;
  if (flags & QEI_TIM_INT) {        // End of T_obs
    LPC_QEI->QEICLR = QEI_TIM_INT;  // Clear interrupt flag
    dir = (LPC_QEI->QEISTAT & 0x01)? -1 : +1;
    _speed_rpm = (dir * k_speed) * LPC_QEI->QEICAP;
    _speed_token = 1;
  }
}


int QEI_get_speed(float *speed_rpm) {
  int ret = 0;
  if (_speed_token) {
    *speed_rpm = _speed_rpm;
    _speed_token = 0;
    ret = 1;
  }
  return ret;
}
