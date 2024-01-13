#include "Application_SC8815.h"
#include "Application.h"

void SoftwareDelay(uint8_t ms)
{
	HAL_Delay(ms);
}

void I2C_WriteRegByte(uint8_t SlaveAddress, uint8_t RegAddress, uint8_t ByteData)
{
	uint8_t data[2] = { RegAddress, ByteData };
	// HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(&hi2c2, SlaveAddress, data, 2, 100);
	// printf("status = %d\r\n", status);
	HAL_I2C_Master_Transmit(&hi2c2, SlaveAddress, data, 2, 100);
}

uint8_t I2C_ReadRegByte(uint8_t SlaveAddress, uint8_t RegAddress)
{
	uint8_t ReceiveData = 0;
	// HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(&hi2c2, SlaveAddress, &RegAddress, 1, 100);
	// printf("status = %d\r\n", status);
	// status = HAL_I2C_Master_Receive(&hi2c2, SlaveAddress, &ReceiveData, 1, 100);
	// printf("status = %d\r\n", status);
	// printf("ReceiveData = %x\r\n", ReceiveData);
	HAL_I2C_Master_Transmit(&hi2c2, SlaveAddress, &RegAddress, 1, 100);
	HAL_I2C_Master_Receive(&hi2c2, SlaveAddress, &ReceiveData, 1, 100);
	return ReceiveData;
}

/**
 *@brief SC8815带载启动
 *
 */
void Application_SC8815_loadStart(void)
{
	SC8815_SFB_Disable();
	HAL_Delay(500);
	SC8815_SFB_Enable();
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
	SC8815_BatteryConfigStruct.IRCOMP = SCBAT_IRCOMP_0mR;
	// SCBAT_VBAT_SEL_Internal 内部反馈
	// SCBAT_VBAT_SEL_External 外部反馈
	SC8815_BatteryConfigStruct.VBAT_SEL = SCBAT_VBAT_SEL_Internal;
	SC8815_BatteryConfigStruct.CSEL = SCBAT_CSEL_4S;
	SC8815_BatteryConfigStruct.VCELL = SCBAT_VCELL_4v50;
	SC8815_BatteryConfig(&SC8815_BatteryConfigStruct);

	//配置 SC8815 硬件参数选项
	SC8815_HardwareInitStruct.IBAT_RATIO = SCHWI_IBAT_RATIO_12x;
	SC8815_HardwareInitStruct.IBUS_RATIO = SCHWI_IBUS_RATIO_6x;
	SC8815_HardwareInitStruct.VBAT_RATIO = SCHWI_VBAT_RATIO_12_5x;
	SC8815_HardwareInitStruct.VBUS_RATIO = SCHWI_VBUS_RATIO_12_5x;
	SC8815_HardwareInitStruct.VINREG_Ratio = SCHWI_VINREG_RATIO_100x;
	SC8815_HardwareInitStruct.SW_FREQ = SCHWI_FREQ_450KHz;
	SC8815_HardwareInitStruct.DeadTime = SCHWI_DT_40ns;
	SC8815_HardwareInitStruct.ICHAR = SCHWI_ICHAR_IBAT;
	SC8815_HardwareInitStruct.TRICKLE = SCHWI_TRICKLE_Disable;
	SC8815_HardwareInitStruct.TERM = SCHWI_TERM_Enable;
	// VBUS电压反馈模式: 
	//   SCHWI_FB_Internal 内部反馈(最高电压25.6V)
	//   SCHWI_FB_External 外部分压电阻反馈(最大输出不建议超过36V[手册上最大输出电压且需考虑外围元件耐压值]) 
	SC8815_HardwareInitStruct.FB_Mode = SCHWI_FB_Internal;
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
	HAL_GPIO_WritePin(SC8815_PSTOP_GPIO_Port, SC8815_PSTOP_Pin, GPIO_PIN_RESET);


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
	extern Application_Config APP_config;
	APP_config.SC8815_Battery_Current_Limit = 5000;
	APP_config.SC8815_VBUS_Current_Limit = 1000;
	APP_config.Set_OutVoltage = 5000;
	SC8815_SetBatteryCurrLimit(APP_config.SC8815_Battery_Current_Limit);
	SC8815_SetBusCurrentLimit(APP_config.SC8815_VBUS_Current_Limit);
	SC8815_SetOutputVoltage(APP_config.Set_OutVoltage);
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
	HAL_Delay(1000);
	Application_SC8815_loadStart();
	printf("SC8815 Init.\n");
}

/**
 *@brief 设置SC8815进入Shutdown Mode
 *
 */
void Application_SC8815_Shutdown(void)
{
	HAL_GPIO_WritePin(SC8815_CE_GPIO_Port, SC8815_CE_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(SC8815_PSTOP_GPIO_Port, SC8815_PSTOP_Pin, GPIO_PIN_SET);
}

/**
 *@brief 设置SC8815进入Standby Mode
 *
 */
void Application_SC8815_Standby(void)
{
	HAL_GPIO_WritePin(SC8815_CE_GPIO_Port, SC8815_CE_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SC8815_PSTOP_GPIO_Port, SC8815_PSTOP_Pin, GPIO_PIN_SET);
}

/**
 *@brief 设置SC8815进入正常工作模式
 *
 */
void Application_SC8815_Run(void)
{
	HAL_GPIO_WritePin(SC8815_CE_GPIO_Port, SC8815_CE_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SC8815_PSTOP_GPIO_Port, SC8815_PSTOP_Pin, GPIO_PIN_RESET);
}
