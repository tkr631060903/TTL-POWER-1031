#include "menu_ui.h"
#include "Application_LCD.h"
#include "Application.h"
#include "pic.h"
#include "string.h"
#include "Application_SC8815.h"
#include <math.h>
#include "husb238.h"

typedef struct
{
    uint8_t index;
    const unsigned char* icon;
    char menu_name[16];
}Menu_NameTypeDef;

extern menu_i32 current_menu_index;
static menu_u8 cursor_main_menu = 0; //主菜单光标位置
static menu_u8 cursor_secondary_menu = 0; //二级菜单光标位置
static uint8_t secondary_menu_index = 0; //二级菜单第一行显示索引
static uint8_t main_menu_index = 0; //主菜单第一行显示索引
#ifdef ENABLE_EN_FONT
static Menu_NameTypeDef menu_name[] = {
    {0, gImage_presset, "PresetConfig"}, {1, gImage_start_presset, "EnablePreset"}, {2, gImage_temp, "OTP"}, {3, gImage_buzzer, "BuzzerConfig"}, {4, gImage_vbus_protect, "OVP"}, {5, gImage_screen, "RotateScreen"}, {6, gImage_about, "About"}
};
#else
static Menu_NameTypeDef menu_name[] = {
    {0, gImage_presset, "预设配置"}, {1, gImage_start_presset, "开启预设"}, {2, gImage_temp, "过温保护"}, {3, gImage_buzzer, "蜂鸣器配置"}, {4, gImage_vbus_protect, "过压保护"}, {5, gImage_screen, "翻转屏幕"}, {6, gImage_about, "关于"}
};
#endif

/**
 * @brief 进入相应的页面
 * 
 * @param index 子页面索引
 * @param KeyValue 触发键值
 */
void Enter_Page(menu_i32 index, menu_u8 KeyValue)
{
    sub_index.Current_Page = index;
    switch (sub_index.Current_Page)
    {
    case 0:
        sub_index.presset_current_index = 0;
        cursor_secondary_menu = 0;
        secondary_menu_index = 0;
        current_menu_index = PRESSET_PAGE;
        presset_page_ui_process(0, KeyValue);
        break;
    case 1:
        sub_index.presset_current_index = 0;
        cursor_secondary_menu = 0;
        secondary_menu_index = 0;
        current_menu_index = PRESSET_START_PAGE;
        presset_start_page_ui_process(0, KeyValue);
        break;
    case 2:
        LCD_Clear();
        SC8815_Config.SC8815_VBUS_IBUS_Step = 1;
        temperature_page_ui_process(APP_config.temperature);
        break;
    case 3:
        LCD_Clear();
        buzzer_page_ui_process(APP_config.lock_buzzer);
        break;
    case 4:
        LCD_Clear();
        SC8815_Config.SC8815_VBUS_IBUS_Step = 1000;
        vbus_protect_page_ui_process(sub_index.VBUS_calibration_current_index);
        break;
    case 5:
        LCD_Clear();
        screen_flip_page_process(0);
        break;
    case 6:
        about_page_ui_process();
        break;
    default:
        break;
    }
}

/**
 * @brief 主页面初始化
 * 
 */
void main_page_init(void)
{
    current_menu_index = MAIN_PAGE;
    LCD_Clear();
    APP_LCD_main_init();
    APP_LCD_main_show();
}

/**
 * @brief 主菜单初始化
 * 
 */
void main_menu_Init(void)
{
    sub_index.main_menu_current_index = 0;
    sub_index.Current_Page = MAIN_MENU_PAGE;
    current_menu_index = MAIN_MENU_PAGE;
    cursor_main_menu = 0;
    main_menu_index = 0;
    //.....刷新回主页面的UI和状态
    LCD_Clear();
    LCD_on_menu_line(0, (uint8_t*)gImage_presset, (uint8_t*)menu_name[0].menu_name);
    LCD_off_menu_line(1, (uint8_t*)gImage_start_presset, (uint8_t*)menu_name[1].menu_name);
    LCD_off_menu_line(2, (uint8_t*)gImage_temp, (uint8_t*)menu_name[2].menu_name);
    LCD_off_menu_line(3, (uint8_t*)gImage_buzzer, (uint8_t*)menu_name[3].menu_name);
}

// 主菜单页面UI处理
void main_menu_page_ui_process(menu_u8 index, menu_u8 KeyValue)
{
    current_menu_index = MAIN_MENU_PAGE;
    uint8_t index_move_flag = 0;
    menu_u8 cursor_temp = cursor_main_menu;
    if (cursor_main_menu == 3 && KeyValue == RIGHT && main_menu_index != (MENU_PAGE_NUM - 4))
    {
        main_menu_index++;
        index_move_flag = 1;
    }
    else if (cursor_main_menu == 0 && KeyValue == LEFT && main_menu_index != 0)
    {
        main_menu_index--;
        index_move_flag = 1;
    }
    if(cursor_main_menu != 3 && KeyValue == RIGHT)
    {
        cursor_main_menu++;
    }
    else if (cursor_main_menu != 0 && KeyValue == LEFT)
    {
        cursor_main_menu--;
    }

    if (index_move_flag == 0 && KeyValue == KEY3_SHORT) {
        LCD_Clear();
    }
    if (index_move_flag || KeyValue == KEY3_SHORT) {
        for (size_t i = 0; i < 4; i++) {
            if (i == cursor_main_menu) {
                LCD_on_menu_line(i, (uint8_t*)menu_name[main_menu_index + i].icon, (uint8_t*)menu_name[main_menu_index + i].menu_name);
            } else {
                LCD_off_menu_line(i, (uint8_t*)menu_name[main_menu_index + i].icon, (uint8_t*)menu_name[main_menu_index + i].menu_name);
            }
        }
    } else {
        LCD_off_menu_line(cursor_temp, (uint8_t*)menu_name[main_menu_index + cursor_temp].icon, (uint8_t*)menu_name[main_menu_index + cursor_temp].menu_name);
        LCD_on_menu_line(cursor_main_menu, (uint8_t*)menu_name[main_menu_index + cursor_main_menu].icon, (uint8_t*)menu_name[main_menu_index + cursor_main_menu].menu_name);
    }
}

void vout_page_ui_process(menu_u8 KeyValue)
{
    current_menu_index = VOUT_PAGE;
    int number = 0, length = 0, digits[5] = {0};
    switch (KeyValue)
    {
    case LEFT:
    case RIGHT:
    case KEY1_SHORT:
    case KEY2_SHORT:
        LCD_show_vset();
        if (SC8815_Config.SC8815_VBUS >= SC8815_Config.SC8815_VBUS_IBUS_Step) {
            // Take the tens, thousands, and hundreds values and store them in variate digits
            number = SC8815_Config.SC8815_VBUS;
            while (number > 0) {
                number /= 10;
                length++;
            }
            number = SC8815_Config.SC8815_VBUS;
            for (int i = length - 1; i >= 0; i--) {
                digits[i] = number / (int)pow(10, i);
                number %= (int)pow(10, i);
            }
        }
        if (SC8815_Config.SC8815_VBUS_IBUS_Step == 100) {
            LCD_ShowIntNum(36, 16, digits[2], 1, BLACK, WHITE, 16);
        } else if (SC8815_Config.SC8815_VBUS_IBUS_Step == 1000) {
            LCD_ShowIntNum(20, 16, digits[3], 1, BLACK, WHITE, 16);
        } else if (SC8815_Config.SC8815_VBUS_IBUS_Step == 10000) {
            LCD_ShowIntNum(12, 16, digits[4], 1, BLACK, WHITE, 16);
        }
        break;
    default:
        break;
    }
}

void iout_page_ui_process(menu_u8 KeyValue)
{
    current_menu_index = IOUT_PAGE;
    int number = 0, length = 0, digits[5] = {0};
    switch (KeyValue)
    {
    case LEFT:
    case RIGHT:
    case KEY1_SHORT:
    case KEY2_SHORT:
        LCD_show_iset();
        if (SC8815_Config.SC8815_IBUS_Limit >= SC8815_Config.SC8815_VBUS_IBUS_Step) {
            // Take the tens, thousands, and hundreds values and store them in variate digits
            number = SC8815_Config.SC8815_IBUS_Limit;
            while (number > 0) {
                number /= 10;
                length++;
            }
            number = SC8815_Config.SC8815_IBUS_Limit;
            for (int i = length - 1; i >= 0; i--) {
                digits[i] = number / (int)pow(10, i);
                number %= (int)pow(10, i);
            }
        }
        if (SC8815_Config.SC8815_VBUS_IBUS_Step == 100) {
            LCD_ShowIntNum(36, 50, digits[2], 1, BLACK, WHITE, 16);
        } else if (SC8815_Config.SC8815_VBUS_IBUS_Step == 1000) {
            LCD_ShowIntNum(20, 50, digits[3], 1, BLACK, WHITE, 16);
        } else if (SC8815_Config.SC8815_VBUS_IBUS_Step == 10000) {
            LCD_ShowIntNum(12, 50, digits[4], 1, BLACK, WHITE, 16);
        }
        break;
    default:
        break;
    }
}

void presset_page_ui_process(menu_u8 index, menu_u8 KeyValue)
{
    char presset_str[10] = {0};
    uint8_t index_move_flag = 0;
    menu_u8 cursor_temp = cursor_secondary_menu;
    if (cursor_secondary_menu == 3 && KeyValue == RIGHT && secondary_menu_index != (SC8815_TIM_WORK_SIZE - 4))
    {
        secondary_menu_index++;
        index_move_flag = 1;
    }
    else if (cursor_secondary_menu == 0 && KeyValue == LEFT && secondary_menu_index != 0)
    {
        secondary_menu_index--;
        index_move_flag = 1;
    }
    if(cursor_secondary_menu != 3 && KeyValue == RIGHT)
    {
        cursor_secondary_menu++;
    }
    else if (cursor_secondary_menu != 0 && KeyValue == LEFT)
    {
        cursor_secondary_menu--;
    }
    
    if (index_move_flag == 0 && (KeyValue == KEY3_SHORT || KeyValue == KEY4_SHORT)) {
        LCD_Clear();
    }
    if (index_move_flag || KeyValue == KEY3_SHORT || KeyValue == KEY4_SHORT) {
        for (size_t i = 0; i < 4; i++) {
#ifdef ENABLE_EN_FONT
            sprintf(presset_str, "Preset:%d", secondary_menu_index + i);
#else
            sprintf(presset_str, "预设:%d", secondary_menu_index + i);
#endif
            if (i == cursor_secondary_menu) {
                LCD_on_menu_line(i, (uint8_t*)gImage_presset, (uint8_t*)presset_str);
            } else {
                LCD_off_menu_line(i, (uint8_t*)gImage_presset, (uint8_t*)presset_str);
            }
        }
    } else {
#ifdef ENABLE_EN_FONT
        sprintf(presset_str, "Preset:%d", secondary_menu_index + cursor_temp);
        LCD_off_menu_line(cursor_temp, (uint8_t*)gImage_presset, (uint8_t*)presset_str);
        sprintf(presset_str, "Preset:%d", secondary_menu_index + cursor_secondary_menu);
        LCD_on_menu_line(cursor_secondary_menu, (uint8_t*)gImage_presset, (uint8_t*)presset_str);
#else
        sprintf(presset_str, "预设:%d", secondary_menu_index + cursor_temp);
        LCD_off_menu_line(cursor_temp, (uint8_t*)gImage_presset, (uint8_t*)presset_str);
        sprintf(presset_str, "预设:%d", secondary_menu_index + cursor_secondary_menu);
        LCD_on_menu_line(cursor_secondary_menu, (uint8_t*)gImage_presset, (uint8_t*)presset_str);
#endif
        
    }
}

void presset_start_page_ui_process(menu_u8 index, menu_u8 KeyValue)
{
    char presset_str[10] = {0};
    uint8_t index_move_flag = 0;
    menu_u8 cursor_temp = cursor_secondary_menu;
    if (cursor_secondary_menu == 3 && KeyValue == RIGHT && secondary_menu_index != (SC8815_TIM_WORK_SIZE - 4))
    {
        secondary_menu_index++;
        index_move_flag = 1;
    }
    else if (cursor_secondary_menu == 0 && KeyValue == LEFT && secondary_menu_index != 0)
    {
        secondary_menu_index--;
        index_move_flag = 1;
    }
    if(cursor_secondary_menu != 3 && KeyValue == RIGHT)
    {
        cursor_secondary_menu++;
    }
    else if (cursor_secondary_menu != 0 && KeyValue == LEFT)
    {
        cursor_secondary_menu--;
    }
    
    if (index_move_flag == 0 && (KeyValue == KEY3_SHORT || KeyValue == KEY4_SHORT)) {
        LCD_Clear();
    }
    if (index_move_flag || KeyValue == KEY3_SHORT || KeyValue == KEY4_SHORT) {
        for (size_t i = 0; i < 4; i++) {
#ifdef ENABLE_EN_FONT
            sprintf(presset_str, "Preset:%d", secondary_menu_index + i);
#else
            sprintf(presset_str, "预设:%d", secondary_menu_index + i);
#endif            
            if (i == cursor_secondary_menu) {
                LCD_on_menu_line(i, (uint8_t*)gImage_start_presset, (uint8_t*)presset_str);
            } else {
                LCD_off_menu_line(i, (uint8_t*)gImage_start_presset, (uint8_t*)presset_str);
            }
        }
    } else {
#ifdef ENABLE_EN_FONT
        sprintf(presset_str, "Preset:%d", secondary_menu_index + cursor_temp);
        LCD_off_menu_line(cursor_temp, (uint8_t*)gImage_start_presset, (uint8_t*)presset_str);
        sprintf(presset_str, "Preset:%d", secondary_menu_index + cursor_secondary_menu);
        LCD_on_menu_line(cursor_secondary_menu, (uint8_t*)gImage_start_presset, (uint8_t*)presset_str);
#else
        sprintf(presset_str, "预设:%d", secondary_menu_index + cursor_temp);
        LCD_off_menu_line(cursor_temp, (uint8_t*)gImage_start_presset, (uint8_t*)presset_str);
        sprintf(presset_str, "预设:%d", secondary_menu_index + cursor_secondary_menu);
        LCD_on_menu_line(cursor_secondary_menu, (uint8_t*)gImage_start_presset, (uint8_t*)presset_str);
#endif        
    }
}

void presset_config_page_ui_process(menu_u8 index)
{
    current_menu_index = PRESSET_CONFIG_PAGE;
    extern SC8815_TIM_WorkTypeDef SC8815_TIM_Work[SC8815_TIM_WORK_SIZE];
    char str[10];
    if (SC8815_TIM_Work[sub_index.presset_current_index].SC8815_VBUS[index] >= 0 && SC8815_TIM_Work[sub_index.presset_current_index].SC8815_VBUS[index] < 10000) {
        sprintf(str, "Vset:0%.2fV ", SC8815_TIM_Work[sub_index.presset_current_index].SC8815_VBUS[index] / 1000);
    } else {
        sprintf(str, "Vset:%.2fV ", SC8815_TIM_Work[sub_index.presset_current_index].SC8815_VBUS[index] / 1000);
    }
    LCD_Fill_DMA(80, 0, LCD_W, 32, RED);
    LCD_ShowString(0, 0, (uint8_t*)str, WHITE, RED, 32, 0);
    memset(str, 0, 10);
    sprintf(str, "Iset:%.2fA", SC8815_TIM_Work[sub_index.presset_current_index].SC8815_IBUS_Limit[index] / 1000);
    LCD_Fill_DMA(80, 34, LCD_W, 66, RED);
    LCD_ShowString(0, 34, (uint8_t*)str, WHITE, RED, 32, 0);
    memset(str, 0, 10);
    sprintf(str, "Time:%ds", SC8815_TIM_Work[sub_index.presset_current_index].SC8815_TIM_Work_second[index]);
    LCD_Fill_DMA(80, 68, LCD_W, 100, RED);
    LCD_ShowString(0, 68, (uint8_t*)str, WHITE, RED, 32, 0);
    memset(str, 0, 10);
    sprintf(str, "Loop:%d", SC8815_TIM_Work[sub_index.presset_current_index].circular);
    LCD_Fill_DMA(80, 102, LCD_W, 135, RED);
    LCD_ShowString(0, 102, (uint8_t*)str, WHITE, RED, 32, 0);
    memset(str, 0, 10);
    sprintf(str, "Step:%d/29", index);
    LCD_ShowString(160, 118, (uint8_t*)str, WHITE, RED, 16, 0);
}

void presset_config_set_page_ui_process(menu_u8 KeyValue)
{
    current_menu_index = PRESSET_CONFIG_SET_PAGE;
    extern presset_config_set_typeDef presset_config_set;
    switch (KeyValue)
    {
    case LEFT:
    case RIGHT:
    case KEY1_SHORT:
    case KEY2_SHORT:
    case KEY3_SHORT:
        presset_config_set_page_show();
        break;
    case KEY4_SHORT:
        switch (presset_config_set.set_flag)
        {
        case PRESSET_SET_VOUT:
        case PRESSET_SET_IOUT:
        case PRESSET_SET_TIME:
            presset_config_set_page_show();
            break;
        case PRESSET_SET_CIRCULAR:
            presset_page_ui_process(0, KeyValue);
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}

void buzzer_page_ui_process(menu_u8 index)
{
    current_menu_index = BUZZER_PAGE;
    switch (index)
    {
    case 0:
#ifdef ENABLE_EN_FONT
        LCD_on_menu_line(0, (uint8_t*)gImage_buzzer, "OpenBuzzer");
        LCD_off_menu_line(1, (uint8_t*)gImage_buzzer, "CloseBuzzer");
#else
        LCD_on_menu_line(0, (uint8_t*)gImage_buzzer, "开启蜂鸣器");
        LCD_off_menu_line(1, (uint8_t*)gImage_buzzer, "关闭蜂鸣器");
#endif        
        break;
    case 1:
#ifdef ENABLE_EN_FONT
        LCD_off_menu_line(0, (uint8_t*)gImage_buzzer, "OpenBuzzer");
        LCD_on_menu_line(1, (uint8_t*)gImage_buzzer, "CloseBuzzer");
#else
        LCD_off_menu_line(0, (uint8_t*)gImage_buzzer, "开启蜂鸣器");
        LCD_on_menu_line(1, (uint8_t*)gImage_buzzer, "关闭蜂鸣器");
#endif
        break;
    default:
        break;
    }
}

void temperature_page_ui_process(float index)
{
    char str[10] = {0};
    int number = 0, length = 0, digits[2] = {0}, show_x = 144;
    current_menu_index = TEMPERATURE_PAGE;
#ifdef ENABLE_EN_FONT
    show_x = 104;
    sprintf(str, "TEMP:%.0fC", index);
#else
    sprintf(str, "温度:%.0fC", index);
#endif    
    LCD_on_menu_line(0, (uint8_t*)gImage_temp, (uint8_t*)str);
    if (index >= SC8815_Config.SC8815_VBUS_IBUS_Step) {
        // Take the tens, thousands, and hundreds values and store them in variate digits
        number = index;
        while (number > 0) {
            number /= 10;
            length++;
        }
        number = index;
        for (int i = length - 1; i >= 0; i--) {
            digits[i] = number / (int)pow(10, i);
            number %= (int)pow(10, i);
        }
    }
    if (SC8815_Config.SC8815_VBUS_IBUS_Step == 1) {
        LCD_ShowIntNum(show_x + 16 * 2, 0, digits[0], 1, RED, WHITE, 32);
    } else if (SC8815_Config.SC8815_VBUS_IBUS_Step == 10) {
        LCD_ShowIntNum(show_x + 16, 0, digits[1], 1, RED, WHITE, 32);
    }
}


void VBUS_calibration_page_ui_process(menu_u8 index)
{
    current_menu_index = VBUS_CALIBRATION_PAGE;
    switch (index)
    {
    case 0:
        // LCD_ShowChinese(60, 32, "开启校准", WHITE, BLACK, 32, 0);
        LCD_ShowString(60, 64, "Yes", BLACK, WHITE, 32, 0);
        LCD_ShowString(156, 64, "No", WHITE, BLACK, 32, 0);
        break;
    case 1:
        LCD_ShowString(60, 64, "Yes", WHITE, BLACK, 32, 0);
        LCD_ShowString(156, 64, "No", BLACK, WHITE, 32, 0);
        break;
    default:
        break;
    }
}

void protect_page_ui_process(menu_u8 index)
{
    current_menu_index = PROTECT_PAGE;
    LCD_Fill_DMA(0, 102, LCD_W, 135, WARNING_YELLOW);
    LCD_show_on_off(0, 102, 16, 16, gImage_left1, WARNING_YELLOW);
    LCD_show_on_off(LCD_W - 16, 102, 16, 16, gImage_right1, WARNING_YELLOW);
    switch (index)
    {
    case VBUS_PROTECT:
#ifdef ENABLE_EN_FONT
        LCD_ShowString(88, 102, "OCP", BLACK, WARNING_YELLOW, 32, 0);
#else
        LCD_ShowChinese(60, 102, "过流保护", BLACK, WARNING_YELLOW, 32, 0);
#endif
        break;
    case TEMP_PROTECT:
#ifdef ENABLE_EN_FONT
        LCD_ShowString(88, 102, "OTP", BLACK, WARNING_YELLOW, 32, 0);
#else
        LCD_ShowChinese(60, 102, "过温保护", BLACK, WARNING_YELLOW, 32, 0);
#endif        
        break;
    case VBAT_PROTECT:
#ifdef ENABLE_EN_FONT
        LCD_ShowString(36, 102, "Input", BLACK, WARNING_YELLOW, 32, 0);
#else
        LCD_ShowChinese(20, 102, "输入电压", BLACK, WARNING_YELLOW, 32, 0);
#endif        
        LCD_ShowString(148, 102, ">25.5V", BLACK, WARNING_YELLOW, 32, 0);
        break;
    case PRESSET_PROTECT:
#ifdef ENABLE_EN_FONT
        LCD_ShowString(36, 102, "Input", BLACK, WARNING_YELLOW, 32, 0);
#else
        LCD_ShowChinese(20, 102, "输入电压", BLACK, WARNING_YELLOW, 32, 0);
#endif        
        LCD_ShowString(148, 102, "<=9V", BLACK, WARNING_YELLOW, 32, 0);
        break;
    default:
        break;
    }
}

void about_page_ui_process(void)
{
    current_menu_index = ABOUT_PAGE;
    char name[sizeof(APP_config.device_name) + 1] = ":";
    LCD_Clear();
    LCD_Fill_DMA(0, 32, LCD_W, 34, GRAY2);
    LCD_Fill_DMA(0, 66, LCD_W, 68, GRAY2);
    LCD_Fill_DMA(0, 100, LCD_W, 102, GRAY2);
    LCD_Fill_DMA(0, 0, LCD_W, 32, RED);
    LCD_Fill_DMA(0, 34, LCD_W, 66, RED);
    LCD_Fill_DMA(0, 68, LCD_W, 100, RED);
    LCD_Fill_DMA(0, 102, LCD_W, 135, RED);
#ifdef ENABLE_EN_FONT
    LCD_ShowString(0, 0, "Name", WHITE, RED, 32, 0);
#else
    LCD_ShowChinese(0, 0, "名称", WHITE, RED, 32, 0);
#endif
    strcat(name, APP_config.device_name);
    LCD_ShowString(64, 0, (uint8_t*)name, WHITE, RED, 32, 0);
    
#ifdef ENABLE_EN_FONT
    LCD_ShowString(0, 34, "Type", WHITE, RED, 32, 0);
    LCD_ShowString(0, 68, "Version", WHITE, RED, 32, 0);
    LCD_ShowString(112, 68, ":1.1.3", WHITE, RED, 32, 0);
#else
    LCD_ShowChinese(0, 34, "型号", WHITE, RED, 32, 0);
    LCD_ShowChinese(0, 68, "版本", WHITE, RED, 32, 0);
    LCD_ShowString(64, 68, ":1.1.3", WHITE, RED, 32, 0);
#endif    
    LCD_ShowString(64, 34, ":PD POCKET", WHITE, RED, 32, 0);
}

void DC_limit_page_ui_process(menu_u8 KeyValue)
{
    current_menu_index = DC_LIMIT_PAGE;
    float temp = 0;
    int number = 0, length = 0, digits[5] = {0};
    char str[10] = {0};
    switch (KeyValue)
    {
    case LEFT:
    case RIGHT:
    case KEY1_SHORT:
    case KEY2_SHORT:
        temp = app_config_save_config.DC_IBAT_Limit / 1000;
        sprintf(str, "%.2fA", temp);
        if (temp < 10) {
            LCD_ShowString(70, 80, (const uint8_t*)str, WHITE, BLACK, 32, 0);
        } else {
            LCD_ShowString(36, 80, (const uint8_t*)str, WHITE, BLACK, 32, 0);
        }
        if (app_config_save_config.DC_IBAT_Limit >= SC8815_Config.SC8815_VBUS_IBUS_Step) {
            // Take the tens, thousands, and hundreds values and store them in variate digits
            number = app_config_save_config.DC_IBAT_Limit;
            while (number > 0) {
                number /= 10;
                length++;
            }
            number = app_config_save_config.DC_IBAT_Limit;
            for (int i = length - 1; i >= 0; i--) {
                digits[i] = number / (int)pow(10, i);
                number %= (int)pow(10, i);
            }
        }
        if (SC8815_Config.SC8815_VBUS_IBUS_Step == 10) {
            LCD_ShowIntNum(118, 80, digits[1], 1, BLACK, WHITE, 32);
        } else if (SC8815_Config.SC8815_VBUS_IBUS_Step == 100) {
            LCD_ShowIntNum(102, 80, digits[2], 1, BLACK, WHITE, 32);
        } else if (SC8815_Config.SC8815_VBUS_IBUS_Step == 1000) {
            LCD_ShowIntNum(70, 80, digits[3], 1, BLACK, WHITE, 32);
        }
        break;
    default:
        break;
    }
}

void vbus_protect_page_ui_process(float index)
{
    char str[10] = {0};
    int number = 0, length = 0, digits[5] = {0}, show_x = 144;
    current_menu_index = VBUS_PROTECT_PAGE;
#ifdef ENABLE_EN_FONT
    show_x = 120;
#endif
    if (SC8815_Config.SC8815_VBUS_protect < 10000)
#ifdef ENABLE_EN_FONT
        sprintf(str, "Volts:0%.1fV", SC8815_Config.SC8815_VBUS_protect / 1000);
#else
        sprintf(str, "电压:0%.1fV", SC8815_Config.SC8815_VBUS_protect / 1000);
#endif
    else
#ifdef ENABLE_EN_FONT
        sprintf(str, "Volts:%.1fV", SC8815_Config.SC8815_VBUS_protect / 1000);
#else
        sprintf(str, "电压:%.1fV", SC8815_Config.SC8815_VBUS_protect / 1000);
#endif        
    LCD_on_menu_line(0, (uint8_t*)gImage_vbus_protect, (uint8_t*)str);
    if (SC8815_Config.SC8815_VBUS_protect >= SC8815_Config.SC8815_VBUS_IBUS_Step) {
        // Take the tens, thousands, and hundreds values and store them in variate digits
        number = SC8815_Config.SC8815_VBUS_protect;
        while (number > 0) {
            number /= 10;
            length++;
        }
        number = SC8815_Config.SC8815_VBUS_protect;
        for (int i = length - 1; i >= 0; i--) {
            digits[i] = number / (int)pow(10, i);
            number %= (int)pow(10, i);
        }
    }
    if (SC8815_Config.SC8815_VBUS_IBUS_Step == 100) {
        LCD_ShowIntNum(show_x + 16 * 4, 0, digits[2], 1, RED, WHITE, 32);
    } else if (SC8815_Config.SC8815_VBUS_IBUS_Step == 1000) {
        LCD_ShowIntNum(show_x + 16 * 2, 0, digits[3], 1, RED, WHITE, 32);
    } else if (SC8815_Config.SC8815_VBUS_IBUS_Step == 10000) {
        LCD_ShowIntNum(show_x + 16, 0, digits[4], 1, RED, WHITE, 32);
    }
}
