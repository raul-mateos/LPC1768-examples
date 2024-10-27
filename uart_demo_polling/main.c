#include <LPC17xx.h>

// Board features:
#define F_CPU     (SystemCoreClock)   // From system_LPC17xx.h
#define F_PCLK    (F_CPU/4)
#define T_SYSTICK (1.0e-3)

#include <string.h>
#include <stdio.h>

void uart0_cfg_fixed();
void uart0_send_char(char x);
void uart0_send_buffer(const void *buf, int n);
char uart0_recv_char();
void uart0_recv_string(void *buf, int n);

char msg1[] = "Enter a text string and press ENTER:\n\r";
char rx_buffer[128];

int main() {
  int len;

  // Serial port initialization:
  uart0_cfg_fixed();
  
  len = strlen(msg1);
  uart0_send_buffer(msg1, len);
  
  uart0_recv_string(rx_buffer, sizeof(rx_buffer)-1);
  
  while(1);
  return 0;
}

//--------------------------------

#include "reg_masks.h"

//  br = 115200
//  div = 25e6/(16*br)    = 13.5634
//  FR = div/floor(div)   = 1.0433
//  => Closest FR = 1.067 = 1/15 => DivAddVal = 1, MulVal = 15

#define FUNCTION_TXD  0x1
#define FUNCTION_RXD  0x1

void uart0_cfg_fixed() {
  LPC_PINCON->PINSEL0 &= ~0xF0U;
  LPC_PINCON->PINSEL0 |=  (FUNCTION_TXD << 4)   | // setup P0.2 as TXD0
                          (FUNCTION_RXD << 6);    // setup P0.3 as RXD0 
  // Set 8N1 mode (8 bits, no parity, 1 stop bit):
  LPC_UART0->LCR = CHAR_8_BITS | STOP_1_BIT;  
  LPC_UART0->LCR |= DLAB_ENABLE;    // Set the baud rate
  LPC_UART0->DLL = 13;              // baud rate 115200
  LPC_UART0->DLM = 0x00;
  LPC_UART0->FDR = (15 << 4) | (1); // FR: DivAddVal = 1, MulVal = 15
  LPC_UART0->LCR &= ~DLAB_ENABLE;
}

void uart0_send_char(char x) {
  while(!(LPC_UART0->LSR & UART_TX_EMPTY));
  LPC_UART0->THR = x;
}

void uart0_send_buffer(const void *buf, int n) {
  const char *p = (const char *) buf;
  while(n > 0) {
    uart0_send_char(*p++);
    n--;
  }
}

char uart0_recv_char() {
  while(!(LPC_UART0->LSR & UART_RX_RDY));
  return (char) LPC_UART0->RBR;
}

// Note: Putty sends \r when 'enter' key is pressed.
#define END_OF_LINE '\r'

void uart0_recv_string(void *buf, int n) {
  char *p = (char *) buf;
  while(n > 0) {
    *p = uart0_recv_char();
    if (*p++ == END_OF_LINE) {
      break;
    }
    n--;
  }
  *p = 0;
}