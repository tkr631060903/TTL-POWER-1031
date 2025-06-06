#include "upgrade.h"
#include "Bootloader.h"
#include "stmflash.h"
#include "usbd_cdc_if.h"
#include "Bootloader.h"
#include "aes.h"

#define APP_CONFIG_FLASH_ADDR     STM32_FLASH_BASE+STM_SECTOR_SIZE*123
#define APP_DATA_BUFF_SIZE  9 * 1024
#define APP_UPGRADE_FLAG_ADDR 0x801FFFE

Application_SaveConfig g_app_config_save_config;
uint16_t g_upgrade_flag = 1;    //升级标志位，0不进入升级 1进入升级 2升级成功 3升级失败
static uint32_t app_addr = APP_ADDR;
static uint8_t app_data_buff[APP_DATA_BUFF_SIZE];   //数据缓存区
static uint32_t app_data_count = 0; //当前数据包的大小
static unsigned char AES128key[16] = "0CoJUm6Qyw8W8jud";
static unsigned char AES_IV[16] = "0102030405060708";

void set_app_upgrade_flag(uint16_t flag)
{
    STMFLASH_Write(APP_UPGRADE_FLAG_ADDR, &flag, 1);
}

uint16_t get_app_upgrade_flag(void)
{
    uint16_t flag;
    STMFLASH_Read(APP_UPGRADE_FLAG_ADDR, &flag, 1);
    return flag;
}

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
    if (i >= STM_SECTOR_SIZE)
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
    if (i >= STM_SECTOR_SIZE)
    {
				uint16_t data[128];
        size_t datalen = sizeof(data);
        memset(data, 0xFF, datalen);
        for (i = 0; i < 4;i++) {
            STMFLASH_Write(APP_CONFIG_FLASH_ADDR + datalen * i, data, datalen >> 1);
        }
        STMFLASH_Write(APP_CONFIG_FLASH_ADDR, (uint16_t*)&g_app_config_save_config, sizeof(Application_SaveConfig) >> 1);
    }
}

void upgrade_process(uint8_t* Buf, uint32_t *Len)
{
    uint16_t size = *Len;
    if (app_data_count < APP_DATA_BUFF_SIZE)
    {
        if (size >= 0x40) {
            aes_decrypt(Buf, Buf, size, AES_IV);
        }
        memcpy(&app_data_buff[app_data_count], Buf, size);

        // memcpy(&app_data_buff[app_data_count], Buf, size);
            app_data_count += size;
        }
    if (size < 0x40)
    {
        STMFLASH_Write(app_addr, (uint16_t*)app_data_buff, app_data_count >> 1);
		for(int i;i<1000000;i++);
            CDC_Transmit_FS("done", strlen("done"));
            set_app_upgrade_flag(2);
            // app_config_save();
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
    // g_app_config_save_config.upgrade_flag = 0;
    set_app_upgrade_flag(0);
    // app_config_save();
    __set_FAULTMASK(1); //关闭所有中断
    NVIC_SystemReset(); //进行软件复位
}

static uint32_t timer = 0;
void upgrade(void)
{
	aes_init(AES128key);
    // g_app_config_save_config.upgrade_flag = 1;
    g_upgrade_flag = get_app_upgrade_flag();
    if (g_upgrade_flag >= 10) {
        g_upgrade_flag = 0;
    }
    app_config_load();    //读取APP参数
	timer = HAL_GetTick();
    //bootloader增加同时长按key1，key2按键3s进入升级模式
    while (HAL_GPIO_ReadPin(GPIOB, KEY1_Pin) == RESET && HAL_GPIO_ReadPin(GPIOB, KEY2_Pin) == RESET) {
        if (HAL_GetTick() - timer > 3000) {
            CDC_Transmit_FS("Upgrade mode!\r\n", strlen("Upgrade mode!\r\n"));
            g_upgrade_flag = 1;
						set_app_upgrade_flag(g_upgrade_flag);
            break;
        }
    }

    if (g_upgrade_flag == 0) {
        iap_load_app();	//跳转到APP的首地址
    }
    while (1)
    {
        if (g_upgrade_flag == 2)
        {
            iap_load_app();	//跳转到APP的首地址
        }
        else if (g_upgrade_flag == 3)
        {
            CDC_Transmit_FS("Upgrade failed!\r\n", strlen("Upgrade failed!\r\n"));
            g_upgrade_flag = 1;
        }
    }
}
