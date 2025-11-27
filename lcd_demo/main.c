#include <LPC17xx.h>
#include <stdint.h>

#include "reg_masks.h"

// Board features:
#define F_CPU       (SystemCoreClock)
#define F_PCLK      (F_CPU/4)

#include "GLCD.h" 
#include <stdio.h>
#include <stdint.h>

static char lcd_buffer[256];
#define FONT_W  8
#define FONT_H  16

int main() {
  uint16_t Xpos, Ypos;
  int line;
  int dist_cm, angle;
  
  // Inicializamos y borramos el display
  LCD_Initialization();
  LCD_Clear(Blue);
  
  
  dist_cm = 1234;
  angle = 180;

  line = 0;
  Xpos = 0; Ypos = line*FONT_H;
  sprintf(lcd_buffer, "distance: %4d", dist_cm);
  GUI_Text(Xpos, Ypos, (uint8_t *)lcd_buffer, White, Blue);

  line = 1;
  Xpos = 0; Ypos = line*FONT_H;
  sprintf(lcd_buffer, "angle: %4d", angle);
  GUI_Text(Xpos, Ypos, (uint8_t *)lcd_buffer, White, Blue);	
    
  while(1);

  return 0;
}
