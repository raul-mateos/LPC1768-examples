#include <LPC17xx.h>
#include <stdint.h>

#include "reg_masks.h"

// QEI API:
void QEI_cfg(float t_glitch);
int QEI_get_angle(float *angle);

// Board features:
#define F_CPU       (SystemCoreClock)
#define F_PCLK      (F_CPU/4)

#define QEI_FUNCTION  0x1

void QEI_cfg(float t_glitch) {
  uint32_t filter;

  LPC_SC->PCLKSEL1  &= ~PCLK_QEI_MASK;
  LPC_SC->PCLKSEL1  |= PCLK_QEI_DIV_4;  // PCLK_QEI = CCLK/4 (25 MHz)
  
  LPC_SC->PCONP     |= PCQEI;       // Quadrature Encoder Interface power On

  LPC_PINCON->PINSEL3 |= (QEI_FUNCTION <<  8);  // P1.20 MCI0
  LPC_PINCON->PINSEL3 |= (QEI_FUNCTION << 14);  // P1.23 MCI1

  filter = (uint32_t)(F_PCLK*t_glitch);

  LPC_QEI->QEICON = QEICON_RESP;      // Reset position counter
  LPC_QEI->QEICONF = QEICONF_CAPMODE; // Resolution 4X
  LPC_QEI->QEIMAXPOS = 0xFFFFFFFF;    // maximum value of the encoder position
  LPC_QEI->FILTER = filter;

  // Interrupt configuration.
  // Because QEIIE register is read only;
  LPC_QEI->QEIIEC = ~0x0U;                        // first clear the register...
  LPC_QEI->QEIIES = QEI_ENCLK_INT | QEI_ERR_INT;  // ...and then enable the ints
  NVIC_SetPriority(QEI_IRQn, 4);
  NVIC_EnableIRQ(QEI_IRQn);
}

volatile int _qei_pos;
volatile int _qei_dir;
volatile int _qei_err;
volatile int _qei_pos_token = 0;

void QEI_IRQHandler(void) {
  uint32_t active_flags;

  active_flags = LPC_QEI->QEIINTSTAT & LPC_QEI->QEIIE;
  LPC_QEI->QEICLR = active_flags;   // Clear active flags

  if (active_flags & QEI_ENCLK_INT) {
    _qei_dir = LPC_QEI->QEISTAT;
    _qei_pos = LPC_QEI->QEIPOS;
    _qei_pos_token = 1;
  }

  if (active_flags & QEI_ERR_INT) {
    _qei_err = 1;
  }
}

int QEI_get_angle(float *angle) {
  *angle = 0;
  return 0;
}

int main() {
  int encoder_pos;
  int encoder_dir;
  float t_glitch = 1.5e-3f;
  
  QEI_cfg(t_glitch);

  while(1) {
    
    if(!_qei_pos_token) {
      // Process encoder position:
      encoder_pos = _qei_pos;
      encoder_dir = _qei_dir;
      _qei_pos_token = 0;
    }
  }
  
  return 0;
}


