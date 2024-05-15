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

#define KEY4_LONG_PRESS_THRESHOLD 2000 // 定义KEY4长按阈值，单位为毫秒

void Application_main(void);
void Application_Error_Handler(void);
void Application_Assert_Failed(void);
void Application_SoftwareDelay(uint16_t time);
void KEY4_Button(void);
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
    normalMode = 0x00U,      // 正常模式
    setVBUSMode = 0x01U,     // 设置VBUS
    setIBUSMode = 0x02U,     // 设置IBUS
    VINProtectMode = 0x03U,  // 输入保护
    VOUTProtectMode = 0x04U, // 输出保护
    fastChargeMode = 0x05U,  // 设置快充
} Sys_ModeTypeDef;           // 系统当前模式

typedef struct
{
    uint8_t LCD_Clear;             // LCD是否清屏
    uint16_t fastCharge_InVoltage; // 快充输入电压v
    Sys_ModeTypeDef Sys_Mode;      // 设置当前为控制电压还是电流参数
} Application_Config;

extern Application_Config APP_config;

#endif
