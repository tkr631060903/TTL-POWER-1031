#include "CH224K.h"
#include "Application.h"
#include "Application_ADC.h"
#include "Application_SC8815.h"
#include <math.h>

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
    CH224K_20V();
    HAL_Delay(100);
    if (fabsf(App_getVBAT_mV() - 20000) < 500)
    {
        APP_config.fastCharge_InVoltage = 20;
        return;
    }
    CH224K_15V();
    HAL_Delay(100);
    if (fabsf(App_getVBAT_mV() - 15000) < 500)
    {
        APP_config.fastCharge_InVoltage = 15;
        return;
    }
    CH224K_12V();
    HAL_Delay(100);
    if (fabsf(App_getVBAT_mV() - 12000) < 500)
    {
        APP_config.fastCharge_InVoltage = 12;
        return;
    }
    CH224K_9V();
    HAL_Delay(100);
    if (fabsf(App_getVBAT_mV() - 9000) < 500)
    {
        APP_config.fastCharge_InVoltage = 9;
        return;
    }
    CH224K_5V();
    HAL_Delay(100);
    if (fabsf(App_getVBAT_mV() - 5000) < 500)
    {
        APP_config.fastCharge_InVoltage = 5;
        return;
    }
}
