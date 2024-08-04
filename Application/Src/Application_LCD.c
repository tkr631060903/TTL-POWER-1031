#include "Application_LCD.h"
#include "Application_ADC.h"
#include "Application_SC8815.h"
#include <string.h>

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

void APP_LCD_main_init(void)
{
    // LCD_DrawPoint(100, 100, LIGHTBLUE);
    // LCD_DrawLine(66, 0, 66, LCD_H, LIGHTBLUE);
    // LCD_DrawLine(0, 0, 0, LCD_H, LIGHTBLUE);
    // LCD_DrawLine(0, 0, LCD_W, 0, LIGHTBLUE);
    // LCD_DrawLine(0, 32, 66, 32, LIGHTBLUE);
    // LCD_DrawLine(0, 67, 66, 67, LIGHTBLUE);
    // LCD_DrawLine(0, 100, 66, 100, LIGHTBLUE);
    // LCD_DrawLine(0, LCD_H - 1, LCD_W, LCD_H - 1, LIGHTBLUE);
    // LCD_DrawLine(LCD_W - 1, 0, LCD_W - 1, LCD_H - 1, LIGHTBLUE);

    LCD_Fill_DMA(66, 0, 67, LCD_H, LIGHTBLUE);
    LCD_Fill_DMA(0, 0, 1, LCD_H, LIGHTBLUE);
    LCD_Fill_DMA(0, 0, LCD_W, 1, LIGHTBLUE);
    LCD_Fill_DMA(0, 32, 66, 33, LIGHTBLUE);
    LCD_Fill_DMA(0, 67, 66, 68, LIGHTBLUE);
    LCD_Fill_DMA(0, 100, 66, 101, LIGHTBLUE);
    LCD_Fill_DMA(0, LCD_H - 1, LCD_W, LCD_H, LIGHTBLUE);
    LCD_Fill_DMA(LCD_W - 1, 0, LCD_W, LCD_H, LIGHTBLUE);

    LCD_ShowString(20, 1, "Vset", LIGHTBLUE, BLACK, 16, 0);
    // LCD_ShowString(15, 16, "5.00V", LIGHTBLUE, BLACK, 16, 0);
    LCD_ShowString(20, 34, "Iset", LIGHTBLUE, BLACK, 16, 0);
    // LCD_ShowString(15, 51, "5.00A", LIGHTBLUE, BLACK, 16, 0);
    LCD_ShowString(23, 68, "Vin", LIGHTBLUE, BLACK, 16, 0);
    // LCD_ShowString(15, 84, "12.0V", LIGHTBLUE, BLACK, 16, 0);
    // LCD_ShowString(1, 102, " ON ", BLACK, GREEN, 32, 0);
    // LCD_ShowString(80, 1, "05.10", MAGENTA, BLACK, 48, 0);
    LCD_ShowString(200, 16, "V", MAGENTA, BLACK, 32, 0);
    // LCD_ShowString(80, 44, "04.98", GREEN, BLACK, 48, 0);
    LCD_ShowString(200, 56, "A", GREEN, BLACK, 32, 0);
    // LCD_ShowString(80, 86, "025.4", ORANGE, BLACK, 48, 0);
    LCD_ShowString(200, 100, "W", ORANGE, BLACK, 32, 0);
    if (HAL_GPIO_ReadPin(SC8815_PSTOP_GPIO_Port, SC8815_PSTOP_Pin) == SET)
    {
        LCD_ShowString(10, 102, "OFF", BLACK, GRAY, 32, 0);
        LCD_Fill_DMA(1, 102, 10, LCD_H - 1, GRAY);
        LCD_Fill_DMA(56, 102, 66, LCD_H - 1, GRAY);
    }
    else {
        LCD_ShowString(1, 102, " ON ", BLACK, GREEN, 32, 0);
    }
}

void APP_LCD_main_show(void)
{
    char str[10];
    float temp = SC8815_Config.SC8815_VBUS / 1000;
    if (temp < 10)
    {
        sprintf(str, "%.2fV", temp);
        LCD_ShowString(15, 16, (const uint8_t*)str, LIGHTBLUE, BLACK, 16, 0);
    }
    else
    {
        sprintf(str, "%.1fV", temp);
        LCD_ShowString(15, 16, (const uint8_t*)str, LIGHTBLUE, BLACK, 16, 0);
    }

    memset(str, 0, 10);
    temp = SC8815_Config.SC8815_IBUS_Limit / 1000;
    sprintf(str, "%.2fA", temp);    
    LCD_ShowString(15, 51, (const uint8_t*)str, LIGHTBLUE, BLACK, 16, 0);

    memset(str, 0, 10);
    temp = App_getVBAT_V();
    if (temp < 10)
    {
        sprintf(str, "%.2fV", temp);
        LCD_ShowString(15, 84, (const uint8_t*)str, LIGHTBLUE, BLACK, 16, 0);
    }
    else
    {
        sprintf(str, "%.1fV", temp);
        LCD_ShowString(15, 84, (const uint8_t*)str, LIGHTBLUE, BLACK, 16, 0);
    }

    memset(str, 0, 10);
    float vbus = App_getVBUS_V();
    if (vbus < 10)
    {
        sprintf(str, "0%.2f", vbus);
        LCD_ShowString(80, 1, (const uint8_t*)str, MAGENTA, BLACK, 48, 0);
    }
    else
    {
        sprintf(str, "%.1f", vbus);
        LCD_ShowString(80, 1, (const uint8_t*)str, MAGENTA, BLACK, 48, 0);
    }

    memset(str, 0, 10);
    float ibus = App_getIBUS_A();
    sprintf(str, "0%.2f", ibus);
    LCD_ShowString(80, 44, (const uint8_t*)str, GREEN, BLACK, 48, 0);

    memset(str, 0, 10);
    float powr = vbus * ibus;
    if (powr < 10)
    {
        sprintf(str, "0%.2f", powr);
        LCD_ShowString(80, 86, (const uint8_t*)str, ORANGE, BLACK, 48, 0);
    }
    else if (powr < 100)
    {
        sprintf(str, "%.1f", powr);
        LCD_ShowString(80, 86, (const uint8_t*)str, ORANGE, BLACK, 48, 0);
    }
    else
    {
        sprintf(str, "%.1f", powr);
        LCD_ShowString(80, 86, (const uint8_t*)str, ORANGE, BLACK, 48, 0);
    }
}
