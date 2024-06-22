#ifndef __MENU_UI_H
#define __MENU_UI_H
#include "menu_conf.h"
#include "menu.h"

void main_page_init(void);
void main_menu_Init(void);
void main_menu_page_ui_process(menu_u8 index);
void vout_page_ui_process(menu_u8 KeyValue);
void iout_page_ui_process(menu_u8 KeyValue);
void presset_page_ui_process(menu_u8 KeyValue);
void buzzer_page_ui_process(menu_u8 KeyValue);
void temp_page_ui_process(menu_u8 KeyValue);
void fastch_page_ui_process(menu_u8 index);

#endif //__MENU_UI_H
