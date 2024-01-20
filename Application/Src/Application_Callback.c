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
    extern volatile Application_Config APP_config;
    switch (GPIO_Pin)
    {
    case KEY1_Pin:
        // printf("KEY1\r\n");
        OLED_Clear();
        SoftwareDelay(50);
        OLED_Clear();
        // __set_FAULTMASK(1); //关闭所有中断
        if (APP_config.SetMod == currentMod || APP_config.SetMod == voltageMod)
        {
            // 步进除10
            if (APP_config.set_Step == 1000)
            {
                APP_config.set_Step = 100;
            }
            break;
        }
        if (APP_config.SetMod == noneMod)
        {
            APP_config.SetMod = currentMod;    // 设置调节电流限流模式
            APP_config.SC8815_VBUS_Current_Limit_Old = APP_config.SC8815_VBUS_Current_Limit;
        }
        // __set_FAULTMASK(0); //开启所有中断
        break;
    case KEY2_Pin:
        OLED_Clear();
        SoftwareDelay(50);
        OLED_Clear();
        // __set_FAULTMASK(1); //关闭所有中断
        if (APP_config.SetMod == currentMod || APP_config.SetMod == voltageMod)
        {
            // 步进乘10
            if (APP_config.set_Step == 100)
            {
                APP_config.set_Step = 1000;
            }
            break;
        }
        if (APP_config.SetMod == noneMod)
        {
            APP_config.SetMod = voltageMod;    // 设置调节输出电压模式
            APP_config.Set_OutVoltage_Old = APP_config.Set_OutVoltage;
        }
        // __set_FAULTMASK(0); //开启所有中断
        break;
    case KEY3_Pin:
        // printf("KEY3\r\n");
        OLED_Clear();
        SoftwareDelay(50);
        OLED_Clear();
        if (APP_config.SetMod != noneMod)
        {
            APP_config.SetMod = noneMod;
            if (APP_config.SetMod == VINErrorMod || currentProtectMod)
            {
                SC8815_SetOutputVoltage(APP_config.Set_OutVoltage);
                SC8815_SetBusCurrentLimit(APP_config.SC8815_VBUS_Current_Limit);
                break;
            }
            APP_config.SC8815_VBUS_Current_Limit = APP_config.SC8815_VBUS_Current_Limit_Old;
            APP_config.Set_OutVoltage = APP_config.Set_OutVoltage_Old;
        }
        else {
            if (HAL_GPIO_ReadPin(SC8815_PSTOP_GPIO_Port, SC8815_PSTOP_Pin) == 0)
            {
                Application_SC8815_Standby();
                // printf("off");
            }
            else {
                Application_SC8815_Run();
                Application_SC8815_loadStart();
                // printf("run");
            }
        }
        break;
    case Rotar_L_Pin:
        if (APP_config.SetMod == noneMod)
        {
        }
        else {
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
                if (((Encoder_A_Last_Value == 0 && Encoder_A_Value == 1) && (Encoder_B_Last_Value == 1 && Encoder_B_Value == 0)) || ((Encoder_A_Last_Value == 1 && Encoder_A_Value == 0) && (Encoder_B_Last_Value == 0 && Encoder_B_Value == 1)))   //逆时针旋转
                {
                    printf("Rotar_L\r\n");     //左
                    if (APP_config.SetMod == currentMod)
                    {
                        if ((int)APP_config.SC8815_VBUS_Current_Limit - APP_config.set_Step <= 300)
                        {
                            APP_config.SC8815_VBUS_Current_Limit = 300;
                        }
                        else {
                            APP_config.SC8815_VBUS_Current_Limit = APP_config.SC8815_VBUS_Current_Limit - APP_config.set_Step;
                        }
                    }
                    else if (APP_config.SetMod == voltageMod)
                    {
                        if ((int)APP_config.Set_OutVoltage - APP_config.set_Step <= 0)
                        {
                            APP_config.SC8815_VBUS_Current_Limit = 0;
                        }
                        else {
                            APP_config.Set_OutVoltage = APP_config.Set_OutVoltage - APP_config.set_Step;
                        }
                    }
                }
                else if (((Encoder_A_Last_Value == 0 && Encoder_A_Value == 1) && (Encoder_B_Last_Value == 0 && Encoder_B_Value == 1)) || ((Encoder_A_Last_Value == 1 && Encoder_A_Value == 0) && (Encoder_B_Last_Value == 1 && Encoder_B_Value == 0)))  //顺时针旋转
                {
                    printf("Rotar_R\r\n");      //右
                    if (APP_config.SetMod == currentMod)
                    {
                        APP_config.SC8815_VBUS_Current_Limit = APP_config.SC8815_VBUS_Current_Limit + APP_config.set_Step;
                        if (APP_config.SC8815_VBUS_Current_Limit >= 6000)
                        {
                            APP_config.SC8815_VBUS_Current_Limit = 6000;
                        }
                    }
                    else if (APP_config.SetMod == voltageMod)
                    {
                        APP_config.Set_OutVoltage = APP_config.Set_OutVoltage + APP_config.set_Step;
                        if (APP_config.Set_OutVoltage >= 36000)
                        {
                            APP_config.Set_OutVoltage = 36000;
                        }
                    }
                }
                Encoder_B_Last_Value = 2;       //清除状态值，不初始化0原因是在全局第一次初始化就是0，为了区别
                Encoder_A_Last_Value = 2;       //清除状态值
                Value_count = 0;               //清除状态值
            }
        }
        break;
    default:
        break;
    }
}
