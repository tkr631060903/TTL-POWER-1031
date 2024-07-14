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
#include "Application.h"
#include "Application_ADC.h"
#include "Application_SC8815.h"
#include "usart.h"
#include "CH224K.h"
#include "APPlication_LCD.h"
#include "Application_BUZZER.h"
#include "menu.h"

/**
 * @brief 应用初始化
 *
 */
void Application_Init(void)
{
    // 初始化串口中断输入
    extern uint8_t uart1_Receive_Data;  //串口1接收缓冲区
    HAL_UART_Receive_IT(&huart1, &uart1_Receive_Data, 1);
    HAL_TIM_Base_Start_IT(&htim3);
    APP_config.fastCharge_InVoltage = 12;
    // CH224K_12V();
    // HAL_Delay(500);
    extern ADC_HandleTypeDef hadc1;
    HAL_ADCEx_Calibration_Start(&hadc1);    // 校准ADC
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&ADC_Value, 4);    // 开始ADC DMA，会导致仿真报错
    HAL_Delay(500);
    APP_config.Sys_Mode = normalMode;    // 设置默认模式
    Application_SC8815_Init();
    Application_CH224K_init();
    LCD_Init();
    LCD_Clear();
    // Application_SC8815_Run();
    extern menu_i32 current_menu_index;
    current_menu_index = MAIN_PAGE;
    APP_LCD_main_init();
    BUZZER_OPEN(100);
    BUZZER_OPEN(300);
    printf("Init Success\r\n");
}
