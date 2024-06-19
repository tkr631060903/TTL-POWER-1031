#include "menu.h"
#include "UART_Debug.h"

Key_Index sub_index;
menu_i32 current_menu_index;

// 菜单操作表定义
static OP_MENU_PAGE g_opStruct[] =
{
    {MAIN_PAGE, main_page_process},
    {MAIN_MENU_PAGE, main_menu_page_process},
    {VOUT_PAGE, vout_page_process},
    {DETECT_PAGE, detect_page_process},
    {LOG_PAGE, log_page_process},
    {VOLUME_PAGE, volume_page_process},
    {BRIGNE_PAGE, backlight_page_process},
    {WLAN_PAGE, wlan_page_process},
    {LANGUAGE_PAGE, language_page_process},
    {SENSIVITY_PAGE, sensivity_page_process},
};

// 跳转到表所对应的页面
static int JUMP_Table(menu_i32 op, menu_u8 KeyValue)
{
    if (op >= sizeof(g_opStruct) / sizeof(g_opStruct[0]) || op < 0)
    {
        printf("unknow operate!\n");
        return -1;
    }
    g_opStruct[op].opfun(KeyValue);
    return 0;
}

// 菜单选择项
// void Menu_Select_Item(menu_i32 current_menu_index, menu_u8 KeyValue)
// {
//     JUMP_Table(current_menu_index, KeyValue);
// }
void Menu_Select_Item(menu_u8 KeyValue)
{
    JUMP_Table(current_menu_index, KeyValue);
}

void main_page_process(menu_u8 KeyValue)
{
    switch (KeyValue)
    {
    case KEY2_LONG:
        main_menu_Init();
        break;
    case KEY2_SHORT:
        current_menu_index = VOUT_PAGE;
        vout_page_ui_process(KeyValue);
        break;
    case KEY1_SHORT:
        Enter_Page(sub_index.main_current_index, KeyValue);
        break;
    default:
        break;
    }
}

// 主菜单页面处理
void main_menu_page_process(menu_u8 KeyValue)
{
    current_menu_index = MAIN_MENU_PAGE;
    switch (KeyValue)
    {
    case LEFT:
        (sub_index.main_current_index > 0) ? (sub_index.main_current_index--) : (sub_index.main_current_index = 0);
        main_page_ui_process(sub_index.main_current_index);
        break;
    case RIGHT:
        (sub_index.main_current_index < 6) ? (sub_index.main_current_index++) : (sub_index.main_current_index = 6);
        main_page_ui_process(sub_index.main_current_index);
        break;

        // 在主页面的时候，短按ENTER按键进入对应的子页面
    case ENTER_SHORT:
        Enter_Page(sub_index.main_current_index, KeyValue);
        break;

        // 在主页面的时候，只要触发了ENTER按键长按即回到开机对应的页面
    case KEY2_LONG:
        Enter_Page(MAIN_PAGE, KeyValue);
        break;
    default:
        break;
    }
}

void vout_page_process(menu_u8 KeyValue)
{
    current_menu_index = VOUT_PAGE;
    switch (KeyValue)
    {
    case LEFT:
        set_vout(KeyValue);
        vout_page_ui_process(KeyValue);
        break;
    case RIGHT:
        set_vout(KeyValue);
        vout_page_ui_process(KeyValue);
        break;
    // case KEY2_SHORT:
    //     vout_page_ui_process(KeyValue);
    //     break;
    case KEY2_LONG:
        Enter_Page(MAIN_PAGE, KeyValue);
        break;
    case KEY3_SHORT:
        //返回上一级
        break;
    case KEY4_SHORT:
        vout_page_ui_process(KeyValue);
        set_vout(KeyValue);
        break;
    default:
        break;
    }
}

void detect_page_process(menu_u8 KeyValue)
{
    // 检测状态机的业务实现
}

void log_page_process(menu_u8 KeyValue)
{
    switch (KeyValue)
    {
    case LEFT:

        break;

    case RIGHT:

        break;

    case ENTER_LONG:

        break;
    }
}

// 音量处理
void volume_page_process(menu_u8 KeyValue)
{
    switch (KeyValue)
    {
    case LEFT:
        (sub_index.volume_level > 0) ? (sub_index.volume_level--) : (sub_index.volume_level = 0);
        volume_page_ui_process(sub_index.volume_level);
        setting_volume(sub_index.volume_level);
        break;

    case RIGHT:
        (sub_index.volume_level < 4) ? (sub_index.volume_level++) : (sub_index.volume_level = 4);
        volume_page_ui_process(sub_index.volume_level);
        setting_volume(sub_index.volume_level);
        break;

    case ENTER_LONG:
        Enter_Page(MAIN_MENU_PAGE, KeyValue);
        main_menu_Init();
        break;
    default:
        break;
    }
}

// 背光处理
void backlight_page_process(menu_u8 KeyValue)
{
    switch (KeyValue)
    {
    case LEFT:
        (sub_index.backlight_level > 0) ? (sub_index.backlight_level--) : (sub_index.backlight_level = 0);
        backlight_page_ui_process(sub_index.backlight_level);
        setting_backlight(sub_index.backlight_level);
        break;

    case RIGHT:
        (sub_index.backlight_level < 4) ? (sub_index.backlight_level++) : (sub_index.backlight_level = 4);
        backlight_page_ui_process(sub_index.backlight_level);
        setting_backlight(sub_index.backlight_level);
        break;

    case ENTER_LONG:
        Enter_Page(MAIN_MENU_PAGE, KeyValue);
        main_menu_Init();
        break;
    default:
        break;
    }
}

// 无线处理
void wlan_page_process(menu_u8 KeyValue)
{
    switch (KeyValue)
    {
    case LEFT:
        (sub_index.wlan_on_off > 0) ? (sub_index.wlan_on_off--) : (sub_index.wlan_on_off = 0);
        wlan_page_ui_process(sub_index.wlan_on_off);
        setting_wlan_status(sub_index.wlan_on_off);
        break;

    case RIGHT:
        (sub_index.wlan_on_off < 1) ? (sub_index.wlan_on_off++) : (sub_index.wlan_on_off = 1);
        wlan_page_ui_process(sub_index.wlan_on_off);
        setting_wlan_status(sub_index.wlan_on_off);
        break;

    case ENTER_LONG:
        Enter_Page(MAIN_MENU_PAGE, KeyValue);
        main_menu_Init();
        break;

    default:
        break;
    }
}

// 语言处理
void language_page_process(menu_u8 KeyValue)
{
    switch (KeyValue)
    {
    case LEFT:
        (sub_index.lang_select > 0) ? (sub_index.lang_select--) : (sub_index.lang_select = 0);
        language_page_ui_process(sub_index.lang_select);
        setting_language(sub_index.lang_select);
        break;

    case RIGHT:
        (sub_index.lang_select < 1) ? (sub_index.lang_select++) : (sub_index.lang_select = 1);
        language_page_ui_process(sub_index.lang_select);
        setting_language(sub_index.lang_select);
        break;

    case ENTER_LONG:
        Enter_Page(MAIN_MENU_PAGE, KeyValue);
        main_menu_Init();
        break;

    default:
        break;
    }
}

// 灵敏度处理
void sensivity_page_process(menu_u8 KeyValue)
{
    switch (KeyValue)
    {
    case LEFT:
        (sub_index.sensivity_select > 0) ? (sub_index.sensivity_select--) : (sub_index.sensivity_select = 0);
        sensivity_ui_process(sub_index.sensivity_select);
        setting_sensivity(sub_index.sensivity_select);
        break;

    case RIGHT:
        (sub_index.sensivity_select < 2) ? (sub_index.sensivity_select++) : (sub_index.sensivity_select = 2);
        sensivity_ui_process(sub_index.sensivity_select);
        setting_sensivity(sub_index.sensivity_select);
        break;

    case ENTER_LONG:
        Enter_Page(MAIN_MENU_PAGE, KeyValue);
        main_menu_Init();
        break;
    default:
        break;
    }
}
