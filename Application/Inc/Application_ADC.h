/**
 *@file Application_ADC.h
 * @author TanKairong (tkr631060903@gmail.com)
 * @brief ADC驱动
 * @version 0.1
 * @date 2024-01-31
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef __Application_ADC_H__
#define __Application_ADC_H__

#include "Application.h"

extern uint16_t ADC_Value[2];

float App_getVBUS_mV(void);
float App_getVBUS_V(void);
float App_getVBAT_mV(void);
float App_getVBAT_V(void);

#endif
