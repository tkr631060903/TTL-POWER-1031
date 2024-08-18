#include "upgrade.h"
#include "Bootloader.h"
#include "stmflash.h"
#include "usbd_cdc_if.h"
#include "Bootloader.h"

#define APP_CONFIG_FLASH_ADDR     STM32_FLASH_BASE+STM_SECTOR_SIZE*119

Application_SaveConfig g_app_config_save_config;

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

uint32_t app_addr = APP_ADDR;
void upgrade_process(uint8_t* Buf, uint32_t *Len)
{
    uint16_t size = *Len;
	STMFLASH_Write(app_addr, (uint16_t*)Buf, size);
    app_addr = app_addr + size;
    //CDC_Transmit_FS(Buf, size);
}

void upgrade(void)
{
    app_config_load();    //读取APP参数
	//iap_load_app();	//跳转到APP的首地址
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
