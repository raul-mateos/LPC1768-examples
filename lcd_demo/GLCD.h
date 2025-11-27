/****************************************Copyright (c)**************************************************                         
**
**                                 http://www.powermcu.com
**
**--------------File Info-------------------------------------------------------------------------------
** File name:			GLCD.h
** Descriptions:		TFT (IO)
**
**------------------------------------------------------------------------------------------------------
** Created by:			AVRman
** Created date:		2011-1-26
** Version:				1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/

#ifndef __GLCD_H 
#define __GLCD_H

/* Includes ------------------------------------------------------------------*/
#include "lpc_types.h"
#include <lpc17xx.h>


/* Private define ------------------------------------------------------------*/

/* LCD Interface */
#define PIN_CS    (1 << 8)
#define PIN_RS		(1 << 27)
#define PIN_WR		(1 << 28)
#define PIN_RD		(1 << 29)   

//#define LCD_CS(x)   ((x) ? GPIO_SetValue(2, PIN_CS ) : GPIO_ClearValue(2, PIN_CS ) );
#define LCD_CS(x)   ((x) ? (LPC_GPIO2->FIOSET = PIN_CS) : (LPC_GPIO2->FIOCLR = PIN_CS));

//#define LCD_RS(x)   ((x) ? GPIO_SetValue(1, PIN_RS ) : GPIO_ClearValue(1, PIN_RS ) );
#define LCD_RS(x)   ((x) ? (LPC_GPIO1->FIOSET = PIN_RS) : (LPC_GPIO1->FIOCLR = PIN_RS));

//#define LCD_WR(x)   ((x) ? GPIO_SetValue(1, PIN_WR ) : GPIO_ClearValue(1, PIN_WR ) );
#define LCD_WR(x)   ((x) ? (LPC_GPIO1->FIOSET = PIN_WR) : (LPC_GPIO1->FIOCLR = PIN_WR));

//#define LCD_RD(x)   ((x) ? GPIO_SetValue(1, PIN_RD ) : GPIO_ClearValue(1, PIN_RD ) );
#define LCD_RD(x)   ((x) ? (LPC_GPIO1->FIOSET = PIN_RD) : (LPC_GPIO1->FIOCLR = PIN_RD));


/* Private define ------------------------------------------------------------*/
#define DISP_ORIENTATION  90  /* angle 0 90 */ 

#if  ( DISP_ORIENTATION == 90 ) || ( DISP_ORIENTATION == 270 )

#define  MAX_X  320
#define  MAX_Y  240   

#elif  ( DISP_ORIENTATION == 0 ) || ( DISP_ORIENTATION == 180 )

#define  MAX_X  240
#define  MAX_Y  320   

#endif

/* LCD color */
#define White          0xFFFF
#define Black          0x0000
#define Grey           0xF7DE
#define Blue           0x001F
#define Blue2          0x051F
#define Red            0xF800
#define Magenta        0xF81F
#define Green          0x07E0
#define Cyan           0x7FFF
#define Yellow         0xFFE0

/******************************************************************************
* Function Name  : RGB565CONVERT
* Description    : 24bit to 16bit color
* Input          : - red: R
*                  - green: G 
*		   - blue: B
* Output         : None
* Return         : RGB color
* Attention	 : None
*******************************************************************************/
#define RGB565CONVERT(red, green, blue)\
(uint16_t)( (( red   >> 3 ) << 11 ) | \
(( green >> 2 ) << 5  ) | \
( blue  >> 3 ))

/* Private function prototypes -----------------------------------------------*/

void LCD_Initialization(void);
void LCD_Clear(uint16_t Color);
uint16_t LCD_GetPoint(uint16_t Xpos,uint16_t Ypos);
void LCD_SetPoint(uint16_t Xpos,uint16_t Ypos,uint16_t point);
void LCD_DrawLine( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1 , uint16_t color );
void PutChar( uint16_t Xpos, uint16_t Ypos, uint8_t ASCI, uint16_t charColor, uint16_t bkColor );
void GUI_Text(uint16_t Xpos, uint16_t Ypos, uint8_t *str,uint16_t Color, uint16_t bkColor);

#endif 

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
