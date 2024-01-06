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

System_Config sys_config;

/**
 *@brief 系统运行
 *
 */
void Application_main()
{
    extern ADC_HandleTypeDef hadc1;
    uint16_t AD_Value[2];
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&AD_Value, 2);
    while (1)
    {
        HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
        printf("VDD_OUT:%f\r\n", (float)AD_Value[0] * (3.3 / 4096));
        HAL_Delay(1000);
        printf("VDD_IN:%f\r\n", (float)AD_Value[1] * (3.3 / 4096));
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
