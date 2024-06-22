#include "menu_ui.h"
#include "Application_LCD.h"
#include "Application.h"

extern menu_i32 current_menu_index;

// 进入相应的页面
void Enter_Page(menu_i32 index, menu_u8 KeyValue)
{
    sub_index.Current_Page = index;
    switch (sub_index.Current_Page)
    {
    case 0:
        presset_page_ui_process(0);
        break;
    case 1:
        buzzer_page_ui_process(0);
        break;
    case 2:
        temp_page_ui_process(0);
        break;
    case 3:
        fastch_page_ui_process(APP_config.fastCharge_InVoltage);
        break;
    default:
        break;
    }
}

void main_page_init(void)
{
    current_menu_index = MAIN_PAGE;
    LCD_Clear();
    APP_LCD_main_show();
}

// 主菜单初始化
void main_menu_Init(void)
{
    sub_index.main_menu_current_index = 0;
    sub_index.Current_Page = MAIN_MENU_PAGE;
    current_menu_index = MAIN_MENU_PAGE;
    //.....刷新回主页面的UI和状态
    LCD_Clear();
    LCD_ShowString(0, 0, "PRESSET  <--", RED, BLACK, 32, 0);
    LCD_ShowString(0, 33, "BUZZER", RED, BLACK, 32, 0);
    LCD_ShowString(0, 66, "TEMP", RED, BLACK, 32, 0);
    LCD_ShowString(0, 99, "FASTCH", RED, BLACK, 32, 0);
}

// 主菜单页面UI处理
void main_menu_page_ui_process(menu_u8 index)
{
    LCD_Clear();
    switch (index)
    {
    case 0:
        LCD_ShowString(0, 0, "PRESSET  <--", RED, BLACK, 32, 0);
        LCD_ShowString(0, 33, "BUZZER", RED, BLACK, 32, 0);
        LCD_ShowString(0, 66, "TEMP", RED, BLACK, 32, 0);
        LCD_ShowString(0, 99, "FASTCH", RED, BLACK, 32, 0);
        break;
    case 1:
        LCD_ShowString(0, 0, "PRESSET", RED, BLACK, 32, 0);
        LCD_ShowString(0, 33, "BUZZER  <--", RED, BLACK, 32, 0);
        LCD_ShowString(0, 66, "TEMP", RED, BLACK, 32, 0);
        LCD_ShowString(0, 99, "FASTCH", RED, BLACK, 32, 0);
        break;
    case 2:
        LCD_ShowString(0, 0, "PRESSET", RED, BLACK, 32, 0);
        LCD_ShowString(0, 33, "BUZZER", RED, BLACK, 32, 0);
        LCD_ShowString(0, 66, "TEMP  <--", RED, BLACK, 32, 0);
        LCD_ShowString(0, 99, "FASTCH", RED, BLACK, 32, 0);
        break;
    case 3:
        LCD_ShowString(0, 0, "PRESSET", RED, BLACK, 32, 0);
        LCD_ShowString(0, 33, "BUZZER", RED, BLACK, 32, 0);
        LCD_ShowString(0, 66, "TEMP", RED, BLACK, 32, 0);
        LCD_ShowString(0, 99, "FASTCH  <--", RED, BLACK, 32, 0);
        break;
    default:
        break;
    }
}

void vout_page_ui_process(menu_u8 KeyValue)
{
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

void presset_page_ui_process(menu_u8 KeyValue)
{
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
        LCD_ShowString(0, 0, "fastCh:5V <--", RED, BLACK, 32, 0);
        LCD_ShowString(0, 33, "fastCh:9V", RED, BLACK, 32, 0);
        LCD_ShowString(0, 66, "fastCh:12V", RED, BLACK, 32, 0);
        LCD_ShowString(0, 99, "fastCh:15V", RED, BLACK, 32, 0);
        break;
    case 9:
        LCD_ShowString(0, 0, "fastCh:5V", RED, BLACK, 32, 0);
        LCD_ShowString(0, 33, "fastCh:9V <--", RED, BLACK, 32, 0);
        LCD_ShowString(0, 66, "fastCh:12V", RED, BLACK, 32, 0);
        LCD_ShowString(0, 99, "fastCh:15V", RED, BLACK, 32, 0);
        break;
    case 12:
        LCD_ShowString(0, 0, "fastCh:5V", RED, BLACK, 32, 0);
        LCD_ShowString(0, 33, "fastCh:9V", RED, BLACK, 32, 0);
        LCD_ShowString(0, 66, "fastCh:12V <--", RED, BLACK, 32, 0);
        LCD_ShowString(0, 99, "fastCh:15V", RED, BLACK, 32, 0);
        break;
    case 15:
        LCD_ShowString(0, 0, "fastCh:5V", RED, BLACK, 32, 0);
        LCD_ShowString(0, 33, "fastCh:9V", RED, BLACK, 32, 0);
        LCD_ShowString(0, 66, "fastCh:12V", RED, BLACK, 32, 0);
        LCD_ShowString(0, 99, "fastCh:15V <--", RED, BLACK, 32, 0);
        break;
    case 20:
        LCD_ShowString(0, 0, "fastCh:9V", RED, BLACK, 32, 0);
        LCD_ShowString(0, 33, "fastCh:12V", RED, BLACK, 32, 0);
        LCD_ShowString(0, 66, "fastCh:15V", RED, BLACK, 32, 0);
        LCD_ShowString(0, 99, "fastCh:20V <--", RED, BLACK, 32, 0);
        break;
    default:
        break;
    }
}
