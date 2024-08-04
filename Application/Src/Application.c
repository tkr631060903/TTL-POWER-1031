/**
 * @file Application.c
 * @author TanKairong (tkr631060903@gmail.com)
 * @brief 应用入口
 * @version 0.1
 * @date 2023-12-27
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "Application.h"
#include "Application_SC8815.h"
#include "Application_BUZZER.h"
#include "menu.h"
#include "Application_LCD.h"

Application_Config APP_config;

/**
 *@brief 系统运行
 *
 */
void Application_main()
{
    // uint32_t starttick = 0;
    extern menu_i32 current_menu_index;
    extern SC8815_ConfigTypeDef SC8815_Config;
    SC8815_Config.sc8815_tim_work_lcd_flush = tim_work_lcd_none;
    while (1)
    {
        // starttick = HAL_GetTick();
        Application_SC8815_loadStart();
        key4_button_process();
        key1_button_process();
        key2_button_process();
        rotary_knob_process();
        key3_button_process();
        // SC8815_Soft_Protect();
        SET_LED1_Status();
        if (current_menu_index == MAIN_PAGE)
        {
            APP_LCD_main_show();
        }
        if (SC8815_Config.sc8815_tim_work_lcd_flush == tim_work_lcd_main)
        {
            LCD_Clear();
            APP_LCD_main_init();
            current_menu_index = MAIN_PAGE;
            SC8815_Config.sc8815_tim_work_lcd_flush = tim_work_lcd_none;
        }
        else if (SC8815_Config.sc8815_tim_work_lcd_flush == tim_work_lcd_running)
        {
            presset_running_page_process(0);
            SC8815_Config.sc8815_tim_work_lcd_flush = tim_work_lcd_none;
        }
        // CDC_Transmit_FS((uint8_t*)&starttick, sizeof(uint32_t)); //CDC_Receive_FS中断接收
        // printf("tick: %d\n", HAL_GetTick() - starttick);
    }
}

/**
 *@brief 错误处理
 *
 */
void Application_Error_Handler()
{
    /* User can add his own implementation to report the HAL error return state */
    printf("Error_Handler");
}

/**
 *@brief 函数参数错误处理
 *
 */
void Application_Assert_Failed()
{
    /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
}

/**
 *@brief 软件延时72MHz时钟配置 = 1ms
 *
 * @param time
 */
void Application_SoftwareDelay(uint16_t time)
{
    uint16_t i = 0;
    while (time--)
    {
        i = 9060;  //自己定义
        while (i--);
    }
}

/**
 *@brief 识别按键4单击并执行相应操作
 *
 */
void key4_button_process(void)
{
    HAL_Delay(50);
    if (HAL_GPIO_ReadPin(KEY4_GPIO_Port, KEY4_Pin) == RESET)
        return;
    Menu_Select_Item(KEY4_SHORT);
    BUZZER_OPEN(100);
}

uint8_t key1_press = 0; //ket1是否按下标志位
/**
 *@brief 识别按键3单击并执行相应操作
 *
 */
void key1_button_process(void)
{
    if (key1_press)
    {
        key1_press = 0;
        BUZZER_OPEN(100);
        Menu_Select_Item(KEY1_SHORT);
    }
}

uint32_t key2PressStartTime = 0;    //key2按下时间
uint8_t key2_press = 0; //ket2是否按下标志位
/**
 *@brief 识别按键2单击和长按并执行相应操作
 *
 */
void key2_button_process(void)
{
    if (key2_press)
    {
        if (HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin) == SET)
        {
            //key2检测到单击
            key2PressStartTime = 0; // 重置计时器
            key2_press = 0;
            BUZZER_OPEN(100);
            Menu_Select_Item(KEY2_SHORT);
            return;
        }
        else if (HAL_GetTick() - key2PressStartTime >= KEY_LONG_PRESS_THRESHOLD)
        {
            //key2检测到长按
            key2PressStartTime = 0; // 重置计时器
            key2_press = 0;
            BUZZER_OPEN(100);
            Menu_Select_Item(KEY2_LONG);
            return;
        }
    }
}

uint8_t key3_press = 0; //ket3是否按下标志位
/**
 *@brief 识别按键3单击并执行相应操作
 *
 */
void key3_button_process(void)
{
    if (key3_press)
    {
        key3_press = 0;
        BUZZER_OPEN(100);
        Menu_Select_Item(KEY3_SHORT);
    }
}

/**
 *@brief 设置LED1状态
 *
 */
void SET_LED1_Status(void)
{
    if (HAL_GPIO_ReadPin(SC8815_PSTOP_GPIO_Port, SC8815_PSTOP_Pin) == GPIO_PIN_RESET && HAL_GPIO_ReadPin(SC8815_CE_GPIO_Port, SC8815_CE_Pin) == GPIO_PIN_RESET)
    {
        HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
    }
    else {
        HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
    }
}

uint8_t rotary_knob_value = 0;
void rotary_knob_process(void)
{
    if (rotary_knob_value == LEFT)
    {
        Menu_Select_Item(RIGHT);
        BUZZER_OPEN(100);
        rotary_knob_value = 0;
    }
    else if (rotary_knob_value == RIGHT)
    {
        Menu_Select_Item(LEFT);
        BUZZER_OPEN(100);
        rotary_knob_value = 0;
    }
}
