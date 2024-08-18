#include "menu_ui.h"
#include "Application_LCD.h"
#include "Application.h"
#include "pic.h"
#include "string.h"
#include "Application_SC8815.h"

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
static uint8_t secondary_menu_indxe = 0; //二级菜单第一行显示索引
static uint8_t main_menu_indxe = 0; //主菜单第一行显示索引
static Menu_NameTypeDef menu_name[] = {
    {0, gImage_presset, "预设配置"}, {1, gImage_start_presset, "开启预设"}, {2, gImage_temp, "过温保护"}, {3, gImage_PDinput, "快充输入"}, {4, gImage_buzzer, "蜂鸣器"}, {5, gImage_FSW, "开关频率"}
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
        secondary_menu_indxe = 0;
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
        secondary_menu_indxe = 0;
        // presset_show_vlaue[0] = 0;
        // presset_show_vlaue[1] = 1;
        // presset_show_vlaue[2] = 2;
        // presset_show_vlaue[3] = 3;
        current_menu_index = PRESSET_START_PAGE;
        presset_page_ui_process(0, KeyValue);
        break;
    case 2:
        temperature_page_ui_process(APP_config.temperature);
        break;
    case 3:
        fastch_page_ui_process(APP_config.fastCharge_InVoltage);
        break;
    case 4:
        buzzer_page_ui_process(APP_config.lock_buzzer);
        break;
    case 5:
        sub_index.FSW_current_index = SCHWI_FREQ_150KHz;
        FSW_page_ui_process(SC8815_HardwareInitStruct.SW_FREQ);
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
    LCD_Clear();
    if (cursor_main_menu == 3 && KeyValue == RIGHT && main_menu_indxe != (MENU_PAGE_NUM - 4))
    {
        main_menu_indxe++;
    }
    else if (cursor_main_menu == 0 && KeyValue == LEFT && main_menu_indxe != 0)
    {
        main_menu_indxe--;
    }
    if(cursor_main_menu != 3 && KeyValue == RIGHT)
    {
        cursor_main_menu++;
    }
    else if (cursor_main_menu != 0 && KeyValue == LEFT)
    {
        cursor_main_menu--;
    }
    for (size_t  i = 0; i < 4; i++)
    {
        LCD_ShowPicture(0, i * 33, 32, 32, (uint8_t *)menu_name[main_menu_indxe + i].icon);
        if (i == cursor_main_menu)
        {
            LCD_ShowChinese(40, i * 33, (uint8_t *)menu_name[main_menu_indxe + i].menu_name, BLACK, LIGHTBLUE, 32, 0);
        }
        else
        {
            LCD_ShowChinese(40, i * 33, (uint8_t *)menu_name[main_menu_indxe + i].menu_name, LIGHTBLUE, BLACK, 32, 0);
        }
    }
}

void vout_page_ui_process(menu_u8 KeyValue)
{
    current_menu_index = VOUT_PAGE;
    LCD_Clear();
    float temp;
    switch (KeyValue)
    {
    case LEFT:
    case RIGHT:
    case KEY2_SHORT:
        LCD_ShowString(0, 0, "SETVOUT:", LIGHTBLUE, BLACK, 32, 0);
        temp = SC8815_Config.SC8815_VBUS / 1000;
        if (temp < 10)
        {
            LCD_ShowFloatNum(128, 0, temp, 3, LIGHTBLUE, BLACK, 32);
            LCD_ShowChar(192, 0, 'V', LIGHTBLUE, BLACK, 32, 0);
        }
        else {
            LCD_ShowFloatNum(128, 0, temp, 4, LIGHTBLUE, BLACK, 32);
            LCD_ShowChar(208, 0, 'V', LIGHTBLUE, BLACK, 32, 0);
        }
        break;
    default:
        break;
    }
}

void iout_page_ui_process(menu_u8 KeyValue)
{
    current_menu_index = IOUT_PAGE;
    LCD_Clear();
    float temp;
    switch (KeyValue)
    {
    case LEFT:
    case RIGHT:
    case KEY1_SHORT:
        LCD_ShowString(0, 0, "SETIOUT:", LIGHTBLUE, BLACK, 32, 0);
        temp = SC8815_Config.SC8815_IBUS_Limit / 1000;
        if (temp < 10)
        {
            LCD_ShowFloatNum(128, 0, temp, 3, LIGHTBLUE, BLACK, 32);
            LCD_ShowChar(192, 0, 'A', LIGHTBLUE, BLACK, 32, 0);
        }
        else {
            LCD_ShowFloatNum(128, 0, temp, 4, LIGHTBLUE, BLACK, 32);
            LCD_ShowChar(208, 0, 'A', LIGHTBLUE, BLACK, 32, 0);
        }
        break;
    default:
        break;
    }
}

void presset_page_ui_process(menu_u8 index, menu_u8 KeyValue)
{
    char presset_str[10];
    LCD_Clear();
	if (cursor_secondary_menu == 3 && KeyValue == RIGHT && secondary_menu_indxe != (SC8815_TIM_WORK_SIZE - 4))
    {
        secondary_menu_indxe++;
    }
    else if (cursor_secondary_menu == 0 && KeyValue == LEFT && secondary_menu_indxe != 0)
    {
        secondary_menu_indxe--;
    }
    if(cursor_secondary_menu != 3 && KeyValue == RIGHT)
    {
        cursor_secondary_menu++;
    }
    else if (cursor_secondary_menu != 0 && KeyValue == LEFT)
    {
        cursor_secondary_menu--;
    }
    for (size_t  i = 0; i < 4; i++)
    {
        memset(presset_str, 0, sizeof(presset_str));
        sprintf(presset_str, ":%d", secondary_menu_indxe + i);
        if (i == cursor_secondary_menu)
        {
            LCD_ShowChinese(0, i * 33, "预设", BLACK, LIGHTBLUE, 32, 0);
            LCD_ShowString(64, i * 33, (uint8_t *)presset_str, BLACK, LIGHTBLUE, 32, 0);
        }
        else
        {
            LCD_ShowChinese(0, i * 33, "预设", LIGHTBLUE, BLACK, 32, 0);
            LCD_ShowString(64, i * 33, (uint8_t *)presset_str, LIGHTBLUE, BLACK, 32, 0);
        }
    }
}

void presset_config_page_ui_process(menu_u8 index)
{
    current_menu_index = PRESSET_CONFIG_PAGE;
    LCD_Clear();
    extern SC8815_TIM_WorkTypeDef SC8815_TIM_Work[SC8815_TIM_WORK_SIZE];
    char str[10];
    sprintf(str, "vset:%.2fv", SC8815_TIM_Work[sub_index.presset_current_index].SC8815_VBUS[index] / 1000);
    LCD_ShowString(0, 0, (uint8_t *)str, LIGHTBLUE, BLACK, 32, 0);
    memset(str, 0, 10);
    sprintf(str, "iset:%.2fA", SC8815_TIM_Work[sub_index.presset_current_index].SC8815_IBUS_Limit[index] / 1000);
    LCD_ShowString(0, 33, (uint8_t *)str, LIGHTBLUE, BLACK, 32, 0);
    memset(str, 0, 10);
    sprintf(str, "time:%ds", SC8815_TIM_Work[sub_index.presset_current_index].SC8815_TIM_Work_second[index]);
    LCD_ShowString(0, 66, (uint8_t *)str, LIGHTBLUE, BLACK, 32, 0);
    memset(str, 0, 10);
    sprintf(str, "loop:%d", SC8815_TIM_Work[sub_index.presset_current_index].circular);
    LCD_ShowString(0, 99, (uint8_t *)str, LIGHTBLUE, BLACK, 16, 0);
    memset(str, 0, 10);
    sprintf(str, "step:%d/29", index);
    LCD_ShowString(100, 99, (uint8_t *)str, LIGHTBLUE, BLACK, 16, 0);
}

void presset_config_set_page_ui_process(menu_u8 KeyValue)
{
    current_menu_index = PRESSET_CONFIG_SET_PAGE;
    extern presset_config_set_typeDef presset_config_set;
    LCD_Clear();
    switch (KeyValue)
    {
    case LEFT:
    case RIGHT:
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
    LCD_Clear();
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
    LCD_Clear();
    if (index == TEMPERATURE_45)
    {
        LCD_ShowChinese(0, 0, "过温保护", BLACK, LIGHTBLUE, 32, 0);
        LCD_ShowString(128, 0, ":45", BLACK, LIGHTBLUE, 32, 0);
        LCD_ShowChinese(0, 33, "过温保护", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowString(128, 33, ":55", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowChinese(0, 66, "过温保护", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowString(128, 66, ":75", LIGHTBLUE, BLACK, 32, 0);
    }
    else if (index == TEMPERATURE_55)
    {
        LCD_ShowChinese(0, 0, "过温保护", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowString(128, 0, ":45", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowChinese(0, 33, "过温保护", BLACK, LIGHTBLUE, 32, 0);
        LCD_ShowString(128, 33, ":55", BLACK, LIGHTBLUE, 32, 0);
        LCD_ShowChinese(0, 66, "过温保护", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowString(128, 66, ":75", LIGHTBLUE, BLACK, 32, 0);
    }
    else if (index == TEMPERATURE_65)
    {
        LCD_ShowChinese(0, 0, "过温保护", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowString(128, 0, ":45", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowChinese(0, 33, "过温保护", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowString(128, 33, ":55", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowChinese(0, 66, "过温保护", BLACK, LIGHTBLUE, 32, 0);
        LCD_ShowString(128, 66, ":75", BLACK, LIGHTBLUE, 32, 0);
    }
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

void FSW_page_ui_process(menu_u8 index)
{
    current_menu_index = FSW_PAGE;
    sub_index.FSW_current_index = index;
    LCD_Clear();
    switch (index)
    {
    case SCHWI_FREQ_150KHz:
        LCD_ShowChinese(0, 0, "升压降压", BLACK, LIGHTBLUE, 32, 0);
        LCD_ShowString(128, 0, ":150KHz", BLACK, LIGHTBLUE, 32, 0);
        LCD_ShowChinese(0, 33, "升降", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowString(64, 33, ":300/150KHz", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowChinese(0, 66, "升压降压", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowString(128, 66, ":300KHz", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowChinese(0, 99, "升压降压", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowString(128, 99, ":450KHz", LIGHTBLUE, BLACK, 32, 0);
        break;
    case SCHWI_FREQ_300KHz_1:
        LCD_ShowChinese(0, 0, "升压降压", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowString(128, 0, ":150KHz", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowChinese(0, 33, "升降", BLACK, LIGHTBLUE, 32, 0);
        LCD_ShowString(64, 33, ":300/150KHz", BLACK, LIGHTBLUE, 32, 0);
        LCD_ShowChinese(0, 66, "升压降压", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowString(128, 66, ":300KHz", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowChinese(0, 99, "升压降压", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowString(128, 99, ":450KHz", LIGHTBLUE, BLACK, 32, 0);
        break;
    case SCHWI_FREQ_300KHz_2:
        LCD_ShowChinese(0, 0, "升压降压", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowString(128, 0, ":150KHz", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowChinese(0, 33, "升降", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowString(64, 33, ":300/150KHz", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowChinese(0, 66, "升压降压", BLACK, LIGHTBLUE, 32, 0);
        LCD_ShowString(128, 66, ":300KHz", BLACK, LIGHTBLUE, 32, 0);
        LCD_ShowChinese(0, 99, "升压降压", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowString(128, 99, ":450KHz", LIGHTBLUE, BLACK, 32, 0);
        break;
    case SCHWI_FREQ_450KHz:
        LCD_ShowChinese(0, 0, "升压降压", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowString(128, 0, ":150KHz", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowChinese(0, 33, "升降", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowString(64, 33, ":300/150KHz", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowChinese(0, 66, "升压降压", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowString(128, 66, ":300KHz", LIGHTBLUE, BLACK, 32, 0);
        LCD_ShowChinese(0, 99, "升压降压", BLACK, LIGHTBLUE, 32, 0);
        LCD_ShowString(128, 99, ":450KHz", BLACK, LIGHTBLUE, 32, 0);
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
    case 0:
        // LCD_ShowChinese(0, 0, "升压降压", BLACK, LIGHTBLUE, 32, 0);
        LCD_ShowString(50, 50, "VBUSPORT!", RED, BLACK, 32, 0);
        break;
    case 1:
        // LCD_ShowChinese(0, 0, "升压降压", BLACK, LIGHTBLUE, 32, 0);
        LCD_ShowString(50, 50, "TEMPPORT!", RED, BLACK, 32, 0);
        break;
    case 2:
        // LCD_ShowChinese(0, 0, "升压降压", BLACK, LIGHTBLUE, 32, 0);
        LCD_ShowString(50, 50, "VBATPORT!", RED, BLACK, 32, 0);
        break;
    default:
        break;
    }
}
