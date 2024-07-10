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
#include "Application_SC8815.h"
#include "usart.h"
#include "Application_BUZZER.h"
#include "UART_Debug.h"
#include "menu.h"

 /**
  * @brief 串口中断回调函数
  *
  * @param huart
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart)
{
    if (huart == &huart1)
    {
        extern char uart1_Cmd[UART_Cmd_Length];
        extern uint8_t cmd_Index;  //串口1命令计数指针
        extern uint8_t uart1_Receive_Data;
        HAL_UART_Receive_IT(&huart1, &uart1_Receive_Data, 1);
        if ((char)uart1_Receive_Data == '\n')
        {
            return;
        }
        // HAL_UART_Transmit(&huart1, &uart1_Receive_Data, 1, 10);    //调式串口回显
        if ((char)uart1_Receive_Data == '\r')
        {
            // if (strcmp("rst\r", uart1_Cmd) == 0)
            // {
            //     printf("SysRest\r\n");
            //     __set_FAULTMASK(1); //关闭所有中断
            //     NVIC_SystemReset(); //进行软件复位
            // }
            if (ascii_process(uart1_Cmd))
            {
                printf("%s ok\r\n", uart1_Cmd);
            }
            else {
                printf("%s error\r\n", uart1_Cmd);
            }
            // 清除uart1_Cmd，uart1_Cmd全置为0
            memset(uart1_Cmd, 0, UART_Cmd_Length);
            cmd_Index = 0;
            return;
        }
        else if (cmd_Index >= UART_Cmd_Length)
        {
            // 清除uart1_Cmd，uart1_Cmd全置为0
            printf("Command too long\r\n");
            memset(uart1_Cmd, 0, UART_Cmd_Length);
            cmd_Index = 0;
            return;
        }
        uart1_Cmd[cmd_Index++] = (char)uart1_Receive_Data;   //保存串口命令到uart1_Cmd
    }
}

/**
 *@brief IO中断回调函数
 *
 * @param GPIO_Pin
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    extern uint8_t key1_press;
    extern uint8_t key2_press;
    extern uint8_t key3_press;
    extern uint32_t key2PressStartTime;
    extern uint8_t fastCharge_list[];
    switch (GPIO_Pin)
    {
    case KEY1_Pin:
        SoftwareDelay(50);
        if (HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin) == RESET)
            return;
        key1_press = 1;
        break;
    case KEY2_Pin:
        if (key2_press)return;
        key2_press = 1;
        if (key2PressStartTime == 0)
        {
            key2PressStartTime = HAL_GetTick();
        }
        break;
    case KEY3_Pin:
        SoftwareDelay(50);
        if (HAL_GPIO_ReadPin(KEY3_GPIO_Port, KEY3_Pin) == RESET)
            return;
        key3_press = 1;
        break;
    default:
        break;
    }
}

int previous;
int left = 0;
int right = 0;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
    if (htim->Instance == htim3.Instance)
    {
        extern uint8_t rotary_knob_value;
        if (HAL_GPIO_ReadPin(Rotar_L_GPIO_Port, Rotar_L_Pin) != previous)
        {
            if (HAL_GPIO_ReadPin(Rotar_R_GPIO_Port, Rotar_R_Pin) != previous)
            {
                right = 1;
                left = 0;
                // printf("right:%d\r\n", right);
                rotary_knob_value = RIGHT;
            }
            else
            {
                left = 1;
                right = 0;
                // printf("left:%d\r\n", left);
                rotary_knob_value = LEFT;
            }
        }
        previous =  HAL_GPIO_ReadPin(Rotar_L_GPIO_Port, Rotar_L_Pin);
    }
}
