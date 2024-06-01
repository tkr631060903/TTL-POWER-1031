#include "menu.h"
#include "UART_Debug.h"

Key_Index sub_index;

// �˵���������
static OP_MENU_PAGE g_opStruct[] =
    {
        {MAIN_PAGE, main_page_process},
        {DETECT_PAGE, detect_page_process},
        {LOG_PAGE, log_page_process},
        {VOLUME_PAGE, volume_page_process},
        {BRIGNE_PAGE, backlight_page_process},
        {WLAN_PAGE, wlan_page_process},
        {LANGUAGE_PAGE, language_page_process},
        {SENSIVITY_PAGE, sensivity_page_process},
};

// ��ת��������Ӧ��ҳ��
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

// �˵�ѡ����
void Menu_Select_Item(menu_i32 current_index, menu_u8 KeyValue)
{
    JUMP_Table(current_index, KeyValue);
}

// ��ҳ�洦��
void main_page_process(menu_u8 KeyValue)
{
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

    // ����ҳ���ʱ�򣬶̰�ENTER���������Ӧ����ҳ��
    case ENTER_SHORT:
        Enter_Page(sub_index.main_current_index, KeyValue);
        break;

    // ����ҳ���ʱ��ֻҪ������ENTER�����������ص�������Ӧ��ҳ��
    case ENTER_LONG:
        Enter_Page(MAIN_PAGE, KeyValue);
        Menu_Main_Init();
        break;
    default:
        break;
    }
}

void detect_page_process(menu_u8 KeyValue)
{
    // ���״̬����ҵ��ʵ��
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

// ��������
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
        Enter_Page(MAIN_PAGE, KeyValue);
        Menu_Main_Init();
        break;
    default:
        break;
    }
}

// ���⴦��
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
        Enter_Page(MAIN_PAGE, KeyValue);
        Menu_Main_Init();
        break;
    default:
        break;
    }
}

// ���ߴ���
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
        Enter_Page(MAIN_PAGE, KeyValue);
        Menu_Main_Init();
        break;

    default:
        break;
    }
}

// ���Դ���
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
        Enter_Page(MAIN_PAGE, KeyValue);
        Menu_Main_Init();
        break;

    default:
        break;
    }
}

// �����ȴ���
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
        Enter_Page(MAIN_PAGE, KeyValue);
        Menu_Main_Init();
        break;
    default:
        break;
    }
}
