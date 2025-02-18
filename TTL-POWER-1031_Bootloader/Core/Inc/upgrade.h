#ifndef __UPGRADE__H__
#define __UPGRADE__H__

#include "main.h"

typedef struct
{
    uint8_t lock_buzzer;     //���������� 1�� 0����
    uint8_t SW_FREQ;        //����Ƶ��
    float temperature;     //�¶ȼ��ֵ
    float SC8815_IBUS_Limit;
    float SC8815_VBUS;
	  float DC_IBAT_Limit;
		char device_name[11];
} Application_SaveConfig;

// extern Application_SaveConfig g_app_config_save_config;

void upgrade(void);
void upgrade_process(uint8_t* Buf, uint32_t* Len);
void upgrade_quit(void);

#endif  //!__UPGRADE__H__
