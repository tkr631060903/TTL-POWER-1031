#ifndef Application_LCD_H
#define Application_LCD_H

#include "Application.h"
#include "lcd.h"
#include "lcd_init.h"

#define LCD_Clear() LCD_Fill_DMA(0, 0, LCD_W, LCD_H, BLACK)
#define LCD_Fill(color) LCD_Fill_DMA(0, 0, LCD_W, LCD_H, color)

void APP_LCD_main_init(void);
void APP_LCD_main_show(void);
void APP_LCD_presset_running_init(void);
void APP_LCD_presset_running_show(void);
void presset_config_set_page_show(void);
void LCD_show_vset(void);
void LCD_show_iset(void);
void LCD_show_icon(uint16_t x, uint16_t y, uint16_t length, uint16_t width, const uint8_t pic[], uint16_t bg_color);
void LCD_show_on_off(uint16_t x, uint16_t y, uint16_t length, uint16_t width, const uint8_t pic[], uint16_t bg_color);
void LCD_fill_line(uint8_t line, uint16_t color);
void LCD_on_menu_line(uint8_t line, uint8_t* image, uint8_t* text);
void LCD_off_menu_line(uint8_t line, uint8_t* image, uint8_t* text);

#endif
