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

typedef enum
{
    SC8815_Run = 0x00U,
    SC8815_Shutdown = 0x01U,
    SC8815_Standby = 0x02U,
    SC8815_LoadStart = 0x03U,
} SC8815_StatusTypeDef;

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
} SC8815_ConfigTypeDef;

extern SC8815_ConfigTypeDef SC8815_Config;

void Application_SC8815_Init(void);
void Application_SC8815_Shutdown(void);
void Application_SC8815_Standby(void);
void Application_SC8815_Run(void);
void Application_SC8815_loadStart(void);
void SC8815_Soft_Protect(void);

#endif
