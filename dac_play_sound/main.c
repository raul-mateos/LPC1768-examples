#include <LPC17xx.h>
#include <stdint.h>

#include "reg_masks.h"

// Board features:
#define F_CPU       (SystemCoreClock)
#define F_PCLK      (F_CPU/4)

void play_cfg(int f_s);
void play_start();
void key_cfg();

int main() {

  play_cfg(8000);
  key_cfg();

  while(1);

  return 0;
}

#include "msg_data.h"

static int sample_idx;

#define DAC_FUNCTION  0x2

static void play_dac_cfg() {
  LPC_SC->PCLKSEL0  &= ~PCLK_DAC_MASK;
  LPC_SC->PCLKSEL0  |= PCLK_DAC_DIV_4;  // PCLK_DAC = CCLK/4 (25 MHz)

  LPC_PINCON->PINSEL1  |= (DAC_FUNCTION << 20); // DAC output = P0.26 (AOUT)
  LPC_PINCON->PINMODE1 |= (PIN_NO_PULL  << 20); // disable pull up & down
  LPC_DAC->DACCTRL = 0;
}

static void play_timer_cfg(int f_s) {
  uint32_t mr;
  LPC_SC->PCONP |= PCTIM1;      // Timer 1 power On
  LPC_TIM1->PR = 0;             // Set prescaler to 1
  LPC_TIM1->MCR = MR0I | MR0R;  // Interrupt & reset
  LPC_TIM1->EMR = 0;            // no op on external match pins

  mr = (uint32_t)((F_PCLK/f_s)-1);
  LPC_TIM1->MR0 = mr;
  LPC_TIM1->TCR = CNT_RST;      // Reset & disabled
  LPC_TIM1->TCR &= ~CNT_RST;

  NVIC_EnableIRQ(TIMER1_IRQn);
  NVIC_SetPriority(TIMER1_IRQn, 4);
}

#define N_SAMPLES (sizeof(msg_data)/sizeof(msg_data[0]))

void TIMER1_IRQHandler() {
  LPC_TIM1->IR = MR0_INT;   // Clear int flag
  
  LPC_DAC->DACR = ((uint32_t) msg_data[sample_idx++]) << 8;
  if (sample_idx == N_SAMPLES) {
    LPC_TIM1->TCR = CNT_RST;    // Reset & disabled
    sample_idx = 0;
  }
}

void play_start() {
  sample_idx = 0;
  LPC_TIM1->TCR = CNT_RST;    // Reset & disabled
  LPC_TIM1->TCR = CNT_EN;     // Count enabled
}

void play_cfg(int f_s) {
  play_dac_cfg();
  play_timer_cfg(f_s);
}

// P2.10  => ISP
// P2.11  => KEY1
// P2.12  => KEY2
// P2.13  => TP_IRQ

#define EINT_FUNCTION   0x1

void key_cfg() {
  // Pushbutton configuration
  LPC_PINCON->PINSEL4 |= (EINT_FUNCTION << 22); // P2.11 as EINT1
  LPC_SC->EXTMODE = (0x1 << 1);         // edge sensitive...
  LPC_SC->EXTPOLAR = 0;                 //...falling-edge sensitive
  NVIC_SetPriority(EINT1_IRQn, 1);
  NVIC_EnableIRQ(EINT1_IRQn);
}

// KEY1: Play
void EINT1_IRQHandler() {
  LPC_SC->EXTINT = (0x1 << 1);    // Clear flag
  play_start();
}
