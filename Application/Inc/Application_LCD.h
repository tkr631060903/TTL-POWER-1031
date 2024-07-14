#ifndef Application_LCD_H
#define Application_LCD_H

#include "Application.h"
#include "lcd.h"
#include "lcd_init.h"

#define LCD_Clear() LCD_Fill_DMA(0, 0, LCD_W, LCD_H, BLACK)

void LCD_show(void);
void APP_LCD_Show(void);
void APP_LCD_main_init(void);
void APP_LCD_main_show(void);
void APP_LCD_Show_SETVOUT(void);
void APP_LCD_Show_SETIOUT(void);
void app_lcd_vout_on(void);
void app_lcd_vout_off(void);

#endif
