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

// System_Config sys_config;
Application_Config APP_config;

void SC8815_Soft_Protect(void)
{
    APP_config.SC8815_VBUS_Voltage = SC8815_Read_VBUS_Voltage();
    if ((APP_config.Set_OutVoltage - APP_config.SC8815_VBUS_Voltage) > 200)
    {
        Application_SC8815_Standby();
        printf("SC8815 Soft Protect\r\n");
    }
}

/**
 *@brief 系统运行
 *
 */
void Application_main()
{
    extern ADC_HandleTypeDef hadc1;
    uint16_t AD_Value[2];
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&AD_Value, 2);
    // Application_SC8815_Standby();
    while (1)
    {
        HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
        // printf("before: VDD_OUT:%.2fV, VDD_IN:%.2fV\r\n", ((float)AD_Value[0] * (3.3 / 4096)) / 0.1, ((float)AD_Value[1] * (3.3 / 4096)) / 0.1);
        // printf("later: VDD_OUT:%.2fV, VDD_IN:%.2fV\r\n", ((float)AD_Value[0] * (3.3 / 4096)) / 0.0975, ((float)AD_Value[1] * (3.3 / 4096)) / 0.0975);
        // printf("VBUS_Voltage:%dmV, VBUS_Current:%dmA\r\n", SC8815_Read_VBUS_Voltage(), SC8815_Read_VBUS_Current());
        // printf("BATT_Voltage:%dmV, BATT_Current:%dmA\r\n", SC8815_Read_BATT_Voltage(), SC8815_Read_BATT_Current());
        // APP_config.SC8815_VBUS_Voltage = SC8815_Read_VBUS_Voltage();
        // printf("%d\r\n", APP_config.Set_OutVoltage);
        // printf("%d\r\n", APP_config.SC8815_VBUS_Voltage);
        // printf("%d\r\n", APP_config.Set_OutVoltage - APP_config.SC8815_VBUS_Voltage);
        printf("%d", HAL_GPIO_ReadPin(KEY4_GPIO_Port, KEY4_Pin));
        HAL_Delay(1000);
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
