#include "menu_ui.h"
#include "lcd.h"
#include "lcd_init.h"
#include "pic.h"
#include "Application_SC8815.h"
#include "Application_ADC.h"

extern menu_i32 current_menu_index;

// 进入相应的页面
void Enter_Page(menu_i32 index, menu_u8 KeyValue)
{
    sub_index.Current_Page = index;
    switch (sub_index.Current_Page)
    {
    case MAIN_PAGE:
        current_menu_index = MAIN_PAGE;
        LCD_Clear();
        main_page_init();
        break;
    case MAIN_MENU_PAGE:
        main_menu_Init();
        break;

    case DETECT_PAGE:

        break;

    case LOG_PAGE:

        break;

        // 进入音量设置页面
    case VOLUME_PAGE:
        volume_page_ui_process(current_mode.Current_Volume_Level);
        break;

        // 进入亮度设置页面
    case BRIGNE_PAGE:
        backlight_page_ui_process(current_mode.Current_Backlight_Level);
        break;

        // 进入无线设置页面
    case WLAN_PAGE:
        wlan_page_process(current_mode.Current_Select_Wlan);
        break;

        // 进入语言设置页面
    case LANGUAGE_PAGE:
        language_page_ui_process(current_mode.Current_Select_Language);
        break;

        // 进入灵敏度设置页面
    case SENSIVITY_PAGE:
        sensivity_page_process(current_mode.Current_Sensivity);
        break;

    default:
        break;
    }
}

void main_page_init(void)
{
    LCD_ShowString(0, 0, "VSET:", RED, BLACK, 32, 0);
    float temp = SC8815_Config.SC8815_VBUS / 1000;
    if (temp < 10)
    {
        LCD_ShowFloatNum(80, 0, temp, 3, RED, BLACK, 32);
        LCD_ShowChar(150, 0, 'V', RED, BLACK, 32, 0);
    }
    else {
        LCD_ShowFloatNum(80, 0, temp, 4, RED, BLACK, 32);
        LCD_ShowChar(160, 0, 'V', RED, BLACK, 32, 0);
    }

    LCD_ShowString(0, 33, "ISET:", RED, BLACK, 32, 0);
    temp = SC8815_Config.SC8815_IBUS_Limit / 1000;
    if (temp < 10)
    {
        LCD_ShowFloatNum(80, 33, temp, 3, RED, BLACK, 32);
        LCD_ShowChar(150, 33, 'A', RED, BLACK, 32, 0);
    }
    else {
        LCD_ShowFloatNum(80, 33, temp, 4, RED, BLACK, 32);
        LCD_ShowChar(160, 33, 'A', RED, BLACK, 32, 0);
    }

    LCD_ShowString(0, 66, "VOUT:", RED, BLACK, 32, 0);
    temp = App_getVBUS_V();
    if (temp < 10)
    {
        LCD_ShowFloatNum(80, 66, temp, 3, RED, BLACK, 32);
        LCD_ShowChar(150, 66, 'V', RED, BLACK, 32, 0);
    }
    else {
        LCD_ShowFloatNum(80, 66, temp, 4, RED, BLACK, 32);
        LCD_ShowChar(160, 66, 'V', RED, BLACK, 32, 0);
    }

    LCD_ShowString(0, 99, "IOUT:", RED, BLACK, 32, 0);
    temp = App_getIBUS_A();
    if (temp < 10)
    {
        LCD_ShowFloatNum(80, 99, temp, 3, RED, BLACK, 32);
        LCD_ShowChar(150, 99, 'A', RED, BLACK, 32, 0);
    }
    else {
        LCD_ShowFloatNum(80, 99, temp, 4, RED, BLACK, 32);
        LCD_ShowChar(160, 99, 'A', RED, BLACK, 32, 0);
    }
}

// 主菜单初始化
void main_menu_Init(void)
{
    sub_index.main_current_index = 0;
    sub_index.Current_Page = MAIN_MENU_PAGE;
    current_menu_index = MAIN_MENU_PAGE;
    //.....刷新回主页面的UI和状态
    LCD_Clear();
    LCD_ShowString(0, 0, "PRESSET  <--", RED, BLACK, 32, 0);
    LCD_ShowString(0, 33, "BUZZER", RED, BLACK, 32, 0);
    LCD_ShowString(0, 66, "TEMP", RED, BLACK, 32, 0);
    LCD_ShowString(0, 99, "FASTCH:", RED, BLACK, 32, 0);
}

// 主页面UI处理
void main_page_ui_process(menu_u8 index)
{
    switch (index)
    {
    case 0:
        break;
    case 1:
        break;
    case 2:
        break;
    case 3:
        break;
    case 4:
        break;
    case 5:
        break;
    case 6:
        break;
    default:
        break;
    }
}

void vout_page_ui_process(menu_u8 KeyValue)
{
    LCD_Clear();
    float temp = 0;
    switch (KeyValue)
    {
    case LEFT:
        LCD_ShowString(0, 0, "SETVOUT:", RED, BLACK, 32, 0);
        temp = SC8815_Config.SC8815_VBUS / 1000;
        if (temp < 10)
        {
            LCD_ShowFloatNum(140, 0, temp, 3, RED, BLACK, 32);
            LCD_ShowChar(220, 0, 'V', RED, BLACK, 32, 0);
        }
        else {
            LCD_ShowFloatNum(140, 0, temp, 4, RED, BLACK, 32);
            LCD_ShowChar(240, 0, 'V', RED, BLACK, 32, 0);
        }
        break;
    case RIGHT:
        LCD_ShowString(0, 0, "SETVOUT:", RED, BLACK, 32, 0);
        temp = SC8815_Config.SC8815_VBUS / 1000;
        if (temp < 10)
        {
            LCD_ShowFloatNum(140, 0, temp, 3, RED, BLACK, 32);
            LCD_ShowChar(220, 0, 'V', RED, BLACK, 32, 0);
        }
        else {
            LCD_ShowFloatNum(140, 0, temp, 4, RED, BLACK, 32);
            LCD_ShowChar(240, 0, 'V', RED, BLACK, 32, 0);
        }
        break;
    case KEY2_SHORT:
        LCD_ShowString(0, 0, "SETVOUT:", RED, BLACK, 32, 0);
        temp = SC8815_Config.SC8815_VBUS / 1000;
        if (temp < 10)
        {
            LCD_ShowFloatNum(140, 0, temp, 3, RED, BLACK, 32);
            LCD_ShowChar(220, 0, 'V', RED, BLACK, 32, 0);
        }
        else {
            LCD_ShowFloatNum(140, 0, temp, 4, RED, BLACK, 32);
            LCD_ShowChar(240, 0, 'V', RED, BLACK, 32, 0);
        }
        break;
    case KEY4_SHORT:
        break;
    default:
        break;
    }
}

// 音量设置ui处理
void volume_page_ui_process(menu_u8 index)
{
    switch (index)
    {
    case 0:

        break;

    case 1:

        break;

    case 2:

        break;

    case 3:

        break;

    case 4:

        break;
    default:
        break;
    }
}

// 背光设置ui处理
void backlight_page_ui_process(menu_u8 index)
{
    switch (index)
    {
    case 0:

        break;

    case 1:

        break;

    case 2:

        break;

    case 3:

        break;

    case 4:

        break;
    default:
        break;
    }
}

// 无线数据上传页面ui处理
void wlan_page_ui_process(menu_u8 index)
{
    switch (index)
    {
    case 0:

        break;
    case 1:

        break;
    default:
        break;
    }
}

// 语言页面ui处理
void language_page_ui_process(menu_u8 index)
{
    switch (index)
    {
    case 0:

        break;
    case 1:

        break;
    default:
        break;
    }
}

// 灵敏度页面ui处理
void sensivity_ui_process(menu_u8 index)
{
    switch (index)
    {
    case 0:

        break;
    case 1:

        break;
    case 2:

        break;
    default:
        break;
    }
}
