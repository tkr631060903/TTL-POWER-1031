#include "Hardware_setting.h"
#include "Application_SC8815.h"
#include "CH224K.h"

/**
 * @brief 设置输出电压
 * 
 * @param KeyValue 触发键值
 */
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
    case KEY1_SHORT:
        if (SC8815_Config.SC8815_VBUS_IBUS_Step == 1000)
        {
            SC8815_Config.SC8815_VBUS_IBUS_Step = 100;
        }
        else if (SC8815_Config.SC8815_VBUS_IBUS_Step == 10000)
        {
            SC8815_Config.SC8815_VBUS_IBUS_Step = 1000;
        }
        break;
    case KEY2_SHORT:
        if (SC8815_Config.SC8815_VBUS_IBUS_Step == 100)
        {
            SC8815_Config.SC8815_VBUS_IBUS_Step = 1000;
        }
        else if (SC8815_Config.SC8815_VBUS_IBUS_Step == 1000)
        {
            SC8815_Config.SC8815_VBUS_IBUS_Step = 10000;
        }
        break;
    case KEY3_SHORT:
        SC8815_Config.SC8815_VBUS_IBUS_Step = 1000;
        SC8815_Config.SC8815_VBUS = SC8815_Config.SC8815_VBUS_Old;
        break;
    case KEY4_SHORT:
        SC8815_SetOutputVoltage(SC8815_Config.SC8815_VBUS);
        SC8815_Config.SC8815_VBUS_IBUS_Step = 1000;
        SC8815_Config.SC8815_VBUS_Old = SC8815_Config.SC8815_VBUS;
        break;
    default:
        break;
    }
}

/**
 * @brief 设置输出电流
 * 
 * @param KeyValue 触发键值
 */
void set_iout(menu_u8 KeyValue)
{
    switch (KeyValue)
    {
    case LEFT:
        SC8815_Config.SC8815_IBUS_Limit = SC8815_Config.SC8815_IBUS_Limit - SC8815_Config.SC8815_VBUS_IBUS_Step;
        if (SC8815_Config.SC8815_IBUS_Limit <= 300)
        {
            SC8815_Config.SC8815_IBUS_Limit = 300;
        }
        break;
    case RIGHT:
        SC8815_Config.SC8815_IBUS_Limit = SC8815_Config.SC8815_IBUS_Limit + SC8815_Config.SC8815_VBUS_IBUS_Step;
        if (SC8815_Config.SC8815_IBUS_Limit >= 6000)
        {
            SC8815_Config.SC8815_IBUS_Limit = 6000;
        }
        break;
    case KEY1_SHORT:
        if (SC8815_Config.SC8815_VBUS_IBUS_Step == 1000)
        {
            SC8815_Config.SC8815_VBUS_IBUS_Step = 100;
        }
        else if (SC8815_Config.SC8815_VBUS_IBUS_Step == 10000)
        {
            SC8815_Config.SC8815_VBUS_IBUS_Step = 1000;
        }
        break;
    case KEY2_SHORT:
        if (SC8815_Config.SC8815_VBUS_IBUS_Step == 100)
        {
            SC8815_Config.SC8815_VBUS_IBUS_Step = 1000;
        }
        else if (SC8815_Config.SC8815_VBUS_IBUS_Step == 1000)
        {
            SC8815_Config.SC8815_VBUS_IBUS_Step = 10000;
        }
        break;
    case KEY3_SHORT:
        SC8815_Config.SC8815_VBUS_IBUS_Step = 1000;
        SC8815_Config.SC8815_IBUS_Limit = SC8815_Config.SC8815_IBUS_Limit_Old;
        break;
    case KEY4_SHORT:
        SC8815_SetBusCurrentLimit(SC8815_Config.SC8815_IBUS_Limit);
        SC8815_Config.SC8815_VBUS_IBUS_Step = 1000;
        SC8815_Config.SC8815_IBUS_Limit_Old = SC8815_Config.SC8815_IBUS_Limit;
        break;
    default:
        break;
    }
}

/**
 * @brief 设置快充输入电压
 * 
 * @param index 快充电压
 */
void set_fastcharge(menu_u32 index)
{
    switch (index)
    {
    case 5:
        CH224K_5V();
        break;
    case 9:
        CH224K_9V();
        break;
    case 12:
        CH224K_12V();
        break;
    case 15:
        CH224K_15V();
        break;
    case 20:
        CH224K_20V();
        break;
    default:
        break;
    }
}
