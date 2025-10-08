#include <stdint.h>

void key_cfg(void);
void adc_cfg_sw_trigger(void);
void adc_sw_cnv_rqt(void);

int main() {
  key_cfg();
  adc_cfg_sw_trigger();
  
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

void adc_cfg_sw_trigger(void) {
  uint32_t div;
  
  LPC_SC->PCLKSEL0  &= ~PCLK_ADC_MASK;
  LPC_SC->PCLKSEL0  |= PCLK_ADC_DIV_4;  // PCLK_ADC = CCLK/4 (25 MHz)
  
  LPC_SC->PCONP |= PCADC;                       // ADC 0 power on
  LPC_PINCON->PINSEL1  |= (ADC_FUNCTION << 18); // P0.25 pin as AD0.2 input
  LPC_PINCON->PINMODE1 |= (PIN_NO_PULL << 18);  // P0.25: disable pull-up & down 
  
  div = DIV_ROUND_UP((uint32_t)F_PCLK, (uint32_t)FREQ_ADC_MAX);

  LPC_ADC->ADCR = (1U << ADC_CHANNEL) |  // channel select
                  ADC_CLK_DIV(div-1)  |
                  ADC_NO_START        |
                  ADC_NOT_PDN;

  LPC_ADC->ADINTEN = ADGINTEN;      // Global interrupt enable
  NVIC_SetPriority(ADC_IRQn, 5);
  NVIC_EnableIRQ(ADC_IRQn);  
}

void adc_sw_cnv_rqt(void) {
  uint32_t reg = LPC_ADC->ADCR;
  reg &= ~ADC_START_MASK;
  reg |= ADC_SW_START;
  LPC_ADC->ADCR = reg;
}

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
}

//-------------------------------------

// External interrupt pins in Mini-dk2
// P2.10  => ISP
// P2.11  => KEY1
// P2.12  => KEY2
// P2.13  => TP_IRQ

#define EINT_FUNCTION   0x1

void key_cfg(void) {
  // Pushbutton configuration
  LPC_PINCON->PINSEL4 |= (EINT_FUNCTION << 22); // P2.11 as EINT1
  LPC_SC->EXTMODE = (0x1 << 1);         // edge sensitive...
  LPC_SC->EXTPOLAR = 0;                 //...falling-edge sensitive
  NVIC_SetPriority(EINT1_IRQn, 1);
  NVIC_EnableIRQ(EINT1_IRQn);
}

// KEY1: SW conversion request
void EINT1_IRQHandler() {
  LPC_SC->EXTINT = (0x1 << 1);    // Clear flag
  
  adc_sw_cnv_rqt();
}

