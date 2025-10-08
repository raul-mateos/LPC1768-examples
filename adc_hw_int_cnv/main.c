#include <stdint.h>

void adc_cfg_int_cnv(void);
void adc_timer_cfg(float f_s);

int main() {
  adc_timer_cfg(20.0E3);    // f_sample = 20 Ksps
  adc_cfg_int_cnv();

  while(1);

  return 0;
}

//-----------------------------------

#include <LPC17xx.h>

#include "reg_masks.h"

// Board features:
#define F_CPU       (SystemCoreClock)
#define F_PCLK      (F_CPU/4)

#define FREQ_ADC_MAX  13e6
#define DIV_ROUND_UP(x, y) (((x)+(y)-1)/(y))

// Using P0.25/AD0.2/I2SRX_SDA/TXD3
// Using P0.25 pin as AD0.2 => adc channel = 2
#define ADC_CHANNEL   2
#define ADC_FUNCTION  0x1

// Using MAT1.1 as internal conversion request

void adc_cfg_int_cnv(void) {
  uint32_t div;
  
  LPC_SC->PCLKSEL0  &= ~PCLK_ADC_MASK;
  LPC_SC->PCLKSEL0  |= PCLK_ADC_DIV_4;  // PCLK_ADC = CCLK/4 (25 MHz)
  
  LPC_SC->PCONP |= PCADC;                       // ADC 0 power on
  LPC_PINCON->PINSEL1  |= (ADC_FUNCTION << 18); // P0.25 pin as AD0.2 input
  LPC_PINCON->PINMODE1 |= (PIN_NO_PULL << 18);  // P0.25: disable pull-up & down 
  
  div = DIV_ROUND_UP((uint32_t)F_PCLK, (uint32_t)FREQ_ADC_MAX);

  LPC_ADC->ADCR = (1U << ADC_CHANNEL) |  // channel select
                  ADC_CLK_DIV(div-1)  |
                  ADC_NOT_PDN         |
                  ADC_MAT1_1_START;    

  LPC_ADC->ADINTEN = ADGINTEN;      // Global interrupt enable
  NVIC_SetPriority(ADC_IRQn, 5);
  NVIC_EnableIRQ(ADC_IRQn);
}


#define ACQ_BUF_LEN 1024
uint16_t acq_buffer[ACQ_BUF_LEN];
int acq_idx = 0;

volatile int16_t sample;
volatile int channel;
volatile int ovr_flag, done_flag;

void ADC_IRQHandler() {
  uint32_t reg;
  // Reading the ADGDR clears the interrupt flag:
  reg = LPC_ADC->ADGDR;
  sample = ADC_GET_RESULT(reg);
  channel = ADC_GET_CHANNEL(reg);
  ovr_flag = (reg & ADC_OVR)? 1 : 0;
  done_flag = (reg & ADC_DONE)? 1 : 0;
  
  
  acq_buffer[acq_idx] = sample;
  acq_idx = (acq_idx == (ACQ_BUF_LEN-1))? 0 : (acq_idx+1);
}

//-------------------------------------

// 2 => P2.10/EINT0/NMI
// 3 => P1.27/CLKOUT/USB_OVRCR/CAP0.1
// 4 => MAT0.1  => P3.26/STCLK/MAT0.1/PWM1.3    (LED2)
// 5 => MAT0.3
// 6 => MAT1.0  => P1.22/MC0B/USB_PWRD/MAT1.0   (USB_PWRD)
// 7 => MAT1.1  => P1.25/MC1A/MAT1.1            (SD_CD)

#define MON_ADC_CNV

// Timer 1 (MAT1.1) is used for the ADC

#define MAT_FUNCTION  0x3

void adc_timer_cfg(float f_s) {
  uint32_t mr;

#ifdef MON_ADC_CNV
  LPC_PINCON->PINSEL3  |= (MAT_FUNCTION << 18); // P1.25 pin as MAT1.1 output
  LPC_PINCON->PINMODE3 |= (PIN_NO_PULL << 18);  // P1.25: disable pull-up & down
#endif
  LPC_SC->PCLKSEL0 &= ~PCLK_TIMER1_MASK;    // Clear bits 3:2 ...
  LPC_SC->PCLKSEL0 |= PCLK_TIMER1_DIV_4;    // Set PCLK = CCLK/0

  LPC_SC->PCONP |= PCTIM1;          // Timer 1 power On
  LPC_TIM1->PR = 0;                 // Set prescaler to 1
  LPC_TIM1->MCR = MR1R;             // clear timer on MR1 match
  LPC_TIM1->EMR &= ~EM_MASK << 6;   // Clear bits 7:6 ...
  LPC_TIM1->EMR |=  EM_TOGGLE << 6; // MAT1.1 toggle on match

  mr = (uint32_t)((F_PCLK/(2*f_s))-1);
  LPC_TIM1->MR1 = mr;
  LPC_TIM1->TCR = CNT_RST;          // Reset & disabled
  LPC_TIM1->TCR = CNT_EN;           // Timer enabled
}



