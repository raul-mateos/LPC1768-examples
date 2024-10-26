#include <LPC17xx.h>
#include <stdio.h>
#include <stdint.h>

#include "reg_masks.h"

// Board features:
#define F_CPU       (SystemCoreClock)   // From system_LPC17xx.h
#define F_PCLK      (F_CPU/4)
#define T_SYSTICK   (1.0e-3)

void init_hw(void);

int main() {
  
  init_hw();
  while(1);
  return 0;
}


typedef struct {
  float length;
  int   value;
} segment_t;

segment_t signal[] = {
  {40e-6, 1},
  {20e-6, 0},
  {80e-6, 1},
  {20e-6, 0},
  {120e-6, 1},
  {400e-6, 0}
};

#define N_SEGS  (sizeof(signal)/sizeof(segment_t))

static int seg_index = 0;

#define PIN_MASK 0x2    // P0.1


void init_hw(void) {
  uint32_t MRx;
  
  // Pin Function Select Register: P0.1 pin as GPIO
  LPC_PINCON->PINSEL0  &= ~(0x3U << 2);  
  LPC_GPIO0->FIODIR |= PIN_MASK;  // set P0.1 as output
  LPC_GPIO0->FIOCLR = PIN_MASK;   // set P0.1 to 0
  
  
  LPC_SC->PCONP     |= PCTIM0;        // Power up Timer 0
  LPC_SC->PCLKSEL0  &= ~(0x3U << 2);  // TIMER0 clk = PCLK (CCLK/4)

  LPC_TIM0->MCR |= MR0R | MR0I;       // MR0: clear cnt & interrupt
  
  // Setup for first segment...
  MRx = (uint32_t) (signal[seg_index].length*F_PCLK - 1);
  LPC_TIM0->MR0 = MRx;
  if (signal[seg_index].value) {
    LPC_GPIO0->FIOSET = PIN_MASK;   // set P0.1
  }
  else {
    LPC_GPIO0->FIOCLR = PIN_MASK;   // clr P0.1
  }
  seg_index = (seg_index == (N_SEGS-1))? 0 : seg_index+1;
  
  LPC_TIM0->TCR = CNT_RST;            // Reset the timer 0
  LPC_TIM0->TCR = CNT_EN;             // Enable the timer 0
  
  NVIC_SetPriority(TIMER0_IRQn, 1);
  NVIC_EnableIRQ(TIMER0_IRQn);
}




void TIMER0_IRQHandler(void) {
  uint32_t ir;
  uint32_t MRx;

  ir = LPC_TIM0->IR;
  if (ir & MR0_INT) {
    LPC_TIM0->IR = MR0_INT;
    MRx = (uint32_t) (signal[seg_index].length*F_PCLK - 1);
    LPC_TIM0->MR0 = MRx;
    if (signal[seg_index].value) {
      LPC_GPIO0->FIOSET = PIN_MASK;   // set P0.1
    }
    else {
      LPC_GPIO0->FIOCLR = PIN_MASK;   // clr P0.1
    }
    seg_index = (seg_index == (N_SEGS-1))? 0 : seg_index+1;
  }
}

