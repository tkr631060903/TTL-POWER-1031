#include "Hardware_setting.h"

Current_Select_Mode current_mode;

// ��������
void setting_volume(menu_u32 index)
{
    switch (index)
    {
    case 0:
        current_mode.Current_Volume_Level = VOLUME_LEVEL0;
        break;

    case 1:
        current_mode.Current_Volume_Level = VOLUME_LEVEL1;
        break;

    case 2:
        current_mode.Current_Volume_Level = VOLUME_LEVEL2;
        break;

    case 3:
        current_mode.Current_Volume_Level = VOLUME_LEVEL3;
        break;

    case 4:
        current_mode.Current_Volume_Level = VOLUME_LEVEL4;
        break;
    default:
        break;
    }
}

// ���ñ���
void setting_backlight(menu_u32 index)
{
    switch (index)
    {
    case 0:
        current_mode.Current_Backlight_Level = BACKLIGHT_LEVEL0;
        break;

    case 1:
        current_mode.Current_Backlight_Level = BACKLIGHT_LEVEL1;
        break;

    case 2:
        current_mode.Current_Backlight_Level = BACKLIGHT_LEVEL2;
        break;

    case 3:
        current_mode.Current_Backlight_Level = BACKLIGHT_LEVEL3;
        break;

    case 4:
        current_mode.Current_Backlight_Level = BACKLIGHT_LEVEL4;
        break;
    default:
        break;
    }
}

// �������������״̬
void setting_wlan_status(menu_u32 index)
{
    switch (index)
    {
    case 0:
        current_mode.Current_Select_Wlan = WLAN_OFF;
        break;
    case 1:
        current_mode.Current_Select_Wlan = WLAN_OFF;
        break;
    default:
        break;
    }
}

// ��������
void setting_language(menu_u32 index)
{
    switch (index)
    {
    case 0:
        current_mode.Current_Select_Language = CHINESE;
        break;
    case 1:
        current_mode.Current_Select_Language = ENGLISH;
        break;
    default:
        break;
    }
}

// ����������
void setting_sensivity(menu_u32 index)
{
    switch (index)
    {
    case 0:
        current_mode.Current_Sensivity = SENSIVITY_HIGH;
        break;
    case 1:
        current_mode.Current_Sensivity = SENSIVITY_MIDDLE;
        break;
    case 2:
        current_mode.Current_Sensivity = SENSIVITY_LOW;
        break;
    default:
        break;
    }
}
