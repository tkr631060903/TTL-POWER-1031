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
#include "msg_cmd.h"
#include "Application_ADC.h"
#include "usbd_cdc_if.h"

static void sc8815_tim_work(void);
static void get_msg(void);

/**
 * @brief 串口中断回调函数
 *
 * @param huart
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart)
{
    if (huart == &huart1)
    {
        extern char uart1_Cmd[Cmd_Length];   //串口1接收缓冲区
        extern uint32_t cmd_Index;  //串口1命令计数指针
        cmd_Index += huart1.RxXferSize;		
		HAL_UART_Receive_IT(&huart1, (uint8_t*)uart1_Cmd, Cmd_Length);
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
    extern uint32_t key3PressStartTime;
    switch (GPIO_Pin)
    {
    case KEY1_Pin:
        SoftwareDelay(100);
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
        key3_press = 1;
        key3PressStartTime = HAL_GetTick();
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
        if (APP_config.msg_get_time > 0)
        {
            APP_config.msg_get_timestamp++;
        }
        sc8815_tim_work();
        get_msg();
        //SC8815_Soft_Protect();

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
        previous = HAL_GPIO_ReadPin(Rotar_L_GPIO_Port, Rotar_L_Pin);
    }
}

static void get_msg(void)
{
    extern Application_Config APP_config;
    uint32_t temp[6];
    if (APP_config.msg_get_timestamp > APP_config.msg_get_time * 1000)
    {
        temp[0] = App_getVBUS_mV();
        temp[1] = App_getIBUS_mA();
        temp[2] = (temp[0] / 1000) * (temp[1] / 1000);
        if (HAL_GPIO_ReadPin(SC8815_PSTOP_GPIO_Port, SC8815_PSTOP_Pin) == GPIO_PIN_SET)
        {
            temp[3] = 0;
        }
        else {
            temp[3] = 1;
        }
        temp[4] = HAL_GetTick() / 1000;
        temp[5] = App_getVBAT_mV();
        for (size_t i = 0; i < 6; i++)
        {
            temp[i] = htonl(temp[i]);
        }
        HAL_UART_Transmit(&huart1, (uint8_t*)&temp, sizeof(uint32_t) * 6, 1000);
        CDC_Transmit_FS((uint8_t*)&temp, sizeof(uint32_t) * 6);
        APP_config.msg_get_timestamp = 0;
    }
}

static void sc8815_tim_work(void)
{
    if (SC8815_Config.SC8815_Status == SC8815_TIM_WORK)
    {
        extern menu_i32 current_menu_index;
        extern presset_config_set_typeDef presset_config_set;
        if (presset_config_set.set_circular <= 0)
        {
            SC8815_SetOutputVoltage(SC8815_Config.SC8815_VBUS);
            SC8815_SetBusCurrentLimit(SC8815_Config.SC8815_IBUS_Limit);
            current_menu_index = MAIN_PAGE;
            SC8815_Config.SC8815_Status = SC8815_Standby;
            Application_SC8815_Standby();
            SC8815_Config.sc8815_tim_work_lcd_flush = tim_work_lcd_main;
            return;
        }
        if (SC8815_Config.sc8815_tim_work_time >= SC8815_TIM_WORK_TIME_FAST) //判断当前开启是否为首次
        {
            if (presset_config_set.set_vbus[SC8815_Config.sc8815_tim_work_step] > 0 && presset_config_set.set_ibus[SC8815_Config.sc8815_tim_work_step] >= 300)
            {
                SC8815_SetOutputVoltage(presset_config_set.set_vbus[SC8815_Config.sc8815_tim_work_step]);
                SC8815_SetBusCurrentLimit(presset_config_set.set_ibus[SC8815_Config.sc8815_tim_work_step]);
                Application_SC8815_Run();
                SC8815_SFB_Disable();
                SoftwareDelay(50);
                SC8815_SFB_Enable();
            }
            else
            {
                Application_SC8815_Standby();
            }
            SC8815_Config.sc8815_tim_work_time = 0;
        }
        else
        {
            if (SC8815_Config.sc8815_tim_work_time >= presset_config_set.set_time[SC8815_Config.sc8815_tim_work_step] * 1000)
            {
                if (SC8815_Config.sc8815_tim_work_step < SC8815_TIM_WORK_STEP - 1)
                {
                    SC8815_Config.sc8815_tim_work_step++;
                    while (presset_config_set.set_time[SC8815_Config.sc8815_tim_work_step] <= 0)
                    {
                        SC8815_Config.sc8815_tim_work_step++;
                        if (SC8815_Config.sc8815_tim_work_step >= SC8815_TIM_WORK_STEP - 1)
                        {
                            SC8815_Config.sc8815_tim_work_time = SC8815_TIM_WORK_TIME_FAST;
                            presset_config_set.set_circular--;
                            SC8815_Config.sc8815_tim_work_step = 0;
                            SC8815_Config.sc8815_tim_work_lcd_flush = tim_work_lcd_running;
                            return;
                        }
                    }
                    SC8815_Config.sc8815_tim_work_time = 0;
                    if (presset_config_set.set_time[SC8815_Config.sc8815_tim_work_step] > 0)
                    {
                        SC8815_Config.sc8815_tim_work_lcd_flush = tim_work_lcd_running;
                    }
                    if (presset_config_set.set_vbus[SC8815_Config.sc8815_tim_work_step] > 0 && presset_config_set.set_ibus[SC8815_Config.sc8815_tim_work_step] >= 300)
                    {
                        SC8815_SetOutputVoltage(presset_config_set.set_vbus[SC8815_Config.sc8815_tim_work_step]);
                        SC8815_SetBusCurrentLimit(presset_config_set.set_ibus[SC8815_Config.sc8815_tim_work_step]);
                        Application_SC8815_Run();
                        SC8815_SFB_Disable();
                        SoftwareDelay(50);
                        SC8815_SFB_Enable();
                    }
                    else
                    {
                        Application_SC8815_Standby();
                    }
                }
                else
                {
                    SC8815_Config.sc8815_tim_work_time = SC8815_TIM_WORK_TIME_FAST;
                    presset_config_set.set_circular--;
                    SC8815_Config.sc8815_tim_work_step = 0;
                    SC8815_Config.sc8815_tim_work_lcd_flush = tim_work_lcd_running;
                }
            }
        }
        SC8815_Config.sc8815_tim_work_time++;
    }
}
