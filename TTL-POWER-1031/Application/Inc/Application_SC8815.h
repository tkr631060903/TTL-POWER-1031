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
#include "Application.h"

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
#define SC8815_VBUS_MAX 36000
#define SC8815_VBUS_MIN 2700
#define SC8815_IBUS_MAX 8000
#define SC8815_IBUS_MIN 500
#define SC8815_TIM_WORK_TIME_FAST 0xFFFFFFFF //判断当前开启是否为首次
#define I2C_MUTEX_TIME_MS  5  //I2C互斥锁时间单位ms

typedef enum
{
    SC8815_Run = 0x00U,
    SC8815_Shutdown = 0x01U,
    SC8815_Standby = 0x02U,
    SC8815_LoadStart = 0x03U,
    SC8815_TIM_WORK,
    SC8815_PORT,
} SC8815_StatusTypeDef;

typedef enum
{
    tim_work_lcd_main = 0x00U,
    tim_work_lcd_running,
    tim_work_lcd_cmd,
    tim_work_lcd_none,
} sc8815_tim_work_lcd_flush_TypeDef;

typedef struct
{
    SC8815_StatusTypeDef SC8815_Status; // 8815状态
    float SC8815_IBAT_Limit;         // 8815电池(输入)限流mA
    float SC8815_IBUS_Limit;            // 8815 VBUS(输出)限流mA
    float SC8815_IBUS_Limit_Old;        // 8815 VBUS(输出)限流mA旧值
    float SC8815_VBUS;                  // 8815 VBUS输出电压mv
    float SC8815_VBUS_Old;              // 8815 VBUS输出电压mv旧值
    float SC8815_VBUS_IBUS_Step;        // 8815 VBUS输出电压/电流步进值
    float SC8815_VBUS_CMD_Step;        // 8815 VBUS输出电压命令步进值
    float SC8815_IBUS_CMD_Step;        // 8815 VBUS输出电流命令步进值
    uint32_t VOUT_Open_Time;            // 8815 输出开启时间
    uint32_t sc8815_tim_work_time;      // 8815 定时工作时间定时器增加值
    uint8_t sc8815_tim_work_step;       // 8815 定时工作当前循环第n个步骤
    uint8_t sc8815_tim_work_lcd_flush;  // 8815 定时工作LCD刷新标志位
    uint8_t sc8815_pfm_delay_ms;  // 8815 关闭PFM延时ms
    uint8_t sc8815_sfb_delay_ms;  // 8815 开启SFB延时ms
    uint8_t sc8815_calibration_ibus_flag;  // 是否开启IBUS校准标志位
} SC8815_ConfigTypeDef;                 // SC8815配置结构体

typedef struct
{
    uint8_t SC8815_Preset_Num;   // 8815预设号
    float SC8815_IBAT_Limit;  // 8815电池(输入)限流mA
    float SC8815_IBUS_Limit;     // 8815 VBUS(输出)限流mA
    float SC8815_VBUS;           // 8815 VBUS输出电压mv
    float SC8815_VBUS_IBUS_Step; // 8815 VBUS输出电压/电流步进值
} SC8815_PresetTypeDef;          // SC8815预设结构体

typedef struct
{
    uint16_t circular;   // 循环次数
    uint32_t SC8815_TIM_Work_second[SC8815_TIM_WORK_STEP]; // 8815定时工作保持时间单位s
    // float SC8815_IBAT_Limit;      // 8815电池(输入)限流mA
    float SC8815_IBUS_Limit[SC8815_TIM_WORK_STEP];     // 8815 VBUS(输出)限流mA
    float SC8815_VBUS[SC8815_TIM_WORK_STEP];           // 8815 VBUS输出电压mv
} SC8815_TIM_WorkTypeDef;            // SC8815定时工作预设结构体

extern SC8815_ConfigTypeDef SC8815_Config;
extern SC8815_TIM_WorkTypeDef SC8815_TIM_Work[SC8815_TIM_WORK_SIZE];

void Application_SC8815_Init(void);
void Application_SC8815_Shutdown(void);
void Application_SC8815_Standby(void);
void Application_SC8815_Run(void);
void Application_SC8815_loadStart(void);
void SC8815_Soft_Protect(void);
void SC8815_Preset_Read(void);
void SC8815_Preset_Save(void);
uint8_t get_i2c_mutex(void);
void set_i2c_mutex(uint8_t status);
uint8_t get_sc8815_power(void);
void set_sc8815_power(uint8_t power);
void i2c_Start(void);
void i2c_WaitAck(void);
void i2c_SendByte(uint8_t dat);
uint8_t i2c_ReadByte(void);
void i2c_Stop(void);
void IIC_delay(void);
void i2c_NAck(void);
void i2c_Ack(void);
uint8_t I2C_ReadRegByte(uint8_t SlaveAddress, uint8_t RegAddress);
void I2C_WriteRegByte(uint8_t SlaveAddress, uint8_t RegAddress, uint8_t ByteData);
void SC8815_Preset_Mode_Quit(void);
void App_SC8815_SetOutputVoltage(float voltage);
void App_SC8815_SetBusCurrentLimit(float voltage);
void SC8815_output_calibration(uint8_t calibration);
void SC8815_IBUS_calibration(void);
void SC8815_auto_output(void);

#endif
