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
#define SC8815_I2C_SCL_1()  HAL_GPIO_WritePin(SC8815_GPIO_PORT_I2C, SC8815_I2C_SCL_Pin, GPIO_PIN_SET)		
/* SCL = 0 */
#define SC8815_I2C_SCL_0()  HAL_GPIO_WritePin(SC8815_GPIO_PORT_I2C, SC8815_I2C_SCL_Pin, GPIO_PIN_RESET)		
/* SDA = 1 */
#define SC8815_I2C_SDA_1()  HAL_GPIO_WritePin(SC8815_GPIO_PORT_I2C, SC8815_I2C_SDA_Pin, GPIO_PIN_SET)		
/* SDA = 0 */
#define SC8815_I2C_SDA_0()  HAL_GPIO_WritePin(SC8815_GPIO_PORT_I2C, SC8815_I2C_SDA_Pin, GPIO_PIN_RESET)		
/* 读SDA口线状态 */
#define SC8815_I2C_SDA_READ()  HAL_GPIO_ReadPin(SC8815_GPIO_PORT_I2C, SC8815_I2C_SDA_Pin)	

#define SC8815_GPIO_PORT_I2C	GPIOB			/* GPIO端口 */
#define SC8815_SCL_Clr() HAL_GPIO_WritePin(SC8815_GPIO_PORT_I2C, SC8815_I2C_SCL_Pin, GPIO_PIN_RESET)
#define SC8815_SCL_Set() HAL_GPIO_WritePin(SC8815_GPIO_PORT_I2C, SC8815_I2C_SCL_Pin, GPIO_PIN_SET)
#define SC8815_SDA_Clr() HAL_GPIO_WritePin(SC8815_GPIO_PORT_I2C, SC8815_I2C_SDA_Pin, GPIO_PIN_RESET)	
#define SC8815_SDA_Set() HAL_GPIO_WritePin(SC8815_GPIO_PORT_I2C, SC8815_I2C_SDA_Pin, GPIO_PIN_SET)	

#define i2c_delay(us) TIM2_Delay1us(us)

typedef enum
{
    SC8815Run = 0x00U,
    SC8815Shutdown = 0x01U,
    SC8815Standby = 0x02U,
    SC8815LoadStart = 0x03U,
} SC8815ModTypeDef;

void Application_SC8815_Init(void);
void Application_SC8815_Shutdown(void);
void Application_SC8815_Standby(void);
void Application_SC8815_Run(void);
void Application_SC8815_loadStart(void);
void SC8815_Soft_Protect(void);

#endif
