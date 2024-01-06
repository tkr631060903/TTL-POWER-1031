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
 *@brief KEY4中断初始化
 * 
 */
void KEY4_INT_Init(void)
{
    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

/**
 * @brief 应用初始化
 *
 */
void Application_Init(void)
{
    KEY4_INT_Init();
    // 初始化串口中断输入
    HAL_UART_Receive_IT(&huart2, &Uart2_ReceiveBuff, 1);
    extern ADC_HandleTypeDef hadc1;
    HAL_ADCEx_Calibration_Start(&hadc1);    // 校准ADC
    CH224K_9V();
    SC8815_Test();
    printf("Init Success\r\n");
}
