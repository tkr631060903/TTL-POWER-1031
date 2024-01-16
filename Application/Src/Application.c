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

volatile Application_Config APP_config;

/**
 *@brief 系统运行
 *
 */
void Application_main()
{
    // extern ADC_HandleTypeDef hadc1;
    // uint16_t AD_Value[2];
    // HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&AD_Value, 2);
    while (1)
    {
        // HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
        // printf("before: VDD_OUT:%.2fV, VDD_IN:%.2fV\r\n", ((float)AD_Value[0] * (3.3 / 4096)) / 0.1, ((float)AD_Value[1] * (3.3 / 4096)) / 0.1);
        // printf("later: VDD_OUT:%.2fV, VDD_IN:%.2fV\r\n", ((float)AD_Value[0] * (3.3 / 4096)) / 0.0975, ((float)AD_Value[1] * (3.3 / 4096)) / 0.0975);
        // printf("VBUS_Voltage:%dmV, VBUS_Current:%dmA\r\n", SC8815_Read_VBUS_Voltage(), SC8815_Read_VBUS_Current());
        // printf("BATT_Voltage:%dmV, BATT_Current:%dmA\r\n", SC8815_Read_BATT_Voltage(), SC8815_Read_BATT_Current());
        // APP_config.SC8815_VBUS_Voltage = SC8815_Read_VBUS_Voltage();
        // printf("%d\r\n", APP_config.Set_OutVoltage);
        // printf("%d\r\n", APP_config.SC8815_VBUS_Voltage);
        // printf("%d\r\n", APP_config.Set_OutVoltage - APP_config.SC8815_VBUS_Voltage);
        // printf("%d", HAL_GPIO_ReadPin(KEY4_GPIO_Port, KEY4_Pin));
        // printf("CE:%d\r\n", HAL_GPIO_ReadPin(SC8815_CE_GPIO_Port, SC8815_CE_Pin));
        // printf("sotp:%d\r\n", HAL_GPIO_ReadPin(SC8815_PSTOP_GPIO_Port, SC8815_PSTOP_Pin));
        APP_OLED_Show();
        KEY4_Button();
        SC8815_Soft_Protect();
        // HAL_Delay(1000);
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
 *@brief SC8815软件保护
 *
 */
void SC8815_Soft_Protect(void)
{
    if (APP_config.SetMod == VINErrorMod || APP_config.SetMod == currentProtectMod)
    {
        return;
    }
    uint16_t temp = SC8815_Read_BATT_Voltage();
    if ((temp <= APP_config.fastCharge_InVoltage - (APP_config.fastCharge_InVoltage * 0.1)) || (temp <= APP_config.DC_Voltage - (APP_config.DC_Voltage * 0.1)))
    {
        // printf("输入供电不足\r\n");
        APP_config.SetMod = VINErrorMod;
        OLED_Clear();
        HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
        Application_SC8815_Standby();
        // HAL_Delay(500);
        // if (SC8815_Read_BATT_Voltage() <= APP_config.Set_OutVoltage - (APP_config.Set_OutVoltage * 0.1))
        // {
        //     // printf("输入供电不足\r\n");
        //     APP_config.SetMod = VINErrorMod;
        //     OLED_Clear();
        //     HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
        //     Application_SC8815_Standby();
        // }
    }
    if (APP_config.SC8815_VBUS_Current_Limit - SC8815_Read_VBUS_Current() <= 100 && SC8815_Read_VBUS_Voltage() <= APP_config.Set_OutVoltage - (APP_config.Set_OutVoltage * 0.1))
    {
        // printf("触发限流保护\r\n");
        APP_config.SetMod = currentProtectMod;
        OLED_Clear();
        HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
        Application_SC8815_Standby();
    }
}

/**
 *@brief 软件延时
 *
 * @param time
 */
void Application_SoftwareDelay(uint16_t time)
{
    uint16_t i = 0;
    while (time--)
    {
        i = 12000;  //自己定义
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
            }
        }
        else {
            buttonPressStartTime = 0; // 重置计时器
        }
    }
    else {
        // KEY4单击设置电压/电流
        if (HAL_GPIO_ReadPin(KEY4_GPIO_Port, KEY4_Pin) == GPIO_PIN_SET)
        {
            if (APP_config.SetMod == voltageMod)
            {
                SC8815_SetOutputVoltage(APP_config.Set_OutVoltage);
                APP_config.SetMod = noneMod;
            }
            else {
                SC8815_SetBusCurrentLimit(APP_config.SC8815_VBUS_Current_Limit);
                APP_config.SetMod = noneMod;
            }
        }
    }
}
