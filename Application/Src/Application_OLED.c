#include "Application_OLED.h"
#include "Application_ADC.h"
#include "Application_SC8815.h"

extern volatile Application_Config APP_config;

void APP_OLED_ShowVIN(uint8_t line)
{
    OLED_ShowString(line, 1, "VIN:");
    OLED_ShowNum(line, 5, APP_config.fastCharge_InVoltage, 2);
    OLED_ShowString(line, 7, "V");
}

void APP_OLED_ShowVSET(uint8_t line)
{
    OLED_ShowString(line, 1, "VSET:");
    if (APP_config.VOUT / 1000 >= 10)
    {
        OLED_ShowNum(line, 6, APP_config.VOUT / 1000, 2);
        OLED_ShowString(line, 8, ".");
        OLED_ShowNum(line, 9, APP_config.VOUT / 100, 1);
        OLED_ShowNum(line, 10, APP_config.VOUT / 10, 1);
        OLED_ShowString(line, 11, "V");
    }
    else {
        OLED_ShowNum(line, 6, APP_config.VOUT / 1000, 1);
        OLED_ShowString(line, 7, ".");
        OLED_ShowNum(line, 8, APP_config.VOUT / 100, 1);
        OLED_ShowNum(line, 9, APP_config.VOUT / 10, 1);
        OLED_ShowString(line, 10, "V");
    }
}

void APP_OLED_ShowISET(uint8_t line)
{
    OLED_ShowString(line, 1, "ISET:");
    if (APP_config.SC8815_VBUS_Current_Limit / 1000 >= 10)
    {
        OLED_ShowNum(line, 6, APP_config.SC8815_VBUS_Current_Limit / 1000, 2);
        OLED_ShowString(line, 8, ".");
        OLED_ShowNum(line, 9, APP_config.SC8815_VBUS_Current_Limit / 100, 1);
        OLED_ShowNum(line, 10, APP_config.SC8815_VBUS_Current_Limit / 10, 1);
        OLED_ShowString(line, 11, "A");
    }
    else {
        OLED_ShowNum(line, 6, APP_config.SC8815_VBUS_Current_Limit / 1000, 1);
        OLED_ShowString(line, 7, ".");
        OLED_ShowNum(line, 8, APP_config.SC8815_VBUS_Current_Limit / 100, 1);
        OLED_ShowNum(line, 9, APP_config.SC8815_VBUS_Current_Limit / 10, 1);
        OLED_ShowString(line, 10, "A");
    }
}

void APP_OLED_ShowVOUT(uint8_t line)
{
    OLED_ShowString(line, 1, "VOUT:");
    uint16_t temp = App_getVBUS_mV();
    // uint16_t temp = SC8815_Read_VBUS_Voltage();
    if (temp / 1000 >= 10)
    {
        OLED_ShowNum(line, 6, temp / 1000, 2);
        OLED_ShowString(line, 8, ".");
        OLED_ShowNum(line, 9, temp / 100, 1);
        OLED_ShowNum(line, 10, temp / 10, 1);
        OLED_ShowString(line, 11, "V");
    }
    else {
        OLED_ShowNum(line, 6, temp / 1000, 1);
        OLED_ShowString(line, 7, ".");
        OLED_ShowNum(line, 8, temp / 100, 1);
        OLED_ShowNum(line, 9, temp / 10, 1);
        OLED_ShowString(line, 10, "V");
    }
}

void APP_OLED_ShowIOUT(uint8_t line)
{
    OLED_ShowString(line, 1, "IOUT:");
    uint16_t temp = SC8815_Read_VBUS_Current();
    if (temp / 1000 >= 10)
    {
        OLED_ShowNum(line, 6, temp / 1000, 2);
        OLED_ShowString(line, 8, ".");
        OLED_ShowNum(line, 9, temp / 100, 1);
        OLED_ShowNum(line, 10, temp / 10, 1);
        OLED_ShowString(line, 11, "A");
    }
    else {
        OLED_ShowNum(line, 6, temp / 1000, 1);
        OLED_ShowString(line, 7, ".");
        OLED_ShowNum(line, 8, temp / 100, 1);
        OLED_ShowNum(line, 9, temp / 10, 1);
        OLED_ShowString(line, 10, "A");
    }
}

void APP_OLED_Show_SETVOUT(void)
{
    // OLED_Clear_Part(2, 1, 16);
    // OLED_Clear_Part(3, 1, 16);
    // OLED_Clear_Part(4, 1, 16);
    OLED_ShowString(1, 1, "SETVOUT:");
    uint16_t temp = APP_config.VOUT;
    if (temp / 1000 >= 10)
    {
        OLED_ShowNum(1, 9, temp / 1000, 2);
        OLED_ShowString(1, 11, ".");
        OLED_ShowNum(1, 12, temp / 100, 1);
        OLED_ShowNum(1, 13, temp / 10, 1);
        OLED_ShowString(1, 14, "V");
    }
    else {
        OLED_ShowNum(1, 9, temp / 1000, 1);
        OLED_ShowString(1, 10, ".");
        OLED_ShowNum(1, 11, temp / 100, 1);
        OLED_ShowNum(1, 12, temp / 10, 1);
        OLED_ShowString(1, 13, "V");
    }
}

void APP_OLED_Show_SETIOUT(void)
{
    OLED_ShowString(1, 1, "SETIOUT:");
    uint16_t temp = APP_config.SC8815_VBUS_Current_Limit;
    if (temp / 1000 >= 10)
    {
        OLED_ShowNum(1, 9, temp / 1000, 2);
        OLED_ShowString(1, 11, ".");
        OLED_ShowNum(1, 12, temp / 100, 1);
        OLED_ShowNum(1, 13, temp / 10, 1);
        OLED_ShowString(1, 14, "A");
    }
    else {
        OLED_ShowNum(1, 9, temp / 1000, 1);
        OLED_ShowString(1, 10, ".");
        OLED_ShowNum(1, 11, temp / 100, 1);
        OLED_ShowNum(1, 12, temp / 10, 1);
        OLED_ShowString(1, 13, "A");
    }
}

void APP_OLED_Show_VINProtectMod(void)
{
    OLED_ShowString(1, 1, "VINProtect");
}

void APP_OLED_Show_VOUTProtectMod(void)
{
    OLED_ShowString(1, 1, "VOUTProtect");
}

void APP_OLED_Show_fastChargeMod(void)
{
    OLED_ShowString(1, 1, "fastCharge");
}

uint32_t OLEDFlushTime = 0;
void APP_OLED_Show(void)
{
    switch (APP_config.SetMod)
    {
    case noneMod:
        if (OLEDFlushTime == 0)
        {
            OLEDFlushTime = HAL_GetTick();
        }
        else if (HAL_GetTick() - OLEDFlushTime >= 300)
        {
            OLEDFlushTime = 0;
            APP_OLED_ShowVSET(1);
            APP_OLED_ShowISET(2);
            APP_OLED_ShowVOUT(3);
            APP_OLED_ShowIOUT(4);
        }
        break;
    case voltageMod:
        APP_OLED_Show_SETVOUT();
        break;
    case currentMod:
        APP_OLED_Show_SETIOUT();
        break;
    case VINProtectMod:
        APP_OLED_Show_VINProtectMod();
        break;
    case VOUTProtectMod:
        APP_OLED_Show_VOUTProtectMod();
        break;
    case fastChargeMod:
        APP_OLED_Show_fastChargeMod();
        break;
    default:
        break;
    }
}
