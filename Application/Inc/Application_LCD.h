#ifndef Application_LCD_H
#define Application_LCD_H

#include "Application.h"
#include "lcd.h"
#include "lcd_init.h"

#define LCD_Clear() LCD_Fill_DMA(0, 0, LCD_W, LCD_H, BLACK)

void LCD_show(void);
void APP_LCD_Show(void);

#endif
