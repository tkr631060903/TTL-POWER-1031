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
    HAL_UART_Transmit(&huart2, (uint8_t*)1, 1, 1000);
    HAL_UART_Receive_IT(&huart2, &Uart2_ReceiveBuff, 1);
    printf("Init Success\r\n");
}
