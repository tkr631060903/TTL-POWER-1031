/**
 * @file Application.c
 * @author TanKairong (tkr631060903@gmail.com)
 * @brief 应用入口
 * @version 0.1
 * @date 2023-12-27
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "Application.h"
#include "Application_ADC.h"
#include "Application_SC8815.h"
#include "Application_BUZZER.h"
#include "APPlication_LCD.h"
#include "CH224K.h"

Application_Config APP_config;

/**
 *@brief 系统运行
 *
 */
void Application_main()
{
    // SC8815_PresetTypeDef SC8815_Preset;
    // SC8815_Preset.SC8815_Preset_Num = 1;
    // SC8815_Preset.SC8815_IBAT_Limit = 2;
    // SC8815_Preset.SC8815_IBUS_Limit = 3;
    // SC8815_Preset.SC8815_VBUS = 4;
    // SC8815_Preset.SC8815_VBUS_IBUS_Step = 5;
    // SC8815_Preset_Save(&SC8815_Preset);
    uint32_t starttick = 0;
    while (1)
    {
        starttick = HAL_GetTick();
        Application_SC8815_loadStart();
        key4_button_process();
        key2_button_process();
        // SC8815_Soft_Protect();
        SET_LED1_Status();
        APP_LCD_Show();
        printf("tick: %d\n", HAL_GetTick() - starttick);
    }
}

/**
 *@brief 系统运行
 *
 */
// void Application_main()
// {
//     uint32_t starttick = 0;
//     uint8_t i = 0;
//     while (1)
//     {
//         starttick = HAL_GetTick();
//         APP_LCD_Show();
//         // if (i)
//         // {
//         //     LCD_Fill_DMA(0, 0, LCD_W, LCD_H, WHITE);
//         //     i = 0;
//         // }
//         // else
//         // {
//         //     LCD_Fill_DMA(0, 0, LCD_W, LCD_H, BLACK);
//         //     i = 1;
//         // }
//         printf("tick: %d\n", HAL_GetTick() - starttick);
//     }
// }

/**
 *@brief 错误处理
 *
 */
void Application_Error_Handler()
{
    /* User can add his own implementation to report the HAL error return state */
    printf("Error_Handler");
}

/**
 *@brief 函数参数错误处理
 *
 */
void Application_Assert_Failed()
{
    /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
}

/**
 *@brief 软件延时72MHz时钟配置 = 1ms
 *
 * @param time
 */
void Application_SoftwareDelay(uint16_t time)
{
    uint16_t i = 0;
    while (time--)
    {
        i = 9060;  //自己定义
        while (i--);
    }
}

/**
 *@brief 识别按键4单击并执行相应操作
 *
 */
void key4_button_process(void)
{
    if (APP_config.Sys_Mode != normalMode)
    {
        // KEY4单击设置电压/电流/快充输入电压
        if (HAL_GPIO_ReadPin(KEY4_GPIO_Port, KEY4_Pin) == GPIO_PIN_SET)
        {
            BUZZER_OPEN(100);
            APP_config.LCD_Clear = 1;
            if (APP_config.Sys_Mode == setVBUSMode)
            {
                SC8815_SetOutputVoltage(SC8815_Config.SC8815_VBUS);
                APP_config.Sys_Mode = normalMode;
                SC8815_Config.SC8815_VBUS_IBUS_Step = 1000;
                SC8815_Config.SC8815_VBUS_Old = SC8815_Config.SC8815_VBUS;
            }
            else if (APP_config.Sys_Mode == setIBUSMode) {
                SC8815_SetBusCurrentLimit(SC8815_Config.SC8815_IBUS_Limit);
                APP_config.Sys_Mode = normalMode;
                SC8815_Config.SC8815_VBUS_IBUS_Step = 1000;
                SC8815_Config.SC8815_IBUS_Limit_Old = SC8815_Config.SC8815_IBUS_Limit;
            }
            else if (APP_config.Sys_Mode == fastChargeMode) {
                switch (APP_config.fastCharge_InVoltage)
                {
                case 5:
                    CH224K_5V();
                    break;
                case 9:
                    CH224K_9V();
                    break;
                case 12:
                    CH224K_12V();
                    break;
                case 15:
                    CH224K_15V();
                    break;
                case 20:
                    CH224K_20V();
                    break;
                default:
                    break;
                }
                APP_config.Sys_Mode = normalMode;
            }
        }
    }
}

uint32_t key2PressStartTime = 0;    //key2按下时间
uint8_t key2_press = 0; //ket2是否按下标志位
/**
 *@brief 识别按键2单击和长按并执行相应操作
 * 
 */
void key2_button_process(void)
{
    if (key2_press)
    {
        if (APP_config.Sys_Mode == normalMode)
        {
            if (HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin) == SET)
            {
                //key2检测到单击
                APP_config.Sys_Mode = setVBUSMode;    // 设置调节输出电压模式
                SC8815_Config.SC8815_VBUS_Old = SC8815_Config.SC8815_VBUS;  // 保存VBUS值
                APP_config.LCD_Clear = 1;
                key2PressStartTime = 0; // 重置计时器
                key2_press = 0;
                BUZZER_OPEN(100);
                return;
            }
            else if (HAL_GetTick() - key2PressStartTime >= KEY4_LONG_PRESS_THRESHOLD)
            {
                //key2检测到长按
                APP_config.Sys_Mode = fastChargeMode;
                APP_config.LCD_Clear = 1;
                key2PressStartTime = 0; // 重置计时器
                key2_press = 0;
                BUZZER_OPEN(100);
                return;
            }
        }
        else if ((APP_config.Sys_Mode == setVBUSMode || APP_config.Sys_Mode == setIBUSMode) && SC8815_Config.SC8815_VBUS_IBUS_Step == 100)
        {
            // 步进乘10
            SC8815_Config.SC8815_VBUS_IBUS_Step = 1000;
            key2PressStartTime = 0; // 重置计时器
            key2_press = 0;
            BUZZER_OPEN(100);
            APP_config.LCD_Clear = 1;
        }
    }
}

/**
 *@brief 设置LED1状态
 *
 */
void SET_LED1_Status(void)
{
    if (HAL_GPIO_ReadPin(SC8815_PSTOP_GPIO_Port, SC8815_PSTOP_Pin) == GPIO_PIN_RESET && HAL_GPIO_ReadPin(SC8815_CE_GPIO_Port, SC8815_CE_Pin) == GPIO_PIN_RESET)
    {
        HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
    }
    else {
        HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
    }
}
