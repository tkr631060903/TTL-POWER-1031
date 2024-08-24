#include "upgrade.h"
#include "Bootloader.h"
#include "stmflash.h"
#include "usbd_cdc_if.h"
#include "Bootloader.h"

#define APP_CONFIG_FLASH_ADDR     STM32_FLASH_BASE+STM_SECTOR_SIZE*119
#define APP_DATA_BUFF_SIZE  9 * 1024

Application_SaveConfig g_app_config_save_config;
static uint32_t app_addr = APP_ADDR;
static uint8_t app_data_buff[APP_DATA_BUFF_SIZE];   //数据缓存区
static uint32_t app_data_count = 0; //当前数据包的大小

void app_config_load(void)
{
    int i;
    for (i = 0; i < STM_SECTOR_SIZE;)
    {
        STMFLASH_ReadBytes(APP_CONFIG_FLASH_ADDR + i, (uint8_t*)&g_app_config_save_config, sizeof(Application_SaveConfig));
        if (g_app_config_save_config.lock_buzzer != 0 && g_app_config_save_config.lock_buzzer != 1)
        {
            if (i == 0)
            {
                g_app_config_save_config.upgrade_flag = 0;
                return;
            }
            else
            {
                i -= sizeof(Application_SaveConfig);
                STMFLASH_ReadBytes(APP_CONFIG_FLASH_ADDR + i, (uint8_t*)&g_app_config_save_config, sizeof(Application_SaveConfig));
                break;
            }
        }
        i += sizeof(Application_SaveConfig);
    }
    if (i > STM_SECTOR_SIZE)
    {
        i -= sizeof(Application_SaveConfig);
        STMFLASH_ReadBytes(APP_CONFIG_FLASH_ADDR + i, (uint8_t*)&g_app_config_save_config, sizeof(Application_SaveConfig));
    }
}

void app_config_save(void)
{
    Application_SaveConfig app_config_save_config_temp;
    int i;
    for (i = 0; i < STM_SECTOR_SIZE;)
    {
        STMFLASH_ReadBytes(APP_CONFIG_FLASH_ADDR + i, (uint8_t*)&app_config_save_config_temp, sizeof(Application_SaveConfig));
        if (app_config_save_config_temp.lock_buzzer != 0 && app_config_save_config_temp.lock_buzzer != 1)
        {
            STMFLASH_Write(APP_CONFIG_FLASH_ADDR + i, (uint16_t*)&g_app_config_save_config, sizeof(Application_SaveConfig) >> 1);
            break;
        }
        i += sizeof(Application_SaveConfig);
    }
    if (i > STM_SECTOR_SIZE)
    {
        STMFLASH_Write(APP_CONFIG_FLASH_ADDR, (uint16_t*)&g_app_config_save_config, sizeof(Application_SaveConfig) >> 1);
    }
}

void upgrade_process(uint8_t* Buf, uint32_t *Len)
{
    uint16_t size = *Len;
	if (app_data_count < APP_DATA_BUFF_SIZE)
    {
        memcpy(&app_data_buff[app_data_count], Buf, size);
        app_data_count += size;
    }
    if (size < 0x40)
    {
        STMFLASH_Write(app_addr, (uint16_t*)app_data_buff, app_data_count >> 1);
		for(int i;i<1000000;i++);
        CDC_Transmit_FS("done", strlen("done"));
        g_app_config_save_config.upgrade_flag = 2;
        app_config_save();
        __set_FAULTMASK(1); //关闭所有中断
        NVIC_SystemReset(); //进行软件复位
        return;
    }
	if (app_data_count >= APP_DATA_BUFF_SIZE)
    {
        //CDC_Transmit_FS("write", strlen("write"));
        STMFLASH_Write(app_addr, (uint16_t*)app_data_buff, app_data_count >> 1);
        memset(app_data_buff, 0, APP_DATA_BUFF_SIZE);
        app_addr += app_data_count;
        app_data_count = 0;
        CDC_Transmit_FS("read", strlen("read"));
    }
    //CDC_Transmit_FS(Buf, size);
}

void upgrade_quit(void)
{
    g_app_config_save_config.upgrade_flag = 0;
    app_config_save();
    __set_FAULTMASK(1); //关闭所有中断
    NVIC_SystemReset(); //进行软件复位
}

void upgrade(void)
{
	g_app_config_save_config.upgrade_flag = 1;
    app_config_load();    //读取APP参数
    if (g_app_config_save_config.upgrade_flag == 0) {
        iap_load_app();	//跳转到APP的首地址
    }
    while (1)
    {
        if (g_app_config_save_config.upgrade_flag == 2)
        {
            iap_load_app();	//跳转到APP的首地址
        }
        else if (g_app_config_save_config.upgrade_flag == 3)
        {
            CDC_Transmit_FS("Upgrade failed!\r\n", strlen("Upgrade failed!\r\n"));
        }
    }
}
