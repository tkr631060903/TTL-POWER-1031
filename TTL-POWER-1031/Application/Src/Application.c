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
#include "Application_ADC.h"
#include "stmflash.h"
#include "string.h"
#include "Application_Callback.h"
#include "husb238.h"

#define APP_CONFIG_FLASH_ADDR     (STM32_FLASH_BASE+STM_SECTOR_SIZE*123)
#define APP_UPGRADE_FLAG_ADDR 0x801FFFE

Application_Config APP_config;
Application_SaveConfig app_config_save_config;

uint32_t APP_LCD_main_show_time = 0;
static uint32_t temperature_protect_time = 0;
extern menu_i32 current_menu_index;

/**
 *@brief 系统运行
 *
 */
void Application_main()
{
    // uint32_t starttick = 0;
    SC8815_Config.sc8815_tim_work_lcd_flush = tim_work_lcd_none;
    while (1)
    {
        // starttick = HAL_GetTick();
        // Application_SC8815_loadStart();
        key4_button_process();
        key1_button_process();
        key2_button_process();
        rotary_knob_process();
        key3_button_process();
        SET_LED1_Status();
        HUSB238_Refresh_Voltage();

        if ((current_menu_index == MAIN_PAGE || current_menu_index == VOUT_PAGE || current_menu_index == IOUT_PAGE) && HAL_GetTick() - APP_LCD_main_show_time >= 100)
        {
            APP_LCD_main_show();
            APP_LCD_main_show_time = HAL_GetTick();
            APP_config.fastCharge_InVoltage = App_getVBAT_V();  //刷新DC输出电压值
        }

        if (SC8815_Config.sc8815_tim_work_lcd_flush == tim_work_lcd_main)
        {
            LCD_Clear();
            APP_LCD_main_init();
            current_menu_index = MAIN_PAGE;
            APP_config.lock_key = 0;
            SC8815_Config.sc8815_tim_work_lcd_flush = tim_work_lcd_none;
        }
        else if ((SC8815_Config.sc8815_tim_work_lcd_flush == tim_work_lcd_running || current_menu_index == PRESSET_RUNNING_PAGE) && HAL_GetTick() - APP_LCD_main_show_time >= 100)
        {
            presset_running_page_process(0);
            APP_LCD_main_show_time = HAL_GetTick();
            // SC8815_Config.sc8815_tim_work_lcd_flush = tim_work_lcd_none;
        }
        else if (SC8815_Config.sc8815_tim_work_lcd_flush == tim_work_lcd_cmd)
        {
            presset_running_page_process(KEY4_SHORT);
            SC8815_Config.SC8815_Status = SC8815_TIM_WORK;
            SC8815_Config.sc8815_tim_work_lcd_flush = tim_work_lcd_running;
        }

        if (HAL_GetTick() - temperature_protect_time >= 5000) {
            if ( APP_config.temperature < App_getTemp() && HAL_GPIO_ReadPin(SC8815_PSTOP_GPIO_Port, SC8815_PSTOP_Pin) == GPIO_PIN_RESET)
            {
                HAL_Delay(500);
                if (APP_config.temperature < App_getTemp())
                {
                    if (SC8815_Config.SC8815_Status == SC8815_TIM_WORK) {
                        SC8815_Preset_Mode_Quit();
                    } else {
                        SC8815_Config.SC8815_Status = SC8815_Standby;
                        Application_SC8815_Standby();
                    }
                    protect_page_ui_process(TEMP_PROTECT);
                }
            }
            temperature_protect_time = HAL_GetTick();
        }
        SC8815_Soft_Protect();

        if (SC8815_Config.SC8815_Status == SC8815_PORT)
        {
            SC8815_Config.SC8815_Status = SC8815_Standby;
            protect_page_ui_process(2);
        }

        if (APP_config.app_config_save_flag && (HAL_GetTick() - APP_config.app_config_save_time) > APP_CONFIG_SAVE_TIME) {
            app_config_save();
            APP_config.app_config_save_flag = 0;
        }

        if (SC8815_Config.sc8815_calibration_flag == 1) {
            SC8815_IBUS_calibration();
            SC8815_Config.sc8815_calibration_flag = 0;
        } else if (SC8815_Config.sc8815_calibration_flag == 2) {
            SC8815_output_calibration(1);
            SC8815_Config.sc8815_calibration_flag = 0;
            main_page_init();
        }

        // printf("temperature:%f\n", App_getTemp_V());
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
   if (HAL_GPIO_ReadPin(KEY4_GPIO_Port, KEY4_Pin) == RESET || APP_config.lock_key == 1)
       return;
   HAL_Delay(100);
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
    if (APP_config.lock_key == 1) {
        key1_press = 0;
        return;
    }
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
    if (APP_config.lock_key == 1) {
        key2_press = 0;
        key2PressStartTime = 0; // 重置计时器
        return;
    }
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
            APP_config.lock_key = 0;
            key2_press = 0;
            BUZZER_OPEN(100);
            Menu_Select_Item(KEY2_LONG);
            return;
        }
    }
}

uint32_t key3PressStartTime = 0;    //key3按下时间
uint8_t key3_press = 0; //key3是否按下标志位
/**
 *@brief 识别按键3单击并执行相应操作
 *
 */
void key3_button_process(void)
{
    if (APP_config.lock_key == 1 && current_menu_index != PRESSET_RUNNING_PAGE) {
        key3_press = 0;
        key3PressStartTime = 0; // 重置计时器
        return;
    }
    if (key3_press)
    {
        if (HAL_GPIO_ReadPin(KEY3_GPIO_Port, KEY3_Pin) == SET && APP_config.lock_key == 0)
        {
            //key3检测到单击
            key3PressStartTime = 0; // 重置计时器
            key3_press = 0;
            BUZZER_OPEN(100);
            Menu_Select_Item(KEY3_SHORT);
            return;
        }
        else if (HAL_GetTick() - key3PressStartTime >= KEY_LONG_PRESS_THRESHOLD && HAL_GPIO_ReadPin(KEY3_GPIO_Port, KEY3_Pin) == RESET)
        {
            //key3检测到长按
            key3PressStartTime = 0; // 重置计时器
            APP_config.lock_key = 0;
            key3_press = 0;
            BUZZER_OPEN(100);
            Menu_Select_Item(KEY3_LONG);
            return;
        }
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
    extern uint8_t USE_HORIZONTAL;
    if (APP_config.lock_key == 1) {
        rotary_knob_value = 0;
        return;
    }
    if (rotary_knob_value == LEFT)
    {
        if (USE_HORIZONTAL == 2) {
            Menu_Select_Item(LEFT);     //向右旋转因为旋钮因为旋钮位置改动逻辑相反了
        } else {
            Menu_Select_Item(RIGHT);    //向左旋转因为旋钮因为旋钮位置改动逻辑相反了
        }
        BUZZER_OPEN(100);
        rotary_knob_value = 0;
    }
    else if (rotary_knob_value == RIGHT)
    {
        if (USE_HORIZONTAL == 2) {
            Menu_Select_Item(RIGHT);    //向左旋转因为旋钮因为旋钮位置改动逻辑相反了
        } else {
            Menu_Select_Item(LEFT);     //向右旋转因为旋钮因为旋钮位置改动逻辑相反了
        }
        BUZZER_OPEN(100);
        rotary_knob_value = 0;
    }
}

uint32_t htonl(uint32_t hostlong)
{
    return ((hostlong & 0xFF000000) >> 24) |
           ((hostlong & 0x00FF0000) >> 8) |
           ((hostlong & 0x0000FF00) << 8) |
           ((hostlong & 0x000000FF) << 24);
}

static void app_config_clear(void)
{
    int i = 0;
    uint16_t data[128];
    size_t datalen = sizeof(data);
    memset(data, 0xFF, datalen);
    for (i = 0; i < 4;i++) {
        STMFLASH_Write(APP_CONFIG_FLASH_ADDR + datalen * i, data, datalen >> 1);
    }
}

void app_config_load(void)
{
    int i, if_config_default = 0;
    extern uint8_t USE_HORIZONTAL;
    for (i = 0; i < STM_SECTOR_SIZE;)
    {
        STMFLASH_ReadBytes(APP_CONFIG_FLASH_ADDR + i, (uint8_t*)&app_config_save_config, sizeof(Application_SaveConfig));
        
        if ((app_config_save_config.lock_buzzer != 0 && app_config_save_config.lock_buzzer != 1) || if_config_default)
        {
            if (i == 0)
            {
                uint32_t CpuID =  *(volatile uint32_t*)(0x1FFFF7E8);
                app_config_save_config.SC8815_VBUS = 5000;
                app_config_save_config.SC8815_IBUS_Limit = 1000;
				app_config_save_config.DC_IBAT_Limit = 3000;
                app_config_save_config.temperature = 65;
                app_config_save_config.lock_buzzer = 0;
                app_config_save_config.SW_FREQ = SCHWI_FREQ_150KHz;
                app_config_save_config.SC8815_SFB = SCHWI_SFB_Enable;
                app_config_save_config.SC8815_VBUS_protect = SC8815_VBUS_MAX;
                app_config_save_config.USE_HORIZONTAL = 3;
                memcpy(app_config_save_config.device_name, "PDP-", 4);
                snprintf(app_config_save_config.device_name + 4, 7, "%X", CpuID);
                app_config_save();
                break;
            }
            else
            {
                i -= sizeof(Application_SaveConfig);
                STMFLASH_ReadBytes(APP_CONFIG_FLASH_ADDR + i, (uint8_t*)&app_config_save_config, sizeof(Application_SaveConfig));
                // 校验是否为新添加数据
                if (app_config_save_config.USE_HORIZONTAL != 2 && app_config_save_config.USE_HORIZONTAL != 3) {
                    if_config_default = 1;
                    app_config_clear();
                    i = 0;
                } else {
                    break;
                }
            }
        }
        i += sizeof(Application_SaveConfig);
    }
    if (i >= STM_SECTOR_SIZE)
    {
        i -= sizeof(Application_SaveConfig);
        STMFLASH_ReadBytes(APP_CONFIG_FLASH_ADDR + i, (uint8_t*)&app_config_save_config, sizeof(Application_SaveConfig));
    }
    SC8815_Config.SC8815_IBUS_Limit = app_config_save_config.SC8815_IBUS_Limit;
    SC8815_Config.SC8815_IBUS_Limit_Old = app_config_save_config.SC8815_IBUS_Limit;
    SC8815_Config.SC8815_VBUS = app_config_save_config.SC8815_VBUS;
    SC8815_Config.SC8815_VBUS_Old = app_config_save_config.SC8815_VBUS;
    SC8815_Config.SC8815_VBUS_protect = app_config_save_config.SC8815_VBUS_protect;
    APP_config.temperature = app_config_save_config.temperature;
    APP_config.lock_buzzer = app_config_save_config.lock_buzzer;
    SC8815_HardwareInitStruct.SW_FREQ = app_config_save_config.SW_FREQ;
    SC8815_HardwareInitStruct.ShortFoldBack = app_config_save_config.SC8815_SFB;
    USE_HORIZONTAL = app_config_save_config.USE_HORIZONTAL;
    memcpy(APP_config.device_name, app_config_save_config.device_name, sizeof(APP_config.device_name));
}

void app_config_save(void)
{
    Application_SaveConfig app_config_save_config_temp;
    int i;
    for (i = 0; i < STM_SECTOR_SIZE;)
    {
        STMFLASH_ReadBytes(APP_CONFIG_FLASH_ADDR + i, (uint8_t*)&app_config_save_config_temp, sizeof(Application_SaveConfig));
        // if (app_config_save_config_temp.lock_buzzer == 0xFF)
        if (app_config_save_config_temp.lock_buzzer != 0 && app_config_save_config_temp.lock_buzzer != 1)
        {
            STMFLASH_Write(APP_CONFIG_FLASH_ADDR + i, (uint16_t*)&app_config_save_config, sizeof(Application_SaveConfig) >> 1);
            break;
        }
        i += sizeof(Application_SaveConfig);
    }
    if (i >= STM_SECTOR_SIZE)
    {
        uint16_t data[128];
        size_t datalen = sizeof(data);
        memset(data, 0xFF, datalen);
        for (i = 0; i < 4;i++) {
            STMFLASH_Write(APP_CONFIG_FLASH_ADDR + datalen * i, data, datalen >> 1);
        }
        STMFLASH_Write(APP_CONFIG_FLASH_ADDR, (uint16_t*)&app_config_save_config, sizeof(Application_SaveConfig) >> 1);
    }
}

//升级标志位，0不进入升级 1进入升级 2升级成功 3升级失败
void set_app_upgrade_flag(uint16_t flag)
{
    STMFLASH_Write(APP_UPGRADE_FLAG_ADDR, &flag, 1);
}

uint32_t GetCPU_ID(void)
{
    uint32_t CpuID[3];							//小端模式
    CpuID[0] = *(uint32_t*)(0x1ffff7e8); //高32位地址
    CpuID[1] = *(uint32_t*)(0x1ffff7ec); //中32位地址
    CpuID[2] = *(uint32_t*)(0x1ffff7f0); //低32位地址
    return CpuID[0] ^ CpuID[1] ^ CpuID[2];
}
