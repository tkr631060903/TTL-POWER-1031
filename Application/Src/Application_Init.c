/**
 * @file Application_Init.c
 * @author TanKairong (tkr631060903@gmail.com)
 * @brief 应用初始化
 * @version 0.1
 * @date 2023-12-27
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "Application_Init.h"

uint8_t Uart2_ReceiveBuff = 0;  //串口2接收缓冲区

/**
 * @brief 应用初始化
 *
 */
void Application_Init(void)
{
    // 初始化串口中断输入
    HAL_UART_Receive_IT(&huart2, &Uart2_ReceiveBuff, 1);
    HAL_Delay(500);
    extern ADC_HandleTypeDef hadc1;
    HAL_ADCEx_Calibration_Start(&hadc1);    // 校准ADC
    extern volatile Application_Config APP_config;
    APP_config.SetMod = noneMod;    // 设置默认模式
    extern ADC_HandleTypeDef hadc1;
    uint16_t ADC_Value[2];    // ADC值,ADC_Value[0] == VDD_OUT,ADC_Value[1] == VDD_IN
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&ADC_Value, 2);    // 开始ADC DMA
    APP_config.DC_Voltage = ((float)ADC_Value[1] * (3.3 / 4096)) / 0.0975;
    Application_CH224K_init();
    Application_SC8815_Init();
    OLED_Init();  //初始化OLED
    printf("Init Success\r\n");
}
