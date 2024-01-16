#include "CH224K.h"
#include "Application.h"

/**
 *@brief 申请快充输入5V电压
 *
 */
void CH224K_5V(void)
{
    HAL_GPIO_WritePin(CH224K_CFG1_GPIO_Port, CH224K_CFG1_Pin, GPIO_PIN_SET);
}

/**
 *@brief 申请快充输入9V电压
 *
 */
void CH224K_9V(void)
{
    HAL_GPIO_WritePin(CH224K_CFG1_GPIO_Port, CH224K_CFG1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(CH224K_CFG2_GPIO_Port, CH224K_CFG2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(CH224K_CFG3_GPIO_Port, CH224K_CFG3_Pin, GPIO_PIN_RESET);
}

/**
 *@brief 申请快充输入12V电压
 *
 */
void CH224K_12V(void)
{
    HAL_GPIO_WritePin(CH224K_CFG1_GPIO_Port, CH224K_CFG1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(CH224K_CFG2_GPIO_Port, CH224K_CFG2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(CH224K_CFG3_GPIO_Port, CH224K_CFG3_Pin, GPIO_PIN_SET);
}

/**
 *@brief 申请快充输入15V电压
 *
 */
void CH224K_15V(void)
{
    HAL_GPIO_WritePin(CH224K_CFG1_GPIO_Port, CH224K_CFG1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(CH224K_CFG2_GPIO_Port, CH224K_CFG2_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(CH224K_CFG3_GPIO_Port, CH224K_CFG3_Pin, GPIO_PIN_SET);
}

/**
 *@brief 申请快充输入20V电压
 *
 */
void CH224K_20V(void)
{
    HAL_GPIO_WritePin(CH224K_CFG1_GPIO_Port, CH224K_CFG1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(CH224K_CFG2_GPIO_Port, CH224K_CFG2_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(CH224K_CFG3_GPIO_Port, CH224K_CFG3_Pin, GPIO_PIN_RESET);
}

/**
 *@brief CH224K初始化，自动申请合适快充输入电压
 *
 */
void Application_CH224K_init(void)
{
    extern volatile Application_Config APP_config;
    // CH224K_20V();
    // HAL_Delay(500);
    // if ((SC8815_Read_BATT_Voltage() - 20000) < 500 && (APP_config.Set_OutVoltage - SC8815_Read_VBUS_Voltage()) < 200)
    // {
    //     APP_config.fastCharge_InVoltage = 20;
    //     return;
    // }
    CH224K_15V();
    HAL_Delay(500);
    if ((SC8815_Read_BATT_Voltage() - 15000) < 500)
    {
        APP_config.fastCharge_InVoltage = 15;
        return;
    }
    CH224K_12V();
    HAL_Delay(500);
    if ((SC8815_Read_BATT_Voltage() - 12000) < 500)
    {
        APP_config.fastCharge_InVoltage = 12;
        return;
    }
    CH224K_9V();
    HAL_Delay(500);
    if ((SC8815_Read_BATT_Voltage() - 9000) < 500)
    {
        APP_config.fastCharge_InVoltage = 9;
        return;
    }
    CH224K_5V();
    HAL_Delay(500);
    if ((SC8815_Read_BATT_Voltage() - 5000) < 500)
    {
        APP_config.fastCharge_InVoltage = 5;
        return;
    }
}
