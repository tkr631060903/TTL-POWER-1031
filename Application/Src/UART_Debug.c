/**
 * @file UART_Debug.c
 * @author TanKairong (tkr631060903@gmail.com)
 * @brief uart1调试串口
 * @version 0.1
 * @date 2023-12-27
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "UART_Debug.h"
#include "Application.h"

extern UART_HandleTypeDef huart2;

/**
 *@brief printf重定义
 * 需要在keil5勾选使用微库(Use MicroLIB)，否则STM32正常上电不启动
 * @param ch 
 * @param f 
 * @return int 
 */
int fputc(int ch, FILE* f)
{
  HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 1000);
  return ch;
}
