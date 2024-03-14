/**
 *@file Application_SC8815.h
 * @author TanKairong (tkr631060903@gmail.com)
 * @brief SC8815应用程序实现
 * @version 0.1
 * @date 2024-01-31
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef Application_SC8815_H
#define Application_SC8815_H

#include "SC8815.h"

typedef enum
{
    SC8815Run = 0x00U,
    SC8815Shutdown = 0x01U,
    SC8815Standby = 0x02U,
    SC8815LoadStart = 0x03U,
} SC8815ModTypeDef;

void Application_SC8815_Init(void);
void Application_SC8815_Shutdown(void);
void Application_SC8815_Standby(void);
void Application_SC8815_Run(void);
void Application_SC8815_loadStart(void);
void SC8815_Soft_Protect(void);

#endif
