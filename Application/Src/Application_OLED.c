#include "Application_OLED.h"

extern volatile Application_Config APP_config;

void APP_OLED_ShowVIN(void)
{
    OLED_ShowString(1, 1, "VIN:");
    OLED_ShowNum(1, 5, APP_config.fastCharge_InVoltage, 2);
    OLED_ShowString(1, 7, "V");
}

void APP_OLED_ShowVSET(void)
{
    OLED_ShowString(2, 1, "VSET:");
    if (APP_config.Set_OutVoltage / 1000 >= 10)
    {
        OLED_ShowNum(2, 6, APP_config.Set_OutVoltage / 1000, 2);
        OLED_ShowString(2, 8, ".");
        OLED_ShowNum(2, 9, APP_config.Set_OutVoltage / 100, 1);
        OLED_ShowNum(2, 10, APP_config.Set_OutVoltage / 10, 1);
        OLED_ShowString(2, 11, "V");
    }
    else {
        OLED_ShowNum(2, 6, APP_config.Set_OutVoltage / 1000, 1);
        OLED_ShowString(2, 7, ".");
        OLED_ShowNum(2, 8, APP_config.Set_OutVoltage / 100, 1);
        OLED_ShowNum(2, 9, APP_config.Set_OutVoltage / 10, 1);
        OLED_ShowString(2, 10, "V");
    }
}

void APP_OLED_ShowISET(void)
{
    OLED_ShowString(3, 1, "ISET:");
    if (APP_config.SC8815_VBUS_Current_Limit / 1000 >= 10)
    {
        OLED_ShowNum(3, 6, APP_config.SC8815_VBUS_Current_Limit / 1000, 2);
        OLED_ShowString(3, 8, ".");
        OLED_ShowNum(3, 9, APP_config.SC8815_VBUS_Current_Limit / 100, 1);
        OLED_ShowNum(3, 10, APP_config.SC8815_VBUS_Current_Limit / 10, 1);
        OLED_ShowString(3, 11, "A");
    }
    else {
        OLED_ShowNum(3, 6, APP_config.SC8815_VBUS_Current_Limit / 1000, 1);
        OLED_ShowString(3, 7, ".");
        OLED_ShowNum(3, 8, APP_config.SC8815_VBUS_Current_Limit / 100, 1);
        OLED_ShowNum(3, 9, APP_config.SC8815_VBUS_Current_Limit / 10, 1);
        OLED_ShowString(3, 10, "A");
    }
}

void APP_OLED_ShowVOUT(void)
{
    OLED_ShowString(4, 1, "VOUT:");
    uint16_t temp = SC8815_Read_VBUS_Voltage();
    if (temp / 1000 >= 10)
    {
        OLED_ShowNum(4, 6, temp / 1000, 2);
        OLED_ShowString(4, 8, ".");
        OLED_ShowNum(4, 9, temp / 100, 1);
        OLED_ShowNum(4, 10, temp / 10, 1);
        OLED_ShowString(4, 11, "V");
    }
    else {
        OLED_ShowNum(4, 6, temp / 1000, 1);
        OLED_ShowString(4, 7, ".");
        OLED_ShowNum(4, 8, temp / 100, 1);
        OLED_ShowNum(4, 9, temp / 10, 1);
        OLED_ShowString(4, 10, "V");
    }
}

void APP_OLED_ShowIOUT(void)
{
    OLED_ShowString(1, 1, "IOUT:");
    uint16_t temp = SC8815_Read_VBUS_Current();
    if (temp / 1000 >= 10)
    {
        OLED_ShowNum(1, 6, temp / 1000, 2);
        OLED_ShowString(1, 8, ".");
        OLED_ShowNum(1, 9, temp / 100, 1);
        OLED_ShowNum(1, 10, temp / 10, 1);
        OLED_ShowString(1, 11, "A");
    }
    else {
        OLED_ShowNum(1, 6, temp / 1000, 1);
        OLED_ShowString(1, 7, ".");
        OLED_ShowNum(1, 8, temp / 100, 1);
        OLED_ShowNum(1, 9, temp / 10, 1);
        OLED_ShowString(1, 10, "A");
    }
}

void APP_OLED_Show_SETVOUT(void)
{
    // OLED_Clear_Part(2, 1, 16);
    // OLED_Clear_Part(3, 1, 16);
    // OLED_Clear_Part(4, 1, 16);
    OLED_ShowString(1, 1, "SETVOUT:");
    uint16_t temp = APP_config.Set_OutVoltage;
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

void APP_OLED_Show_VINErrorMod(void)
{
    OLED_ShowString(1, 1, "VINErrorMod");
}

void APP_OLED_Show_currentProtectMod(void)
{
    OLED_ShowString(1, 1, "APP_OLED_Show_currentProtectMod");
}

void APP_OLED_Show_fastChargeMod(void)
{

}

void APP_OLED_Show(void)
{
    if (APP_config.SetMod == noneMod)
    {
        // APP_OLED_ShowVIN();
        APP_OLED_ShowVSET();
        APP_OLED_ShowISET();    
        APP_OLED_ShowVOUT();
        APP_OLED_ShowIOUT();
        HAL_Delay(300);
    }
    else if (APP_config.SetMod == voltageMod) {
        APP_OLED_Show_SETVOUT();
    }
    else if (APP_config.SetMod == currentMod) {
        APP_OLED_Show_SETIOUT();
    }
    else if (APP_config.SetMod == VINErrorMod) {
        APP_OLED_Show_VINErrorMod();
    }
    else if (APP_config.SetMod == currentProtectMod) {
        APP_OLED_Show_currentProtectMod();
    }
}
