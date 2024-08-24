#ifndef __UPGRADE__H__
#define __UPGRADE__H__

#include "main.h"

typedef struct
{
    uint8_t lock_buzzer;     //���������� 1�� 0����
    uint8_t SW_FREQ;        //����Ƶ��
    uint8_t upgrade_flag;   //������־λ��0���������� 1�������� 2�����ɹ� 3����ʧ��
    float temperature;     //�¶ȼ��ֵ
    float SC8815_IBUS_Limit;
    float SC8815_VBUS;
} Application_SaveConfig;

extern Application_SaveConfig g_app_config_save_config;

void upgrade(void);
void upgrade_process(uint8_t* Buf, uint32_t* Len);
void upgrade_quit(void);

#endif  //!__UPGRADE__H__
