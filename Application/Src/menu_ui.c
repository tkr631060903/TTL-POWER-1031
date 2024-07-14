#include "menu_ui.h"
#include "Application_LCD.h"
#include "Application.h"
#include "pic.h"

extern menu_i32 current_menu_index;

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
        presset_page_ui_process(sub_index.presset_current_index);
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
    LCD_ShowChinese(40, 0, "预设", BLACK, LIGHTBLUE, 32, 0);
    LCD_ShowPicture(0, 0, 32, 32, gImage_presset);
    LCD_ShowChinese(40, 33, "蜂鸣器", LIGHTBLUE, BLACK, 32, 0);
    LCD_ShowPicture(0, 33, 32, 32, gImage_buzzer);
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
        LCD_ShowChinese(40, 0, "预设", BLACK, LIGHTBLUE, 32, 0);
        LCD_ShowPicture(0, 0, 32, 32, gImage_presset);
        LCD_ShowChinese(40, 33, "蜂鸣器", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowPicture(0, 33, 32, 32, gImage_buzzer);
        LCD_ShowChinese(40, 66, "过温保护", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowPicture(0, 66, 32, 32, gImage_temp);
        LCD_ShowChinese(40, 99, "快充输入", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowPicture(0, 99, 32, 32, gImage_PDinput);
        break;
    case 1:
        LCD_ShowChinese(40, 0, "预设", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowPicture(0, 0, 32, 32, gImage_presset);
        LCD_ShowChinese(40, 33, "蜂鸣器", BLACK, LIGHTBLUE, 32, 0);
        LCD_ShowPicture(0, 33, 32, 32, gImage_buzzer);
        LCD_ShowChinese(40, 66, "过温保护", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowPicture(0, 66, 32, 32, gImage_temp);
        LCD_ShowChinese(40, 99, "快充输入", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowPicture(0, 99, 32, 32, gImage_PDinput);
        break;
    case 2:
        LCD_ShowChinese(40, 0, "预设", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowPicture(0, 0, 32, 32, gImage_presset);
        LCD_ShowChinese(40, 33, "蜂鸣器", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowPicture(0, 33, 32, 32, gImage_buzzer);
        LCD_ShowChinese(40, 66, "过温保护", BLACK, LIGHTBLUE, 32, 0);
        LCD_ShowPicture(0, 66, 32, 32, gImage_temp);
        LCD_ShowChinese(40, 99, "快充输入", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowPicture(0, 99, 32, 32, gImage_PDinput);
        break;
    case 3:
        LCD_ShowChinese(40, 0, "预设", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowPicture(0, 0, 32, 32, gImage_presset);
        LCD_ShowChinese(40, 33, "蜂鸣器", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowPicture(0, 33, 32, 32, gImage_buzzer);
        LCD_ShowChinese(40, 66, "过温保护", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowPicture(0, 66, 32, 32, gImage_temp);
        LCD_ShowChinese(40, 99, "快充输入", BLACK, LIGHTBLUE, 32, 0);
        LCD_ShowPicture(0, 99, 32, 32, gImage_PDinput);
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

//增加光标变量，光标非最大或最小值时光标移动，反正则菜单项移动
void presset_page_ui_process(menu_u8 index)
{
    current_menu_index = PRESSET_PAGE;
    LCD_Clear();
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
