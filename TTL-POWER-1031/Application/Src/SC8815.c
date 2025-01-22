/*---------------------------------------------------------------------------/
/  SC8815 ͨ�ù̼��� V0.2 (C)Sghz��2021
/----------------------------------------------------------------------------/
/ SC8815���Ǹ����������������֤���߿�������������
/
/ ��Ȩ���� (C) 2021��Sghz����������Ȩ����
/
/ 1.Դ��������·ַ����뱣��������Ȩ����������������������������
/
/ ������ɰ�Ȩ�����ߺ͹����ߡ���ԭ�����ṩ���뱾�����ص��κε��������е����Ρ�
/ ��Ȩ�����˻����߶�ʹ�ñ������ɵ��κ��𺦸Ų�����
/---------------------------------------------------------------------------*/

//����ͷ�ļ�
#include "SC8815.h"
#include "Application.h"

SC8815_BatteryConfigTypeDef SC8815_BatteryConfigStruct = { 0 };
SC8815_HardwareInitTypeDef SC8815_HardwareInitStruct = { 0 };
SC8815_InterruptStatusTypeDef SC8815_InterruptMaskInitStruct = { 0 };
extern float SCHW_VBUS_RSHUNT;

/****************************************
* @brief    ��ʼ�� SC8815
*****************************************/
void SC8815_Init(void)
{

	/****���� SC8815...****/
	//->���� PSTOP Ϊ��
	//->���� CE Ϊ��
	//SoftwareDelay(5);   //��Ҫ��������ʱ

	//���� SC8815 ��ز���ѡ��
	SC8815_BatteryConfigStruct.IRCOMP = SCBAT_IRCOMP_20mR;
	// SCBAT_VBAT_SEL_Internal �ڲ�����
	// SCBAT_VBAT_SEL_External �ⲿ����
	SC8815_BatteryConfigStruct.VBAT_SEL = SCBAT_VBAT_SEL_Internal;
	SC8815_BatteryConfigStruct.CSEL = SCBAT_CSEL_2S;
	SC8815_BatteryConfigStruct.VCELL = SCBAT_VCELL_4v20;
	SC8815_BatteryConfig(&SC8815_BatteryConfigStruct);

	//���� SC8815 Ӳ������ѡ��
	SC8815_HardwareInitStruct.IBAT_RATIO = SCHWI_IBAT_RATIO_12x;
	SC8815_HardwareInitStruct.IBUS_RATIO = SCHWI_IBUS_RATIO_6x;
	SC8815_HardwareInitStruct.VBAT_RATIO = SCHWI_VBAT_RATIO_12_5x;
	SC8815_HardwareInitStruct.VBUS_RATIO = SCHWI_VBUS_RATIO_12_5x;
	SC8815_HardwareInitStruct.VINREG_Ratio = SCHWI_VINREG_RATIO_100x;
	SC8815_HardwareInitStruct.SW_FREQ = SCHWI_FREQ_300KHz_2;
	SC8815_HardwareInitStruct.DeadTime = SCHWI_DT_40ns;
	SC8815_HardwareInitStruct.ICHAR = SCHWI_ICHAR_IBAT;
	SC8815_HardwareInitStruct.TRICKLE = SCHWI_TRICKLE_Enable;
	SC8815_HardwareInitStruct.TERM = SCHWI_TERM_Enable;
	// VBUS��ѹ����ģʽ: 
	//   SCHWI_FB_Internal �ڲ�����(��ߵ�ѹ25.6V)
	//   SCHWI_FB_External �ⲿ��ѹ���跴��(�����������鳬��36V[�ֲ�����������ѹ���迼����ΧԪ����ѹֵ]) 
	SC8815_HardwareInitStruct.FB_Mode = SCHWI_FB_External;
	SC8815_HardwareInitStruct.TRICKLE_SET = SCHWI_TRICKLE_SET_60;
	SC8815_HardwareInitStruct.OVP = SCHWI_OVP_Disable;
	SC8815_HardwareInitStruct.DITHER = SCHWI_DITHER_Disable;
	SC8815_HardwareInitStruct.SLEW_SET = SCHWI_SLEW_1mV_us;
	SC8815_HardwareInitStruct.ADC_SCAN = SCHWI_ADC_Enable;
	SC8815_HardwareInitStruct.ILIM_BW = SCHWI_ILIM_BW_1_25KHz;
	SC8815_HardwareInitStruct.LOOP = SCHWI_LOOP_Improved;
	SC8815_HardwareInitStruct.ShortFoldBack = SCHWI_SFB_Enable;
	SC8815_HardwareInitStruct.EOC = SCHWI_EOC_1_10;
	SC8815_HardwareInitStruct.PFM = SCHWI_PFM_Disable;
	SC8815_HardwareInit(&SC8815_HardwareInitStruct);

	//���� SC8815 �ж�����ѡ��
	SC8815_InterruptMaskInitStruct.AC_OK = sENABLE;
	SC8815_InterruptMaskInitStruct.INDET = sENABLE;
	SC8815_InterruptMaskInitStruct.VBUS_SHORT = sENABLE;
	SC8815_InterruptMaskInitStruct.OTP = sENABLE;
	SC8815_InterruptMaskInitStruct.EOC = sENABLE;
	SC8815_ConfigInterruptMask(&SC8815_InterruptMaskInitStruct);

	// �����ڲ�ADC���������ú�Ĵ�����ֵΪʵʱ��������
	SC8815_ADC_Enable();
	SC8815_PGATE_Disable();

	/***���ڿ������� PSTOP ����Ϊ��, ���� SC8815 ����ת��****/



	/*** ʾ��1, ����Ϊ���ģʽ,��غ� VBUS ���� 1.2A, VINREG ����Ϊ 10V ****/
	//SC8815_SetBatteryCurrLimit(sys_config.charge_current);
	//SC8815_SetBusCurrentLimit(sys_config.charge_current);
	// ���öԵ�س��ʱ�ⲿ��͵�ѹֵ���Ի�ȡ��������
	SC8815_VINREG_SetVoltage(4000);
	SC8815_OTG_Disable();


	/*** ʾ��2, ����Ϊ����ŵ�ģʽ,��غ� VBUS ���� 3A, �����ѹ ����Ϊ 12V ****/
//    SC8815_SetBatteryCurrLimit(5000);
//    SC8815_SetBusCurrentLimit(3000);
//    SC8815_SetOutputVoltage(sys_config.volt);
//    SC8815_OTG_Enable();


	/*** ʾ��3, ��ȡ SC8815 ADC ���� ****/
	//uint16_t VbusVolt = SC8815_Read_VBUS_Voltage();
	//uint16_t VbusCurr = SC8815_Read_VBUS_Current();
	//uint16_t BattVolt = SC8815_Read_BATT_Voltage();
	//uint16_t BattCurr = SC8815_Read_BATT_Current();


	/*** ʾ��4, ��ȡ SC8815 �ж�״̬ ****/
//	SC8815_ReadInterrupStatus(&SC8815_InterruptMaskInitStruct);     //MCU �յ� SC8815 �жϺ���ô˺����� SC8815 �ж� (���ж�״̬������ж�״̬λ)
//	if (SC8815_InterruptMaskInitStruct.AC_OK == 1)                  //��� AC_OK �ж��Ƿ񴥷�
//	{
//			// AC_OK �жϴ������
//	}
//	else if (SC8815_InterruptMaskInitStruct.EOC == 1)
//	{
//			// EOC �жϴ������
//	}

	// printf("SC8815 Init.\n");
}

/****************************************
* @brief    ���� SC8815 ��ز���
* @param    SC8815_BatteryConfigStruct ָ�� SC8815 ������ýṹ ��ָ��
* @note		��������֮ǰ������ SC8815 ����ʱ 5ms ȷ����������,����Ҫ SC8815 PSTOP ����Ϊ�߲��ܽ��д�Ӳ������
*****************************************/
void SC8815_BatteryConfig(SC8815_BatteryConfigTypeDef* SC8815_BatteryConfigStruct)
{
	uint8_t tmp;
	tmp = SC8815_BatteryConfigStruct->IRCOMP;
	tmp |= SC8815_BatteryConfigStruct->VBAT_SEL;
	tmp |= SC8815_BatteryConfigStruct->CSEL;
	tmp |= SC8815_BatteryConfigStruct->VCELL;
	I2C_WriteRegByte(SC8815_ADDR, SCREG_VBAT_SET, tmp);
}

/****************************************
* @brief    ��ʼ�� SC8815 Ӳ������
* @param    SC8815_HardwareInitStruct ָ�� SC8815 Ӳ����ʼ���ṹ ��ָ��
* @note		��������֮ǰ������ SC8815 ����ʱ 5ms ȷ����������,����Ҫ SC8815 PSTOP ����Ϊ�߲��ܽ��д�Ӳ������
*****************************************/
void SC8815_HardwareInit(SC8815_HardwareInitTypeDef* SC8815_HardwareInitStruct)
{
	uint8_t tmp;

	//��������
	tmp = I2C_ReadRegByte(SC8815_ADDR, SCREG_RATIO) & 0xE0; //��ȡ�Ĵ����еı���λ(��Щ����λ���ܶ�)
	tmp |= SC8815_HardwareInitStruct->IBAT_RATIO;           //װ�����ò���
	tmp |= SC8815_HardwareInitStruct->IBUS_RATIO;
	tmp |= SC8815_HardwareInitStruct->VBAT_RATIO;
	tmp |= SC8815_HardwareInitStruct->VBUS_RATIO;
	I2C_WriteRegByte(SC8815_ADDR, SCREG_RATIO, tmp);    //д�ؼĴ���

	//Ӳ������0
	tmp = I2C_ReadRegByte(SC8815_ADDR, SCREG_CTRL0_SET) & 0x60;
	tmp |= SC8815_HardwareInitStruct->VINREG_Ratio;
	tmp |= SC8815_HardwareInitStruct->SW_FREQ;
	tmp |= SC8815_HardwareInitStruct->DeadTime;
	I2C_WriteRegByte(SC8815_ADDR, SCREG_CTRL0_SET, tmp);

	//Ӳ������1
	tmp = I2C_ReadRegByte(SC8815_ADDR, SCREG_CTRL1_SET) & 0x03;
	tmp |= SC8815_HardwareInitStruct->ICHAR;
	tmp |= SC8815_HardwareInitStruct->TRICKLE;
	tmp |= SC8815_HardwareInitStruct->TERM;
	tmp |= SC8815_HardwareInitStruct->FB_Mode;
	tmp |= SC8815_HardwareInitStruct->TRICKLE_SET;
	tmp |= SC8815_HardwareInitStruct->OVP;
	I2C_WriteRegByte(SC8815_ADDR, SCREG_CTRL1_SET, tmp);

	//Ӳ������2
	tmp = (I2C_ReadRegByte(SC8815_ADDR, SCREG_CTRL2_SET) & 0xF0) | 0x08;
	tmp |= SC8815_HardwareInitStruct->DITHER;
	tmp |= SC8815_HardwareInitStruct->SLEW_SET;
	I2C_WriteRegByte(SC8815_ADDR, SCREG_CTRL2_SET, tmp);

	//Ӳ������3(û�б���λ)
	tmp = SC8815_HardwareInitStruct->ADC_SCAN;
	tmp |= SC8815_HardwareInitStruct->ILIM_BW;
	tmp |= SC8815_HardwareInitStruct->LOOP;
	tmp |= SC8815_HardwareInitStruct->ShortFoldBack;
	tmp |= SC8815_HardwareInitStruct->EOC;
	tmp |= SC8815_HardwareInitStruct->PFM;
	I2C_WriteRegByte(SC8815_ADDR, SCREG_CTRL3_SET, tmp);

	//������ɺ��Ҫ����ʱ
	SoftwareDelay(5);
}
/****************************************
* @brief    ���� SC8815 �ж����� (�ж�ʹ�ܻ�ʧ��)
* @param    InterruptStatusStruct ָ�� SC8815 �ж�״̬���ýṹ ��ָ��
*****************************************/
void SC8815_ConfigInterruptMask(SC8815_InterruptStatusTypeDef* InterruptStatusStruct)
{
	uint8_t tmp;
	tmp = I2C_ReadRegByte(SC8815_ADDR, SCREG_MASK) & 0x91;
	tmp |= InterruptStatusStruct->AC_OK ? 0x40 : 0x00;
	tmp |= InterruptStatusStruct->INDET ? 0x20 : 0x00;
	tmp |= InterruptStatusStruct->VBUS_SHORT ? 0x08 : 0x00;
	tmp |= InterruptStatusStruct->OTP ? 0x04 : 0x00;
	tmp |= InterruptStatusStruct->EOC ? 0x02 : 0x00;
	I2C_WriteRegByte(SC8815_ADDR, SCREG_MASK, tmp);
}

/****************************************
* @brief    ��ȡ SC8815 ���ж�״̬
* @param    InterruptStatusStruct ָ�� SC8815 �ж�״̬�ṹ ��ָ��
*****************************************/
void SC8815_ReadInterrupStatus(SC8815_InterruptStatusTypeDef* InterruptStatusStruct)
{
	uint8_t tmp;
	tmp = I2C_ReadRegByte(SC8815_ADDR, SCREG_MASK);     //��ȡ״̬�Ĵ���
	InterruptStatusStruct->AC_OK = (tmp & 0x40) >> 6;   //����ÿһ��λ
	InterruptStatusStruct->INDET = (tmp & 0x20) >> 5;
	InterruptStatusStruct->VBUS_SHORT = (tmp & 0x08) >> 3;
	InterruptStatusStruct->OTP = (tmp & 0x04) >> 2;
	InterruptStatusStruct->EOC = (tmp & 0x02) >> 1;
}

/****************************************
* @brief    ��ȡ SC8815 ���� ADC �� VBUS ��ѹ�Ĳ������
* @return   ��λΪ mV �� VBUS ��ѹ
*****************************************/
uint16_t SC8815_Read_VBUS_Voltage(void)
{
	uint8_t tmp1, tmp2;
	double RATIO_Value;

	RATIO_Value = ((I2C_ReadRegByte(SC8815_ADDR, SCREG_RATIO) & 0x01) == 1) ? 5 : 12.5; //ȡ�� VBUS ��ѹ�ı���
	tmp1 = I2C_ReadRegByte(SC8815_ADDR, SCREG_VBUS_FB_VALUE);           //��ȡ ADC ֵ�Ĵ���1
	tmp2 = I2C_ReadRegByte(SC8815_ADDR, SCREG_VBUS_FB_VALUE2) >> 6;     //��ȡ ADC ֵ�Ĵ���2

	//���� VBUS ��ѹֵ
	// return (uint16_t)((4 * tmp1 + tmp2 + 1) * RATIO_Value) * 2 * sys_config.volt_correct;
	return (uint16_t)((4 * tmp1 + tmp2 + 1) * RATIO_Value) * 2;
}
/****************************************
* @brief    ��ȡ SC8815 ���� ADC �� VBUS �����Ĳ������
* @return   ��λΪ mA �� VBUS ���� (SC8815 �޵�����������)
*****************************************/
uint16_t SC8815_Read_VBUS_Current(void)
{
	uint8_t tmp1, tmp2;
	uint16_t RATIO_Value;

	RATIO_Value = ((I2C_ReadRegByte(SC8815_ADDR, SCREG_RATIO) & 0x0C) == 4) ? 6 : 3;    //ȡ�� IBUS �ı���
	tmp1 = I2C_ReadRegByte(SC8815_ADDR, SCREG_IBUS_VALUE);              //��ȡ ADC ֵ�Ĵ���1
	tmp2 = I2C_ReadRegByte(SC8815_ADDR, SCREG_IBUS_VALUE2) >> 6;        //��ȡ ADC ֵ�Ĵ���2

	//���� VBUS ����ֵ
	return ((uint16_t)(50 * RATIO_Value) * (4 * tmp1 + tmp2 + 1)) / (3 * SCHW_VBUS_RSHUNT);
}
/****************************************
* @brief    ��ȡ SC8815 ���� ADC �Ե�ص�ѹ�Ĳ������
* @return   ��λΪ mV �ĵ�ص�ѹ
*****************************************/
uint16_t SC8815_Read_BATT_Voltage(void)
{
	uint8_t tmp1, tmp2;
	double RATIO_Value;
	//	if(I2C_ReadRegByte(SC8815_ADDR, SCREG_VBAT_SET) &0x20){
	//		// �ⲿ����
	//	}else{
	//		// �ڲ�����
	//	}
	RATIO_Value = ((I2C_ReadRegByte(SC8815_ADDR, SCREG_RATIO) & 0x02) == 2) ? 5 : 12.5; //ȡ�õ�ص�ѹ�ı���
	tmp1 = I2C_ReadRegByte(SC8815_ADDR, SCREG_VBAT_FB_VALUE);           //��ȡ ADC ֵ�Ĵ���1
	tmp2 = I2C_ReadRegByte(SC8815_ADDR, SCREG_VBAT_FB_VALUE2) >> 6;     //��ȡ ADC ֵ�Ĵ���2

	//���ص�ص�ѹֵ
	return (uint16_t)((4 * tmp1 + tmp2 + 1) * RATIO_Value) * 2;
}
/****************************************
* @brief    ��ȡ SC8815 ���� ADC �Ե�ص����Ĳ������
* @return   ��λΪ mA �ĵ�ص��� (SC8815 �޵�����������)
*****************************************/
uint16_t SC8815_Read_BATT_Current(void)
{
	uint8_t tmp1, tmp2;
	uint16_t RATIO_Value;

	RATIO_Value = ((I2C_ReadRegByte(SC8815_ADDR, SCREG_RATIO) & 0x10) == 16) ? 12 : 6;  //ȡ�� IBAT �ı���
	tmp1 = I2C_ReadRegByte(SC8815_ADDR, SCREG_IBAT_VALUE);              //��ȡ ADC ֵ�Ĵ���1
	tmp2 = I2C_ReadRegByte(SC8815_ADDR, SCREG_IBAT_VALUE2) >> 6;        //��ȡ ADC ֵ�Ĵ���2

	//���� IBAT ����ֵ
	return ((50 * RATIO_Value) * (4 * tmp1 + tmp2 + 1)) / (3 * SCHW_BATT_RSHUNT);
}
/****************************************
* @brief    ��ȡ SC8815 ���� ADC �� ADIN_PIN ��ѹ�Ĳ������
* @return   ��λΪ mV �� ADIN_PIN ��ѹ
*****************************************/
uint16_t SC8815_Read_ADIN_Voltage(void)
{
	uint8_t tmp1, tmp2;

	tmp1 = I2C_ReadRegByte(SC8815_ADDR, SCREG_ADIN_VALUE);          //��ȡ ADC ֵ�Ĵ���1
	tmp2 = I2C_ReadRegByte(SC8815_ADDR, SCREG_ADIN_VALUE2) >> 6;    //��ȡ ADC ֵ�Ĵ���2

	//���� ADIN_PIN ��ѹֵ
	return (uint16_t)(4 * tmp1 + tmp2 + 1) * 2;
}

/****************************************
* @brief    ���� SC8815 �� OTG �������ʱ�������ѹ
* @param    NewVolt �µĵ�λΪ mV �������ѹ�趨ֵ
* @note     �������볬�������趨ֵ�ĵ�ѹֵ, �������� 1024mV,  ���� 1145mV �����¼������������
*****************************************/
void SC8815_SetOutputVoltage(uint16_t NewVolt)
{
	uint16_t tmp1, tmp2;
	double RATIO_Value;

	// NewVolt *= sys_config.sc8815_correct;
	//�ж� VBUS ��ѹ������ģʽ
	if (I2C_ReadRegByte(SC8815_ADDR, SCREG_CTRL1_SET) & 0x10)	// 
	{
		RATIO_Value = (double)SCHW_FB_RUP / SCHW_FB_RDOWM + 1.0;    //���������ѹ����
		tmp1 = (NewVolt / RATIO_Value) / 2;                         //�����Ӧ�Ĳο���ѹ
		// printf("RATIO_Value = %f.\n", RATIO_Value);
		//�õ� VBUSREF �Ĵ��� 2 ��ֵ
		for (tmp2 = 0; tmp2 < 3; tmp2++)
		{
			if (((tmp1 - tmp2 - 1) % 4) == 0)
			{
				break;
			}
		}

		//�õ� VBUSREF �Ĵ��� 1 ��ֵ
		tmp1 = (tmp1 - tmp2 - 1) / 4;
		// printf("tmp1 = %d, tmp2 = %d.\n", (uint8_t)tmp1, (uint8_t)tmp2 << 6);
		//д�뵽 SC8815 VBUSREF_E_SET �Ĵ���
		// printf("tmp1 = %d, tmp2 = %d.\n", (uint8_t)tmp1, (uint8_t)tmp2 << 6);
		I2C_WriteRegByte(SC8815_ADDR, SCREG_VBUSREF_E_SET, (uint8_t)tmp1);
		I2C_WriteRegByte(SC8815_ADDR, SCREG_VBUSREF_E_SET2, (uint8_t)tmp2 << 6);
	}
	else
	{
		RATIO_Value = ((I2C_ReadRegByte(SC8815_ADDR, SCREG_RATIO) & 0x01) == 1) ? 5 : 12.5; //ȡ�� VBUS ��ѹ�ı���
		// printf("RATIO_Value = %f.\n", RATIO_Value);
		tmp1 = NewVolt / RATIO_Value / 2;   //�����Ӧ�Ĳο���ѹ

		//�õ� VBUSREF �Ĵ��� 2 ��ֵ
		for (tmp2 = 0; tmp2 < 3; tmp2++)
		{
			if (((tmp1 - tmp2 - 1) % 4) == 0)
			{
				break;
			}
		}

		//�õ� VBUSREF �Ĵ��� 1 ��ֵ
		tmp1 = (tmp1 - tmp2 - 1) / 4;
		// printf("tmp1 = %d, tmp2 = %d.\n", (uint8_t)tmp1, (uint8_t)tmp2 << 6);
		//д�뵽 SC8815 VBUSREF_I_SET �Ĵ���
		I2C_WriteRegByte(SC8815_ADDR, SCREG_VBUSREF_I_SET, (uint8_t)tmp1);
		I2C_WriteRegByte(SC8815_ADDR, SCREG_VBUSREF_I_SET2, (uint8_t)tmp2 << 6);
	}

	//	printf("SC8815_SetOutputVoltage = %d.\n",NewVolt);
}
/****************************************
* @brief    ���� SC8815 VBUS ·���ϵ�����ֵ,˫��ͨ��
* @param    NewILIM �µĵ�λΪ mA ����������趨ֵ
* @note     ��С������ֵ��Ӧ���� 300mA, �������볬�������趨ֵ�ĵ���ֵ
*****************************************/
void SC8815_SetBusCurrentLimit(uint16_t NewILIM)
{
	uint8_t tmp;
	uint16_t RATIO_Value;

	RATIO_Value = ((I2C_ReadRegByte(SC8815_ADDR, SCREG_RATIO) & 0x0C) == 4) ? 6 : 3;    //ȡ�� IBUS �ı���
	tmp = (16 * (NewILIM) * (SCHW_VBUS_RSHUNT)) / (625 * RATIO_Value) - 1;              //���� LIM ����ֵ
	I2C_WriteRegByte(SC8815_ADDR, SCREG_IBUS_LIM_SET, tmp);                             //д�뵽 SC8815 �Ĵ���

	//printf("SC8815_SetBusCurrentLimit = %d.\n",NewILIM);
}
/****************************************
* @brief    ���� SC8815 ���·���ϵ�����ֵ,˫��ͨ��
* @param    NewILIM �µĵ�λΪ mA �ĵ�������趨ֵ
* @note     ��С������ֵ��Ӧ���� 300mA, �������볬�������趨ֵ�ĵ���ֵ
*****************************************/
void SC8815_SetBatteryCurrLimit(uint16_t NewILIM)
{
	uint8_t tmp;
	uint16_t RATIO_Value;

	RATIO_Value = ((I2C_ReadRegByte(SC8815_ADDR, SCREG_RATIO) & 0x10) == 16) ? 12 : 6; //ȡ�� IBAT �ı���
	// tmp = (16 * (NewILIM) * (SCHW_VBUS_RSHUNT)) / (625 * RATIO_Value) - 1;             //���� LIM ����ֵ
	tmp = (16 * (NewILIM) * (SCHW_BATT_RSHUNT)) / (625 * RATIO_Value) - 1;             //���� LIM ����ֵ
	I2C_WriteRegByte(SC8815_ADDR, SCREG_IBAT_LIM_SET, tmp);                            //д�뵽 SC8815 �Ĵ���

	//printf("SC8815_SetBatteryCurrLimit = %d real=%d tmp=%02X.\n",NewILIM,SC8815_GetBatteryCurrLimit(),tmp);
}
/****************************************
* @brief    ���� SC8815 VINREG ��ѹֵ (���� MPPT�����ⲿ��ѹ�ﵽָ��ֵʱ�ſ�ʼ�Ե�س��)
* @param    NewVolt �µĵ�λΪ mV �� VINREG ��ѹ�趨ֵ
* @note     �������볬�������趨ֵ�ĵ�ѹֵ
*****************************************/
void SC8815_VINREG_SetVoltage(uint16_t NewVolt)
{
	uint16_t RATIO_Value;
	RATIO_Value = ((I2C_ReadRegByte(SC8815_ADDR, SCREG_CTRL0_SET) & 0x10) == 16) ? 40 : 100;    //ȡ�� VINREG �ı���
	I2C_WriteRegByte(SC8815_ADDR, SCREG_VINREG_SET, (NewVolt / RATIO_Value) - 1);               //д�뵽 SC8815 �Ĵ���
}

/****************************************
* @brief    ���� SC8815 VBUS��ѹ����ģʽ
* @return   ��λΪ mV �ĵ�ѹֵ
*****************************************/
void SC8815_SetVBUSFBMode(uint8_t mode)
{
	SC8815_HardwareInitStruct.FB_Mode &= ~0x10;
	SC8815_HardwareInitStruct.FB_Mode |= mode;
	SC8815_HardwareInit(&SC8815_HardwareInitStruct);
}

uint8_t SC8815_GetVBUSFBMode(void)
{
	return SC8815_HardwareInitStruct.FB_Mode;
}

/****************************************
* @brief    ��ȡ SC8815 �� OTG �������ʱ�������ѹ�趨ֵ
* @return   ��λΪ mV �������ѹ�趨ֵ
*****************************************/
uint16_t SC8815_GetOutputVoltage(void)
{
	uint16_t tmp1, tmp2;
	double RATIO_Value;

	//�ж� VBUS ��ѹ������ģʽ
	if (I2C_ReadRegByte(SC8815_ADDR, SCREG_CTRL1_SET) & 0x10)
	{
		//��ȡ VBUSREF_E_SET �Ĵ���
		tmp1 = I2C_ReadRegByte(SC8815_ADDR, SCREG_VBUSREF_E_SET);
		tmp2 = I2C_ReadRegByte(SC8815_ADDR, SCREG_VBUSREF_E_SET2 >> 6);

		//���������ѹ����
		RATIO_Value = (double)SCHW_FB_RUP / SCHW_FB_RDOWM + 1.0;
	}
	else
	{
		//��ȡ VBUSREF_E_SET �Ĵ���
		tmp1 = I2C_ReadRegByte(SC8815_ADDR, SCREG_VBUSREF_E_SET);
		tmp2 = I2C_ReadRegByte(SC8815_ADDR, SCREG_VBUSREF_E_SET2 >> 6);

		//ȡ��VBUS��ѹ�ı���
		RATIO_Value = ((I2C_ReadRegByte(SC8815_ADDR, SCREG_RATIO) & 0x01) == 1) ? 5 : 12.5;
	}

	//���ض�Ӧ�������ѹ
	return (uint16_t)((4 * tmp1 + tmp2 + 1) * RATIO_Value) * 2;
}
/****************************************
* @brief    ��ȡ SC8815 VBUS ·���ϵ������趨ֵ
* @return   ��λΪ mA �� VBUS ·�������趨ֵ
*****************************************/
uint16_t SC8815_GetBusCurrentLimit(void)
{
	uint8_t tmp;
	uint16_t RATIO_Value;

	RATIO_Value = ((I2C_ReadRegByte(SC8815_ADDR, SCREG_RATIO) & 0x0C) == 4) ? 6 : 3; //ȡ�� IBUS �ı���
	tmp = I2C_ReadRegByte(SC8815_ADDR, SCREG_IBUS_LIM_SET);                          //ȡ�� IBUS �����Ĵ���ֵ

	//����IBUS����ֵ
	return (uint16_t)((uint32_t)(625 * (RATIO_Value) * (tmp + 1)) / (16 * (SCHW_VBUS_RSHUNT)));
}
/****************************************
* @brief    ��ȡ SC8815 ���·���ϵ������趨ֵ
* @return   ��λΪ mA �ĵ��·�������趨ֵ
*****************************************/
uint16_t SC8815_GetBatteryCurrLimit(void)
{
	uint8_t tmp;
	uint16_t RATIO_Value;

	RATIO_Value = ((I2C_ReadRegByte(SC8815_ADDR, SCREG_RATIO) & 0x10) == 16) ? 12 : 6; //ȡ�� IBAT �ı���
	tmp = I2C_ReadRegByte(SC8815_ADDR, SCREG_IBAT_LIM_SET);                            //ȡ�� IBAT �����Ĵ���ֵ

	//����IBAT����ֵ
	return (uint16_t)((uint32_t)(625 * (RATIO_Value) * (tmp + 1)) / (16 * (SCHW_BATT_RSHUNT)));
}
/****************************************
* @brief    ��ȡ SC8815 VINREG ��ѹ�趨ֵ
* @return   ��λΪ mV �� VINREG ��ѹ�趨ֵ
*****************************************/
uint16_t SC8815_VINREG_GetVoltage(void)
{
	uint8_t tmp;
	uint16_t RATIO_Value;
	RATIO_Value = ((I2C_ReadRegByte(SC8815_ADDR, SCREG_CTRL0_SET) & 0x10) == 16) ? 40 : 100; //ȡ�� VINREG �ı���
	tmp = I2C_ReadRegByte(SC8815_ADDR, SCREG_VINREG_SET);                                    //ȡ�� VINREG �Ĵ���ֵ
	return tmp * RATIO_Value;
}
/****************************************
* @brief    ��ȡ SC8815 OTG ģʽ�µ����������ѹ
* @return   ��λΪ mV �ĵ�ѹֵ
*****************************************/
uint16_t SC8815_GetMaxOutputVoltage(void)
{
	double RATIO_Value;

	//�ж� VBUS ��ѹ������ģʽ
	//if (I2C_ReadRegByte(SC8815_ADDR, SCREG_CTRL1_SET) & 0x10)
	if (I2C_ReadRegByte(SC8815_ADDR, SCREG_CTRL1_SET) & 0x80)
	{
		//���������ѹ����
		RATIO_Value = (double)SCHW_FB_RUP / SCHW_FB_RDOWM + 1.0;
	}
	else
	{
		//ȡ�� VBUS ��ѹ�ı���
		RATIO_Value = ((I2C_ReadRegByte(SC8815_ADDR, SCREG_RATIO) & 0x01) == 1) ? 5 : 12.5;
	}

	//������������ѹֵ
	return 2048 * RATIO_Value;
}
/****************************************
* @brief    ��ȡ SC8815 OTG ģʽ�µ������ѹ����ֵ
* @return   ��λΪ mV �ĵ�ѹֵ
*****************************************/
uint16_t SC8815_GetOutputVoltageSetp(void)
{
	double RATIO_Value;

	//�ж� VBUS ��ѹ������ģʽ
	//if (I2C_ReadRegByte(SC8815_ADDR, SCREG_CTRL1_SET) & 0x10)
	if (I2C_ReadRegByte(SC8815_ADDR, SCREG_CTRL1_SET) & 0x80)
	{
		//���������ѹ����
		RATIO_Value = (double)SCHW_FB_RUP / SCHW_FB_RDOWM + 1.0;
	}
	else
	{
		//ȡ�� VBUS ��ѹ�ı���
		RATIO_Value = ((I2C_ReadRegByte(SC8815_ADDR, SCREG_RATIO) & 0x01) == 1) ? 5 : 12.5;
	}

	//���������ѹ����ֵ
	return 2 * RATIO_Value;
}


/****************************************
* @brief    �� OTG ����ŵ�ģʽ
*****************************************/
void SC8815_OTG_Enable(void)
{
	//���� EN_OTG λ
	I2C_WriteRegByte(SC8815_ADDR, SCREG_CTRL0_SET, I2C_ReadRegByte(SC8815_ADDR, SCREG_CTRL0_SET) | 0x80);
}
/****************************************
* @brief    �ر� OTG ����ŵ�ģʽ, SC8815 �����ڳ��ģʽ
*****************************************/
void SC8815_OTG_Disable(void)
{
	//��� EN_OTG λ
	I2C_WriteRegByte(SC8815_ADDR, SCREG_CTRL0_SET, I2C_ReadRegByte(SC8815_ADDR, SCREG_CTRL0_SET) & 0x7F);
}
/****************************************
* @brief    ���� VINREG ������Ϊ 40x
*****************************************/
void SC8815_VINREG_SetRatio_40x(void)
{
	//���� VINREG_RATIO λ
	I2C_WriteRegByte(SC8815_ADDR, SCREG_CTRL0_SET, I2C_ReadRegByte(SC8815_ADDR, SCREG_CTRL0_SET) | 0x10);
}
/****************************************
* @brief    ���� VINREG ������Ϊ 100x
*****************************************/
void SC8815_VINREG_SetRatio_100x(void)
{
	//��� VINREG_RATIO λ
	I2C_WriteRegByte(SC8815_ADDR, SCREG_CTRL0_SET, I2C_ReadRegByte(SC8815_ADDR, SCREG_CTRL0_SET) & 0xEF);
}
/****************************************
* @brief    �� OTG ����ŵ�ģʽ�е� VBUS ��ѹ��������
*****************************************/
void SC8815_OVP_Enable(void)
{
	//��� DIS_OVP λ
	I2C_WriteRegByte(SC8815_ADDR, SCREG_CTRL1_SET, I2C_ReadRegByte(SC8815_ADDR, SCREG_CTRL1_SET) & 0xFB);
}
/****************************************
* @brief    �ر� OTG ����ŵ�ģʽ�е� VBUS ��ѹ��������
*****************************************/
void SC8815_OVP_Disable(void)
{
	//���� DIS_OVP λ
	I2C_WriteRegByte(SC8815_ADDR, SCREG_CTRL1_SET, I2C_ReadRegByte(SC8815_ADDR, SCREG_CTRL1_SET) | 0x04);
}
/****************************************
* @brief    �� PGATE ���Ź���, ����͵�ƽ�� PMOS
*****************************************/
void SC8815_PGATE_Enable(void)
{
	//���� EN_PGATE λ
	I2C_WriteRegByte(SC8815_ADDR, SCREG_CTRL3_SET, I2C_ReadRegByte(SC8815_ADDR, SCREG_CTRL3_SET) | 0x80);
}
/****************************************
* @brief    �ر� PGATE ���Ź���, ����ߵ�ƽ�ر� PMOS
*****************************************/
void SC8815_PGATE_Disable(void)
{
	//��� EN_PGATE λ
	I2C_WriteRegByte(SC8815_ADDR, SCREG_CTRL3_SET, I2C_ReadRegByte(SC8815_ADDR, SCREG_CTRL3_SET) & 0x7F);
}
/****************************************
* @brief    �� GPO ���Ź���, ����͵�ƽ
*****************************************/
void SC8815_GPO_Enable(void)
{
	//���� GPO_CTRL λ
	I2C_WriteRegByte(SC8815_ADDR, SCREG_CTRL3_SET, I2C_ReadRegByte(SC8815_ADDR, SCREG_CTRL3_SET) | 0x40);
}
/****************************************
* @brief    �ر� GPO ���Ź���, �������״̬
*****************************************/
void SC8815_GPO_Disable(void)
{
	//��� GPO_CTRL λ
	I2C_WriteRegByte(SC8815_ADDR, SCREG_CTRL3_SET, I2C_ReadRegByte(SC8815_ADDR, SCREG_CTRL3_SET) & 0xBF);
}
/****************************************
* @brief    �� ADC ɨ��, ��ʱ���Զ�ȡ ADC ����
*****************************************/
void SC8815_ADC_Enable(void)
{
	//���� AD_START λ
	I2C_WriteRegByte(SC8815_ADDR, SCREG_CTRL3_SET, I2C_ReadRegByte(SC8815_ADDR, SCREG_CTRL3_SET) | 0x20);
}
/****************************************
* @brief    �ر� ADC ɨ��, ��Լ 1-2mA �ĺĵ�
*****************************************/
void SC8815_ADC_Disable(void)
{
	//��� AD_START λ
	I2C_WriteRegByte(SC8815_ADDR, SCREG_CTRL3_SET, I2C_ReadRegByte(SC8815_ADDR, SCREG_CTRL3_SET) & 0xDF);
}
/****************************************
* @brief    �� OTG ����ŵ�ģʽ�е� VBUS �� VBAT ��·��������
*****************************************/
void SC8815_SFB_Enable(void)
{
	//��� DIS_ShortFoldBack λ
	I2C_WriteRegByte(SC8815_ADDR, SCREG_CTRL3_SET, I2C_ReadRegByte(SC8815_ADDR, SCREG_CTRL3_SET) & 0xFB);
}
/****************************************
* @brief    �ر� OTG ����ŵ�ģʽ�е� VBUS �� VBAT ��·��������
*****************************************/
void SC8815_SFB_Disable(void)
{
	//���� DIS_ShortFoldBack λ
	I2C_WriteRegByte(SC8815_ADDR, SCREG_CTRL3_SET, I2C_ReadRegByte(SC8815_ADDR, SCREG_CTRL3_SET) | 0x04);
}
/****************************************
* @brief    �� OTG ģʽ�����������µ� PFM ģʽ
*****************************************/
void SC8815_PFM_Enable(void)
{
	//���� EN_PFM λ
	I2C_WriteRegByte(SC8815_ADDR, SCREG_CTRL3_SET, I2C_ReadRegByte(SC8815_ADDR, SCREG_CTRL3_SET) | 0x01);
}
/****************************************
* @brief    �ر� OTG ģʽ�����������µ� PFM ģʽ
*****************************************/
void SC8815_PFM_Disable(void)
{
	//��� EN_PFM λ
	I2C_WriteRegByte(SC8815_ADDR, SCREG_CTRL3_SET, I2C_ReadRegByte(SC8815_ADDR, SCREG_CTRL3_SET) & 0xFE);
}

/****************************************
* @brief    ��� OTG �Ƿ��ڴ�״̬
* @return   OTG ���ܵ�״̬ (1b �� 0b)
*****************************************/
uint8_t SC8815_OTG_IsEnable(void)
{
	//���� OTG ��״̬
	return (I2C_ReadRegByte(SC8815_ADDR, SCREG_CTRL0_SET) & 0x80) ? 1 : 0;
}
/****************************************
* @brief    ��ȡ VINREG ������
* @return   VINREG ������ (40 �� 100)
*****************************************/
uint8_t SC8815_VINREG_GetRatio(void)
{
	//���� VINREG ������
	return (I2C_ReadRegByte(SC8815_ADDR, SCREG_CTRL0_SET) & 0x10) ? 40 : 100;
}
/****************************************
* @brief    ��� OTG ģʽ�� OVP(��ѹ����) �����Ƿ��ڴ�״̬
* @return   OVP ���ܵ�״̬ (1b �� 0b)
*****************************************/
uint8_t SC8815_OVP_IsEnable(void)
{
	//���� OVP ��״̬
	return (I2C_ReadRegByte(SC8815_ADDR, SCREG_CTRL1_SET) & 0x04) ? 0 : 1;
}
/****************************************
* @brief    ��� PGATE ���Ź����Ƿ��ڴ�״̬
* @return   PGATE ���Ź��ܵ�״̬ (1b �� 0b)
*****************************************/
uint8_t SC8815_PGATE_IsEnable(void)
{
	//���� PGATE ��״̬
	return (I2C_ReadRegByte(SC8815_ADDR, SCREG_CTRL3_SET) & 0x80) ? 1 : 0;
}
/****************************************
* @brief    ��� GPO ���Ź����Ƿ��ڴ�״̬
* @return   GPO ���Ź��ܵ�״̬ (1b �� 0b)
*****************************************/
uint8_t SC8815_GPO_IsEnable(void)
{
	//���� GPO ��״̬
	return (I2C_ReadRegByte(SC8815_ADDR, SCREG_CTRL3_SET) & 0x40) ? 1 : 0;
}
/****************************************
* @brief    ��� ADC ɨ���Ƿ��ڴ�״̬
* @return   ADC ɨ���״̬ (1b �� 0b)
*****************************************/
uint8_t SC8815_ADC_IsEnable(void)
{
	//���� ADC ��״̬
	return (I2C_ReadRegByte(SC8815_ADDR, SCREG_CTRL3_SET) & 0x20) ? 1 : 0;
}
/****************************************
* @brief    ��� OTG ģʽ�� ��·���� �����Ƿ��ڴ�״̬
* @return   ��·�������ܵ�״̬ (1b �� 0b)
*****************************************/
uint8_t SC8815_SFB_IsEnable(void)
{
	//���� ��·�������� ��״̬
	return (I2C_ReadRegByte(SC8815_ADDR, SCREG_CTRL3_SET) & 0x04) ? 0 : 1;
}
/****************************************
* @brief    ��� OTG ģʽ�е� PFM ģʽ�Ƿ��ڴ�״̬
* @return   PFM ģʽ��״̬ (1b �� 0b)
*****************************************/
uint8_t SC8815_PFM_IsEnable(void)
{
	//���� PFM ��״̬
	return (I2C_ReadRegByte(SC8815_ADDR, SCREG_CTRL3_SET) & 0x01) ? 1 : 0;
}


// ��������
/****************************************
* @brief    ���� SC8815 �� OTG �������ʱ�������ѹ
* @param    NewCell Ϊ���������֧������1-4��ֵ
* @note
*****************************************/
void SC8815_SetBatteryCell(uint8_t NewCell)
{
	//	if(NewCell <1 || NewCell>4)return;
	//	
	//	uint8_t reg = SCBAT_CSEL_1S;
	//	switch(NewCell)
	//	{
	//		case 1:{reg = SCBAT_CSEL_1S;break;}
	//		case 2:{reg = SCBAT_CSEL_2S;break;}
	//		case 3:{reg = SCBAT_CSEL_3S;break;}
	//		case 4:{reg = SCBAT_CSEL_4S;break;}
	//	}
	uint8_t tmp = I2C_ReadRegByte(SC8815_ADDR, SCREG_VBAT_SET);
	tmp &= ~0x18;
	tmp |= NewCell;
	I2C_WriteRegByte(SC8815_ADDR, SCREG_VBAT_SET, tmp);
}
/****************************************
* @brief    ��ȡ�������
* @return   ����ֵΪ��ֵ1-4����ʾ�������
*****************************************/
uint8_t SC8815_GetBatteryCell(void)
{
	uint8_t tmp = I2C_ReadRegByte(SC8815_ADDR, SCREG_VBAT_SET);
	uint8_t cell = 1;
	if (tmp & SCBAT_CSEL_2S) {
		cell = 2;
	}
	else if (tmp & SCBAT_CSEL_3S) {
		cell = 3;
	}
	else if (tmp & SCBAT_CSEL_4S) {
		cell = 4;
	}
	return cell;
}

/****************************************
* @brief    ���� SC8815 �����л�Ƶ��
* @param    NewFreq ��ֵ����Ϊ 1=����Ϊ150KHz 2=����Ϊת��300KHz���150KHz 3=������Ϊ300KHz 4=����Ϊ450KHz
* @note
*****************************************/
void SC8815_SetSWFreq(uint8_t NewFreq)
{
	uint8_t swFreq;
	switch (NewFreq)
	{
	case 1: { swFreq = SCHWI_FREQ_150KHz;break; }
	case 2: { swFreq = SCHWI_FREQ_300KHz_1;break; }
	case 3: { swFreq = SCHWI_FREQ_300KHz_2;break; }
	case 4: { swFreq = SCHWI_FREQ_450KHz;break; }
	default:return;
	}

	uint8_t tmp;
	tmp = I2C_ReadRegByte(SC8815_ADDR, SCREG_CTRL0_SET) & 0x60;
	tmp &= ~0x0C;
	tmp |= swFreq;
	I2C_WriteRegByte(SC8815_ADDR, SCREG_CTRL0_SET, tmp);
}

/****************************************
* @brief    ��ȡ�����л�Ƶ��
* @return   ����ֵ 1=����Ϊ150KHz 2=����Ϊת��300KHz���150KHz 3=������Ϊ300KHz 4=����Ϊ450KHz
*****************************************/
uint8_t SC8815_GetSWFreq(void)
{
	uint8_t tmp;
	tmp = I2C_ReadRegByte(SC8815_ADDR, SCREG_CTRL0_SET);

	tmp &= 0x0C;
	switch (tmp)
	{
	case SCHWI_FREQ_150KHz: { return 1; }
	case SCHWI_FREQ_300KHz_1: { return 2; }
	case SCHWI_FREQ_300KHz_2: { return 3; }
	case SCHWI_FREQ_450KHz: { return 4; }
	default:return 0;
	}
}

/****************************************
* @brief    ���� SC8815 ����ʱ��
* @param    NewDT ��ֵ����Ϊ 1=20ns 2=40ns 3=60ns 4=80ns
* @note
*****************************************/
void SC8815_SetDeadTime(uint8_t NewDT)
{
	uint8_t val;
	switch (NewDT)
	{
	case 1: { val = SCHWI_DT_20ns;break; }
	case 2: { val = SCHWI_DT_40ns;break; }
	case 3: { val = SCHWI_DT_60ns;break; }
	case 4: { val = SCHWI_DT_80ns;break; }
	default:return;
	}

	uint8_t tmp;
	tmp = I2C_ReadRegByte(SC8815_ADDR, SCREG_CTRL0_SET) & ~0x03;
	tmp |= val;
	I2C_WriteRegByte(SC8815_ADDR, SCREG_CTRL0_SET, tmp);
}

/****************************************
* @brief    ��ȡ����ʱ��
* @return   ����ֵ����Ϊ  1=20ns 2=40ns 3=60ns 4=80ns
*****************************************/
uint8_t SC8815_GetDeadTime(void)
{
	uint8_t tmp;
	tmp = I2C_ReadRegByte(SC8815_ADDR, SCREG_CTRL0_SET) & ~0x03;
	switch (tmp)
	{
	case SCHWI_DT_20ns: { return 1; }
	case SCHWI_DT_40ns: { return 2; }
	case SCHWI_DT_60ns: { return 3; }
	case SCHWI_DT_80ns: { return 4; }
	default:return 0;
	}
}

/****************************************
* @brief    ���� SC8815 ������·��Ӧ����
* @param    NewILIMBW ��ֵ����Ϊ 1=5KHz 2=1.25KHz
* @note
*****************************************/
void SC8815_SetILIMBW(uint8_t NewILIMBW)
{
	uint8_t val;
	switch (NewILIMBW)
	{
	case 1: { val = SCHWI_ILIM_BW_5KHz;break; }
	case 2: { val = SCHWI_ILIM_BW_1_25KHz;break; }
	default:return;
	}

	uint8_t tmp;
	tmp = I2C_ReadRegByte(SC8815_ADDR, SCREG_CTRL3_SET) & ~0x10;
	tmp |= val;
	I2C_WriteRegByte(SC8815_ADDR, SCREG_CTRL3_SET, tmp);
}

/****************************************
* @brief    ��ȡ������·��Ӧ����
* @return   ����ֵ 1=5KHz 2=1.25KHz
*****************************************/
uint8_t SC8815_GetILIMBW(void)
{
	uint8_t tmp;
	tmp = I2C_ReadRegByte(SC8815_ADDR, SCREG_CTRL2_SET);
	return (tmp & SCHWI_ILIM_BW_1_25KHz) ? 2 : 1;
}

/****************************************
* @brief    ���� SC8815 ��·��Ӧģʽ
* @param    NewLoop ��ֵ����Ϊ 1=������·��Ӧ 2=���ƻ�·��Ӧ
* @note
*****************************************/
void SC8815_SetLoop(uint8_t NewLoop)
{
	uint8_t val;
	switch (NewLoop)
	{
	case 1: { val = SCHWI_LOOP_Normal;break; }
	case 2: { val = SCHWI_LOOP_Improved;break; }
	default:return;
	}

	uint8_t tmp;
	tmp = I2C_ReadRegByte(SC8815_ADDR, SCREG_CTRL3_SET) & ~0x08;
	tmp |= val;
	I2C_WriteRegByte(SC8815_ADDR, SCREG_CTRL3_SET, tmp);
}

/****************************************
* @brief    ��ȡ��·��Ӧģʽ
* @return   ����ֵ 1=������·��Ӧ 2=���ƻ�·��Ӧ
*****************************************/
uint8_t SC8815_GetLoop(void)
{
	uint8_t tmp;
	tmp = I2C_ReadRegByte(SC8815_ADDR, SCREG_CTRL2_SET);
	return (tmp & SCHWI_LOOP_Improved) ? 2 : 1;
}

/****************************************
* @brief    ����VBUS����ģʽ
* @param    NewVal ��ֵ����Ϊ
						SCHWI_FB_Internal		OTG ����ŵ�ģʽ VBUS ��ѹ����ʹ�� VBUS_PIN ��оƬ�ڲ�����
						SCHWI_FB_External 	OTG ����ŵ�ģʽ VBUS ��ѹ����ʹ�� FB_PIN �ϵ��ⲿ��ѹ�������練��
* @note
*****************************************/
void SC8815_SetOTGFBMode(uint8_t NewVal)
{
	uint8_t tmp;
	//Ӳ������1
	tmp = I2C_ReadRegByte(SC8815_ADDR, SCREG_CTRL1_SET) & 0x03;
	tmp &= ~0x10;
	tmp |= NewVal;
	I2C_WriteRegByte(SC8815_ADDR, SCREG_CTRL1_SET, tmp);
}

/**
 *@brief ��ȡVBUS SHORT״̬
 * 
 * @return ����ֵ0δ���뱣��ģʽ������ֵ1���뱣��ģʽ 
 */
uint8_t SC8815_GetVBUSShort(void)
{
	uint8_t temp = I2C_ReadRegByte(SC8815_ADDR, SCREG_STATUS) & 0x08;
	return (temp == 0x08) ? 1 : 0;
}

/*****END OF FILE****/
