#ifndef __MENU_UI_H
#define __MENU_UI_H

#include "menu_conf.h"
#include "menu.h"
#include "Application.h"

void main_page_init(void);
void main_menu_Init(void);
void main_menu_page_ui_process(menu_u8 index, menu_u8 KeyValue);
void vout_page_ui_process(menu_u8 KeyValue);
void iout_page_ui_process(menu_u8 KeyValue);
void presset_page_ui_process(menu_u8 index, menu_u8 KeyValue);
void presset_config_page_ui_process(menu_u8 index);
void presset_config_set_page_ui_process(menu_u8 KeyValue);
void buzzer_page_ui_process(menu_u8 KeyValue);
void temperature_page_ui_process(float index);
void fastch_page_ui_process(menu_u8 KeyValue, uint16_t fastCharge_InVoltage);
void FSW_page_ui_process(menu_u8 index);
void protect_page_ui_process(menu_u8 index);
void about_page_ui_process(void);

#endif //__MENU_UI_H
