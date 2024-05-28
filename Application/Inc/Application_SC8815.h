/**
 *@file Application_SC8815.h
 * @author TanKairong (tkr631060903@gmail.com)
 * @brief SC8815应用程序实现
 * @version 0.1
 * @date 2024-01-31
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef Application_SC8815_H
#define Application_SC8815_H

#include "SC8815.h"
#include "tim.h"
 // #include "Application.h"

#define SC8815_WRITE_ADDR 0xE8
#define SC8815_READ_ADDR 0xE9

/* 定义读写SCL和SDA的宏，已增加代码的可移植性和可阅读性 */
/* SCL = 1 */
#define SC8815_I2C_SCL_1() HAL_GPIO_WritePin(SC8815_GPIO_PORT_I2C, SC8815_I2C_SCL_Pin, GPIO_PIN_SET)
/* SCL = 0 */
#define SC8815_I2C_SCL_0() HAL_GPIO_WritePin(SC8815_GPIO_PORT_I2C, SC8815_I2C_SCL_Pin, GPIO_PIN_RESET)
/* SDA = 1 */
#define SC8815_I2C_SDA_1() HAL_GPIO_WritePin(SC8815_GPIO_PORT_I2C, SC8815_I2C_SDA_Pin, GPIO_PIN_SET)
/* SDA = 0 */
#define SC8815_I2C_SDA_0() HAL_GPIO_WritePin(SC8815_GPIO_PORT_I2C, SC8815_I2C_SDA_Pin, GPIO_PIN_RESET)
/* 读SDA口线状态 */
#define SC8815_I2C_SDA_READ() HAL_GPIO_ReadPin(SC8815_GPIO_PORT_I2C, SC8815_I2C_SDA_Pin)

#define SC8815_GPIO_PORT_I2C GPIOB /* GPIO端口 */
#define SC8815_SCL_Clr() HAL_GPIO_WritePin(SC8815_GPIO_PORT_I2C, SC8815_I2C_SCL_Pin, GPIO_PIN_RESET)
#define SC8815_SCL_Set() HAL_GPIO_WritePin(SC8815_GPIO_PORT_I2C, SC8815_I2C_SCL_Pin, GPIO_PIN_SET)
#define SC8815_SDA_Clr() HAL_GPIO_WritePin(SC8815_GPIO_PORT_I2C, SC8815_I2C_SDA_Pin, GPIO_PIN_RESET)
#define SC8815_SDA_Set() HAL_GPIO_WritePin(SC8815_GPIO_PORT_I2C, SC8815_I2C_SDA_Pin, GPIO_PIN_SET)

#define i2c_delay(us) TIM2_Delay1us(us)

#define SC8815_PRESET_ADDR 0x800F000   // 8815预设地址
#define SC8815_TIM_WORK_ADDR 0x800F400   // 8815定时工作地址

typedef enum
{
    SC8815_Run = 0x00U,
    SC8815_Shutdown = 0x01U,
    SC8815_Standby = 0x02U,
    SC8815_LoadStart = 0x03U,
} SC8815_StatusTypeDef; // SC8815工作状态结构体

typedef struct
{
    SC8815_StatusTypeDef SC8815_Status; // 8815状态
    uint16_t SC8815_IBAT_Limit;         // 8815电池(输入)限流mA
    float SC8815_IBUS_Limit;            // 8815 VBUS(输出)限流mA
    float SC8815_IBUS_Limit_Old;        // 8815 VBUS(输出)限流mA旧值
    float SC8815_VBUS;                  // 8815 VBUS输出电压mv
    float SC8815_VBUS_Old;              // 8815 VBUS输出电压mv旧值
    float SC8815_VBUS_IBUS_Step;        // 8815 VBUS输出电压/电流步进值
    uint32_t VOUT_Open_Time;            // 8815 输出开启时间
} SC8815_ConfigTypeDef;                 // SC8815配置结构体

typedef struct
{
    uint8_t SC8815_Preset_Num;   // 8815预设号
    uint16_t SC8815_IBAT_Limit;  // 8815电池(输入)限流mA
    float SC8815_IBUS_Limit;     // 8815 VBUS(输出)限流mA
    float SC8815_VBUS;           // 8815 VBUS输出电压mv
    float SC8815_VBUS_IBUS_Step; // 8815 VBUS输出电压/电流步进值
} SC8815_PresetTypeDef;          // SC8815预设结构体

typedef struct
{
    uint8_t SC8815_TIM_Work_Num;     // 8815定时工作号
    // uint8_t SC8815_TIM_Work_Length;  // 8815定时工作长度,考虑使用保持时间来判断是否结束
    uint16_t SC8815_TIM_Work_second; // 8815定时工作保持时间单位s
    uint16_t SC8815_IBAT_Limit;      // 8815电池(输入)限流mA
    float SC8815_IBUS_Limit[40];     // 8815 VBUS(输出)限流mA
    float SC8815_VBUS[40];           // 8815 VBUS输出电压mv
} SC8815_TIM_WorkTypeDef;            // SC8815定时工作结构体

extern SC8815_ConfigTypeDef SC8815_Config;

void Application_SC8815_Init(void);
void Application_SC8815_Shutdown(void);
void Application_SC8815_Standby(void);
void Application_SC8815_Run(void);
void Application_SC8815_loadStart(void);
void SC8815_Soft_Protect(void);
void SC8815_Preset_Save(SC8815_PresetTypeDef* SC8815_Preset);

#endif
