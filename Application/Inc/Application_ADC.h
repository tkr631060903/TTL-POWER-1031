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
#ifndef Application_ADC_H
#define Application_ADC_H

#include "Application.h"

#define SAMPLING_RATE 0.0008056640625  // 3.3/4096

extern uint16_t ADC_Value[4];

float App_getVBUS_mV(void);
float App_getVBUS_V(void);
float App_getIBUS_mA(void);
float App_getIBUS_A(void);
float App_getVBAT_mV(void);
float App_getVBAT_V(void);

#endif
