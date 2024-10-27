#include <LPC17xx.h>

// Board features:
#define F_CPU     (SystemCoreClock)   // From system_LPC17xx.h
#define F_PCLK    (F_CPU/4)
#define T_SYSTICK (1.0e-3)

#include <string.h>
#include <stdio.h>

int uart0_cfg(int baudrate);
int uart0_send_string(const char *s);
int uart0_get_tx_busy();
int uart0_recv_string(char *txt);
int uart0_get_rx_busy();

char msg1[] = "Enter a text string and press ENTER:\n\r";
char rx_buffer[128];

int main() {
  int ret;

  // Serial port initialization:
  ret = uart0_cfg(9600);
  if(ret < 0) {
    return -1;
  }

  ret = uart0_send_string(msg1);
  if (!ret) {
    // Wait until the string is transfered!
    while(uart0_get_tx_busy());
  }
      
  ret = uart0_recv_string(rx_buffer);
  if (!ret) {
    while(1) {
      // Check if a new string has been received
      if (!uart0_get_rx_busy()) {
        break;
      }
    }
  }
  
  while(1);
  return 0;
}

//--------------------------------

// Accepted Error baud rate value (in percent unit)
#define UART_ACCEPTED_BAUDRATE_ERROR    3

#include "reg_masks.h"


static int uart0_set_baudrate(unsigned int baudrate) {
  int errorStatus = -1; // failure

  unsigned int uClk = SystemCoreClock/4;
  unsigned int calcBaudrate = 0;
  unsigned int temp = 0;

  unsigned int mulFracDiv, dividerAddFracDiv;
  unsigned int divider = 0;
  unsigned int mulFracDivOptimal = 1;
  unsigned int dividerAddOptimal = 0;
  unsigned int dividerOptimal = 0;

  unsigned int relativeError = 0;
  unsigned int relativeOptimalError = 100000;

  uClk = uClk >> 4; // div by 16

  //  The formula is :
  //  BaudRate= uClk * (mulFracDiv/(mulFracDiv+dividerAddFracDiv) / (16 * DLL)
  //
  //  The value of mulFracDiv and dividerAddFracDiv should comply to the following expressions:
  //  0 < mulFracDiv <= 15, 0 <= dividerAddFracDiv <= 15
  for (mulFracDiv = 1; mulFracDiv <= 15; mulFracDiv++) {
    for (dividerAddFracDiv = 0; dividerAddFracDiv <= 15; dividerAddFracDiv++) {
      temp = (mulFracDiv * uClk) / (mulFracDiv + dividerAddFracDiv);
      divider = temp / baudrate;
      if ((temp % baudrate) > (baudrate / 2))
        divider++;

      if (divider > 2 && divider < 65536) {
        calcBaudrate = temp / divider;
        if (calcBaudrate <= baudrate) {
          relativeError = baudrate - calcBaudrate;
        } else {
          relativeError = calcBaudrate - baudrate;
        }

        if (relativeError < relativeOptimalError) {
          mulFracDivOptimal = mulFracDiv;
          dividerAddOptimal = dividerAddFracDiv;
          dividerOptimal = divider;
          relativeOptimalError = relativeError;
          if (relativeError == 0)
            break;
        }
      }
    }

    if (relativeError == 0)
      break;
  }

  if (relativeOptimalError < ((baudrate * UART_ACCEPTED_BAUDRATE_ERROR) / 100)) {
    LPC_UART0->LCR |= DLAB_ENABLE; 	// importante poner a 1
    LPC_UART0->DLM = (unsigned char) ((dividerOptimal >> 8) & 0xFF);
    LPC_UART0->DLL = (unsigned char) dividerOptimal;
    LPC_UART0->LCR &= ~DLAB_ENABLE;	// importante poner a 0

    LPC_UART0->FDR = ((mulFracDivOptimal << 4) & 0xF0) | (dividerAddOptimal & 0x0F);
    errorStatus = 0; // success
  }

  return errorStatus;
}

//--------------------------------

static volatile int uart_err_flag;
static volatile int rx_err_flag;

static volatile int tx_busy = 0;
static volatile int rx_busy = 0;

static const char *tx_ptr;
static char *rx_ptr;


#define FUNCTION_TXD  0x1
#define FUNCTION_RXD  0x1

int uart0_cfg(int baudrate) {
  int ret;
  
  LPC_PINCON->PINSEL0 &= ~0xF0U;
  LPC_PINCON->PINSEL0 |=  (FUNCTION_TXD << 4)   | // Setup P0.2 as TXD0
                          (FUNCTION_RXD << 6);    // Setup P0.3 as RXD0 

  LPC_UART0->LCR = CHAR_8_BITS | STOP_1_BIT;  // Set 8N1 mode (8 bits, no parity, 1 stop bit)
  ret = uart0_set_baudrate(baudrate);         // Set the baud rate

  LPC_UART0->IER = UART_THRE_IE | UART_RBR_IE;  // Enable UART TX and RX interrupt (for LPC17xx UART)
  NVIC_EnableIRQ(UART0_IRQn);                   // Enable the UART interrupt (for Cortex-CM3 NVIC)
  return ret;
}

int uart0_send_string(const char *txt) {
  int ret = -1;
  if(!tx_busy) {
    tx_ptr = txt;
    LPC_UART0->THR = *tx_ptr++;    
    tx_busy = 1;
    ret = 0;
  }
  return ret;
}

int uart0_get_tx_busy() {
  return tx_busy;
}

int uart0_recv_string(char *txt) {
  int ret = -1;
  if(!rx_busy) {
    rx_ptr = txt;
    rx_busy = 1;
    ret = 0;
  }
  return ret;
}

int uart0_get_rx_busy() {
  return rx_busy;
}

// Note: Putty sends \r when 'enter' key is pressed.

//#define END_OF_LINE '\n'
#define END_OF_LINE '\r'


void UART0_IRQHandler() {
  int int_id;
  
  int_id = GET_INT_ID(LPC_UART0->IIR);
  switch(int_id) {
    case RDA_INT_ID:  // Rx data ready
      if (rx_busy) {
        *rx_ptr = LPC_UART0->RBR;
        if (*rx_ptr++ == END_OF_LINE) {
          *rx_ptr = 0;
          rx_busy = 0;
        }
      }
      else {
        rx_err_flag = 1;
      }
      break;
    case THRE_INT_ID: // Tx data ready
      if (*tx_ptr) {
        LPC_UART0->THR = *tx_ptr++;
      }
      else {
        tx_busy = 0;
      }
      break;
    default:
      uart_err_flag = 1;      
  }
}

