#include <LPC17xx.h>
#include <stdint.h>

#include "reg_masks.h"

// Board features:
#define F_CPU       (SystemCoreClock)
#define F_PCLK      (F_CPU/4)

void tone_cfg(int freq);
void tone_enable(int enable);

int main() {

  tone_cfg(1000);
  tone_enable(1);

  while(1);

  return 0;
}

#include <math.h>
#define N_POINTS    16
#define TONE_FREQ   5000

#define DAC_N_BITS  10
#define DAC_N_LEVELS  (1U << DAC_N_BITS)
#define DAC_MID_RANGE (1U << (DAC_N_BITS-1))

#define M_PI 3.14159265358979323846f

static uint16_t sample_table[N_POINTS];
static int sample_idx;

static void tone_init_samples() {
  int i;
  float x;
  for(i = 0; i < N_POINTS; i++) {
    x = DAC_MID_RANGE+(DAC_MID_RANGE-1)*sinf((2*M_PI/N_POINTS)*i);
    sample_table[i] = ((uint16_t)x) << 6;
  }
  sample_idx = 0;
}

static void tone_gen_sample() {
  LPC_DAC->DACR = (uint32_t) sample_table[sample_idx];
  sample_idx = (sample_idx == N_POINTS-1)? 0 : sample_idx+1;
}

#define DAC_FUNCTION  0x2

static void tone_dac_cfg() {
  LPC_SC->PCLKSEL0  &= ~PCLK_DAC_MASK;
  LPC_SC->PCLKSEL0  |= PCLK_DAC_DIV_4;  // PCLK_DAC = CCLK/4 (25 MHz)

  LPC_PINCON->PINSEL1  |= (DAC_FUNCTION << 20); // DAC output = P0.26 (AOUT)
  LPC_PINCON->PINMODE1 |= (PIN_NO_PULL  << 20); // disable pull up & down
  LPC_DAC->DACCTRL = 0;
}

static void tone_timer_cfg(int freq) {
  uint32_t mr;
  LPC_SC->PCONP |= PCTIM0;      // Timer 0 power On
  LPC_TIM0->PR = 0;             // Set prescaler to 1
  LPC_TIM0->MCR = MR0I | MR0R;  // Interrupt & reset
  LPC_TIM0->EMR = 0;            // no op on external match pins

  mr = (uint32_t)((F_PCLK/(freq*N_POINTS))-1);
  LPC_TIM0->MR0 = mr;
  LPC_TIM0->TCR = CNT_RST;      // Reset & disabled
  LPC_TIM0->TCR &= ~CNT_RST;

  NVIC_EnableIRQ(TIMER0_IRQn);
  NVIC_SetPriority(TIMER0_IRQn, 4);
}

void TIMER0_IRQHandler(void) {
  LPC_TIM0->IR = MR0_INT;   // Clear int flag
  tone_gen_sample();
}

void tone_enable(int enable) {
  if (enable)
    LPC_TIM0->TCR |= CNT_EN;    // Count enabled
  else
    LPC_TIM0->TCR &= ~CNT_EN;   // Count disabled
}

void tone_cfg(int freq) {
  tone_init_samples();
  tone_dac_cfg();
  tone_timer_cfg(freq);
}
