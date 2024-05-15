/**
 * @file UART_Debug.h
 * @author TanKairong (tkr631060903@gmail.com)
 * @brief uart1调试串口
 * @version 0.1
 * @date 2023-12-27
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef UART_Debug_H
#define UART_Debug_H

#include <stdio.h>

#define UART_Cmd_Length 20

int fputc(int ch, FILE* f);
int ascii_process(char *cmd);

#endif
