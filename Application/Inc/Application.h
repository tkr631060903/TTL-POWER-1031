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
#ifndef __Application_H__
#define __Application_H__

#include "main.h"
#include "UART_Debug.h"

#define KEY4_LONG_PRESS_THRESHOLD 2000 // 定义KEY4长按阈值，单位为毫秒
// #define Get_VBUS_ADC_V ((float)ADC_Value[0] * (3.3 / 4096) / 0.0975)   // 获取MCU ADC采样VBUS单位V
// #define Get_VBAT_ADC_V ((float)ADC_Value[1] * (3.3 / 4096) / 0.0975)   // 获取MCU ADC采样VBAT单位V
#define Get_VBUS_ADC_mV ((10.754 * ((float)ADC_Value[0] * (3.3 / 4096)) - 0.1974) * 1000)   // 获取MCU ADC采样VBUS单位mV
#define Get_VBAT_ADC_mV ((10.754 * ((float)ADC_Value[1] * (3.3 / 4096)) - 0.1974) * 1000)    // 获取MCU ADC采样VBAT单位mV


void Application_main(void);
void Application_Error_Handler(void);
void Application_Assert_Failed(void);
void Application_SoftwareDelay(uint16_t time);
void KEY4_Button(void);
void SC8815_Soft_Protect(void);
void SET_LED1_Status(void);

typedef enum
{
    APP_OK = 0x00U,
    APP_ERROR = 0x01U,
    APP_BUSY = 0x02U,
    APP_TIMEOUT = 0x03U
} APP_StatusTypeDef;

typedef enum
{
    voltageMod = 0x00U,
    currentMod = 0x01U,
    noneMod = 0x02U,
    VINProtectMod = 0x03U,
    VOUTProtectMod = 0x04U,
    fastChargeMod = 0x05U,
} SetModTypeDef;

typedef struct
{
    uint16_t Set_OutVoltage;   // 设定输出电压mv
    uint16_t Set_OutVoltage_Old;   // 设定输出电压旧值mv
    uint16_t fastCharge_InVoltage;  // 快充输入电压v
    uint16_t SC8815_Battery_Current_Limit;  // 8815电池(输入)限流mA
    uint16_t SC8815_VBUS_Current_Limit; // 8815 VBUS(输出)限流mA
    uint16_t SC8815_VBUS_Current_Limit_Old; // 8815 VBUS(输出)限流旧值mA
    SetModTypeDef SetMod;   // 设置当前为控制电压还是电流参数
    uint16_t DC_Voltage;   // DC电压单位mV
    uint16_t set_Step;   // 设置电压/电流步进
    uint8_t SC8815Mod;
}Application_Config;

extern volatile Application_Config APP_config;

#endif
