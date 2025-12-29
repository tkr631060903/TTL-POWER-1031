#include "fast_charge.h"
#include "husb238.h"
#include "CH224Q.h"
#include "Application_ADC.h"
#include "Application_SC8815.h"
#include "Application_LCD.h"
#include "menu_ui.h"
#include <string.h>

uint8_t fast_charge_input_limited = 26;
uint8_t fast_ic_type = 0xFF;    //0为CH224Q, 1为HUSB238

static uint8_t refresh_count = 0;
static uint32_t last_refresh_time = 0;

void HUSB238_Refresh_Voltage(void)
{
    if (refresh_count > 10 || HAL_GetTick() - last_refresh_time < 5000) {
        return;
    }
    refresh_count++;
    if (fast_ic_type != 1) {
        refresh_count++;
        return;
    }
    uint8_t index = 0;
    float PD_Power = 0;
    HUSB238_SELECT_Voltage_e votlage[6] = { PDO_5V, PDO_9V, PDO_12V, PDO_15V, PDO_18V, PDO_20V };
    HUSB238_Capability_t PDCapabilities[6];
    HUSB238_ExtractCap(PDCapabilities);
    for (int i = 0; i < 6; i++) {
        if (PDCapabilities[i].detected == true && PDCapabilities[i].voltage * PDCapabilities[i].current >= PD_Power) {
            PD_Power = PDCapabilities[i].voltage * PDCapabilities[i].current;
            index = i;
        }
    }
    if (PD_Power > 0 && PDCapabilities[index].voltage > APP_config.fastCharge_InVoltage) {
        APP_config.fastCharge_InVoltage = PDCapabilities[index].voltage;
        APP_config.fastCharge_InCurrent = PDCapabilities[index].current;
        SC8815_Config.SC8815_IBAT_Limit = PDCapabilities[index].current * 1000;
        HUSB238_SelVoltage(votlage[index]);
    }
    last_refresh_time = HAL_GetTick();
}

void fast_charge_init(void)
{
    extern Application_SaveConfig app_config_save_config;
    // PD 芯片相关变量
    uint8_t index = 0, flag = 1;
    float PD_Power = 0, voltage = 0, current = 0;
    char str[16];
    HUSB238_SELECT_Voltage_e votlage[6] = { PDO_5V, PDO_9V, PDO_12V, PDO_15V, PDO_18V, PDO_20V };
    HUSB238_Capability_t PDCapabilities[6] = {0};

    uint8_t status = get_ch224q_status();

    if (status == ACTICATE_NONE) {
        flag = HUSB238_ExtractCap(PDCapabilities);
        for (int i = 0; i < 6; i++) {
            if (PDCapabilities[i].detected == true && PDCapabilities[i].voltage * PDCapabilities[i].current >= PD_Power) {
                // printf("| 输出电压: %dV | 输出电流: %.2fA\n", PDCapabilities[i].voltage, PDCapabilities[i].current);
                voltage = PDCapabilities[i].voltage;
                current = PDCapabilities[i].current;
                PD_Power = PDCapabilities[i].voltage * PDCapabilities[i].current;
                index = i;
            }
        }
        if (flag) {
            fast_ic_type = 1;
        } else {
            fast_ic_type = 0;
        }
    } else {
        if (status == ACTICATE_TRUE) {
            current = get_PD_current() / 1000;
        }
        voltage = App_getVBAT_V();
        PD_Power = (int)voltage * (int)current;
        refresh_count = 11;
        fast_charge_input_limited = 30;
        fast_ic_type = 0;
    }

    if (PD_Power > 0) {
#ifdef ENABLE_EN_FONT
        LCD_ShowString(32, 32, "FastCharge", WHITE, BLACK, 32, 0);
#else
        LCD_ShowChinese(20, 32, "快充输入能力", WHITE, BLACK, 32, 0);
#endif
        if (PD_Power < 100) {
            sprintf(str, "%dV %.1fA %.1fW", (int)voltage, current, PD_Power);
        } else {
            sprintf(str, "%dV %.1fA %.0fW", (int)voltage, current, PD_Power);
        }
        LCD_ShowString(6, 80, (const uint8_t*)str, WHITE, BLACK, 32, 0);
        APP_config.fastCharge_InVoltage = voltage;
        APP_config.fastCharge_InCurrent = current;
        SC8815_Config.SC8815_IBAT_Limit = current * 1000;
        HAL_Delay(1000);
        if (status == ACTICATE_NONE) {
            HUSB238_SelVoltage(votlage[index]);
        }
    } else {
        SC8815_Config.SC8815_VBUS_IBUS_Step = 1000;
        if (flag) {
            fast_charge_input_limited = 26;
        } else {
            fast_charge_input_limited = 30;
        }
#ifdef ENABLE_EN_FONT
        LCD_ShowString(16, 32, "InputCurrent", WHITE, BLACK, 32, 0);
#else
        LCD_ShowChinese(20, 32, "设置输入电流", WHITE, BLACK, 32, 0);
#endif        
        if (app_config_save_config.is_set_SC8815_IBUS_Limit) {
            app_config_save_config.DC_IBAT_Limit = 5500;
            SC8815_Config.SC8815_IBAT_Limit = app_config_save_config.DC_IBAT_Limit;
            APP_config.fastCharge_InCurrent = app_config_save_config.DC_IBAT_Limit / 1000;
            APP_config.fastCharge_InVoltage = App_getVBAT_V();
        } else {
            DC_limit_page_ui_process(KEY2_SHORT);
        }
    }
}

//0为CH224Q, 1为HUSB238
uint8_t get_fast_ic_type(void)
{
    return fast_ic_type;
}
