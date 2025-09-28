#include <LPC17xx.h>
#include <stdint.h>

// PWM generation API:
void pwm_config(float Tpwm);
void pwm_set_period(float Tpwm);
float pwm_get_period(void);
void pwm_set_duty_cycle(float duty_cycle);
float pwm_get_duty_cycle(void);
void pwm_stop(void);

int main() {

  pwm_config(1.0/20e3);
  pwm_set_duty_cycle(0.75);
  
  while(1);
  
  return 0;
}

#include <LPC17xx.h>
#include <stdint.h>
#include "reg_masks.h"

#define PCLK_CCLK_DIV_4   0x0
#define PCLK_CCLK_DIV_1   0x1
#define PCLK_CCLK_DIV_2   0x2
#define PCLK_CCLK_DIV_8   0x3

// Board features:
#define F_CPU       (SystemCoreClock)
#define F_PCLK      (F_CPU/4)

// Using PWM1.2 output at pin 3.25

void pwm_set_period(float Tpwm) {
  uint32_t m;
  m = (uint32_t)(F_PCLK*Tpwm-1);
  LPC_PWM1->MR0 = m;              // Match Register 0: set PWM period
  LPC_PWM1->LER |= M0LEN;         // PWM Latch Enable Register: Match 0 latch enabled
}

float pwm_get_period(void) {
  float Tpwm;
  unsigned int m;
  m = LPC_PWM1->MR0;
  Tpwm = ((float)(m+1))/F_PCLK;
  return Tpwm;
}

void pwm_config(float Tpwm) {
  LPC_SC->PCONP |= PCPWM1;                    // PWM1 power On

  LPC_SC->PCLKSEL0 &= ~(0x3 << 12);           // Clear bits 13:12 ...
  LPC_SC->PCLKSEL0 |= PCLK_CCLK_DIV_4 << 12;  // Set PCLK = CCLK/4 
  
  LPC_PINCON->PINSEL7 &= ~(0x3 << 18);    // Clear P3.25 (PWM1.2) function
  LPC_PINCON->PINSEL7 |=  (0x3 << 18);    // Set P3.25 as PWM1.2 output

  LPC_PWM1->TCR = CNT_RST;            // Timer counter reset, disabled and PWM disabled

  LPC_PWM1->MR2 = 0;                  // Match Register 2: set duty_cycle = 0
  LPC_PWM1->LER |= M2LEN;             // PWM Latch Enable Register: PWM2 latch enabled

  pwm_set_period(Tpwm);

  LPC_PWM1->MCR |= MR0R;              // Reset timer on Match0
  LPC_PWM1->PCR &= ~PWMSEL2;          // Set single mode for PWM output (1.2)
  LPC_PWM1->PCR |= PWMENA2;           // PWM output enabled (1.2)
  LPC_PWM1->TCR = CNT_EN | PWM_EN;    // Enable timer, start timer and PWM enabled
}

void pwm_set_duty_cycle(float duty_cycle) {
  uint32_t m;   // MR0
  uint32_t n;   // MRx
  
  // Saturate duty cycle value
  duty_cycle = (duty_cycle > 1.0)? 1.0 : duty_cycle;
  duty_cycle = (duty_cycle < 0.0)? 0.0 : duty_cycle;
  
  m = LPC_PWM1->MR0;
  n = (uint32_t) (duty_cycle*(m+1)-1);
  LPC_PWM1->MR2 = n;
  LPC_PWM1->LER |= M2LEN;
}

float pwm_get_duty_cycle(void) {
  unsigned int m, n;
  float duty_cycle;

  m = LPC_PWM1->MR0;
  n = LPC_PWM1->MR2;
  duty_cycle = ((float)(n+1))/((float)(m+1));
  return duty_cycle;
}

void pwm_stop(void) {
  LPC_PWM1->MR2 = 0;        // set duty_cycle = 0
  LPC_PWM1->LER |= M2LEN;   // PWM2 latch enabled
}
