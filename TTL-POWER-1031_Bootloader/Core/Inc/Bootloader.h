#ifndef _BOOTLOADER_H_
#define _BOOTLOADER_H_
 
#include "main.h"
 
 
#define  APP_ADDR  0x08004000   //Ӧ�ó����׵�ַ���� 
 
/*ѡ���Կ�����Ӧ��LOG��Ϣ*/
//#define BOOTLOADER_LOG		
 
#define CLOSE_ALL_INT()  __set_PRIMASK(1)	//�ر������ж�
typedef void (*APP_FUNC)(); 				//����ָ�����Ͷ���
 
void iap_load_app(void);	//��ת����
 
#endif
