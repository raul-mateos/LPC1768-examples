#include <LPC17xx.h>
#include <stdio.h>
#include <stdint.h>

#include "reg_masks.h"

#include "GLCD.h"

// Board features:
#define F_CPU       (SystemCoreClock)   // From system_LPC17xx.h
#define F_PCLK      (F_CPU/4)
#define F_WDCLK     (F_PCLK/4)


// Set current time and date
void RTC_set_time(void) {
  LPC_RTC->YEAR   = 2024;
  LPC_RTC->MONTH  = 9;
  LPC_RTC->DOM    = 21;
  LPC_RTC->DOW    = 1;
  LPC_RTC->HOUR   = 21;
  LPC_RTC->MIN    = 9;
  LPC_RTC->SEC    = 0;
}

volatile uint8_t secs_token;

void RTC_init(void) {
  secs_token = 0;

  LPC_SC->PCONP |= PCRTC;       // Power ON

  LPC_RTC->CCR  &= ~RTC_CLKEN;  // Disable the RTC before the changes
  LPC_RTC->CCR  |= RTC_CCALEN;  // Calibration disabled
  LPC_RTC->CIIR |= RTC_IMSEC;   // Interrupt every second

  LPC_RTC->ILR = RTC_RTCCIF;    // Clear flag
  RTC_set_time();

  LPC_RTC->RTC_AUX = RTC_OSCF;      // Clear osc failure flag
  LPC_RTC->RTC_AUXEN = RTC_OSCFEN;  // Enable osc. failure irq
  LPC_RTC->CCR  |= RTC_CLKEN;       // Enable the RTC
}

void RTC_IRQHandler(void) {
  if (LPC_RTC->ILR & RTC_RTCCIF) {
    LPC_RTC->ILR = RTC_RTCCIF;
    secs_token = 1;
  }

  if (LPC_RTC->RTC_AUX & RTC_OSCF) {
    LPC_RTC->RTC_AUX = RTC_OSCF;
  }
}

void WDT_feed(void) {
  LPC_WDT->WDFEED = 0xAA;
  LPC_WDT->WDFEED = 0x55;
}

void WDT_init(void) {
  LPC_WDT->WDTC     = F_WDCLK*2;              // Timeout = 2 secs.
  LPC_WDT->WDCLKSEL = WDT_WDSEL_PCLK;         // Clock =PCLK
  LPC_WDT->WDMOD    = WDT_WDEN | WDT_WDRESET; // Enable & Reset if Timeout
  WDT_feed();
}



char buffer[25];

int main() {
  // Inicialize and clear the display
  LCD_Initialization();
  LCD_Clear(Blue);


  RTC_init();
  WDT_init();

  NVIC_SetPriorityGrouping(2);
  NVIC_EnableIRQ(RTC_IRQn);

  while(1) {
    while(!secs_token);
    secs_token = 0;

    sprintf(buffer, "%02d/%02d/%04d   %02d:%02d:%02d",
        LPC_RTC->DOM,
        LPC_RTC->MONTH,
        LPC_RTC->YEAR,
        LPC_RTC->HOUR,
        LPC_RTC->MIN,
        LPC_RTC->SEC);

    GUI_Text(10, 10, (uint8_t *) buffer, White, Blue);
    WDT_feed();
  }

  return 0;
}
