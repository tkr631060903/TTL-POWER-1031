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

unsigned int Value_count = 0;            //状态计数
int Encoder_A_Last_Value = 0;            //第一次A项的值
int Encoder_B_Last_Value = 0;            //第一次B项的值
int Encoder_A_Value = 0;                 //第二次A项的值
int Encoder_B_Value = 0;                 //第二次B项的值
/**
 *@brief IO中断回调函数
 *
 * @param GPIO_Pin
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    switch (GPIO_Pin)
    {
    case KEY1_Pin:
        printf("KEY1\r\n");
        break;
    case KEY2_Pin:
        printf("KEY2\r\n");
        __set_FAULTMASK(1); //关闭所有中断
        NVIC_SystemReset(); //进行软件复位
        break;
    case KEY3_Pin:
        // printf("KEY3\r\n");
        if (HAL_GPIO_ReadPin(SC8815_PSTOP_GPIO_Port, SC8815_PSTOP_Pin) == 0)
        {
            Application_SC8815_Standby();
            HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
            printf("off");
        }
        else {
            HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
            Application_SC8815_Run();
            Application_SC8815_loadStart();
            printf("run");
        }
        break;
    case Rotar_L_Pin:
        // 解析旋转编码器
        if (Value_count == 0) {               //边缘计数值，计数两次边缘值
            Encoder_A_Last_Value = HAL_GPIO_ReadPin(Rotar_L_GPIO_Port, Rotar_L_Pin);   //捕获A项的值
            Encoder_B_Last_Value = HAL_GPIO_ReadPin(Rotar_R_GPIO_Port, Rotar_R_Pin);   //捕获B项的值
            Value_count = 1;               //开始第一次计数
        }
        else if (Value_count == 1) {         //完成一个边缘捕获
            Encoder_A_Value = HAL_GPIO_ReadPin(Rotar_L_GPIO_Port, Rotar_L_Pin);        //捕获A项的值
            Encoder_B_Value = HAL_GPIO_ReadPin(Rotar_R_GPIO_Port, Rotar_R_Pin);        //捕获B项的值
            //状态判断处理
            if (((Encoder_A_Last_Value == 0 && Encoder_A_Value == 1) && (Encoder_B_Last_Value == 1 && Encoder_B_Value == 0)) || ((Encoder_A_Last_Value == 1 && Encoder_A_Value == 0) && (Encoder_B_Last_Value == 0 && Encoder_B_Value == 1))) {        //逆时针旋转
                printf("Rotar_L\r\n");     //左
            }
            else if (((Encoder_A_Last_Value == 0 && Encoder_A_Value == 1) && (Encoder_B_Last_Value == 0 && Encoder_B_Value == 1)) || ((Encoder_A_Last_Value == 1 && Encoder_A_Value == 0) && (Encoder_B_Last_Value == 1 && Encoder_B_Value == 0))) {  //顺时针旋转
                printf("Rotar_R\r\n");      //右
            }
            Encoder_B_Last_Value = 2;       //清除状态值，不初始化0原因是在全局第一次初始化就是0，为了区别
            Encoder_A_Last_Value = 2;       //清除状态值
            Value_count = 0;               //清除状态值
        }
        break;
    default:
        break;
    }
}
