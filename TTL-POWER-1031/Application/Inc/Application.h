/**
 * @file Application.h
 * @author TanKairong (tkr631060903@gmail.com)
 * @brief 应用入口
 * @version 0.1
 * @date 2023-12-27
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef Application_H
#define Application_H

#include "main.h"
#include "UART_Debug.h"

#define KEY_LONG_PRESS_THRESHOLD 1000 // 定义KEY4长按阈值，单位为毫秒
#define SC8815_TIM_WORK_SIZE 10 //预设保存数量
#define SC8815_TIM_WORK_STEP 30 //每个预设设定步骤

#define TEMPERATURE_50 (float)1.343
#define TEMPERATURE_60 (float)1.085
#define TEMPERATURE_70 (float)0.825

#define APP_CONFIG_SAVE_TIME  30000

typedef enum
{
    APP_OK = 0x00U,
    APP_ERROR = 0x01U,
    APP_BUSY = 0x02U,
    APP_TIMEOUT = 0x03U
} APP_StatusTypeDef;

typedef struct
{
    uint8_t lock_key;   //锁定按键 1锁 0不锁
    uint8_t lock_buzzer;     //蜂鸣器锁定 1锁 0不锁
    uint8_t app_config_save_flag;   //应用参数保存标志位 1保存 0不保存
    float temperature;     //温度监控值
    uint16_t msg_get_time;   //信息上报时间间隔
    uint32_t msg_get_timestamp;   //信息上报时间戳
    uint16_t fastCharge_InVoltage; // 快充输入电压v
    float fastCharge_InCurrent; // 快充最大输入电流A
    uint32_t app_config_save_time;   //应用参数保存时间
} Application_Config;

typedef struct
{
    uint8_t lock_buzzer;     //蜂鸣器锁定 1锁 0不锁
    uint8_t SW_FREQ;        //开关频率
    uint8_t upgrade_flag;   //升级标志位，0不进入升级 1进入升级 2升级成功 3升级失败
    float temperature;     //温度监控值
    float SC8815_IBUS_Limit;
    float SC8815_VBUS;
    float DC_IBAT_Limit;
} Application_SaveConfig;

extern Application_Config APP_config;
extern Application_SaveConfig app_config_save_config;

void Application_main(void);
void Application_Error_Handler(void);
void Application_Assert_Failed(void);
void Application_SoftwareDelay(uint16_t time);
void key4_button_process(void);
void key1_button_process(void);
void key2_button_process(void);
void key3_button_process(void);
void SET_LED1_Status(void);
void rotary_knob_process(void);
uint32_t htonl(uint32_t hostlong);
void app_config_load(void);
void app_config_save(void);

#endif
