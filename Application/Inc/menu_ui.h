#ifndef __MENU_UI_H
#define __MENU_UI_H
#include "menu_conf.h"
#include "menu.h"

void main_page_init(void);
void main_menu_Init(void);
void main_page_ui_process(menu_u8 index);
void vout_page_ui_process(menu_u8 KeyValue);
void volume_page_ui_process(menu_u8 index);
void backlight_page_ui_process(menu_u8 index);
void wlan_page_ui_process(menu_u8 index);
void language_page_ui_process(menu_u8 index);
void sensivity_ui_process(menu_u8 index);

#endif //__MENU_UI_H
