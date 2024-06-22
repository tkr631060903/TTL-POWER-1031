#include "Application_LCD.h"
#include "Application_ADC.h"
#include "Application_SC8815.h"

void LCD_show(void)
{
    LCD_ShowString(0, 0, "VBAT:", RED, BLACK, 32, 0);
    LCD_ShowFloatNum(80, 0, App_getVBAT_V(), 4, RED, BLACK, 32);
    LCD_ShowChar(160, 0, 'V', RED, BLACK, 32, 0);
    LCD_ShowString(0, 33, "VBUS:", RED, BLACK, 32, 0);
    LCD_ShowFloatNum(80, 33, App_getVBUS_V(), 4, RED, BLACK, 32);
    LCD_ShowChar(160, 33, 'V', RED, BLACK, 32, 0);
    LCD_ShowString(0, 66, "IBUS:", RED, BLACK, 32, 0);
    LCD_ShowFloatNum(80, 66, App_getIBUS_mA(), 4, RED, BLACK, 32);
    LCD_ShowString(160, 66, "mA", RED, BLACK, 32, 0);
    LCD_ShowString(0, 99, "88VBUS:", RED, BLACK, 32, 0);
    LCD_ShowIntNum(110, 99, SC8815_Read_VBUS_Voltage(), 5, RED, BLACK, 32);
    LCD_ShowString(190, 99, "mV", RED, BLACK, 32, 0);
}

extern Application_Config APP_config;

void APP_LCD_ShowVIN()
{
    LCD_ShowString(0, 0, "VIN:", RED, BLACK, 32, 0);
    LCD_ShowIntNum(60, 0, APP_config.fastCharge_InVoltage, 2, RED, BLACK, 32);
    LCD_ShowChar(100, 0, 'V', RED, BLACK, 32, 0);
}

void APP_LCD_ShowVSET()
{
    LCD_ShowString(0, 0, "VSET:", RED, BLACK, 32, 0);
    float temp = SC8815_Config.SC8815_VBUS / 1000;
    if (temp < 10)
    {
        LCD_ShowFloatNum(80, 0, temp, 3, RED, BLACK, 32);
        LCD_ShowChar(150, 0, 'V', RED, BLACK, 32, 0);
    }
    else {
        LCD_ShowFloatNum(80, 0, temp, 4, RED, BLACK, 32);
        LCD_ShowChar(160, 0, 'V', RED, BLACK, 32, 0);
    }
}

void APP_LCD_ShowISET()
{
    LCD_ShowString(0, 33, "ISET:", RED, BLACK, 32, 0);
    float temp = SC8815_Config.SC8815_IBUS_Limit / 1000;
    if (temp < 10)
    {
        LCD_ShowFloatNum(80, 33, temp, 3, RED, BLACK, 32);
        LCD_ShowChar(150, 33, 'A', RED, BLACK, 32, 0);
    }
    else {
        LCD_ShowFloatNum(80, 33, temp, 4, RED, BLACK, 32);
        LCD_ShowChar(160, 33, 'A', RED, BLACK, 32, 0);
    }
}

void APP_LCD_ShowVOUT()
{
    LCD_ShowString(0, 66, "VOUT:", RED, BLACK, 32, 0);
    float temp = App_getVBUS_V();
    if (temp < 10)
    {
        LCD_ShowFloatNum(80, 66, temp, 3, RED, BLACK, 32);
        LCD_ShowChar(150, 66, 'V', RED, BLACK, 32, 0);
    }
    else {
        LCD_ShowFloatNum(80, 66, temp, 4, RED, BLACK, 32);
        LCD_ShowChar(160, 66, 'V', RED, BLACK, 32, 0);
    }
}

void APP_LCD_ShowIOUT()
{
    LCD_ShowString(0, 99, "IOUT:", RED, BLACK, 32, 0);
    float temp = App_getIBUS_A();
    if (temp < 10)
    {
        LCD_ShowFloatNum(80, 99, temp, 3, RED, BLACK, 32);
        LCD_ShowChar(150, 99, 'A', RED, BLACK, 32, 0);
    }
    else {
        LCD_ShowFloatNum(80, 99, temp, 4, RED, BLACK, 32);
        LCD_ShowChar(160, 99, 'A', RED, BLACK, 32, 0);
    }
}

void APP_LCD_Show_SETVOUT(void)
{
    LCD_ShowString(0, 0, "SETVOUT:", RED, BLACK, 32, 0);
    float temp = SC8815_Config.SC8815_VBUS / 1000;
    if (temp < 10)
    {
        LCD_ShowFloatNum(140, 0, temp, 3, RED, BLACK, 32);
        LCD_ShowChar(220, 0, 'V', RED, BLACK, 32, 0);
    }
    else {
        LCD_ShowFloatNum(140, 0, temp, 4, RED, BLACK, 32);
        LCD_ShowChar(240, 0, 'V', RED, BLACK, 32, 0);
    }
}

void APP_LCD_Show_SETIOUT(void)
{
    LCD_ShowString(0, 0, "SETIOUT:", RED, BLACK, 32, 0);
    float temp = SC8815_Config.SC8815_IBUS_Limit / 1000;
    if (temp < 10)
    {
        LCD_ShowFloatNum(140, 0, temp, 3, RED, BLACK, 32);
        LCD_ShowChar(220, 0, 'A', RED, BLACK, 32, 0);
    }
    else {
        LCD_ShowFloatNum(140, 0, temp, 4, RED, BLACK, 32);
        LCD_ShowChar(240, 0, 'A', RED, BLACK, 32, 0);
    }
}

void APP_LCD_Show_VINProtectMode(void)
{
    LCD_ShowString(0, 0, "VINProtect:", RED, BLACK, 32, 0);
}

void APP_LCD_Show_VOUTProtectMode(void)
{
    LCD_ShowString(0, 0, "VOUTProtect:", RED, BLACK, 32, 0);
}

void APP_LCD_Show_fastChargeMod(void)
{
    LCD_ShowString(0, 0, "fastCharge:", RED, BLACK, 32, 0);
    if (APP_config.fastCharge_InVoltage < 10)
    {
        LCD_ShowIntNum(80, 33, APP_config.fastCharge_InVoltage, 3, RED, BLACK, 32);
        LCD_ShowChar(150, 33, 'V', RED, BLACK, 32, 0);
    }
    else {
        LCD_ShowIntNum(80, 33, APP_config.fastCharge_InVoltage, 4, RED, BLACK, 32);
        LCD_ShowChar(160, 33, 'V', RED, BLACK, 32, 0);
    }
}

void APP_LCD_Show(void)
{
    if (APP_config.LCD_Clear)
    {
        LCD_Clear();
        APP_config.LCD_Clear = 0;
        return;
    }
    switch (APP_config.Sys_Mode)
    {
    case normalMode:
        APP_LCD_ShowVSET();
        APP_LCD_ShowISET();
        APP_LCD_ShowVOUT();
        APP_LCD_ShowIOUT();
        break;
    case setVBUSMode:
        APP_LCD_Show_SETVOUT();
        break;
    case setIBUSMode:
        APP_LCD_Show_SETIOUT();
        break;
    case VINProtectMode:
        APP_LCD_Show_VINProtectMode();
        break;
    case VOUTProtectMode:
        APP_LCD_Show_VOUTProtectMode();
        break;
    case fastChargeMode:
        APP_LCD_Show_fastChargeMod();
        break;
    default:
        break;
    }
}

void APP_LCD_main_show(void)
{
    LCD_ShowString(0, 0, "VSET:", RED, BLACK, 32, 0);
    float temp = SC8815_Config.SC8815_VBUS / 1000;
    if (temp < 10)
    {
        LCD_ShowFloatNum(80, 0, temp, 3, RED, BLACK, 32);
        LCD_ShowChar(150, 0, 'V', RED, BLACK, 32, 0);
    }
    else {
        LCD_ShowFloatNum(80, 0, temp, 4, RED, BLACK, 32);
        LCD_ShowChar(160, 0, 'V', RED, BLACK, 32, 0);
    }

    LCD_ShowString(0, 33, "ISET:", RED, BLACK, 32, 0);
    temp = SC8815_Config.SC8815_IBUS_Limit / 1000;
    if (temp < 10)
    {
        LCD_ShowFloatNum(80, 33, temp, 3, RED, BLACK, 32);
        LCD_ShowChar(150, 33, 'A', RED, BLACK, 32, 0);
    }
    else {
        LCD_ShowFloatNum(80, 33, temp, 4, RED, BLACK, 32);
        LCD_ShowChar(160, 33, 'A', RED, BLACK, 32, 0);
    }

    LCD_ShowString(0, 66, "VOUT:", RED, BLACK, 32, 0);
    temp = App_getVBUS_V();
    if (temp < 10)
    {
        LCD_ShowFloatNum(80, 66, temp, 3, RED, BLACK, 32);
        LCD_ShowChar(150, 66, 'V', RED, BLACK, 32, 0);
    }
    else {
        LCD_ShowFloatNum(80, 66, temp, 4, RED, BLACK, 32);
        LCD_ShowChar(160, 66, 'V', RED, BLACK, 32, 0);
    }

    LCD_ShowString(0, 99, "IOUT:", RED, BLACK, 32, 0);
    temp = App_getIBUS_A();
    if (temp < 10)
    {
        LCD_ShowFloatNum(80, 99, temp, 3, RED, BLACK, 32);
        LCD_ShowChar(150, 99, 'A', RED, BLACK, 32, 0);
    }
    else {
        LCD_ShowFloatNum(80, 99, temp, 4, RED, BLACK, 32);
        LCD_ShowChar(160, 99, 'A', RED, BLACK, 32, 0);
    }
}
