#include "Hardware_setting.h"
#include "Application_SC8815.h"
#include "menu.h"
#include "string.h"
#include "husb238.h"
#include "Application_ADC.h"
#include "Application_LCD.h"

/**
 * @brief 设置输出电压
 * 
 * @param KeyValue 触发键值
 */
void set_vout(menu_u8 KeyValue)
{
    uint32_t in_power = (APP_config.fastCharge_InVoltage * APP_config.fastCharge_InCurrent) * 1000000;
    uint32_t out_power = SC8815_Config.SC8815_VBUS * SC8815_Config.SC8815_IBUS_Limit;
    switch (KeyValue)
    {
    case LEFT:
        SC8815_Config.SC8815_VBUS = SC8815_Config.SC8815_VBUS - SC8815_Config.SC8815_VBUS_IBUS_Step;
        if (SC8815_Config.SC8815_VBUS <= 2700)
        {
            SC8815_Config.SC8815_VBUS = 2700;
        }
        App_SC8815_SetOutputVoltage(SC8815_Config.SC8815_VBUS);
        SC8815_Config.SC8815_VBUS_Old = SC8815_Config.SC8815_VBUS;
        app_config_save_config.SC8815_VBUS = SC8815_Config.SC8815_VBUS;
        APP_config.app_config_save_flag = 1;
        APP_config.app_config_save_time = HAL_GetTick();
        break;
    case RIGHT:
        SC8815_Config.SC8815_VBUS = SC8815_Config.SC8815_VBUS + SC8815_Config.SC8815_VBUS_IBUS_Step;
        if (SC8815_Config.SC8815_VBUS >= 36000)
        {
            SC8815_Config.SC8815_VBUS = 36000;
        } else if (out_power > in_power) {
            if (SC8815_Config.SC8815_IBUS_Limit > 300) {
                SC8815_Config.SC8815_IBUS_Limit = in_power / SC8815_Config.SC8815_VBUS;
                if (SC8815_Config.SC8815_IBUS_Limit >= 300) {
                    SC8815_Config.SC8815_IBUS_Limit_Old = SC8815_Config.SC8815_IBUS_Limit;
                    SC8815_SetBusCurrentLimit(SC8815_Config.SC8815_IBUS_Limit);
                    LCD_show_iset();
                } else {
                    SC8815_Config.SC8815_IBUS_Limit = SC8815_Config.SC8815_IBUS_Limit_Old;
                }
            } else {
                SC8815_Config.SC8815_VBUS = SC8815_Config.SC8815_VBUS - SC8815_Config.SC8815_VBUS_IBUS_Step;
            }
        }
        App_SC8815_SetOutputVoltage(SC8815_Config.SC8815_VBUS);
        SC8815_Config.SC8815_VBUS_Old = SC8815_Config.SC8815_VBUS;
        app_config_save_config.SC8815_VBUS = SC8815_Config.SC8815_VBUS;
        APP_config.app_config_save_flag = 1;
        APP_config.app_config_save_time = HAL_GetTick();
        break;
    case KEY1_SHORT:
        if (SC8815_Config.SC8815_VBUS_IBUS_Step == 100) {
            SC8815_Config.SC8815_VBUS_IBUS_Step = 10;
        } else if (SC8815_Config.SC8815_VBUS_IBUS_Step == 1000) {
            SC8815_Config.SC8815_VBUS_IBUS_Step = 100;
        } else if (SC8815_Config.SC8815_VBUS_IBUS_Step == 10000) {
            SC8815_Config.SC8815_VBUS_IBUS_Step = 1000;
        }
        break;
    case KEY2_SHORT:
        if (SC8815_Config.SC8815_VBUS_IBUS_Step == 10) {
            SC8815_Config.SC8815_VBUS_IBUS_Step = 100;
        } else if (SC8815_Config.SC8815_VBUS_IBUS_Step == 100) {
            SC8815_Config.SC8815_VBUS_IBUS_Step = 1000;
        } else if (SC8815_Config.SC8815_VBUS_IBUS_Step == 1000) {
            SC8815_Config.SC8815_VBUS_IBUS_Step = 10000;
        }
        break;
    case KEY3_SHORT:
        SC8815_Config.SC8815_VBUS_IBUS_Step = 1000;
        SC8815_Config.SC8815_VBUS = SC8815_Config.SC8815_VBUS_Old;
        break;
    // case KEY4_SHORT:
    //     SC8815_SetOutputVoltage(SC8815_Config.SC8815_VBUS);
    //     SC8815_Config.SC8815_VBUS_IBUS_Step = 1000;
    //     SC8815_Config.SC8815_VBUS_Old = SC8815_Config.SC8815_VBUS;
    //     app_config_save_config.SC8815_VBUS = SC8815_Config.SC8815_VBUS;
    //     app_config_save();
    //     break;
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
    uint32_t in_power = (APP_config.fastCharge_InVoltage * APP_config.fastCharge_InCurrent) * 1000000;
    uint32_t out_power = SC8815_Config.SC8815_VBUS * SC8815_Config.SC8815_IBUS_Limit;
    switch (KeyValue)
    {
    case LEFT:
        SC8815_Config.SC8815_IBUS_Limit = SC8815_Config.SC8815_IBUS_Limit - SC8815_Config.SC8815_VBUS_IBUS_Step;
        if (SC8815_Config.SC8815_IBUS_Limit <= 300)
        {
            SC8815_Config.SC8815_IBUS_Limit = 300;
        }
        SC8815_SetBusCurrentLimit(SC8815_Config.SC8815_IBUS_Limit);
        SC8815_Config.SC8815_IBUS_Limit_Old = SC8815_Config.SC8815_IBUS_Limit;
        app_config_save_config.SC8815_IBUS_Limit = SC8815_Config.SC8815_IBUS_Limit;
        APP_config.app_config_save_flag = 1;
        APP_config.app_config_save_time = HAL_GetTick();
        break;
    case RIGHT:
        SC8815_Config.SC8815_IBUS_Limit = SC8815_Config.SC8815_IBUS_Limit + SC8815_Config.SC8815_VBUS_IBUS_Step;
        if (SC8815_Config.SC8815_IBUS_Limit >= 6000)
        {
            SC8815_Config.SC8815_IBUS_Limit = 6000;
        } else if (out_power > in_power) {
            if (SC8815_Config.SC8815_VBUS > 2700) {
                SC8815_Config.SC8815_VBUS = in_power / SC8815_Config.SC8815_IBUS_Limit;
                if (SC8815_Config.SC8815_VBUS >= 2700) {
                    SC8815_Config.SC8815_VBUS_Old = SC8815_Config.SC8815_VBUS;
                    App_SC8815_SetOutputVoltage(SC8815_Config.SC8815_VBUS);
                    LCD_show_vset();
                } else {
                    SC8815_Config.SC8815_VBUS = SC8815_Config.SC8815_VBUS_Old;
                }
            } else {
                SC8815_Config.SC8815_IBUS_Limit = SC8815_Config.SC8815_IBUS_Limit - SC8815_Config.SC8815_VBUS_IBUS_Step;
            }
        }
        SC8815_SetBusCurrentLimit(SC8815_Config.SC8815_IBUS_Limit);
        SC8815_Config.SC8815_IBUS_Limit_Old = SC8815_Config.SC8815_IBUS_Limit;
        app_config_save_config.SC8815_IBUS_Limit = SC8815_Config.SC8815_IBUS_Limit;
        APP_config.app_config_save_flag = 1;
        APP_config.app_config_save_time = HAL_GetTick();
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
        break;
    case KEY3_SHORT:
        SC8815_Config.SC8815_VBUS_IBUS_Step = 1000;
        SC8815_Config.SC8815_IBUS_Limit = SC8815_Config.SC8815_IBUS_Limit_Old;
        break;
    // case KEY4_SHORT:
    //     SC8815_SetBusCurrentLimit(SC8815_Config.SC8815_IBUS_Limit);
    //     SC8815_Config.SC8815_VBUS_IBUS_Step = 1000;
    //     SC8815_Config.SC8815_IBUS_Limit_Old = SC8815_Config.SC8815_IBUS_Limit;
    //     app_config_save_config.SC8815_IBUS_Limit = SC8815_Config.SC8815_IBUS_Limit;
    //     app_config_save();
    //     break;
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
    uint8_t count = 0;
    HUSB238_Capability_t PDCapabilities[6];
    HUSB238_ExtractCap(PDCapabilities);
    for (int i = 0; i < 6; i++) {
        if (PDCapabilities[i].detected == true) {
            PDCapabilities[count].detected = PDCapabilities[i].detected;
            PDCapabilities[count].current = PDCapabilities[i].current;
            PDCapabilities[count].voltage = PDCapabilities[i].voltage;
            count++;
        }
    }
    APP_config.fastCharge_InVoltage = PDCapabilities[sub_index.fastch_current_index].voltage;
    APP_config.fastCharge_InCurrent = PDCapabilities[sub_index.fastch_current_index].current;
    SC8815_Config.SC8815_IBAT_Limit = PDCapabilities[sub_index.fastch_current_index].current * 1000;
    uint32_t in_power = APP_config.fastCharge_InVoltage * APP_config.fastCharge_InCurrent * 1000000;
	while (in_power <= SC8815_Config.SC8815_IBUS_Limit * SC8815_Config.SC8815_VBUS) {
		if (SC8815_Config.SC8815_IBUS_Limit > 300) {
			SC8815_Config.SC8815_IBUS_Limit -= 100;
		} else {
			SC8815_Config.SC8815_VBUS -= 100;
		}
    }
    SC8815_Config.SC8815_IBUS_Limit_Old = SC8815_Config.SC8815_IBUS_Limit;
	SC8815_Config.SC8815_VBUS_Old = SC8815_Config.SC8815_VBUS;
    SC8815_Config.SC8815_Status = SC8815_Standby;
	Application_SC8815_Standby();
    App_SC8815_SetOutputVoltage(SC8815_Config.SC8815_VBUS);
    SC8815_SetBatteryCurrLimit(SC8815_Config.SC8815_IBAT_Limit);
    switch (APP_config.fastCharge_InVoltage) {
    case 5:
        HUSB238_SelVoltage(PDO_5V);
        break;
    case 9:
        HUSB238_SelVoltage(PDO_9V);
        break;
    case 12:
        HUSB238_SelVoltage(PDO_12V);
        break;
    case 15:
        HUSB238_SelVoltage(PDO_15V);
        break;
    case 18:
        HUSB238_SelVoltage(PDO_18V);
        break;
    case 20:
        HUSB238_SelVoltage(PDO_20V);
        break;
    default:
        break;
    }
}

/**
 * @brief 设置预设参数
 * 
 * @param KeyValue 触发键值
 */
void set_presset_config(menu_u8 KeyValue)
{
    extern presset_config_set_typeDef presset_config_set;
    switch (KeyValue)
    {
    case LEFT:
        switch (presset_config_set.set_flag)
        {
        case PRESSET_SET_VOUT:
            presset_config_set.set_vbus[presset_config_set.current_index] -= presset_config_set.set_setp;
            if (presset_config_set.set_vbus[presset_config_set.current_index] <= 2700)
            {
                presset_config_set.set_vbus[presset_config_set.current_index] = 2700;
            }
            break;
        case PRESSET_SET_IOUT:
            presset_config_set.set_ibus[presset_config_set.current_index] -= presset_config_set.set_setp;
            if (presset_config_set.set_ibus[presset_config_set.current_index] <= 300)
            {
                presset_config_set.set_ibus[presset_config_set.current_index] = 300;
            }
            break;
        case PRESSET_SET_TIME:
            if (presset_config_set.set_time[presset_config_set.current_index] <= presset_config_set.set_setp)
                presset_config_set.set_time[presset_config_set.current_index] = 0;
            else
                presset_config_set.set_time[presset_config_set.current_index] -= presset_config_set.set_setp;
            break;
        case PRESSET_SET_CIRCULAR:
            if (presset_config_set.set_circular - presset_config_set.set_setp <= 0)
                presset_config_set.set_circular = 0;
            else
                presset_config_set.set_circular -= presset_config_set.set_setp;
            break;
        default:
            break;
        }
        break;
    case RIGHT:
        switch (presset_config_set.set_flag)
        {
        case PRESSET_SET_VOUT:
            presset_config_set.set_vbus[presset_config_set.current_index] += presset_config_set.set_setp;
            if (presset_config_set.set_vbus[presset_config_set.current_index] >= 36000)
            {
                presset_config_set.set_vbus[presset_config_set.current_index] = 36000;
            }
            if (presset_config_set.set_vbus[presset_config_set.current_index] <= 2700) {
                presset_config_set.set_vbus[presset_config_set.current_index] = 2700;
            }
            break;
        case PRESSET_SET_IOUT:
            presset_config_set.set_ibus[presset_config_set.current_index] += presset_config_set.set_setp;
            if (presset_config_set.set_ibus[presset_config_set.current_index] >= 6000)
            {
                presset_config_set.set_ibus[presset_config_set.current_index] = 6000;
            }
            if (presset_config_set.set_ibus[presset_config_set.current_index] <= 300)
            {
                presset_config_set.set_ibus[presset_config_set.current_index] = 300;
            }
            break;
        case PRESSET_SET_TIME:
            presset_config_set.set_time[presset_config_set.current_index] += presset_config_set.set_setp;
            if (presset_config_set.set_time[presset_config_set.current_index] >= 65535)
            {
                presset_config_set.set_time[presset_config_set.current_index] = 65535;
            }
            break;
        case PRESSET_SET_CIRCULAR:
            presset_config_set.set_circular += presset_config_set.set_setp;
            if (presset_config_set.set_circular >= 65535)
            {
                presset_config_set.set_circular = 65535;
            }
            break;
        default:
            break;
        }
        break;
    case KEY1_SHORT:
        if (presset_config_set.set_flag == PRESSET_SET_VOUT || presset_config_set.set_flag == PRESSET_SET_IOUT)
        {
            if (presset_config_set.set_setp == 1000)
                presset_config_set.set_setp = 100;
            else if (presset_config_set.set_setp == 10000)
                presset_config_set.set_setp = 1000;
            break;
        }
        else
        {
            if (presset_config_set.set_setp == 10)
                presset_config_set.set_setp = 1;
            else if (presset_config_set.set_setp == 100)
                presset_config_set.set_setp = 10;
            else if (presset_config_set.set_setp == 1000)
                presset_config_set.set_setp = 100;
            break;
        }
    case KEY2_SHORT:
        if (presset_config_set.set_flag == PRESSET_SET_VOUT || presset_config_set.set_flag == PRESSET_SET_IOUT)
        {
            if (presset_config_set.set_setp == 100)
                presset_config_set.set_setp = 1000;
            else if (presset_config_set.set_setp == 1000)
                presset_config_set.set_setp = 10000;
            break;
        }
        else
        {
            if (presset_config_set.set_setp == 1)
                presset_config_set.set_setp = 10;
            else if (presset_config_set.set_setp == 10)
                presset_config_set.set_setp = 100;
            else if (presset_config_set.set_setp == 100)
                presset_config_set.set_setp = 1000;
            break;
        }
    case KEY4_SHORT:
        memcpy(&SC8815_TIM_Work[sub_index.presset_current_index].circular, &presset_config_set.set_circular, sizeof(uint16_t));
        memcpy(&SC8815_TIM_Work[sub_index.presset_current_index].SC8815_TIM_Work_second, &presset_config_set.set_time, sizeof(uint16_t) * SC8815_TIM_WORK_STEP);
        memcpy(&SC8815_TIM_Work[sub_index.presset_current_index].SC8815_IBUS_Limit, &presset_config_set.set_ibus, sizeof(float) * SC8815_TIM_WORK_STEP);
        memcpy(&SC8815_TIM_Work[sub_index.presset_current_index].SC8815_VBUS, &presset_config_set.set_vbus, sizeof(float) * SC8815_TIM_WORK_STEP);
        SC8815_Preset_Save();
        break;
    default:
        break;
    }
}

void set_dc_limit(menu_u8 KeyValue)
{
    float DC_IBAT_Limit_temp;
    switch (KeyValue) {
    case LEFT:
        app_config_save_config.DC_IBAT_Limit = app_config_save_config.DC_IBAT_Limit - SC8815_Config.SC8815_VBUS_IBUS_Step;
        if (app_config_save_config.DC_IBAT_Limit <= 500) {
            app_config_save_config.DC_IBAT_Limit = 500;
        }
        break;
    case RIGHT:
        app_config_save_config.DC_IBAT_Limit = app_config_save_config.DC_IBAT_Limit + SC8815_Config.SC8815_VBUS_IBUS_Step;
        if (app_config_save_config.DC_IBAT_Limit >= 5500) {
            app_config_save_config.DC_IBAT_Limit = 5500;
        }
        break;
    case KEY1_SHORT:
        if (SC8815_Config.SC8815_VBUS_IBUS_Step == 100) {
            SC8815_Config.SC8815_VBUS_IBUS_Step = 10;
        } else if (SC8815_Config.SC8815_VBUS_IBUS_Step == 1000) {
            SC8815_Config.SC8815_VBUS_IBUS_Step = 100;
        }
        break;
    case KEY2_SHORT:
        if (SC8815_Config.SC8815_VBUS_IBUS_Step == 10) {
            SC8815_Config.SC8815_VBUS_IBUS_Step = 100;
        } else if (SC8815_Config.SC8815_VBUS_IBUS_Step == 100) {
            SC8815_Config.SC8815_VBUS_IBUS_Step = 1000;
        }
        break;
    case KEY4_SHORT:
        SC8815_Config.SC8815_VBUS_IBUS_Step = 1000;
        DC_IBAT_Limit_temp = app_config_save_config.DC_IBAT_Limit;
        SC8815_Config.SC8815_IBAT_Limit = app_config_save_config.DC_IBAT_Limit;
        APP_config.fastCharge_InCurrent = app_config_save_config.DC_IBAT_Limit / 1000;
        APP_config.fastCharge_InVoltage = App_getVBAT_V();
        while ((APP_config.fastCharge_InVoltage * APP_config.fastCharge_InCurrent * 1000000) <= SC8815_Config.SC8815_IBUS_Limit * SC8815_Config.SC8815_VBUS) {
			if (SC8815_Config.SC8815_IBUS_Limit > 300) {
				SC8815_Config.SC8815_IBUS_Limit -= 100;
			} else {
				SC8815_Config.SC8815_VBUS -= 100;
			}
		}
        app_config_load();
        if (DC_IBAT_Limit_temp != app_config_save_config.DC_IBAT_Limit) {
            app_config_save_config.DC_IBAT_Limit = DC_IBAT_Limit_temp;
            app_config_save();
        }
        SC8815_SetBatteryCurrLimit(SC8815_Config.SC8815_IBAT_Limit);
        break;
    default:
        break;
    }
}
