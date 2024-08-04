#include "menu_ui.h"
#include "Application_LCD.h"
#include "Application.h"
#include "pic.h"
#include "string.h"
#include "Application_SC8815.h"

extern menu_i32 current_menu_index;
menu_u8 cursor = 0; //光标位置
uint8_t presset_show_vlaue[] = {0, 1, 2, 3};   //当前显示的预设值

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
        cursor = 0;
        presset_show_vlaue[0] = 0;
        presset_show_vlaue[1] = 1;
        presset_show_vlaue[2] = 2;
        presset_show_vlaue[3] = 3;
        current_menu_index = PRESSET_PAGE;
        presset_page_ui_process(0, KeyValue);
        break;
    case 1:
        sub_index.presset_current_index = 0;
        cursor = 0;
        presset_show_vlaue[0] = 0;
        presset_show_vlaue[1] = 1;
        presset_show_vlaue[2] = 2;
        presset_show_vlaue[3] = 3;
        current_menu_index = PRESSET_START_PAGE;
        presset_page_ui_process(0, KeyValue);
        break;
    case 2:
        temp_page_ui_process(0);
        break;
    case 3:
        fastch_page_ui_process(APP_config.fastCharge_InVoltage);
        break;
    case 4:
        buzzer_page_ui_process(0);
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
void main_menu_page_ui_process(menu_u8 index)
{
    current_menu_index = MAIN_MENU_PAGE;
    LCD_Clear();
    switch (index)
    {
    case 0:
        LCD_ShowChinese(40, 0, "预设配置", BLACK, LIGHTBLUE, 32, 0);
        LCD_ShowPicture(0, 0, 32, 32, gImage_presset);
        LCD_ShowChinese(40, 33, "开启预设", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowPicture(0, 33, 32, 32, gImage_start_presset);
        LCD_ShowChinese(40, 66, "过温保护", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowPicture(0, 66, 32, 32, gImage_temp);
        LCD_ShowChinese(40, 99, "快充输入", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowPicture(0, 99, 32, 32, gImage_PDinput);
        break;
    case 1:
        LCD_ShowChinese(40, 0, "预设配置", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowPicture(0, 0, 32, 32, gImage_presset);
        LCD_ShowChinese(40, 33, "开启预设", BLACK, LIGHTBLUE, 32, 0);
        LCD_ShowPicture(0, 33, 32, 32, gImage_start_presset);
        LCD_ShowChinese(40, 66, "过温保护", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowPicture(0, 66, 32, 32, gImage_temp);
        LCD_ShowChinese(40, 99, "快充输入", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowPicture(0, 99, 32, 32, gImage_PDinput);
        break;
    case 2:
        LCD_ShowChinese(40, 0, "预设配置", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowPicture(0, 0, 32, 32, gImage_presset);
        LCD_ShowChinese(40, 33, "开启预设", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowPicture(0, 33, 32, 32, gImage_start_presset);
        LCD_ShowChinese(40, 66, "过温保护", BLACK, LIGHTBLUE, 32, 0);
        LCD_ShowPicture(0, 66, 32, 32, gImage_temp);
        LCD_ShowChinese(40, 99, "快充输入", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowPicture(0, 99, 32, 32, gImage_PDinput);
        break;
    case 3:
        LCD_ShowChinese(40, 0, "预设配置", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowPicture(0, 0, 32, 32, gImage_presset);
        LCD_ShowChinese(40, 33, "开启预设", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowPicture(0, 33, 32, 32, gImage_start_presset);
        LCD_ShowChinese(40, 66, "过温保护", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowPicture(0, 66, 32, 32, gImage_temp);
        LCD_ShowChinese(40, 99, "快充输入", BLACK, LIGHTBLUE, 32, 0);
        LCD_ShowPicture(0, 99, 32, 32, gImage_PDinput);
        break;
    case 4:
        LCD_ShowChinese(40, 0, "开启预设", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowPicture(0, 0, 32, 32, gImage_start_presset);
        LCD_ShowChinese(40, 33, "过温保护", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowPicture(0, 33, 32, 32, gImage_temp);
        LCD_ShowChinese(40, 66, "快充输入", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowPicture(0, 66, 32, 32, gImage_PDinput);
        LCD_ShowChinese(40, 99, "蜂鸣器", BLACK, LIGHTBLUE, 32, 0);
        LCD_ShowPicture(0, 99, 32, 32, gImage_buzzer);
        break;
    default:
        break;
    }
}

void vout_page_ui_process(menu_u8 KeyValue)
{
    current_menu_index = VOUT_PAGE;
    LCD_Clear();
    switch (KeyValue)
    {
    case LEFT:
        APP_LCD_Show_SETVOUT();
        break;
    case RIGHT:
        APP_LCD_Show_SETVOUT();
        break;
    case KEY2_SHORT:
        APP_LCD_Show_SETVOUT();
        break;
    default:
        break;
    }
}

void iout_page_ui_process(menu_u8 KeyValue)
{
    current_menu_index = IOUT_PAGE;
    LCD_Clear();
    switch (KeyValue)
    {
    case LEFT:
        APP_LCD_Show_SETIOUT();
        break;
    case RIGHT:
        APP_LCD_Show_SETIOUT();
        break;
    case KEY1_SHORT:
        APP_LCD_Show_SETIOUT();
        break;
    default:
        break;
    }
}

void presset_page_ui_process(menu_u8 index, menu_u8 KeyValue)
{
    
    char presset_str[10];
    LCD_Clear();
		if (cursor == 3 && KeyValue == RIGHT && presset_show_vlaue[3] != 9)
    {
        presset_show_vlaue[0]++;
        presset_show_vlaue[1]++;
        presset_show_vlaue[2]++;
        presset_show_vlaue[3]++;
    }
    else if (cursor == 0 && KeyValue == LEFT && presset_show_vlaue[0] != 0)
    {
        presset_show_vlaue[0]--;
        presset_show_vlaue[1]--;
        presset_show_vlaue[2]--;
        presset_show_vlaue[3]--;
    }
    if(cursor != 3 && KeyValue == RIGHT)
    {
        cursor++;
    }
    else if (cursor != 0 && KeyValue == LEFT)
    {
        cursor--;
    }
    if (cursor == 0)
    {
        sprintf(presset_str, ":%d", presset_show_vlaue[0]);
        LCD_ShowChinese(0, 0, "预设", BLACK, LIGHTBLUE, 32, 0);
        LCD_ShowString(64, 0, (uint8_t *)presset_str, BLACK, LIGHTBLUE, 32, 0);
    }
    else
    {
        sprintf(presset_str, ":%d", presset_show_vlaue[0]);
        LCD_ShowChinese(0, 0, "预设", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowString(64, 0, (uint8_t *)presset_str, LIGHTBLUE, BLACK, 32, 0);
    }

    memset(presset_str, 0, sizeof(presset_str));
    if (cursor == 1)
    {
        sprintf(presset_str, ":%d", presset_show_vlaue[1]);
        LCD_ShowChinese(0, 33, "预设", BLACK, LIGHTBLUE, 32, 0);
        LCD_ShowString(64, 33, (uint8_t *)presset_str, BLACK, LIGHTBLUE, 32, 0);
    }
    else
    {
        sprintf(presset_str, ":%d", presset_show_vlaue[1]);
        LCD_ShowChinese(0, 33, "预设", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowString(64, 33, (uint8_t *)presset_str, LIGHTBLUE, BLACK, 32, 0);
    }

    memset(presset_str, 0, sizeof(presset_str));
    if (cursor == 2)
    {
        sprintf(presset_str, ":%d", presset_show_vlaue[2]);
        LCD_ShowChinese(0, 66, "预设", BLACK, LIGHTBLUE, 32, 0);
        LCD_ShowString(64, 66, (uint8_t *)presset_str, BLACK, LIGHTBLUE, 32, 0);
    }
    else
    {
        sprintf(presset_str, ":%d", presset_show_vlaue[2]);
        LCD_ShowChinese(0, 66, "预设", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowString(64, 66, (uint8_t *)presset_str, LIGHTBLUE, BLACK, 32, 0);
    }

    memset(presset_str, 0, sizeof(presset_str));
    if (cursor == 3)
    {
        sprintf(presset_str, ":%d", presset_show_vlaue[3]);
        LCD_ShowChinese(0, 99, "预设", BLACK, LIGHTBLUE, 32, 0);
        LCD_ShowString(64, 99, (uint8_t *)presset_str, BLACK, LIGHTBLUE, 32, 0);
    }
    else
    {
        sprintf(presset_str, ":%d", presset_show_vlaue[3]);
        LCD_ShowChinese(0, 99, "预设", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowString(64, 99, (uint8_t *)presset_str, LIGHTBLUE, BLACK, 32, 0);
    }
}

void presset_config_page_ui_process(menu_u8 index)
{
    current_menu_index = PRESSET_CONFIG_PAGE;
    LCD_Clear();
    extern SC8815_TIM_WorkTypeDef SC8815_TIM_Work[SC8815_TIM_WORK_SIZE];
    char str[10];
    sprintf(str, "vset:%.2fv", SC8815_TIM_Work[sub_index.presset_current_index].SC8815_VBUS[index] / 1000);
    LCD_ShowString(0, 0, (uint8_t *)str, RED, BLACK, 32, 0);
    memset(str, 0, 10);
    sprintf(str, "iset:%.2fA", SC8815_TIM_Work[sub_index.presset_current_index].SC8815_IBUS_Limit[index] / 1000);
    LCD_ShowString(0, 33, (uint8_t *)str, RED, BLACK, 32, 0);
    memset(str, 0, 10);
    sprintf(str, "time:%ds", SC8815_TIM_Work[sub_index.presset_current_index].SC8815_TIM_Work_second[index]);
    LCD_ShowString(0, 66, (uint8_t *)str, RED, BLACK, 32, 0);
    memset(str, 0, 10);
    sprintf(str, "circular:%d", SC8815_TIM_Work[sub_index.presset_current_index].circular);
    LCD_ShowString(0, 99, (uint8_t *)str, RED, BLACK, 16, 0);
    memset(str, 0, 10);
    sprintf(str, "page:%d/29", index);
    LCD_ShowString(100, 99, (uint8_t *)str, RED, BLACK, 16, 0);
}

void presset_config_set_page_ui_process(menu_u8 KeyValue)
{
    current_menu_index = PRESSET_CONFIG_SET_PAGE;
    extern presset_config_set_typeDef presset_config_set;
    LCD_Clear();
    char str[10];
    switch (KeyValue)
    {
    case LEFT:
        switch (presset_config_set.set_flag)
        {
        case PRESSET_SET_VOUT:
            sprintf(str, "vset:%.2fV", presset_config_set.set_vbus[presset_config_set.current_index] / 1000);
            LCD_ShowString(0, 0, (uint8_t *)str, RED, BLACK, 32, 0);
            break;
        case PRESSET_SET_IOUT:
            sprintf(str, "iset:%.2fA", presset_config_set.set_ibus[presset_config_set.current_index] / 1000);
            LCD_ShowString(0, 0, (uint8_t *)str, RED, BLACK, 32, 0);
            break;
        case PRESSET_SET_TIME:
            sprintf(str, "time:%ds", presset_config_set.set_time[presset_config_set.current_index]);
            LCD_ShowString(0, 0, (uint8_t *)str, RED, BLACK, 32, 0);
            break;
        case PRESSET_SET_CIRCULAR:
            sprintf(str, "circular:%d", presset_config_set.set_circular);
            LCD_ShowString(0, 0, (uint8_t *)str, RED, BLACK, 32, 0);
            break;
        default:
            break;
        }
        break;
    case RIGHT:
        switch (presset_config_set.set_flag)
        {
        case PRESSET_SET_VOUT:
            sprintf(str, "vset:%.2fV", presset_config_set.set_vbus[presset_config_set.current_index] / 1000);
            LCD_ShowString(0, 0, (uint8_t *)str, RED, BLACK, 32, 0);
            break;
        case PRESSET_SET_IOUT:
            sprintf(str, "iset:%.2fA", presset_config_set.set_ibus[presset_config_set.current_index] / 1000);
            LCD_ShowString(0, 0, (uint8_t *)str, RED, BLACK, 32, 0);
            break;
        case PRESSET_SET_TIME:
            sprintf(str, "time:%ds", presset_config_set.set_time[presset_config_set.current_index]);
            LCD_ShowString(0, 0, (uint8_t *)str, RED, BLACK, 32, 0);
            break;
        case PRESSET_SET_CIRCULAR:
            sprintf(str, "circular:%d", presset_config_set.set_circular);
            LCD_ShowString(0, 0, (uint8_t *)str, RED, BLACK, 32, 0);
            break;
        default:
            break;
        }
        break;
    case KEY3_SHORT:
        sprintf(str, "circular:%d", presset_config_set.set_circular);
        LCD_ShowString(0, 0, (uint8_t *)str, RED, BLACK, 32, 0);
        break;
    case KEY4_SHORT:
        switch (presset_config_set.set_flag)
        {
        case PRESSET_SET_VOUT:
            sprintf(str, "vset:%.2fV", presset_config_set.set_vbus[presset_config_set.current_index] / 1000);
            LCD_ShowString(0, 0, (uint8_t *)str, RED, BLACK, 32, 0);
            break;
        case PRESSET_SET_IOUT:
            sprintf(str, "iset:%.2fA", presset_config_set.set_ibus[presset_config_set.current_index] / 1000);
            LCD_ShowString(0, 0, (uint8_t *)str, RED, BLACK, 32, 0);
            break;
        case PRESSET_SET_TIME:
            sprintf(str, "time:%ds", presset_config_set.set_time[presset_config_set.current_index]);
            LCD_ShowString(0, 0, (uint8_t *)str, RED, BLACK, 32, 0);
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

void buzzer_page_ui_process(menu_u8 KeyValue)
{
}

void temp_page_ui_process(menu_u8 KeyValue)
{
}

void fastch_page_ui_process(menu_u8 index)
{
    current_menu_index = FASTCH_PAGE;
    sub_index.fastch_current_index = index;
    LCD_Clear();
    switch (index)
    {
    case 5:
        LCD_ShowChinese(0, 0, "快充输入", BLACK, LIGHTBLUE, 32, 0);
        LCD_ShowString(128, 0, ":5V", BLACK, LIGHTBLUE, 32, 0);
        LCD_ShowChinese(0, 33, "快充输入", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowString(128, 33, ":9V", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowChinese(0, 66, "快充输入", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowString(128, 66, ":12V", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowChinese(0, 99, "快充输入", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowString(128, 99, ":15V", LIGHTBLUE, BLACK, 32, 0);
        break;
    case 9:
        LCD_ShowChinese(0, 0, "快充输入", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowString(128, 0, ":5V", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowChinese(0, 33, "快充输入", BLACK, LIGHTBLUE, 32, 0);
        LCD_ShowString(128, 33, ":9V", BLACK, LIGHTBLUE, 32, 0);
        LCD_ShowChinese(0, 66, "快充输入", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowString(128, 66, ":12V", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowChinese(0, 99, "快充输入", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowString(128, 99, ":15V", LIGHTBLUE, BLACK, 32, 0);
        break;
    case 12:
        LCD_ShowChinese(0, 0, "快充输入", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowString(128, 0, ":5V", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowChinese(0, 33, "快充输入", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowString(128, 33, ":9V", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowChinese(0, 66, "快充输入", BLACK, LIGHTBLUE, 32, 0);
        LCD_ShowString(128, 66, ":12V", BLACK, LIGHTBLUE, 32, 0);
        LCD_ShowChinese(0, 99, "快充输入", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowString(128, 99, ":15V", LIGHTBLUE, BLACK, 32, 0);
        break;
    case 15:
        LCD_ShowChinese(0, 0, "快充输入", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowString(128, 0, ":5V", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowChinese(0, 33, "快充输入", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowString(128, 33, ":9V", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowChinese(0, 66, "快充输入", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowString(128, 66, ":12V", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowChinese(0, 99, "快充输入", BLACK, LIGHTBLUE, 32, 0);
        LCD_ShowString(128, 99, ":15V", BLACK, LIGHTBLUE, 32, 0);
        break;
    case 20:
        LCD_ShowChinese(0, 0, "快充输入", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowString(128, 0, ":9V", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowChinese(0, 33, "快充输入", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowString(128, 33, ":12V", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowChinese(0, 66, "快充输入", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowString(128, 66, ":15V", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowChinese(0, 99, "快充输入", BLACK, LIGHTBLUE, 32, 0);
        LCD_ShowString(128, 99, ":20V", BLACK, LIGHTBLUE, 32, 0);
        break;
    default:
        break;
    }
}
