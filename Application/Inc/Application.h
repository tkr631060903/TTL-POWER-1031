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
#include <stdio.h>
#include "usart.h"
#include "Application_Init.h"
#include "UART_Debug.h"
#include "SC8815.h"
#include "Application_SC8815.h"
#include "CH224K.h"
#include "OLED.h"

void Application_main(void);
void Application_Error_Handler(void);
void Application_Assert_Failed(void);
void Application_SoftwareDelay(uint16_t time);

typedef enum
{
    APP_OK = 0x00U,
    APP_ERROR = 0x01U,
    APP_BUSY = 0x02U,
    APP_TIMEOUT = 0x03U
} APP_StatusTypeDef;

typedef struct
{
    uint16_t Set_OutVoltage;   // 设定输出电压mv
    uint16_t fastCharge_InVoltage;  // 快充输入电压v
    uint8_t isVDD_OUT;  // 是否输出VDD
    uint16_t VDD_OUT_Current;
    uint16_t VDD_IN_Current;
    uint16_t SC8815_VBUS_Voltage;
    uint16_t SC8815_Battery_Voltage;
    uint16_t SC8815_VBUS_Current;
    uint16_t SC8815_Battery_Current;
    uint16_t SC8815_Battery_Current_Limit;  // 8815电池(输入)限流
    uint16_t SC8815_VBUS_Current_Limit; // 8815 VBUS(输出)限流
}Application_Config;

// 定义全局参数
typedef struct
{
    char			version[4];						// 版本号
    uint32_t	normal;
    uint16_t 	volt;									// 输出电压 mV
    uint16_t 	ibus_current;							// 限流			mA
    uint16_t 	bat_volt;							// 电池数量
    uint16_t 	charge_current;					// 充电限流	mA
    uint16_t	sleep_time;						// 休眠等待时长 单位:秒
    int8_t		work_switch;					// 升降压模式工作开关 0未工作 1工作
    uint8_t		display;							// 显示模式 DISPLAY_MODEL
    //	0 默认显示样式 
    //	1 专业版风格 
    //	
    float			volt_correct;					// 电压测量修正
    float			sc8815_correct;				// 升降压数值修正
    uint32_t	low_battery;					// 低电量保护阀值 单位:mV
    uint8_t		beep_volume;					// 提示音量大小 0-99
    uint8_t		alarm_volume;					// 报警音量大小 0-99
    uint8_t		safety_protect_mode;	// 短路保护模式 0 关闭输出 1 保持限流输出
    uint8_t		noload_turnoff;				// 空载自动关闭输出
    uint16_t	noload_holdtime;			// 空载保持输出时长
    uint8_t		fan_run_temp;					// 风扇运行温度
    uint8_t		protection;						// 保护模式配置
    //   0x01 位 短路保护模式，值为 0 默认限流 1 断开输出
    //   0x02 位 输出锁定模式，值为 0 默认打开输了时允许调节输出电压 1 打开输出情况下锁定输出电压
    //   0x04 位 未定义

    uint8_t   normal1;							// 未定义
} System_Config;

extern System_Config sys_config;

#endif
