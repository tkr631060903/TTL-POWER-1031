#include "Bootloader.h"
#include "stdint.h"
#include "string.h"
 
void iap_load_app(void)
{
	unsigned char i = 0;
	APP_FUNC jump2app;//定义一个函数指针
    
    /* 栈顶地址是否合法 */
    if(((*(uint32_t *)APP_ADDR)&0x2FFE0000) == 0x20000000)
	{
		/* 关闭全局中断 */
		__set_PRIMASK(1);
        /* 获取复位地址 */
		jump2app=(APP_FUNC)*(volatile uint32_t *)(APP_ADDR+4);	
		/* 设置栈指针 */
		__set_MSP(*(volatile uint32_t *)APP_ADDR);
		
	#ifdef BOOTLOADER_LOG	
		HAL_UART_Transmit(&huart1,(uint8_t*)"Bootloader end load app\r\n",(uint16_t)strlen("Bootloader end load app\r\n"),0xf);
	#endif
		/* 关闭滴答定时器，复位到默认值 */
        SysTick->CTRL = 0;
        SysTick->LOAD = 0;
		SysTick->VAL = 0;
		/* 关闭所有中断，清除所有中断挂起标志 */
		for (i = 0; i < 8; i++)
        {
            NVIC->ICER[i]=0xFFFFFFFF;
            NVIC->ICPR[i]=0xFFFFFFFF;
        }
		/* 设置所有时钟到默认状态 */
		HAL_RCC_DeInit();
		/* 使能全局中断 */
        __set_PRIMASK(0);
        
        /* 在RTOS工程，这条语句很重要，设置为特权级模式，使用MSP指针 */
        __set_CONTROL(0);
		/* 跳转至APP */
		jump2app();
    }
	
#ifdef BOOTLOADER_LOG
    else
    {
		HAL_UART_Transmit(&huart1,(uint8_t*)"APP Not Found!\n",(uint16_t)strlen("APP Not Found!\n"),0xf);
    }
#endif
	
}
