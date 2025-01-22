/**
 *@file Application_SC8815.c
 * @author TanKairong (tkr631060903@gmail.com)
 * @brief SC8815应用程序实现
 * @version 0.1
 * @date 2024-01-31
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "Application_SC8815.h"
#include "Application.h"
#include "Application_BUZZER.h"
#include "Application_ADC.h"
#include "stmflash.h"
#include <string.h>
#include "menu_ui.h"
#include <math.h>
#include "Application_LCD.h"
#include "usbd_cdc_if.h"

#define SC8815_TIM_WORK_FLASH_SAVE_ADDR     STM32_FLASH_BASE+STM_SECTOR_SIZE*124
#define SC8815_OUTPUT_CALIBRATION_TABLE_ADDR    	STM32_FLASH_BASE+STM_SECTOR_SIZE*121
#define ALPHA_PARAMETER 0.1
#define CALIBRATION_TABLE_VALUE(calibration_table, voltage) calibration_table[((int)voltage - SC8815_VBUS_MIN) / 100]	//获取校准表值,-2700表示=>输出电压最小值为2.7V
#define CALIBRATION_IBUS_TABLE_VALUE(calibration_table, current) calibration_table[((int)current - SC8815_IBUS_MIN) / 100]	//获取校准表值,-500表示=>输出电流最小值为0.5A
#define SCHW_VBUS_RSHUNT_DEFAULT 5

SC8815_ConfigTypeDef SC8815_Config;
SC8815_TIM_WorkTypeDef SC8815_TIM_Work[SC8815_TIM_WORK_SIZE] = { 0 };
static uint8_t i2c_mutex = 0;	//I2C总线互斥信号,0表示总线空闲，1表示总线忙
static uint8_t sc8815_power = 105; //SC8815输出功率值
float SCHW_VBUS_RSHUNT = SCHW_VBUS_RSHUNT_DEFAULT;

uint8_t get_i2c_mutex(void)
{
	return i2c_mutex;
}

void set_i2c_mutex(uint8_t status)
{
	i2c_mutex = status;
}

uint8_t get_sc8815_power(void)
{
	return sc8815_power;
}

void set_sc8815_power(uint8_t power)
{
	sc8815_power = power;
}

/**
 *@brief 软件延时
 *
 * @param ms 毫秒
 */
void SoftwareDelay(uint8_t ms)
{
	Application_SoftwareDelay(ms);
}

//延时
void IIC_delay(void)
{
	uint8_t i;
	for (i = 0; i < 10; i++);
	// for (i = 0; i < 250; i++);
	// i2c_delay(5);
	// i2c_delay(1);
}

/**
 *@brief SC8815软件I2C起始信号
 *
 */
void i2c_Start(void)
{
	SC8815_SDA_Set();
	SC8815_SCL_Set();
	IIC_delay();
	SC8815_SDA_Clr();
	IIC_delay();
	SC8815_SCL_Clr();
	IIC_delay();
}

//结束信号
void i2c_Stop(void)
{
	SC8815_SDA_Clr();
	SC8815_SCL_Set();
	IIC_delay();
	SC8815_SDA_Set();
}

//等待信号响应
void i2c_WaitAck(void) //测数据信号的电平
{
	SC8815_SDA_Set();
	IIC_delay();
	SC8815_SCL_Set();
	IIC_delay();
	SC8815_SCL_Clr();
	IIC_delay();
}

/**
 *@brief SC8815软件I2C非应答信号
 *
 */
void i2c_NAck(void)
{
	SC8815_I2C_SDA_1();    //cpu驱动SDA=1
	IIC_delay();
	SC8815_I2C_SCL_1();    //产生一个高电平时钟
	IIC_delay();
	SC8815_I2C_SCL_0();
	IIC_delay();
}

/**
 *@brief SC8815软件I2C应答信号
 *
 */
void i2c_Ack(void)
{
    SC8815_I2C_SDA_0(); /* CPU驱动SDA = 0 */
    IIC_delay();
    SC8815_I2C_SCL_1(); /* CPU产生1个时钟 */
    IIC_delay();
    SC8815_I2C_SCL_0();
    IIC_delay();
    SC8815_I2C_SDA_1(); /* CPU释放SDA总线 */
}

//写入一个字节
void i2c_SendByte(uint8_t dat)
{
	uint8_t i;
	for (i = 0;i < 8;i++)
	{
		if (dat & 0x80)//将dat的8位从最高位依次写入
		{
			SC8815_SDA_Set();
		}
		else
		{
			SC8815_SDA_Clr();
		}
		IIC_delay();
		SC8815_SCL_Set();
		IIC_delay();
		SC8815_SCL_Clr();//将时钟信号设置为低电平
		dat <<= 1;
	}
}

/**
 *@brief SC8815软件I2C读取一个字节
 *
 * @return uint8_t 读取到的数据
 */
uint8_t i2c_ReadByte(void)
{
	uint8_t i;
	uint8_t value = 0;
	/*读取到第一个bit为数据的bit7*/
	for (i = 0;i < 8;i++)
	{
		value <<= 1;
		SC8815_I2C_SCL_1();
		IIC_delay();
		if (SC8815_I2C_SDA_READ())
		{
			value++;
		}
		SC8815_I2C_SCL_0();
		IIC_delay();
	}
	return value;
}

uint8_t I2C_ReadRegByte(uint8_t SlaveAddress, uint8_t RegAddress)
{
	i2c_Start();
	i2c_SendByte(SlaveAddress & 0xfe);	//write
	i2c_WaitAck();
	i2c_SendByte(RegAddress);
	i2c_WaitAck();
	i2c_Start();
	i2c_SendByte(SlaveAddress | 1);	//read
	i2c_WaitAck();
	uint8_t data = i2c_ReadByte();
	i2c_NAck();
	i2c_Stop();
	return data;
}

void I2C_WriteRegByte(uint8_t SlaveAddress, uint8_t RegAddress, uint8_t ByteData)
{
	uint16_t check_timeout = 0;
	do
	{
		i2c_Start();
		i2c_SendByte(SlaveAddress & 0xfe);	//write
		i2c_WaitAck();
		i2c_SendByte(RegAddress);
		i2c_WaitAck();
		i2c_SendByte(ByteData);
		i2c_WaitAck();
		i2c_Stop();
	} while (I2C_ReadRegByte(SlaveAddress, RegAddress) != ByteData && check_timeout++ < 1000 && SlaveAddress == SC8815_WRITE_ADDR);
	if (check_timeout >= 1000)
	{
		printf("SysRest-->SC8815 Write Reg Error!\r\n");
		// __set_FAULTMASK(1); //关闭所有中断
		// NVIC_SystemReset(); //进行软件复位
	}
}


/**
 *@brief SC8815带载启动
 *
 */
void Application_SC8815_loadStart(void)
{
	if (SC8815_Config.SC8815_Status == SC8815_LoadStart)
	{
		Application_SC8815_Run();
		SC8815_SFB_Disable();
		// HAL_Delay(10);
		// Application_SoftwareDelay(10);
		// SC8815_SFB_Enable();
		SC8815_Config.sc8815_sfb_delay_ms = 20;	//最小值为1
		SC8815_Config.VOUT_Open_Time = HAL_GetTick();
	}
}

void Application_SC8815_Init(void)
{
	extern SC8815_BatteryConfigTypeDef SC8815_BatteryConfigStruct;
	/****启动 SC8815...****/
	//->设置 PSTOP 为高
	HAL_GPIO_WritePin(SC8815_PSTOP_GPIO_Port, SC8815_PSTOP_Pin, GPIO_PIN_SET);
	//->设置 CE 为低
	HAL_GPIO_WritePin(SC8815_CE_GPIO_Port, SC8815_CE_Pin, GPIO_PIN_RESET);
	SoftwareDelay(50);   //必要的启动延时

	//配置 SC8815 电池参数选项
	SC8815_BatteryConfigStruct.IRCOMP = SCBAT_IRCOMP_20mR;
	// SCBAT_VBAT_SEL_Internal 内部反馈
	// SCBAT_VBAT_SEL_External 外部反馈
	SC8815_BatteryConfigStruct.VBAT_SEL = SCBAT_VBAT_SEL_External;
	SC8815_BatteryConfigStruct.CSEL = SCBAT_CSEL_3S;
	SC8815_BatteryConfigStruct.VCELL = SCBAT_VCELL_4v10;
	SC8815_BatteryConfig(&SC8815_BatteryConfigStruct);

	//配置 SC8815 硬件参数选项
	SC8815_HardwareInitStruct.IBAT_RATIO = SCHWI_IBAT_RATIO_12x;
	SC8815_HardwareInitStruct.IBUS_RATIO = SCHWI_IBUS_RATIO_6x;
	SC8815_HardwareInitStruct.VBAT_RATIO = SCHWI_VBAT_RATIO_12_5x;
	SC8815_HardwareInitStruct.VBUS_RATIO = SCHWI_VBUS_RATIO_12_5x;
	SC8815_HardwareInitStruct.VINREG_Ratio = SCHWI_VINREG_RATIO_100x;
	SC8815_HardwareInitStruct.SW_FREQ = app_config_save_config.SW_FREQ;
	SC8815_HardwareInitStruct.DeadTime = SCHWI_DT_40ns;
	SC8815_HardwareInitStruct.ICHAR = SCHWI_ICHAR_IBAT;
	SC8815_HardwareInitStruct.TRICKLE = SCHWI_TRICKLE_Disable;
	SC8815_HardwareInitStruct.TERM = SCHWI_TERM_Enable;
	// VBUS电压反馈模式: 
	//   SCHWI_FB_Internal 内部反馈(最高电压25.6V)
	//   SCHWI_FB_External 外部分压电阻反馈(最大输出不建议超过36V[手册上最大输出电压且需考虑外围元件耐压值]) 
	SC8815_HardwareInitStruct.FB_Mode = SCHWI_FB_External;
	SC8815_HardwareInitStruct.TRICKLE_SET = SCHWI_TRICKLE_SET_60;
	SC8815_HardwareInitStruct.OVP = SCHWI_OVP_Enable;
	SC8815_HardwareInitStruct.DITHER = SCHWI_DITHER_Disable;
	SC8815_HardwareInitStruct.SLEW_SET = SCHWI_SLEW_1mV_us;
	SC8815_HardwareInitStruct.ADC_SCAN = SCHWI_ADC_Enable;
	SC8815_HardwareInitStruct.ILIM_BW = SCHWI_ILIM_BW_1_25KHz;
	SC8815_HardwareInitStruct.LOOP = SCHWI_LOOP_Improved;
	SC8815_HardwareInitStruct.ShortFoldBack = SCHWI_SFB_Enable;	// 需开启短路保护避免烧器件
	SC8815_HardwareInitStruct.EOC = SCHWI_EOC_1_10;
	SC8815_HardwareInitStruct.PFM = SCHWI_PFM_Disable;
	// SC8815_HardwareInitStruct.PFM = SCHWI_PFM_Enable;
	SC8815_HardwareInit(&SC8815_HardwareInitStruct);

	//配置 SC8815 中断屏蔽选项
	extern SC8815_InterruptStatusTypeDef SC8815_InterruptMaskInitStruct;
	SC8815_InterruptMaskInitStruct.AC_OK = sENABLE;
	SC8815_InterruptMaskInitStruct.INDET = sENABLE;
	SC8815_InterruptMaskInitStruct.VBUS_SHORT = sENABLE;
	SC8815_InterruptMaskInitStruct.OTP = sENABLE;
	SC8815_InterruptMaskInitStruct.EOC = sENABLE;
	SC8815_ConfigInterruptMask(&SC8815_InterruptMaskInitStruct);

	// 启用内部ADC采样，启用后寄存器中值为实时采样数据
	SC8815_ADC_Disable();
	SC8815_PGATE_Disable();

	/***现在可以设置 PSTOP 引脚为低, 启动 SC8815 功率转换****/
	// HAL_GPIO_WritePin(SC8815_PSTOP_GPIO_Port, SC8815_PSTOP_Pin, GPIO_PIN_RESET);


	/*** 示例1, 设置为充电模式,电池和 VBUS 限流 1.2A, VINREG 设置为 10V ****/
	//SC8815_SetBatteryCurrLimit(sys_config.charge_current);
	//SC8815_SetBusCurrentLimit(sys_config.charge_current);
	// 设置对电池充电时外部最低电压值，以获取最大充电电流
	// SC8815_VINREG_SetVoltage(4000);
	// SC8815_OTG_Disable();


	/*** 示例2, 设置为反向放电模式,电池和 VBUS 限流 3A, 输出电压 设置为 12V ****/
	// SC8815_SetBatteryCurrLimit(5000);
	// SC8815_SetBusCurrentLimit(3000);
	// SC8815_SetOutputVoltage(12000);
	// SC8815_Config.SC8815_IBAT_Limit = 12000;
	// SC8815_Config.SC8815_IBUS_Limit = 1000;
	// SC8815_Config.SC8815_IBUS_Limit_Old = 1000;
	// SC8815_Config.SC8815_VBUS = 5000;
	// SC8815_Config.SC8815_VBUS_Old = 5000;
	SC8815_auto_output();
	SC8815_Config.SC8815_IBUS_Limit_Old = SC8815_Config.SC8815_IBUS_Limit;
	SC8815_Config.SC8815_VBUS_Old = SC8815_Config.SC8815_VBUS;
	SC8815_SetBatteryCurrLimit(SC8815_Config.SC8815_IBAT_Limit);
	App_SC8815_SetBusCurrentLimit(SC8815_Config.SC8815_IBUS_Limit);
	App_SC8815_SetOutputVoltage(SC8815_Config.SC8815_VBUS);
	SC8815_OTG_Enable();

	/*** 示例3, 读取 SC8815 ADC 数据 ****/
	//uint16_t VbusVolt = SC8815_Read_VBUS_Voltage();
	//uint16_t VbusCurr = SC8815_Read_VBUS_Current();
	//uint16_t BattVolt = SC8815_Read_BATT_Voltage();
	//uint16_t BattCurr = SC8815_Read_BATT_Current();


	/*** 示例4, 读取 SC8815 中断状态 ****/
//	SC8815_ReadInterrupStatus(&SC8815_InterruptMaskInitStruct);     //MCU 收到 SC8815 中断后调用此函数读 SC8815 中断 (读中断状态后将清除中断状态位)
//	if (SC8815_InterruptMaskInitStruct.AC_OK == 1)                  //检查 AC_OK 中断是否触发
//	{
//			// AC_OK 中断处理代码
//	}
//	else if (SC8815_InterruptMaskInitStruct.EOC == 1)
//	{
//			// EOC 中断处理代码
//	}
	// Application_SC8815_Run();

	SC8815_Config.SC8815_VBUS_IBUS_Step = 1000;
	SC8815_Config.SC8815_Status = SC8815_Standby;
	Application_SC8815_Standby();
	SC8815_Preset_Read();
    SC8815_output_calibration(0);
	printf("SC8815 Init.\n");
}


/**
 *@brief SC8815软件保护
 *
 */
void SC8815_Soft_Protect(void)
{
	if (HAL_GPIO_ReadPin(SC8815_PSTOP_GPIO_Port, SC8815_PSTOP_Pin) == GPIO_PIN_SET) {
		return;
	}
	float output_voltage = 0;
    extern presset_config_set_typeDef presset_config_set;
	if (SC8815_Config.SC8815_Status == SC8815_TIM_WORK) {
		output_voltage = presset_config_set.set_vbus[SC8815_Config.sc8815_tim_work_step];
	} else {
		output_voltage = SC8815_Config.SC8815_VBUS;
	}
	if (HAL_GetTick() - SC8815_Config.VOUT_Open_Time >= 300 && App_getVBUS_mV() < output_voltage / 2) {
		HAL_Delay(300);
		if (App_getVBUS_mV() < output_voltage / 2) {
			if (SC8815_Config.SC8815_Status == SC8815_TIM_WORK) {
				SC8815_Preset_Mode_Quit();
			} else {
				SC8815_Config.SC8815_Status = SC8815_Standby;
				Application_SC8815_Standby();
			}
			protect_page_ui_process(VBUS_PROTECT);
		}
	}
	// if (HAL_GetTick() - SC8815_Config.VOUT_Open_Time >= 100)
	// {
	// 	if (App_getIBUS_mA() >= SC8815_Config.SC8815_IBUS_Limit || App_getVBUS_mV() <= SC8815_Config.SC8815_VBUS - (SC8815_Config.SC8815_VBUS * 0.1)) // 输出保护
	// 	{
	// 		// uint16_t VBUS = 0, IBUS = 0;
	// 		// for (uint8_t i = 0; i < 5; i++)
	// 		// {
	// 		//     if (VBUS == 0)
	// 		//     {
	// 		//         VBUS = App_getVBUS_mV();
	// 		//         IBUS = SC8815_Read_VBUS_Current();
	// 		//     }
	// 		//     else {
	// 		//         VBUS = (VBUS + App_getVBUS_mV()) / 2;
	// 		//         IBUS = (IBUS + SC8815_Read_VBUS_Current()) / 2;
	// 		//     }
	// 		//     printf("VBUS:%dmV, IBUS:%dmA\r\n", VBUS, IBUS);
	// 		//     HAL_Delay(10);
	// 		// }
	// 		// if (IBUS >= SC8815_Config.SC8815_IBUS_Limit || VBUS <= SC8815_Config.SC8815_VBUS - (SC8815_Config.SC8815_VBUS * 0.1))
	// 		// {
	// 		//     printf("VBUS/5:%dmV, IBUS/5:%dmA\r\n", VBUS, IBUS);
	// 		//     // printf("触发限流保护\r\n");
	// 		//     Application_SC8815_Standby();
	// 		//     APP_config.Sys_Mode = VOUTProtectMode;
	// 		//     BUZZER_OPEN(200);
	// 		// }
	// 		SC8815_Config.SC8815_Status = SC8815_PORT;
	// 		Application_SC8815_Standby();
    //         // protect_page_ui_process(0);
	// 		// BUZZER_OPEN(500);
	// 	}
	// }
}

/**
 *@brief 设置SC8815进入Shutdown Mode
 *
 */
void Application_SC8815_Shutdown(void)
{
	HAL_GPIO_WritePin(SC8815_CE_GPIO_Port, SC8815_CE_Pin, GPIO_PIN_SET);
	// HAL_GPIO_WritePin(SC8815_PSTOP_GPIO_Port, SC8815_PSTOP_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(POWER_RELEASE_GPIO_Port, POWER_RELEASE_Pin, GPIO_PIN_SET);	//放电
}

/**
 *@brief 设置SC8815进入Standby Mode
 *
 */
void Application_SC8815_Standby(void)
{
	HAL_GPIO_WritePin(SC8815_CE_GPIO_Port, SC8815_CE_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SC8815_PSTOP_GPIO_Port, SC8815_PSTOP_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(POWER_RELEASE_GPIO_Port, POWER_RELEASE_Pin, GPIO_PIN_SET);	//放电
}

/**
 *@brief 设置SC8815进入正常工作模式
 *
 */
void Application_SC8815_Run(void)
{
	HAL_GPIO_WritePin(POWER_RELEASE_GPIO_Port, POWER_RELEASE_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SC8815_CE_GPIO_Port, SC8815_CE_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SC8815_PSTOP_GPIO_Port, SC8815_PSTOP_Pin, GPIO_PIN_RESET);
}

void SC8815_Preset_Read(void)
{
	STMFLASH_ReadBytes(SC8815_TIM_WORK_FLASH_SAVE_ADDR, (uint8_t*)&SC8815_TIM_Work, sizeof(SC8815_TIM_WorkTypeDef)* SC8815_TIM_WORK_SIZE);
	if (SC8815_TIM_Work[0].circular == 0xffff)
	{
		memset(SC8815_TIM_Work, 0, sizeof(SC8815_TIM_WorkTypeDef) * SC8815_TIM_WORK_SIZE);
		SC8815_Preset_Save();
	}
}

void SC8815_Preset_Save(void)
{
	STMFLASH_Write(SC8815_TIM_WORK_FLASH_SAVE_ADDR, (uint16_t*)SC8815_TIM_Work, (sizeof(SC8815_TIM_WorkTypeDef) * SC8815_TIM_WORK_SIZE) >> 1);
}

void SC8815_Preset_Mode_Quit(void)
{
  extern menu_i32 current_menu_index;
	current_menu_index = MAIN_PAGE;
	SC8815_Config.SC8815_Status = SC8815_Standby;
	Application_SC8815_Standby();
	SC8815_Config.sc8815_tim_work_lcd_flush = tim_work_lcd_main;
	App_SC8815_SetOutputVoltage(SC8815_Config.SC8815_VBUS);
	App_SC8815_SetBusCurrentLimit(SC8815_Config.SC8815_IBUS_Limit);
}

void App_SC8815_SetOutputVoltage(float voltage)
{
	float* calibration_table = (float*)((uint32_t)SC8815_OUTPUT_CALIBRATION_TABLE_ADDR);
	extern presset_config_set_typeDef presset_config_set;
	float voltage_target = voltage, vbus_old = 0;
	if (SC8815_Config.SC8815_Status == SC8815_TIM_WORK) {
		if (SC8815_Config.sc8815_tim_work_step == 0) {
			vbus_old = SC8815_Config.SC8815_VBUS_Old;
		} else {
			vbus_old = presset_config_set.set_vbus[SC8815_Config.sc8815_tim_work_step - 1];
		}
	} else {
		vbus_old = SC8815_Config.SC8815_VBUS_Old;
	}
	float voltage_temp = vbus_old;
	voltage = voltage + (float)CALIBRATION_TABLE_VALUE(calibration_table, voltage);
	if (SC8815_Config.SC8815_Status == SC8815_LoadStart) {
		if (voltage_target - vbus_old >= 9000) {
			SC8815_PFM_Enable();
			for (int i = 0; i < 40; i++) {
				voltage_temp = ALPHA_PARAMETER * voltage + (1 - ALPHA_PARAMETER) * voltage_temp;
				SC8815_SetOutputVoltage(voltage_temp);
			}
			SC8815_SetOutputVoltage(voltage);
			SC8815_PFM_Disable();
		} else if (vbus_old - voltage_target >= 9000 || (vbus_old - voltage_target >= 900 && App_getVBAT_V() <= 9.5) || App_getVBAT_V() <= 9.5) {
			SC8815_PFM_Enable();
			SC8815_SetOutputVoltage(voltage);
			SC8815_Config.sc8815_pfm_delay_ms = 250;	//最小值为1
			// HAL_GPIO_WritePin(POWER_RELEASE_GPIO_Port, POWER_RELEASE_Pin, GPIO_PIN_SET);	//放电
			// if (vbus_old - voltage_target >= 9000) {
			// 	SC8815_Config.sc8815_pfm_delay_ms = 250;
			// }
			// SC8815_PFM_Disable();
		} else {
			SC8815_SetOutputVoltage(voltage);
		}
	} else {
		SC8815_SetOutputVoltage(voltage);
	}
	SC8815_Config.VOUT_Open_Time = HAL_GetTick();
}

void App_SC8815_SetBusCurrentLimit(float current)
{
	float* calibration_table = (float*)((uint32_t)SC8815_OUTPUT_CALIBRATION_TABLE_ADDR + sizeof(float) * 334);
	SCHW_VBUS_RSHUNT = (float)CALIBRATION_IBUS_TABLE_VALUE(calibration_table, current);
	if (SCHW_VBUS_RSHUNT > 8 || SCHW_VBUS_RSHUNT < 4) {
		SCHW_VBUS_RSHUNT = SCHW_VBUS_RSHUNT_DEFAULT;
	}
	SC8815_SetBusCurrentLimit(current);
}

/**
 *@brief 校准SC8815输出电压从2.7V到36V以0.1V的间隔校准，总共校准334个电压点
 * 
 * @param calibration 是否开启校准标志位
 */
void SC8815_output_calibration(uint8_t calibration)
{
	float* calibration_table_temp = (float*)SC8815_TIM_Work;
	float voltage = SC8815_VBUS_MIN;
	uint16_t* calibration_table = (uint16_t*)((uint32_t)SC8815_OUTPUT_CALIBRATION_TABLE_ADDR);
	if (calibration_table[0] == 0xffff || calibration == 1) {
		printf("SC8815 output calibration table is empty.\n");
		char percentage_str[5] = {0};
		int percentage;
		LCD_Clear();
		LCD_ShowChinese(30, 50, "校准电压", LIGHTBLUE, BLACK, 32, 0);
		SC8815_Config.SC8815_Status = SC8815_Standby;
    	Application_SC8815_Standby();
		memset(SC8815_TIM_Work, 0, sizeof(SC8815_TIM_WorkTypeDef) * SC8815_TIM_WORK_SIZE);
		while (App_getVBUS_mV() >= 100) {
			HAL_Delay(10);
		}
		SC8815_SetOutputVoltage(voltage);
		SC8815_Config.SC8815_Status = SC8815_LoadStart;
		Application_SC8815_loadStart();
		HAL_Delay(5000);
		for (int i = 0; i < 334; i++) {
			SC8815_SetOutputVoltage(voltage);
			HAL_Delay(500);
			calibration_table_temp[i] = voltage - App_getVBUS_mV();
			voltage += 100;
			percentage = i * 0.3;
			sprintf(percentage_str, "%d%%", percentage);
			LCD_ShowString(172, 50, (const uint8_t*)percentage_str, LIGHTBLUE, BLACK, 32, 0);
			LCD_Fill_DMA(0, LCD_H - 10, percentage * 2.4, LCD_H, LIGHTBLUE);
			if (percentage >= 100) {
				LCD_Fill_DMA(0, LCD_H - 10, LCD_W, LCD_H, LIGHTBLUE);
			}
		}
		STMFLASH_Write(SC8815_OUTPUT_CALIBRATION_TABLE_ADDR, (uint16_t*)calibration_table_temp, sizeof(float) * 334 >> 1);
		SC8815_Preset_Read();
		SC8815_Config.SC8815_Status = SC8815_Standby;
    Application_SC8815_Standby();
		App_SC8815_SetOutputVoltage(SC8815_Config.SC8815_VBUS);
	}
}

/**
 *@brief 校准SC8815输出电流从0.5A到5.5A以0.1A的间隔校准，总共校准51个电流点
 * 
 */
void SC8815_IBUS_calibration(void)
{
	uint16_t ibus_calibration = SC8815_IBUS_MIN;
	float* calibration_table_temp = (float*)SC8815_TIM_Work;
	float calibration_error = 0.03;
	SCHW_VBUS_RSHUNT = 8;
	App_SC8815_SetOutputVoltage(12000);
	SC8815_SetBusCurrentLimit(ibus_calibration);
	SC8815_Config.SC8815_Status = SC8815_LoadStart;
	Application_SC8815_loadStart();
	HAL_Delay(1000);
	memset(SC8815_TIM_Work, 0, sizeof(SC8815_TIM_WorkTypeDef) * SC8815_TIM_WORK_SIZE);
	for (int i = 0; i < (SC8815_IBUS_MAX - SC8815_IBUS_MIN) / 100 + 1; i++) {
		usb_printf("ibus_calibration:%d\n", ibus_calibration);
		while (1) {
			if (App_getIBUS_mA() >= ibus_calibration - ibus_calibration * calibration_error && App_getIBUS_mA() <= ibus_calibration + ibus_calibration * calibration_error) {
				calibration_table_temp[i] = SCHW_VBUS_RSHUNT;
				break;
			}
			if (SCHW_VBUS_RSHUNT <= 4) {
				calibration_table_temp[i] = SCHW_VBUS_RSHUNT_DEFAULT;
				break;
			}
			SCHW_VBUS_RSHUNT -= 0.1;
			SC8815_SetBusCurrentLimit(ibus_calibration);
			HAL_Delay(500);
		}
		ibus_calibration += 100;
		if (ibus_calibration <= 700) {
			SCHW_VBUS_RSHUNT = 8;
			calibration_error = 0.03;
		} else {
			SCHW_VBUS_RSHUNT = 6.5;
			calibration_error = 0.02;
		}
		SC8815_SetBusCurrentLimit(ibus_calibration);
		HAL_Delay(500);
	}
	STMFLASH_Write(SC8815_OUTPUT_CALIBRATION_TABLE_ADDR + sizeof(float) * 334, (uint16_t*)calibration_table_temp, (sizeof(float) * ((SC8815_IBUS_MAX - SC8815_IBUS_MIN) / 100 + 1) >> 1));
	SC8815_Config.SC8815_Status = SC8815_Standby;
	Application_SC8815_Standby();
	SC8815_Preset_Read();
	App_SC8815_SetOutputVoltage(SC8815_Config.SC8815_VBUS);
	App_SC8815_SetBusCurrentLimit(SC8815_Config.SC8815_IBUS_Limit);
}

void SC8815_auto_output(void)
{
	uint32_t in_power = APP_config.fastCharge_InVoltage * APP_config.fastCharge_InCurrent * 1000000;
	if (in_power > 0) {
		while (in_power <= SC8815_Config.SC8815_IBUS_Limit * SC8815_Config.SC8815_VBUS) {
			if (SC8815_Config.SC8815_IBUS_Limit > SC8815_IBUS_MIN) {
				SC8815_Config.SC8815_IBUS_Limit -= 100;
			} else {
				SC8815_Config.SC8815_VBUS -= 100;
			}
		}
	}
}
