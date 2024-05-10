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

Application_Config APP_config;

/**
 *@brief 系统运行
 *
 */
void Application_main()
{
    while (1)
    {
        KEY4_Button();
        // SC8815_Soft_Protect();
        SET_LED1_Status();
        APP_LCD_Show();
        printf("ADC-->VBAT:%fV, VBUS:%fV, IBUS:%fmA\r\n", App_getVBAT_V(), App_getVBUS_V(), App_getIBUS_mA());
        printf("SC8815-->VBAT:%dmV, VBUS:%dmV, IBUS:%dmA\r\n", SC8815_Read_BATT_Voltage(), SC8815_Read_VBUS_Voltage(), SC8815_Read_BATT_Current());
    }
}

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
 *@brief 识别按键4单击和长按，并执行相应操作
 *
 */
uint32_t buttonPressStartTime = 0;
void KEY4_Button(void)
{
    if (APP_config.SetMod == noneMod)
    {
        if (HAL_GPIO_ReadPin(KEY4_GPIO_Port, KEY4_Pin) == GPIO_PIN_SET) {
            if (buttonPressStartTime == 0) {
                buttonPressStartTime = HAL_GetTick();
            }
            uint32_t currentTime = HAL_GetTick();
            if (currentTime - buttonPressStartTime >= KEY4_LONG_PRESS_THRESHOLD) {
                // 检测到长按
                APP_config.SetMod = fastChargeMod;
                printf("fastChargeMod\r\n");
            }
        }
        else {
            buttonPressStartTime = 0; // 重置计时器
        }
    }
    else {
        // KEY4单击设置电压/电流/快充输入电压
        if (HAL_GPIO_ReadPin(KEY4_GPIO_Port, KEY4_Pin) == GPIO_PIN_SET)
        {
            BUZZER_OPEN(100);
            APP_config.LCD_Clear = 1;
            if (APP_config.SetMod == voltageMod)
            {
                SC8815_SetOutputVoltage(APP_config.VOUT);
                APP_config.SetMod = noneMod;
                APP_config.set_Step = 1000;
            }
            else if (APP_config.SetMod == currentMod) {
                SC8815_SetBusCurrentLimit(APP_config.SC8815_VBUS_Current_Limit);
                APP_config.SetMod = noneMod;
                APP_config.set_Step = 1000;
            }
            else if (APP_config.SetMod == fastChargeMod) {

            }
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
