/**
 *@file Application_ADC.c
 * @author TanKairong (tkr631060903@gmail.com)
 * @brief ADC驱动
 * @version 0.1
 * @date 2024-01-31
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "Application_ADC.h"

uint16_t ADC_Value[2];    // ADC值,ADC_Value[0] == VBUS,ADC_Value[1] == VBAT

/**
 *@brief 使用ADC获取VBUS电压
 * 
 * @return VBUS单位mV 
 */
float App_getVBUS_mV(void)
{
    return ((10.754 * ((float)ADC_Value[0] * (3.3 / 4096)) - 0.1974) * 1000);
}

/**
 *@brief 使用ADC获取VBUS电压
 * 
 * @return VBUS单位V 
 */
float App_getVBUS_V(void)
{
    return 10.754 * ((float)ADC_Value[0] * (3.3 / 4096)) - 0.1974;
}

/**
 *@brief 使用ADC获取VBAT电压
 * 
 * @return VBAT单位mV 
 */
float App_getVBAT_mV(void)
{
    return ((10.754 * ((float)ADC_Value[1] * (3.3 / 4096)) - 0.1974) * 1000);
}

/**
 *@brief 使用ADC获取VBAT电压
 * 
 * @return VBAT单位V 
 */
float App_getVBAT_V(void)
{
    return 10.754 * ((float)ADC_Value[1] * (3.3 / 4096)) - 0.1974;
}
