#include "Hardware_setting.h"
#include "Application_SC8815.h"

Current_Select_Mode current_mode;

// ��������
void set_vout(menu_u8 KeyValue)
{
    switch (KeyValue)
    {
    case LEFT:
        SC8815_Config.SC8815_VBUS = SC8815_Config.SC8815_VBUS - SC8815_Config.SC8815_VBUS_IBUS_Step;
        if (SC8815_Config.SC8815_VBUS <= 0)
        {
            SC8815_Config.SC8815_VBUS = 0;
        }
        break;
    case RIGHT:
        SC8815_Config.SC8815_VBUS = SC8815_Config.SC8815_VBUS + SC8815_Config.SC8815_VBUS_IBUS_Step;
        if (SC8815_Config.SC8815_VBUS >= 36000)
        {
            SC8815_Config.SC8815_VBUS = 36000;
        }
        break;
    case KEY4_SHORT:
        SC8815_Config.SC8815_VBUS_Old = SC8815_Config.SC8815_VBUS;
        break;
    default:
        break;
    }
}

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
