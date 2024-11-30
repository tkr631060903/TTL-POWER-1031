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
#include "Application_Callback.h"
#include "husb238.h"
#include "ina226.h"

/**
 * @brief 应用初始化
 *
 */
void Application_Init(void)
{
    extern ADC_HandleTypeDef hadc1;
    extern char uart1_Cmd[Cmd_Length];   //串口1接收缓冲区
    extern menu_i32 current_menu_index;
    HAL_GPIO_WritePin(POWER_RELEASE_GPIO_Port, POWER_RELEASE_Pin, GPIO_PIN_SET);	//放电
    HAL_Delay(500);
    LCD_Init();
    LCD_Clear();
    // 初始化串口中断输入
    HAL_UART_Receive_IT(&huart1, (uint8_t*)uart1_Cmd, Cmd_Length);
    HAL_TIM_Base_Start_IT(&htim3);
    HAL_ADCEx_Calibration_Start(&hadc1);    // 校准ADC
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&ADC_Value, 4);    // 开始ADC DMA，会导致仿真报错
    app_config_load();
    husb238_init();
    INA226_Init();
    HAL_Delay(500);
    Application_SC8815_Init();
    // Application_SC8815_Run();
    if (current_menu_index != DC_LIMIT_PAGE) {
        LCD_Clear();
        current_menu_index = MAIN_PAGE;
        APP_LCD_main_init();
    }
    BUZZER_OPEN(400);
    printf("Init Success\r\n");
}
