#include <LPC17xx.h>
#include <stdio.h>
#include <stdint.h>

#include "reg_masks.h"

// Board features:
#define F_CPU       (SystemCoreClock)
#define F_PCLK      (F_CPU/4)
#define T_SYSTICK   (1.0e-3)

void init_hw(float Tout);

int main() {
  
  init_hw(160e-9);
  while(1);
  return 0;
}

void init_hw(float Tout) {
  uint32_t MRx;
  
  // Pin Function Select Register: P3.25 pin as MAT0.0
  LPC_PINCON->PINSEL7  &= ~(0x3U << 18);
  LPC_PINCON->PINSEL7  |= 0x2 << 18;
  
  LPC_SC->PCONP     |= PCTIM0;        // Power up Timer 0
  LPC_SC->PCLKSEL0  &= ~(0x3U << 2);  // TIMER0 clk = PCLK (CCLK/4)

  LPC_TIM0->MCR |= MR0R;              // MR0: clear cnt
  
  // Setup for first segment...
  MRx = ((uint32_t) (F_PCLK*Tout/2)) - 1;
  LPC_TIM0->MR0 = MRx;

  LPC_TIM0->EMR |=  0x01;             // set MAT0.0
  LPC_TIM0->EMR &= ~0x01U;            // clr MAT0.0
  LPC_TIM0->EMR |= (0x3 << 4) ;       // MAT0.0 toggle on match
    
  LPC_TIM0->TCR = CNT_RST;            // Reset the timer 0
  LPC_TIM0->TCR = CNT_EN;             // Enable the timer 0
}
