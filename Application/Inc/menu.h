#ifndef __MENU_H
#define __MENU_H
#include "menu_ui.h"
#include "menu_conf.h"
#include "Hardware_setting.h"

typedef struct Menu_Key_Index
{
    menu_u8 main_menu_current_index;
    menu_u8 presset_current_index;
    menu_u8 fastch_current_index;
    menu_u8 Current_Page;
} Key_Index;
extern Key_Index sub_index;

typedef void (*menu_op_func)(menu_u8);
typedef struct OP_STRUCT
{
    int op_menu;        // 操作菜单
    menu_op_func opfun; // 操作方法
} OP_MENU_PAGE;

typedef enum
{
    MAIN_PAGE = 0,  // 开机页面
    MAIN_MENU_PAGE, // 主菜单页面
    VOUT_PAGE,      // 设置输出电压页面
    IOUT_PAGE,      // 设置输出限流页面
    PRESSET_PAGE,   // 设置预设页面
    BUZZER_PAGE,    // 设置蜂鸣器页面
    TEMP_PAGE,      // 设置温度预警页面
    FASTCH_PAGE,    // 设置快充输入页面
} OP_PAGE;

void Enter_Page(menu_i32 index, menu_u8 KeyValue);
void Menu_Select_Item(menu_u8 KeyValue);

void main_page_process(menu_u8 KeyValue);
void main_menu_page_process(menu_u8 KeyValue);
void vout_page_process(menu_u8 KeyValue);
void iout_page_process(menu_u8 KeyValue);
void presset_page_process(menu_u8 KeyValue);
void buzzer_page_process(menu_u8 KeyValue);
void temp_page_process(menu_u8 KeyValue);
void fastch_page_process(menu_u8 KeyValue);

#endif //__MENU_H
