#ifndef __UPGRADE__H__
#define __UPGRADE__H__

#include "main.h"

typedef struct
{
    uint8_t lock_buzzer;     //蜂鸣器锁定 1锁 0不锁
    uint8_t SW_FREQ;        //开关频率
    uint8_t upgrade_flag;   //升级标志位，0不进入升级 1进入升级 2升级成功 3升级失败
    float temperature;     //温度监控值
    float SC8815_IBUS_Limit;
    float SC8815_VBUS;
} Application_SaveConfig;

extern Application_SaveConfig g_app_config_save_config;

void upgrade(void);
void upgrade_process(uint8_t* Buf, uint32_t* Len);
void upgrade_quit(void);

#endif  //!__UPGRADE__H__
