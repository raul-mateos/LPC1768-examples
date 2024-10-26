#include <LPC17xx.h>
#include <stdint.h>

#include "reg_masks.h"

// Board features:
#define F_CPU       (SystemCoreClock)
#define F_PCLK      (F_CPU/4)

void loopback_cfg(unsigned int tone_freq, unsigned int fs_adc);
void loopback_enable(int enable);
void key_cfg();

int main() {

  loopback_cfg(1000, 16000);
//  loopback_enable(1);
  key_cfg();

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

static void loopback_init_samples() {
  int i;
  float x;
  for(i = 0; i < N_POINTS; i++) {
    x = DAC_MID_RANGE+(DAC_MID_RANGE-1)*sinf((2*M_PI/N_POINTS)*i);
    sample_table[i] = ((uint16_t)x) << 6;
  }
  sample_idx = 0;
}

static void loopback_gen_sample() {
  LPC_DAC->DACR = (uint32_t) sample_table[sample_idx];
  sample_idx = (sample_idx == N_POINTS-1)? 0 : sample_idx+1;
}

#define DAC_FUNCTION  0x2

static void loopback_dac_cfg() {
  LPC_SC->PCLKSEL0  &= ~PCLK_DAC_MASK;
  LPC_SC->PCLKSEL0  |= PCLK_DAC_DIV_4;  // PCLK_DAC = CCLK/4 (25 MHz)

  LPC_PINCON->PINSEL1  |= (DAC_FUNCTION << 20); // DAC output = P0.26 (AOUT)
  LPC_PINCON->PINMODE1 |= (PIN_NO_PULL  << 20); // disable pull up & down
  LPC_DAC->DACCTRL = 0;
}

// Timer 0 is used for the ADC

static void loopback_dac_timer_cfg(int freq) {
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
  loopback_gen_sample();
}

void loopback_enable(int enable) {
  if (enable)
    LPC_TIM0->TCR |= CNT_EN;    // Count enabled
  else
    LPC_TIM0->TCR &= ~CNT_EN;   // Count disabled
}


// Timer 1 is used for the ADC

static void loopback_adc_timer_cfg(unsigned int f_s) {
  uint32_t mr;
  LPC_SC->PCONP |= PCTIM1;          // Timer 1 power On
  LPC_TIM1->PR = 0;                 // Set prescaler to 1
  LPC_TIM1->MCR = MR0R;             // clear timer on match
  LPC_TIM1->EMR |= EM_TOGGLE << 4;  // MAT1.0 toggle on match

  mr = (uint32_t)((F_PCLK/(2*f_s))-1);
  LPC_TIM1->MR0 = mr;
  LPC_TIM1->TCR = CNT_RST;          // Reset & disabled
  LPC_TIM1->TCR = CNT_EN;           // Timer enabled
}

#define FREQ_ADC_MAX  13e6
#define DIV_ROUND_UP(x, y) (((x)+(y)-1)/(y))

// Using P0.25/AD0.2/I2SRX_SDA/TXD3
// Using P0.25 pin as AD0.2 => adc channel = 2
#define ADC_CHANNEL   2
#define ADC_FUNCTION  0x1

static void loopback_adc_cfg() {
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
                  ADC_MAT1_0_START;    

  LPC_ADC->ADINTEN = ADGINTEN;
  NVIC_SetPriority(ADC_IRQn, 5);
  NVIC_EnableIRQ(ADC_IRQn);
}

static void adc_cfg_sw_trigger() {
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

  LPC_ADC->ADINTEN = ADGINTEN;
  NVIC_SetPriority(ADC_IRQn, 5);
  NVIC_EnableIRQ(ADC_IRQn);  
}




// ADGINTEN = 1
// La interrupción se dispara cuando el bit DONE de AD0GDR se activa.
// El bit AD0GDR cuando concluye una conversión EN CUALQUIER CANAL
//    => Esto NO se puede usar en modo burst! 
// ADGINTEN = 0
// La interrupción se dispara cuando para alguno de los canales cuyas
// interrupciones se han habilitado individualmente en AD0INTEN se 
// ha completado la conversión. Este hecho viene indicado por el bit
// DONE del registro de datos de dicho canal AD0DRx
//    => Esto SE DEBE usar en modo burst! 

typedef struct {
  unsigned              :  4;  // reserved
  unsigned int result   : 12; // 
  unsigned              :  8;  // reserved
  unsigned int channel  :  3; // 
  unsigned              :  3;  // reserved
  unsigned int overrun  :  1; // 
  unsigned int done     :  1; // 
} ADGDR;

int size = sizeof(ADGDR);

int16_t sample;
int channel;
int ovr_flag, done_flag;

void ADC_IRQHandler() {
  uint32_t reg;
  uint32_t status;
  
  reg = sizeof(ADGDR);
  reg = LPC_ADC->ADGDR;
  sample = ADC_GET_RESULT(reg);
  channel = ADC_GET_CHANNEL(reg);
  ovr_flag = (reg & ADC_OVR)? 1 : 0;
  done_flag = (reg & ADC_DONE)? 1 : 0;
}

void loopback_cfg(unsigned int tone_freq, unsigned int fs_adc) {
//  loopback_init_samples();
//  loopback_dac_cfg();
//  loopback_dac_timer_cfg(tone_freq);

  loopback_adc_cfg();
  loopback_adc_timer_cfg(fs_adc);  
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
  uint32_t reg;
  LPC_SC->EXTINT = (0x1 << 1);    // Clear flag
  
  reg = LPC_ADC->ADCR;
  reg &= ~ADC_START_MASK;
  reg |= ADC_SW_START;
  LPC_ADC->ADCR = reg;
}

