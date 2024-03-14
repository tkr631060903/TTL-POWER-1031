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

extern uint16_t ADC_Value[2];

uint16_t App_getVBUS_mV(void);
uint16_t App_getVBUS_V(void);
uint16_t App_getVBAT_mV(void);
uint16_t App_getVBAT_V(void);

#endif
