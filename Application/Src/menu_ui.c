#include "menu_ui.h"

// ������Ӧ��ҳ��
void Enter_Page(menu_i32 index, menu_u8 KeyValue)
{
    sub_index.Current_Page = index;
    switch (sub_index.Current_Page)
    {
    case MAIN_PAGE:
        Menu_Main_Init();
        break;

    case DETECT_PAGE:

        break;

    case LOG_PAGE:

        break;

    // ������������ҳ��
    case VOLUME_PAGE:
        volume_page_ui_process(current_mode.Current_Volume_Level);
        break;

    // ������������ҳ��
    case BRIGNE_PAGE:
        backlight_page_ui_process(current_mode.Current_Backlight_Level);
        break;

    // ������������ҳ��
    case WLAN_PAGE:
        wlan_page_process(current_mode.Current_Select_Wlan);
        break;

    // ������������ҳ��
    case LANGUAGE_PAGE:
        language_page_ui_process(current_mode.Current_Select_Language);
        break;

    // ��������������ҳ��
    case SENSIVITY_PAGE:
        sensivity_page_process(current_mode.Current_Sensivity);
        break;

    default:
        break;
    }
}

// ���˵���ʼ��
void Menu_Main_Init(void)
{
    sub_index.main_current_index = 0;
    sub_index.Current_Page = MAIN_PAGE;
    //.....ˢ�»���ҳ���UI��״̬
}

// ��ҳ��UI����
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

// ��������ui����
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

// ��������ui����
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

// ���������ϴ�ҳ��ui����
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

// ����ҳ��ui����
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

// ������ҳ��ui����
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
