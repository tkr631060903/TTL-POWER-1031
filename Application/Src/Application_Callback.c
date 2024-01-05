/**
 * @file Application_Callback.c
 * @author TanKairong (tkr631060903@gmail.com)
 * @brief 中断回调函数
 * @version 0.1
 * @date 2023-12-27
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "Application_Callback.h"
#include <string.h>

char uart2_Cmd[UART_Cmd_Length];
int cmd_Index = 0;  //串口1命令计数指针

/**
 * @brief 串口中断回调函数
 *
 * @param huart
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart)
{
    if (huart == &huart2)
    {
        extern uint8_t Uart2_ReceiveBuff;
        HAL_UART_Transmit(&huart2, &Uart2_ReceiveBuff, 1, 1000);    //调式串口回显
        uart2_Cmd[cmd_Index] = (char)Uart2_ReceiveBuff;   //保持串口命令到uart2_Cmd
        cmd_Index++;
        if (cmd_Index >= UART_Cmd_Length)
        {
            // 初始化uart2_Cmd，uart2_Cmd全置为0
            memset(uart2_Cmd, 0, UART_Cmd_Length);
            cmd_Index = 0;
            // while (cmd_Index > 0)
            // {
            //     uart2_Cmd[cmd_Index] = 0;
            //     cmd_Index--;
            // }
            // uart2_Cmd[0] = 0;
        }
        if (Uart2_ReceiveBuff == '\r')
        {
            printf("\n");
            if (strcmp("rst\r", uart2_Cmd) == 0)
            {
                printf("SysRest\r\n");
                __set_FAULTMASK(1); //关闭所有中断
                NVIC_SystemReset(); //进行软件复位
            }
            else {
                printf("Not Cmd\r\n");
            }
            // 初始化uart2_Cmd，uart2_Cmd全置为0
            memset(uart2_Cmd, 0, UART_Cmd_Length);
            cmd_Index = 0;
        }
        HAL_UART_Receive_IT(&huart2, &Uart2_ReceiveBuff, 1);
    }
}
