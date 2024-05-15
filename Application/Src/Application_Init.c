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
#include "Application.h"
#include "Application_ADC.h"
#include "Application_SC8815.h"
#include "usart.h"
#include "CH224K.h"
#include "APPlication_LCD.h"

uint8_t Uart1_ReceiveBuff = 0;  //串口1接收缓冲区

/**
 * @brief 应用初始化
 *
 */
void Application_Init(void)
{
    // 初始化串口中断输入
    HAL_UART_Receive_IT(&huart1, &Uart1_ReceiveBuff, 1);
    HAL_Delay(100);
    extern ADC_HandleTypeDef hadc1;
    HAL_ADCEx_Calibration_Start(&hadc1);    // 校准ADC
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&ADC_Value, 4);    // 开始ADC DMA
    APP_config.Sys_Mode = normalMode;    // 设置默认模式
    Application_CH224K_init();
    Application_SC8815_Init();
    LCD_Init();
    LCD_Fill_DMA(0, 0, LCD_W, LCD_H, BLACK);
    // Application_SC8815_Run();
    printf("Init Success\r\n");
}
