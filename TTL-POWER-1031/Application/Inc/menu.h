#ifndef __MENU_H
#define __MENU_H
#include "menu_ui.h"
#include "menu_conf.h"
#include "Hardware_setting.h"
#include "Application.h"

#define MENU_PAGE_NUM 7

typedef struct Menu_Key_Index
{
    menu_u8 main_menu_current_index;
    menu_u8 presset_current_index;
    menu_u8 presset_config_current_index;
    menu_u8 presset_config_set_current_index;
    menu_u8 buzzer_current_index;
    float temperature_current_index;
    menu_u8 fastch_current_index;
    menu_u8 VBUS_calibration_current_index;
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
    PRESSET_CONFIG_PAGE,   // 预设参数页面
    PRESSET_CONFIG_SET_PAGE,   // 预设参数设置页面
    PRESSET_START_PAGE,   // 开启预设页面
    PRESSET_RUNNING_PAGE,   // 预设运行页面
    BUZZER_PAGE,    // 设置蜂鸣器页面
    TEMPERATURE_PAGE,      // 设置温度预警页面
    FASTCH_PAGE,    // 设置快充输入页面
    VBUS_CALIBRATION_PAGE,    // vbus校准页面
    PROTECT_PAGE,   //保护状态页面
    ABOUT_PAGE,   //保护状态页面
    DC_LIMIT_PAGE,  //DC电流限制页面
} OP_PAGE;

typedef enum
{
    PRESSET_SET_VOUT = 0, // 设置电压
    PRESSET_SET_IOUT,     // 设置电流
    PRESSET_SET_TIME,     // 设置保持时间
    PRESSET_SET_CIRCULAR, // 设置循环次数
} presset_config_set_value;

typedef struct
{
    uint8_t set_flag;   //预设参数设置标志0设置电压 1设置电流 2设置保持时间 3设置循环次数
    uint8_t current_index; //当前预设参数索引
    uint16_t set_setp;   //参数设置步进
    uint16_t set_circular;
    uint32_t set_time[SC8815_TIM_WORK_STEP];
    float set_vbus[SC8815_TIM_WORK_STEP];
    float set_ibus[SC8815_TIM_WORK_STEP];
} presset_config_set_typeDef;

void Enter_Page(menu_i32 index, menu_u8 KeyValue);
void Menu_Select_Item(menu_u8 KeyValue);

void main_page_process(menu_u8 KeyValue);
void main_menu_page_process(menu_u8 KeyValue);
void vout_page_process(menu_u8 KeyValue);
void iout_page_process(menu_u8 KeyValue);
void presset_page_process(menu_u8 KeyValue);
void presset_config_page_process(menu_u8 KeyValue);
void presset_config_set_page_process(menu_u8 KeyValue);
void presset_start_page_process(menu_u8 KeyValue);
void presset_running_page_process(menu_u8 KeyValue);
void buzzer_page_process(menu_u8 KeyValue);
void temperature_page_process(menu_u8 KeyValue);
void fastch_page_process(menu_u8 KeyValue);
void VBUS_calibration_page_process(menu_u8 KeyValue);
void protect_page_process(menu_u8 KeyValue);
void about_page_process(menu_u8 KeyValue);
void DC_limit_page_process(menu_u8 KeyValue);

#endif //__MENU_H
