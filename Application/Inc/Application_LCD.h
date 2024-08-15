#ifndef Application_LCD_H
#define Application_LCD_H

#include "Application.h"
#include "lcd.h"
#include "lcd_init.h"

#define LCD_Clear() LCD_Fill_DMA(0, 0, LCD_W, LCD_H, BLACK)

void APP_LCD_main_init(void);
void APP_LCD_main_show(void);
void APP_LCD_presset_running_init(void);
void APP_LCD_presset_running_show(void);
void presset_config_set_page_show(void);

#endif
