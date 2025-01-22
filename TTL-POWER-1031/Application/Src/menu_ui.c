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
// static uint8_t presset_show_vlaue[] = {0, 1, 2, 3};   //当前显示的预设值
static uint8_t secondary_menu_index = 0; //二级菜单第一行显示索引
static uint8_t main_menu_index = 0; //主菜单第一行显示索引
static Menu_NameTypeDef menu_name[] = {
    {0, gImage_presset, "预设配置"}, {1, gImage_start_presset, "开启预设"}, {2, gImage_temp, "过温保护"}, {3, gImage_PDinput, "快充输入"}, {4, gImage_buzzer, "蜂鸣器"}, {5, gImage_calibration, "校准电压"}, {6, gImage_about, "关于"}
};

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
        // presset_show_vlaue[0] = 0;
        // presset_show_vlaue[1] = 1;
        // presset_show_vlaue[2] = 2;
        // presset_show_vlaue[3] = 3;
        current_menu_index = PRESSET_PAGE;
        presset_page_ui_process(0, KeyValue);
        break;
    case 1:
        sub_index.presset_current_index = 0;
        cursor_secondary_menu = 0;
        secondary_menu_index = 0;
        // presset_show_vlaue[0] = 0;
        // presset_show_vlaue[1] = 1;
        // presset_show_vlaue[2] = 2;
        // presset_show_vlaue[3] = 3;
        current_menu_index = PRESSET_START_PAGE;
        presset_page_ui_process(0, KeyValue);
        break;
    case 2:
        LCD_Clear();
        temperature_page_ui_process(APP_config.temperature);
        break;
    case 3:
        cursor_secondary_menu = 0;
        secondary_menu_index = 0;
        LCD_Clear();
        fastch_page_ui_process(KeyValue, APP_config.fastCharge_InVoltage);
        break;
    case 4:
        LCD_Clear();
        buzzer_page_ui_process(APP_config.lock_buzzer);
        break;
    case 5:
        sub_index.VBUS_calibration_current_index = 0;
        LCD_Clear();
        VBUS_calibration_page_ui_process(sub_index.VBUS_calibration_current_index);
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
    LCD_ShowChinese(40, 0, "预设配置", BLACK, LIGHTBLUE, 32, 0);
    LCD_ShowPicture(0, 0, 32, 32, gImage_presset);
    LCD_ShowChinese(40, 33, "开启预设", LIGHTBLUE, BLACK, 32, 0);
    LCD_ShowPicture(0, 33, 32, 32, gImage_start_presset);
    LCD_ShowChinese(40, 66, "过温保护", LIGHTBLUE, BLACK, 32, 0);
    LCD_ShowPicture(0, 66, 32, 32, gImage_temp);
    LCD_ShowChinese(40, 99, "快充输入", LIGHTBLUE, BLACK, 32, 0);
    LCD_ShowPicture(0, 99, 32, 32, gImage_PDinput);
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
            LCD_RowClear(i, 32, BLACK);
            LCD_ShowPicture(0, i * 33, 32, 32, (uint8_t*)menu_name[main_menu_index + i].icon);
            if (i == cursor_main_menu) {
                LCD_ShowChinese(40, i * 33, (uint8_t*)menu_name[main_menu_index + i].menu_name, BLACK, LIGHTBLUE, 32, 0);
            } else {
                LCD_ShowChinese(40, i * 33, (uint8_t*)menu_name[main_menu_index + i].menu_name, LIGHTBLUE, BLACK, 32, 0);
            }
        }
    } else {
	    LCD_Fill_DMA(32, cursor_temp * 32, LCD_W, (cursor_temp + 1) * 32, BLACK);
        LCD_ShowChinese(40, cursor_temp * 33, (uint8_t*)menu_name[main_menu_index + cursor_temp].menu_name, LIGHTBLUE, BLACK, 32, 0);
	    LCD_Fill_DMA(32, cursor_main_menu * 32, LCD_W, (cursor_main_menu + 1) * 32, BLACK);
        LCD_ShowChinese(40, cursor_main_menu * 33, (uint8_t*)menu_name[main_menu_index + cursor_main_menu].menu_name, BLACK, LIGHTBLUE, 32, 0);
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
            LCD_ShowIntNum(16 * 2, 16, digits[2], 1, BLACK, LIGHTBLUE, 16);
        } else if (SC8815_Config.SC8815_VBUS_IBUS_Step == 1000) {
            LCD_ShowIntNum(16, 16, digits[3], 1, BLACK, LIGHTBLUE, 16);
        } else if (SC8815_Config.SC8815_VBUS_IBUS_Step == 10000) {
            LCD_ShowIntNum(7, 16, digits[4], 1, BLACK, LIGHTBLUE, 16);
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
            LCD_ShowIntNum(15 * 2, 51, digits[2], 1, BLACK, LIGHTBLUE, 16);
        } else if (SC8815_Config.SC8815_VBUS_IBUS_Step == 1000) {
            LCD_ShowIntNum(15, 51, digits[3], 1, BLACK, LIGHTBLUE, 16);
        } else if (SC8815_Config.SC8815_VBUS_IBUS_Step == 10000) {
            LCD_ShowIntNum(7, 51, digits[4], 1, BLACK, LIGHTBLUE, 16);
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
            sprintf(presset_str, ":%d", secondary_menu_index + i);
            if (i == cursor_secondary_menu) {
                LCD_ShowChinese(0, i * 33, "预设", BLACK, LIGHTBLUE, 32, 0);
                LCD_ShowString(64, i * 33, (uint8_t *)presset_str, BLACK, LIGHTBLUE, 32, 0);
            } else {
                LCD_ShowChinese(0, i * 33, "预设", LIGHTBLUE, BLACK, 32, 0);
                LCD_ShowString(64, i * 33, (uint8_t *)presset_str, LIGHTBLUE, BLACK, 32, 0);
            }
        }
    } else {
        sprintf(presset_str, ":%d", secondary_menu_index + cursor_temp);
        LCD_ShowChinese(0, cursor_temp * 33, "预设", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowString(64, cursor_temp * 33, (uint8_t*)presset_str, LIGHTBLUE, BLACK, 32, 0);
        sprintf(presset_str, ":%d", secondary_menu_index + cursor_secondary_menu);
        LCD_ShowChinese(0, cursor_secondary_menu * 33, "预设", BLACK, LIGHTBLUE, 32, 0);
        LCD_ShowString(64, cursor_secondary_menu * 33, (uint8_t *)presset_str, BLACK, LIGHTBLUE, 32, 0);
    }
}

void presset_config_page_ui_process(menu_u8 index)
{
    current_menu_index = PRESSET_CONFIG_PAGE;
    extern SC8815_TIM_WorkTypeDef SC8815_TIM_Work[SC8815_TIM_WORK_SIZE];
    char str[10];
    sprintf(str, "vset:%.2fv", SC8815_TIM_Work[sub_index.presset_current_index].SC8815_VBUS[index] / 1000);
	LCD_Fill_DMA(80, 0 * 32, LCD_W, (0 + 1) * 32, BLACK);
    LCD_ShowString(0, 0, (uint8_t*)str, LIGHTBLUE, BLACK, 32, 0);
    memset(str, 0, 10);
    sprintf(str, "iset:%.2fA", SC8815_TIM_Work[sub_index.presset_current_index].SC8815_IBUS_Limit[index] / 1000);
	LCD_Fill_DMA(80, 1 * 32, LCD_W, (1 + 1) * 32, BLACK);
    LCD_ShowString(0, 33, (uint8_t *)str, LIGHTBLUE, BLACK, 32, 0);
    memset(str, 0, 10);
    sprintf(str, "time:%ds", SC8815_TIM_Work[sub_index.presset_current_index].SC8815_TIM_Work_second[index]);
	LCD_Fill_DMA(80, 2 * 32, LCD_W, (2 + 1) * 32, BLACK);
    LCD_ShowString(0, 66, (uint8_t *)str, LIGHTBLUE, BLACK, 32, 0);
    memset(str, 0, 10);
    sprintf(str, "loop:%d", SC8815_TIM_Work[sub_index.presset_current_index].circular);
	LCD_Fill_DMA(32, 3 * 32, 100, (3 + 1) * 32, BLACK);
    LCD_ShowString(0, 99, (uint8_t *)str, LIGHTBLUE, BLACK, 16, 0);
    memset(str, 0, 10);
    sprintf(str, "step:%d/29", index);
    LCD_ShowString(100, 99, (uint8_t *)str, LIGHTBLUE, BLACK, 16, 0);
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
        LCD_ShowChinese(0, 0, "开启蜂鸣器", BLACK, LIGHTBLUE, 32, 0);
        LCD_ShowChinese(0, 33, "关闭蜂鸣器", LIGHTBLUE, BLACK, 32, 0);
        break;
    case 1:
        LCD_ShowChinese(0, 0, "开启蜂鸣器", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowChinese(0, 33, "关闭蜂鸣器", BLACK, LIGHTBLUE, 32, 0);
        break;
    default:
        break;
    }
}

void temperature_page_ui_process(float index)
{
    current_menu_index = TEMPERATURE_PAGE;
    sub_index.temperature_current_index = index;
    if (index == TEMPERATURE_65)
    {
        LCD_ShowChinese(0, 0, "过温保护", BLACK, LIGHTBLUE, 32, 0);
        LCD_ShowString(128, 0, ":50", BLACK, LIGHTBLUE, 32, 0);
        LCD_ShowChinese(0, 33, "过温保护", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowString(128, 33, ":60", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowChinese(0, 66, "过温保护", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowString(128, 66, ":70", LIGHTBLUE, BLACK, 32, 0);
    }
    else if (index == TEMPERATURE_75)
    {
        LCD_ShowChinese(0, 0, "过温保护", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowString(128, 0, ":50", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowChinese(0, 33, "过温保护", BLACK, LIGHTBLUE, 32, 0);
        LCD_ShowString(128, 33, ":60", BLACK, LIGHTBLUE, 32, 0);
        LCD_ShowChinese(0, 66, "过温保护", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowString(128, 66, ":70", LIGHTBLUE, BLACK, 32, 0);
    }
    else if (index == TEMPERATURE_85)
    {
        LCD_ShowChinese(0, 0, "过温保护", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowString(128, 0, ":50", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowChinese(0, 33, "过温保护", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowString(128, 33, ":60", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowChinese(0, 66, "过温保护", BLACK, LIGHTBLUE, 32, 0);
        LCD_ShowString(128, 66, ":70", BLACK, LIGHTBLUE, 32, 0);
    }
}

void fastch_page_ui_process(menu_u8 KeyValue, uint16_t fastCharge_InVoltage)
{
    uint8_t count = 0, index_move_flag = 0;
    char str[10] = {0};
    int i;
    menu_u8 cursor_temp = cursor_secondary_menu;
    current_menu_index = FASTCH_PAGE;
    HUSB238_Capability_t PDCapabilities[6];
    HUSB238_ExtractCap(PDCapabilities);
    for (i = 0; i < 6; i++) {
        if (PDCapabilities[i].detected == true) {
            PDCapabilities[count].detected = PDCapabilities[i].detected;
            PDCapabilities[count].current = PDCapabilities[i].current;
            PDCapabilities[count].voltage = PDCapabilities[i].voltage;
            count++;
        }
    }
    if (count == 0) {
        LCD_ShowChinese(36, 30, "未检测到", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowChinese(68, 62, "快充输入", LIGHTBLUE, BLACK, 32, 0);
        return;
    }

    if (KeyValue == KEY4_SHORT) {
        for (i = 0; i < count; i++) {
            if (PDCapabilities[i].voltage == fastCharge_InVoltage) {
                sub_index.fastch_current_index = i;
            }
        }
        if (count < 4) {
            secondary_menu_index = 0;
            cursor_secondary_menu = sub_index.fastch_current_index;
        } else if (count - sub_index.fastch_current_index >= 4) {
            secondary_menu_index = sub_index.fastch_current_index;
            cursor_secondary_menu = 0;
        } else if (count - sub_index.fastch_current_index < 4) {
            secondary_menu_index = count - 4;
            cursor_secondary_menu = sub_index.fastch_current_index - 1;
        }
    }

    if (cursor_secondary_menu == 3 && KeyValue == RIGHT && secondary_menu_index != (count - 4)) {
        secondary_menu_index++;
        index_move_flag = 1;
    } else if (cursor_secondary_menu == 0 && KeyValue == LEFT && secondary_menu_index != 0) {
        secondary_menu_index--;
        index_move_flag = 1;
    }
    if (cursor_secondary_menu != 3 && KeyValue == RIGHT) {
        cursor_secondary_menu++;
    } else if (cursor_secondary_menu != 0 && KeyValue == LEFT) {
        cursor_secondary_menu--;
    }
    if (KeyValue == LEFT) {
        (sub_index.fastch_current_index > 0) ? (sub_index.fastch_current_index--) : (sub_index.fastch_current_index = 0);
    } else if (KeyValue == RIGHT) {
        (sub_index.fastch_current_index < (count - 1)) ? (sub_index.fastch_current_index++) : (sub_index.fastch_current_index = count - 1);
    }

    if (count >= 4) {
        count = 4;
    }
    if (index_move_flag ||  KeyValue == KEY4_SHORT) {
        for (size_t i = 0; i < count; i++) {
            sprintf(str, ":%dV %.1fA", PDCapabilities[secondary_menu_index + i].voltage, PDCapabilities[secondary_menu_index + i].current);
            LCD_RowClear(i, 32, BLACK);
            if (i == cursor_secondary_menu) {
                LCD_ShowChinese(0, i * 33, "快充", BLACK, LIGHTBLUE, 32, 0);
                LCD_ShowString(64, i * 33, (uint8_t*)str, BLACK, LIGHTBLUE, 32, 0);
            } else {
                LCD_ShowChinese(0, i * 33, "快充", LIGHTBLUE, BLACK, 32, 0);
                LCD_ShowString(64, i * 33, (uint8_t*)str, LIGHTBLUE, BLACK, 32, 0);
            }
        }
    } else {
        sprintf(str, ":%dV %.1fA", PDCapabilities[secondary_menu_index + cursor_temp].voltage, PDCapabilities[secondary_menu_index + cursor_temp].current);
        LCD_RowClear(cursor_temp, 32, BLACK);
        LCD_ShowChinese(0, cursor_temp * 33, "快充", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowString(64, cursor_temp * 33, (uint8_t*)str, LIGHTBLUE, BLACK, 32, 0);
        sprintf(str, ":%dV %.1fA", PDCapabilities[secondary_menu_index + cursor_secondary_menu].voltage, PDCapabilities[secondary_menu_index + cursor_secondary_menu].current);
        LCD_RowClear(cursor_secondary_menu, 32, BLACK);
        LCD_ShowChinese(0, cursor_secondary_menu * 33, "快充", BLACK, LIGHTBLUE, 32, 0);
        LCD_ShowString(64, cursor_secondary_menu * 33, (uint8_t*)str, BLACK, LIGHTBLUE, 32, 0);
    }
}

void VBUS_calibration_page_ui_process(menu_u8 index)
{
    current_menu_index = VBUS_CALIBRATION_PAGE;
    switch (index)
    {
    case 0:
        LCD_ShowChinese(60, 32, "开启校准", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowString(60, 64, "Yes", BLACK, LIGHTBLUE, 32, 0);
        LCD_ShowString(156, 64, "No", LIGHTBLUE, BLACK, 32, 0);
        break;
    case 1:
        LCD_ShowString(60, 64, "Yes", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowString(156, 64, "No", BLACK, LIGHTBLUE, 32, 0);
        break;
    default:
        break;
    }
}

void protect_page_ui_process(menu_u8 index)
{
    current_menu_index = PROTECT_PAGE;
    LCD_Clear();
    switch (index)
    {
    case VBUS_PROTECT:
        LCD_ShowString(100, 50, "SCP", RED, BLACK, 32, 0);
        break;
    case TEMP_PROTECT:
        LCD_ShowChinese(50, 50, "过温保护", RED, BLACK, 32, 0);
        break;
    case VBAT_PROTECT:
        LCD_ShowString(50, 50, "VBATPORT!", RED, BLACK, 32, 0);
        break;
    case PRESSET_PROTECT:
        LCD_ShowString(10, 50, "NeedInput>=9V", RED, BLACK, 32, 0);
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
    LCD_ShowChinese(0, 0, "名称", LIGHTBLUE, BLACK, 32, 0);
    strcat(name, APP_config.device_name);
    LCD_ShowString(64, 0, (uint8_t*)name, LIGHTBLUE, BLACK, 32, 0);
    LCD_ShowChinese(0, 33, "型号", LIGHTBLUE, BLACK, 32, 0);
    LCD_ShowString(64, 33, ":PD  POCKET", LIGHTBLUE, BLACK, 32, 0);
    LCD_ShowChinese(0, 66, "版本", LIGHTBLUE, BLACK, 32, 0);
    LCD_ShowString(64, 66, ":1.1.0", LIGHTBLUE, BLACK, 32, 0);
    // LCD_ShowString(0, 0, "PD Pocket", LIGHTBLUE, BLACK, 32, 0);
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
            LCD_ShowString(70, 80, (const uint8_t*)str, LIGHTBLUE, BLACK, 32, 0);
        } else {
            LCD_ShowString(36, 80, (const uint8_t*)str, LIGHTBLUE, BLACK, 32, 0);
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
            LCD_ShowIntNum(118, 80, digits[1], 1, BLACK, LIGHTBLUE, 32);
        } else if (SC8815_Config.SC8815_VBUS_IBUS_Step == 100) {
            LCD_ShowIntNum(102, 80, digits[2], 1, BLACK, LIGHTBLUE, 32);
        } else if (SC8815_Config.SC8815_VBUS_IBUS_Step == 1000) {
            LCD_ShowIntNum(70, 80, digits[3], 1, BLACK, LIGHTBLUE, 32);
        }
        break;
    default:
        break;
    }
}
