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

SC8815_ConfigTypeDef SC8815_Config;
static SC8815_PresetTypeDef SC8815_Preset[10];
static SC8815_TIM_WorkTypeDef SC8815_TIM_Work[10];

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
	// uint8_t i;
	// for (i = 0; i < 10; i++);
	i2c_delay(5);
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
	i2c_SendByte(SC8815_WRITE_ADDR);
	i2c_WaitAck();
	i2c_SendByte(RegAddress);
	i2c_WaitAck();
	i2c_Start();
	i2c_SendByte(SC8815_READ_ADDR);
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
		i2c_SendByte(SC8815_WRITE_ADDR);
		i2c_WaitAck();
		i2c_SendByte(RegAddress);
		i2c_WaitAck();
		i2c_SendByte(ByteData);
		i2c_WaitAck();
		i2c_Stop();
	} while (I2C_ReadRegByte(SlaveAddress, RegAddress) != ByteData && check_timeout++ < 1000);
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
		HAL_Delay(50);
		// Application_SoftwareDelay(50);
		SC8815_SFB_Enable();
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
	SC8815_HardwareInitStruct.SW_FREQ = SCHWI_FREQ_300KHz_2;
	SC8815_HardwareInitStruct.DeadTime = SCHWI_DT_60ns;
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
	SC8815_ADC_Enable();
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
	SC8815_Config.SC8815_IBAT_Limit = 12000;
	SC8815_Config.SC8815_IBUS_Limit = 1000;
	SC8815_Config.SC8815_VBUS = 5000;
	SC8815_SetBatteryCurrLimit(SC8815_Config.SC8815_IBAT_Limit);
	SC8815_SetBusCurrentLimit(SC8815_Config.SC8815_IBUS_Limit);
	SC8815_SetOutputVoltage(SC8815_Config.SC8815_VBUS);
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
	printf("SC8815 Init.\n");
}


/**
 *@brief SC8815软件保护
 *
 */
void SC8815_Soft_Protect(void)
{
	if (HAL_GPIO_ReadPin(SC8815_PSTOP_GPIO_Port, SC8815_PSTOP_Pin) == GPIO_PIN_SET || APP_config.Sys_Mode == VINProtectMode || APP_config.Sys_Mode == VOUTProtectMode)
	{
		return;
	}
	if ((App_getVBAT_mV() <= APP_config.fastCharge_InVoltage - (APP_config.fastCharge_InVoltage * 0.1))) // 输入保护
	{
		// uint16_t VBAT = 0;
		// for (uint8_t i = 0; i < 5; i++)
		// {
		//     if (VBAT == 0)
		//     {
		//         VBAT = App_getVBAT_mV();
		//     }
		//     else {
		//         VBAT = (VBAT + App_getVBAT_mV()) / 2;
		//     }
		//     HAL_Delay(10);
		// }
		// if ((VBAT <= APP_config.fastCharge_InVoltage - (APP_config.fastCharge_InVoltage * 0.1)))
		// {
		//     Application_SC8815_Standby();
		//     APP_config.Sys_Mode = VINProtectMode;
		//     BUZZER_OPEN(200);
		// }
		SC8815_Config.SC8815_Status = SC8815_Standby;
		Application_SC8815_Standby();
		APP_config.Sys_Mode = VINProtectMode;
		BUZZER_OPEN(500);
	}
	if (HAL_GetTick() - SC8815_Config.VOUT_Open_Time >= 100)
	{
		if (App_getIBUS_mA() >= SC8815_Config.SC8815_IBUS_Limit || App_getVBUS_mV() <= SC8815_Config.SC8815_VBUS - (SC8815_Config.SC8815_VBUS * 0.1)) // 输出保护
		{
			// uint16_t VBUS = 0, IBUS = 0;
			// for (uint8_t i = 0; i < 5; i++)
			// {
			//     if (VBUS == 0)
			//     {
			//         VBUS = App_getVBUS_mV();
			//         IBUS = SC8815_Read_VBUS_Current();
			//     }
			//     else {
			//         VBUS = (VBUS + App_getVBUS_mV()) / 2;
			//         IBUS = (IBUS + SC8815_Read_VBUS_Current()) / 2;
			//     }
			//     printf("VBUS:%dmV, IBUS:%dmA\r\n", VBUS, IBUS);
			//     HAL_Delay(10);
			// }
			// if (IBUS >= SC8815_Config.SC8815_IBUS_Limit || VBUS <= SC8815_Config.SC8815_VBUS - (SC8815_Config.SC8815_VBUS * 0.1))
			// {
			//     printf("VBUS/5:%dmV, IBUS/5:%dmA\r\n", VBUS, IBUS);
			//     // printf("触发限流保护\r\n");
			//     Application_SC8815_Standby();
			//     APP_config.Sys_Mode = VOUTProtectMode;
			//     BUZZER_OPEN(200);
			// }
			SC8815_Config.SC8815_Status = SC8815_Standby;
			Application_SC8815_Standby();
			APP_config.Sys_Mode = VOUTProtectMode;
			BUZZER_OPEN(500);
		}
	}
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

SC8815_PresetTypeDef SC8815_Preset2;
void SC8815_Preset_Save(SC8815_PresetTypeDef* SC8815_Preset)
{
	SC8815_PresetTypeDef SC8815_Preset3;
	STMFLASH_Write(FLASH_SAVE_ADDR, (uint16_t*)SC8815_Preset, sizeof(SC8815_PresetTypeDef));
	STMFLASH_Read(FLASH_SAVE_ADDR, (uint16_t*)&SC8815_Preset2, sizeof(SC8815_PresetTypeDef));
}
