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
    case Rotar_L_Pin:
        // 解析旋转编码器
        if (Value_count == 0)
        {               //边缘计数值，计数两次边缘值
            Encoder_A_Last_Value = HAL_GPIO_ReadPin(Rotar_L_GPIO_Port, Rotar_L_Pin);   //捕获A项的值
            Encoder_B_Last_Value = HAL_GPIO_ReadPin(Rotar_R_GPIO_Port, Rotar_R_Pin);   //捕获B项的值
            Value_count = 1;               //开始第一次计数
        }
        else if (Value_count == 1)
        {
            //完成一个边缘捕获
            Encoder_A_Value = HAL_GPIO_ReadPin(Rotar_L_GPIO_Port, Rotar_L_Pin);        //捕获A项的值
            Encoder_B_Value = HAL_GPIO_ReadPin(Rotar_R_GPIO_Port, Rotar_R_Pin);        //捕获B项的值
            //状态判断处理
            if (((Encoder_A_Last_Value == 0 && Encoder_A_Value == 1) && (Encoder_B_Last_Value == 1 && Encoder_B_Value == 0)) || ((Encoder_A_Last_Value == 1 && Encoder_A_Value == 0) && (Encoder_B_Last_Value == 0 && Encoder_B_Value == 1)))   //逆时针旋转
            {
                // printf("Rotar_L\r\n");     //左
                extern uint8_t rotary_knob_value;
                rotary_knob_value = LEFT;
                BUZZER_OPEN(100);
            }
            else if (((Encoder_A_Last_Value == 0 && Encoder_A_Value == 1) && (Encoder_B_Last_Value == 0 && Encoder_B_Value == 1)) || ((Encoder_A_Last_Value == 1 && Encoder_A_Value == 0) && (Encoder_B_Last_Value == 1 && Encoder_B_Value == 0)))  //顺时针旋转
            {
                // printf("Rotar_R\r\n");      //右
                extern uint8_t rotary_knob_value;
                rotary_knob_value = RIGHT;
                BUZZER_OPEN(100);
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
