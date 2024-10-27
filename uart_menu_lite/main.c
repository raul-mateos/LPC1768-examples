#include <LPC17xx.h>

#include <string.h>
#include <stdio.h>
#include "uart_lite.h"

// Forward function declaration:
void leds_cfg();
int uart0_fputs(const char *s);
char *uart0_gets(char *s);


#define LED_1 (1 << 25) // Led 1 is P3_25
#define LED_2 (1 << 26) // Led 2 is P3_26

typedef struct {
  char *key;
  int value;
} dict_entry;

dict_entry menu_dict[] = {
  {"enciende", 1},
  {"apaga", 2},
  {"fin", 3}  
};

#define MENU_DICT_LEN (sizeof(menu_dict)/sizeof(dict_entry))

char rx_msg[128];   // Rx message buffer

int main() {
  int ret;
  int key, opt, i; 
  
  leds_cfg();

  // Serial port initialization:
  ret = uart0_cfg(9600);
  if(ret < 0) {
    return -1;
  }

  uart0_fputs("Esto es una prueba de la UART0 del LPC1768 a 9600 baudios\n\r");
  
  uart0_fputs("Introduce tu nombre y apellidos\n\r");  
  uart0_gets(rx_msg);
      
  do {
    uart0_fputs("Introduce comando:\n\r");
    uart0_gets(rx_msg);
    for(i = 0, opt = 0; i < MENU_DICT_LEN; i++) {
      if(!strcmp(rx_msg, menu_dict[i].key)) {
        opt = menu_dict[i].value;
        break;
      }
    }
    
    switch(opt) {
      case 1:  // enciende
        LPC_GPIO3->FIOCLR = LED_2;
        break;
      case 2:  // apaga
        LPC_GPIO3->FIOSET = LED_2;
        break;
      case 3:  // fin
        break;
      default:
        uart0_fputs("Comando erroneo:\n\r");
    }
  }
  while(opt != 3);
  
  return 0;
}

void leds_cfg() {
  LPC_GPIO3->FIODIR |= (LED_1 | LED_2); // Led pins as output
  LPC_GPIO3->FIOSET  = (LED_1 | LED_2);
}

int uart0_fputs(const char *s) {
  int ret;
  ret = uart0_send_string(s);
  if (!ret) {
    // Wait until the string is transfered!
    while(uart0_get_tx_busy());   
  }
  return ret;
}

char *uart0_gets(char *s) {
  int ret;
  char *p;
  ret = uart0_recv_string(s);
  if (!ret) {
    // Wait until the string is transfered!
    while(uart0_get_rx_busy());
    // Remove the '\n' character:
    for(p = s; *p; p++);
    p[-1] = 0;
  }
  else {
    s = NULL;
  }
  return s;
}
