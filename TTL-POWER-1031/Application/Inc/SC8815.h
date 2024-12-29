/*---------------------------------------------------------------------------/
/  SC8815 ͨ�ù̼���ͷ�ļ� V0.2 (C)Sghz��2021
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


//�ݹ��������
#ifndef SC8815_H
#define SC8815_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

//������׼���Ͷ���ͷ�ļ�
#include <stdint.h>

//ʹ�ô˿���Ҫ�ṩ�����ⲿ����,����SC8815����Ҫʹ�õĺ���
extern void I2C_WriteRegByte(uint8_t SlaveAddress, uint8_t RegAddress, uint8_t ByteData);   //ͨ��I2C���豸�Ĵ���дһ���ֽ�
extern uint8_t I2C_ReadRegByte(uint8_t SlaveAddress, uint8_t RegAddress);                   //ͨ��I2C���豸�Ĵ�����һ���ֽ�
extern void SoftwareDelay(uint8_t ms);                                                      //�����ʱ����

//���� SC8815 ����·���ϵĸв����ֵ, ӦΪ 10 �� 5 ���������� (��λ:mOhm)
#define SCHW_VBUS_RSHUNT        5.3          //VBUS ����·���ϵĸв����ֵ
#define SCHW_BATT_RSHUNT        5.3          //��ص���·���ϵĸв����ֵ

//���� FB �����ϵķ�ѹ������ֵ, ʹ�õ� VBUS ����ģʽΪ�ⲿ����ʱ, ��ͨ����Щֵ�����Ӧ�����ѹ (��λ:Ohm)
#define SCHW_FB_RUP             120000      //RUP Ϊ�� FB ���ӵ� VBUS ֮��ĵ���ֵ
#define SCHW_FB_RDOWM           6800        //RDOWM Ϊ�� FB ���ӵ� GND ֮��ĵ���ֵ


//����״̬����
#define sENABLE                 0x01        //ʹ�ܹ���
#define sDISABLE                0x00        //ʧ�ܹ���

//SC8815 �Ĵ�����ַ����
// #define SC8815_ADDR             0x74        //SC8815 I2C �豸��ַ
#define SC8815_ADDR             0x74 << 1        //SC8815 I2C �豸��ַ
#define SCREG_VBAT_SET          0x00        //������üĴ���
#define SCREG_VBUSREF_I_SET     0x01        //VBUS �ڲ���׼�Ĵ���1
#define SCREG_VBUSREF_I_SET2    0x02        //VBUS �ڲ���׼�Ĵ���2
#define SCREG_VBUSREF_E_SET     0x03        //VBUS �ⲿ��׼�Ĵ���1
#define SCREG_VBUSREF_E_SET2    0x04        //VBUS �ⲿ��׼�Ĵ���2
#define SCREG_IBUS_LIM_SET      0x05        //VBUS �������üĴ���
#define SCREG_IBAT_LIM_SET      0x06        //����������üĴ���
#define SCREG_VINREG_SET        0x07        //VINREG ���üĴ���
#define SCREG_RATIO             0x08        //�������üĴ���
#define SCREG_CTRL0_SET         0x09        //Ӳ�����üĴ���0
#define SCREG_CTRL1_SET         0x0A        //Ӳ�����üĴ���1
#define SCREG_CTRL2_SET         0x0B        //Ӳ�����üĴ���2
#define SCREG_CTRL3_SET         0x0C        //Ӳ�����üĴ���3
#define SCREG_VBUS_FB_VALUE     0x0D        //VBUS ��ѹ�����Ĵ���1
#define SCREG_VBUS_FB_VALUE2    0x0E        //VBUS ��ѹ�����Ĵ���2
#define SCREG_VBAT_FB_VALUE     0x0F        //VBAT ��ѹ�����Ĵ���1
#define SCREG_VBAT_FB_VALUE2    0x10        //VBAT ��ѹ�����Ĵ���2
#define SCREG_IBUS_VALUE        0x11        //BUS ���������Ĵ���1
#define SCREG_IBUS_VALUE2       0x12        //BUS ���������Ĵ���2
#define SCREG_IBAT_VALUE        0x13        //BAT ���������Ĵ���1
#define SCREG_IBAT_VALUE2       0x14        //BAT ���������Ĵ���2
#define SCREG_ADIN_VALUE        0x15        //ADIN ���ŵ�ѹ�����Ĵ���1
#define SCREG_ADIN_VALUE2       0x16        //ADIN ���ŵ�ѹ�����Ĵ���2
#define SCREG_STATUS            0x17        //�ж�״̬�Ĵ���
#define SCREG_MASK              0x19        //�ж����μĴ���


//SC8815 ������ýṹ�嶨��
typedef struct
{
    uint8_t IRCOMP;             /*������貹������.
                                    -�������Ӧȡ SCBAT_IRCOMP �ж����ֵ*/

    uint8_t VBAT_SEL;           /*��ص�ѹ����ģʽ����.
                                    -�������Ӧȡ SCBAT_VSEL �ж����ֵ*/

    uint8_t CSEL;               /*��ش�����������.
                                    -�������Ӧȡ SCBAT_CSEL �ж����ֵ*/

    uint8_t VCELL;              /*���õ��ڵ�صĵ�ѹ.
                                    -�������Ӧȡ SCBAT_VCELL �ж����ֵ*/

}SC8815_BatteryConfigTypeDef;

//SC8815 Ӳ����ʼ���ṹ�嶨��
typedef struct
{
    uint8_t IBAT_RATIO;         /*��ص�����������.
                                    -�������Ӧȡ SCHWI_IBAT_RATIO �ж����ֵ*/

    uint8_t IBUS_RATIO;         /*VBUS ������������.
                                    -�������Ӧȡ SCHWI_IBUS_RATIO �ж����ֵ*/

    uint8_t VBAT_RATIO;         /*��ص�ѹ��������.
                                    -�������Ӧȡ SCHWI_VBAT_RATIO �ж����ֵ*/

    uint8_t VBUS_RATIO;         /*VBUS ��ѹ��������.
                                    -�������Ӧȡ SCHWI_VBUS_RATIO �ж����ֵ*/

    uint8_t VINREG_Ratio;       /*VINREG ��ѹ��������.
                                    -�������Ӧȡ SCHWI_VINREG_RATIO �ж����ֵ*/

    uint8_t SW_FREQ;            /*���� SC8815 �Ŀ���Ƶ��.
                                    -�������Ӧȡ SCHWI_FREQ �ж����ֵ*/

    uint8_t DeadTime;           /*���ÿ�������ʱ��.
                                    -�������Ӧȡ SCHWI_DT �ж����ֵ*/

    uint8_t ICHAR;              /*���ó������ο�,�������������ֹ���������ڴ˲ο�.
                                    -�������Ӧȡ SCHWI_ICHAR �ж����ֵ*/

    uint8_t TRICKLE;            /*���������繦�ܵĹرպʹ�.
                                    -�������Ӧȡ SCHWI_TRICKLE �ж����ֵ*/

    uint8_t TERM;               /*�򿪻�رճ���Զ���ֹ����.
                                    -�������Ӧȡ SCHWI_TERM �ж����ֵ*/

    uint8_t FB_Mode;            /*���� SC8815 ����ŵ�ģʽ�� VBUS ��ѹ����ģʽ.
                                    -�������Ӧȡ SCHWI_FB_MODE �ж����ֵ*/

    uint8_t TRICKLE_SET;        /*������������λբֵ.
                                    -�������Ӧȡ SCHWI_TRICKLE_SET �ж����ֵ*/

    uint8_t OVP;                /*�򿪻�ر� SC8815 ����ŵ�ģʽ�еĹ�ѹ��������.
                                    -�������Ӧȡ SCHWI_OVP �ж����ֵ*/

    uint8_t DITHER;             /*�򿪻�ر� SC8815 Ƶ�ʶ�������.
                                    -�������Ӧȡ SCHWI_DITHER �ж����ֵ*/

    uint8_t SLEW_SET;           /*���� SC8815 ����ŵ�ģʽ��VBUS��̬�仯������.
                                    -�������Ӧȡ SCHWI_SLEW_SET �ж����ֵ*/

    uint8_t ADC_SCAN;           /*�򿪻�ر� SC8815 �ڲ�ADCת������.
                                    -�������Ӧȡ SCHWI_ADC_SCAN �ж����ֵ*/

    uint8_t ILIM_BW;            /*���� SC8815 ������·�Ĵ���.
                                    -�������Ӧȡ SCHWI_ILIM_BW �ж����ֵ*/

    uint8_t LOOP;               /*���� SC8815 ��·��Ӧ����ģʽ.
                                    -�������Ӧȡ SCHWI_LOOP �ж����ֵ*/

    uint8_t ShortFoldBack;      /*�򿪻�ر� SC8815 ����ŵ�ģʽ�е� VBUS ��·��������.
                                    -�������Ӧȡ SCHWI_SFB �ж����ֵ*/

    uint8_t EOC;                /*���� SC8815 �������ļ��բֵ.
                                    -�������Ӧȡ SCHWI_EOC �ж����ֵ*/

    uint8_t PFM;                /*�򿪻�ر� SC8815 ����ŵ��������µ� PFM ģʽ.
                                    -�������Ӧȡ SCHWI_PFM �ж����ֵ*/

}SC8815_HardwareInitTypeDef;

//SC8815 �ж�״̬�ṹ�嶨��
typedef struct
{
    uint8_t AC_OK;              /*�� ACIN ���Ŵ���⵽�����������ж�.*/

    uint8_t INDET;              /*�� INDET ���Ŵ���⵽�����������ж�.*/

    uint8_t VBUS_SHORT;         /*����ŵ�ģʽ�¼�⵽ VBUS ��·�����ж�.*/

    uint8_t OTP;                /*���� OTP(����) �����ж�.*/

    uint8_t EOC;                /*���� EOC(������) �����ж�.*/

    /*�������ж�����ʱ,��Ա����Ϊ sENABLE(1) ��ʾ���жϣ����� sDISABLE(0) Ϊ��ʾ�ر��ж�
    * ��������ȡ�ж�״̬ʱ,��ԱֵΪ 1 ��ʾ�жϷ���,Ϊ 0 ��ʾ�ж�δ����.
    * ��ȡ�ж�״̬�Ĵ���������ж�״̬.*/

}SC8815_InterruptStatusTypeDef;


/*
* SC8815 ������ýṹ���������
* @{
*/
    // SCBAT_IRCOMP ��������
    #define SCBAT_IRCOMP_0mR            0x00    //�������Ϊ 0 mOhm
    #define SCBAT_IRCOMP_20mR           0x40    //�������Ϊ 20 mOhm
    #define SCBAT_IRCOMP_40mR           0x80    //�������Ϊ 40 mOhm
    #define SCBAT_IRCOMP_80mR           0xC0    //�������Ϊ 80 mOhm

    // SCBAT_VSEL ��������
    #define SCBAT_VBAT_SEL_Internal     0x00    //��ص�ѹ����Ϊ�ڲ�����(VBATS����ֱ�����ӵ��)
    #define SCBAT_VBAT_SEL_External     0x20    //�ⲿ����(��ص�ѹ������ѹ��������VBATS����)

    // SCBAT_CSEL ��������
    #define SCBAT_CSEL_1S               0x00    //��ؽ���Ϊ1��
    #define SCBAT_CSEL_2S               0x08    //��ؽ���Ϊ2��
    #define SCBAT_CSEL_3S               0x10    //��ؽ���Ϊ3��
    #define SCBAT_CSEL_4S               0x18    //��ؽ���Ϊ4��

    // SCBAT_VCELL ��������
    #define SCBAT_VCELL_4v10            0x00    //���ڵ�ص�ѹΪ4.10V
    #define SCBAT_VCELL_4v20            0x01    //���ڵ�ص�ѹΪ4.20V
    #define SCBAT_VCELL_4v25            0x02    //���ڵ�ص�ѹΪ4.25V
    #define SCBAT_VCELL_4v30            0x03    //���ڵ�ص�ѹΪ4.30V
    #define SCBAT_VCELL_4v35            0x04    //���ڵ�ص�ѹΪ4.35V
    #define SCBAT_VCELL_4v40            0x05    //���ڵ�ص�ѹΪ4.40V
    #define SCBAT_VCELL_4v45            0x06    //���ڵ�ص�ѹΪ4.45V
    #define SCBAT_VCELL_4v50            0x07    //���ڵ�ص�ѹΪ4.50V
/*
* @}
*/

/*
* SC8815 Ӳ����ʼ���ṹ���������
* @{
*/
    //* SCHWI_IBAT_RATIO ��������
    #define SCHWI_IBAT_RATIO_6x         0x00    //��ص�������Ϊ 6x (10mOhm �в����ʱ�������Ϊ 6A)
    #define SCHWI_IBAT_RATIO_12x        0x10    //��ص�������Ϊ 12x (10mOhm �в����ʱ�������Ϊ 12A)

    // SCHWI_IBUS_RATIO ��������
    #define SCHWI_IBUS_RATIO_6x         0x04    //VBUS ��������Ϊ 6x (10mOhm �в����ʱ�������Ϊ 6A)
    #define SCHWI_IBUS_RATIO_3x         0x08    //VBUS ��������Ϊ 3x (10mOhm �в����ʱ�������Ϊ 3A)

    // SCHWI_VBAT_RATIO ��������
    #define SCHWI_VBAT_RATIO_12_5x      0x00    //��ص�ѹ����Ϊ 12.5x, ֻӰ�� SC8815 ���� ADC ������Χ, ������ֵΪ 25.6V (������2�����ϵ��Ӧ��)
    #define SCHWI_VBAT_RATIO_5x         0x02    //��ص�ѹ����Ϊ 5x, ֻӰ�� SC8815 ���� ADC ������Χ, ������ֵΪ 10.24V (������3�����µ��Ӧ��)

    // SCHWI_VBUS_RATIO ��������
    #define SCHWI_VBUS_RATIO_12_5x      0x00    //VBUS ��ѹ����Ϊ 12.5x, Ӱ������ ADC ������Χ�� OTG ���������ѹ��Χ, ������ֵΪ 25.6V, ����25mV
    #define SCHWI_VBUS_RATIO_5x         0x01    //VBUS ��ѹ����Ϊ 5x, Ӱ������ ADC ������Χ�� OTG ���������ѹ��Χ, ������ֵΪ 10.24V,  ����10mV

    // SCHWI_VINREG_RATIO ��������
    #define SCHWI_VINREG_RATIO_100x     0x00    //VINREG ����Ϊ 100x (100mV ��������,���ֵΪ 25.6V)
    #define SCHWI_VINREG_RATIO_40x      0x10    //VINREG ����Ϊ 40x (40mV ��������,���ֵΪ 10.24V)

    // SCHWI_FREQ ��������
    #define SCHWI_FREQ_150KHz           0x00    //����Ƶ��Ϊ150KHz (��ѹ�ͽ�ѹģʽ���� 150KHz)
    #define SCHWI_FREQ_300KHz_1         0x04    //����Ƶ��Ϊ300KHz (��ѹģʽ 300KHz, ��ѹģʽ 150KHz)
    #define SCHWI_FREQ_300KHz_2         0x08    //����Ƶ��Ϊ300KHz (��ѹ�ͽ�ѹģʽ���� 300KHz)
    #define SCHWI_FREQ_450KHz           0x0C    //����Ƶ��Ϊ450KHz (��ѹ�ͽ�ѹģʽ���� 450KHz)

    // SCHWI_DT ��������
    #define SCHWI_DT_20ns               0x00    //����ʱ��Ϊ20ns
    #define SCHWI_DT_40ns               0x01    //����ʱ��Ϊ40ns
    #define SCHWI_DT_60ns               0x02    //����ʱ��Ϊ60ns
    #define SCHWI_DT_80ns               0x03    //����ʱ��Ϊ80ns

    // SCHWI_ICHAR ��������
    #define SCHWI_ICHAR_IBUS            0x00    //ѡ�� VBUS ������Ϊ�ο�
    #define SCHWI_ICHAR_IBAT            0x80    //ѡ�� ���(IBAT) ������Ϊ�ο�

    // SCHWI_TRICKLE ��������
    #define SCHWI_TRICKLE_Enable        0x00    //��������
    #define SCHWI_TRICKLE_Disable       0x40    //�ر�������

    // SCHWI_TERM ��������
    #define SCHWI_TERM_Enable           0x00    //�򿪳���Զ���ֹ
    #define SCHWI_TERM_Disable          0x20    //�رճ���Զ���ֹ

    // SCHWI_FB_MODE ��������
    #define SCHWI_FB_Internal           0x00    //OTG ����ŵ�ģʽ VBUS ��ѹ����ʹ�� VBUS_PIN ��оƬ�ڲ�����
    #define SCHWI_FB_External           0x10    //OTG ����ŵ�ģʽ VBUS ��ѹ����ʹ�� FB_PIN �ϵ��ⲿ��ѹ�������練��

    // SCHWI_TRICKLE_SET ��������
    #define SCHWI_TRICKLE_SET_70        0x00    //��������λբֵΪ��ص�ѹ���õ�70%
    #define SCHWI_TRICKLE_SET_60        0x08    //��������λբֵΪ��ص�ѹ���õ�60%

    // SCHWI_OVP ��������
    #define SCHWI_OVP_Enable            0x00    //�� SC8815 ����ŵ�ģʽ�µ� VBUS ��ѹ����
    #define SCHWI_OVP_Disable           0x04    //�ر� SC8815 ����ŵ�ģʽ�µ� VBUS ��ѹ����

    // SCHWI_DITHER ��������
    #define SCHWI_DITHER_Disable        0x00    //�ر� SC8815 Ƶ�ʶ�������,PGATE����������PMOS����
    #define SCHWI_DITHER_Enable         0x04    //�� SC8815 Ƶ�ʶ�������,PGATE������������Ƶ�ʶ���

    // SCHWI_SLEW_SET ��������
    #define SCHWI_SLEW_1mV_us           0x00    //����ŵ�ģʽ�� VBUS ��̬�仯������ = 1mV/us
    #define SCHWI_SLEW_2mV_us           0x01    //����ŵ�ģʽ�� VBUS ��̬�仯������ = 2mV/us
    #define SCHWI_SLEW_4mV_us           0x02    //����ŵ�ģʽ�� VBUS ��̬�仯������ = 4mV/us
    #define SCHWI_SLEW_8mV_us           0x03    //����ŵ�ģʽ�� VBUS ��̬�仯������ = 8mV/us

    // SCHWI_ADC_SCAN ��������
    #define SCHWI_ADC_Disable           0x00    //�ر� SC8815 ADC ת��,��ʡ1-2mA�Ĺ���
    #define SCHWI_ADC_Enable            0x20    //�� SC8815 ADC ת��,��ʱ���Զ�ȡ��ѹ��������

    // SCHWI_ILIM_BW ��������
    #define SCHWI_ILIM_BW_5KHz          0x00    //SC8815 �������ƻ�·�Ĵ���Ϊ 5KHz
    #define SCHWI_ILIM_BW_1_25KHz       0x10    //SC8815 �������ƻ�·�Ĵ���Ϊ 1.25KHz
    
    // SCHWI_LOOP ��������
    #define SCHWI_LOOP_Normal           0x00    //SC8815 ��·��ӦģʽΪ ������·��Ӧ
    #define SCHWI_LOOP_Improved         0x08    //SC8815 ��·��ӦģʽΪ ���ƻ�·��Ӧ

    // SCHWI_SFB ��������
    #define SCHWI_SFB_Enable            0x00    //�� SC8815 ����ŵ�ģʽ�е� VBUS �� BATT ��·����
    #define SCHWI_SFB_Disable           0x04    //�ر� SC8815 ����ŵ�ģʽ�е� VBUS �� BATT ��·����

    // SCHWI_EOC ��������
    #define SCHWI_EOC_1_25              0x00    //SC8815 �������ļ��բֵΪ���������õ�1/25
    #define SCHWI_EOC_1_10              0x02    //SC8815 �������ļ��բֵΪ���������õ�1/10

    // SCHWI_PFM ��������
    #define SCHWI_PFM_Disable           0x00    //�ر� SC8815 ����ŵ��������µ� PFM ģʽ
    #define SCHWI_PFM_Enable            0x01    //�� SC8815 ����ŵ��������µ� PFM ģʽ
/*
* @}
*/

//��ʼ�� SC8815
void SC8815_Init(void);

//SC8815 Ӳ�����ó�ʼ������
void SC8815_BatteryConfig(SC8815_BatteryConfigTypeDef *SC8815_BatteryConfigStruct);     //���� SC8815 ��ز��� (��Ҫ SC8815 PSTOP ����Ϊ�߲��ܽ�������)
void SC8815_HardwareInit(SC8815_HardwareInitTypeDef *SC8815_HardwareInitStruct);        //��ʼ�� SC8815 Ӳ������ (��Ҫ SC8815 PSTOP ����Ϊ�߲��ܽ�������)
void SC8815_ConfigInterruptMask(SC8815_InterruptStatusTypeDef *InterruptStatusStruct);  //���� SC8815 �ж����� (�ж�ʹ�ܻ�ʧ��)

//SC8815 ��ȡ�ж�״̬����
void SC8815_ReadInterrupStatus(SC8815_InterruptStatusTypeDef *InterruptStatusStruct);

//SC8815 ��ȡ����ADCת���������
uint16_t SC8815_Read_VBUS_Voltage(void);            //��ȡ VBUS ��ѹ
uint16_t SC8815_Read_VBUS_Current(void);            //��ȡ VBUS ����
uint16_t SC8815_Read_BATT_Voltage(void);            //��ȡ��ص�ѹ
uint16_t SC8815_Read_BATT_Current(void);            //��ȡ��ص���
uint16_t SC8815_Read_ADIN_Voltage(void);            //��ȡ ADIN_PIN ��ѹ

//SC8815 ���ò���ֵ����
void SC8815_SetOutputVoltage(uint16_t NewVolt);     //���� OTG ���������ѹ
void SC8815_SetBusCurrentLimit(uint16_t NewILIM);   //���� VBUS ����
void SC8815_SetBatteryCurrLimit(uint16_t NewILIM);  //���õ������
void SC8815_VINREG_SetVoltage(uint16_t NewVolt);    //���� VINREG ��ѹ

void SC8815_SetVBUSFBMode(uint8_t mode);						//����VBUS��ѹ����ģʽ SCHWI_FB_Internal �ڲ������� SCHWI_FB_External �ⲿ����
uint8_t SC8815_GetVBUSFBMode(void);									//��ȡVBUS��ѹ����ģʽ

//SC8815 ��ȡ��������ֵ����
uint16_t SC8815_GetOutputVoltage(void);             //��ȡ OTG ���������ѹ
uint16_t SC8815_GetBusCurrentLimit(void);           //��ȡ VBUS ����
uint16_t SC8815_GetBatteryCurrLimit(void);          //��ȡ�������
uint16_t SC8815_VINREG_GetVoltage(void);            //��ȡ VINREG ��ѹ
uint16_t SC8815_GetMaxOutputVoltage(void);          //��ȡ��� OTG �������ѹ
uint16_t SC8815_GetOutputVoltageSetp(void);         //��ȡ OTG �����ѹ����ֵ

//SC8815 ����Ӳ�����ú��� (����Ҫ SC8815 PSTOP ����Ϊ��)
void SC8815_OTG_Enable(void);                       //�� OTG ����ŵ�ģʽ
void SC8815_OTG_Disable(void);                      //�ر� OTG ����ŵ�ģʽ, SC8815 �ָ������ģʽ
void SC8815_VINREG_SetRatio_40x(void);              //���� VINREG ������Ϊ 40x
void SC8815_VINREG_SetRatio_100x(void);             //���� VINREG ������Ϊ 100x
void SC8815_OVP_Enable(void);                       //�� OTG ģʽ�е� VBUS ��ѹ��������
void SC8815_OVP_Disable(void);                      //�ر� OTG ģʽ�е� VBUS ��ѹ��������
void SC8815_PGATE_Enable(void);                     //�� PGATE ���Ź���, ����͵�ƽ�� PMOS
void SC8815_PGATE_Disable(void);                    //�ر� PGATE ���Ź���, ����ߵ�ƽ�ر� PMOS
void SC8815_GPO_Enable(void);                       //�� GPO ���Ź���, ����͵�ƽ
void SC8815_GPO_Disable(void);                      //�ر� GPO ���Ź���, �������״̬
void SC8815_ADC_Enable(void);                       //�� ADC ɨ��, ��ʱ���Զ�ȡ ADC ����
void SC8815_ADC_Disable(void);                      //�ر� ADC ɨ��, ��Լ 1-2mA �ĺĵ�
void SC8815_SFB_Enable(void);                       //�� OTG ģʽ�е� VBUS �� VBAT ��·��������
void SC8815_SFB_Disable(void);                      //�ر� OTG ģʽ�е� VBUS �� VBAT ��·��������
void SC8815_PFM_Enable(void);                       //�� OTG ģʽ�����������µ� PFM ģʽ
void SC8815_PFM_Disable(void);                      //�ر� OTG ģʽ�����������µ� PFM ģʽ

//SC8815 ��ȡӲ������״̬����
uint8_t SC8815_OTG_IsEnable(void);                  //��� OTG �Ƿ��ڴ�״̬
uint8_t SC8815_VINREG_GetRatio(void);               //��ȡ VINREG ������
uint8_t SC8815_OVP_IsEnable(void);                  //��� OTG ģʽ�� OVP �����Ƿ��ڴ�״̬
uint8_t SC8815_PGATE_IsEnable(void);                //��� PGATE ���Ź����Ƿ��ڴ�״̬
uint8_t SC8815_GPO_IsEnable(void);                  //��� GPO ���Ź����Ƿ��ڴ�״̬
uint8_t SC8815_ADC_IsEnable(void);                  //��� ADC ɨ���Ƿ��ڴ�״̬
uint8_t SC8815_SFB_IsEnable(void);                  //��� OTG ģʽ�� ��·���� �����Ƿ��ڴ�״̬
uint8_t SC8815_PFM_IsEnable(void);                  //��� OTG ģʽ�е� PFM ģʽ�Ƿ��ڴ�״̬

// ��������
void SC8815_SetBatteryCell(uint8_t NewCell);				//���õ������ 			��ֵ����Ϊ����1-4 ��ʾ1-4�ڵ��
uint8_t SC8815_GetBatteryCell(void);								//��ȡ����������� 	����ֵΪ1-4
void SC8815_SetSWFreq(uint8_t NewFreq);							//���ÿ����л�Ƶ�� 	��ֵ����Ϊ 1=����Ϊ150KHz 2=����Ϊת��300KHz���150KHz 3=������Ϊ300KHz 4=����Ϊ450KHz
uint8_t SC8815_GetSWFreq(void);											//��ȡ�����л�Ƶ�� 	����ֵ 1=����Ϊ150KHz 2=����Ϊת��300KHz���150KHz 3=������Ϊ300KHz 4=����Ϊ450KHz
void SC8815_SetDeadTime(uint8_t NewDT);							//��������ʱ�� 			��ֵ����Ϊ 1=20ns 2=40ns 3=60ns 4=80ns
uint8_t SC8815_GetDeadTime(void);										//��ȡ����ʱ�� 			����ֵ����Ϊ 1=20ns 2=40ns 3=60ns 4=80ns
void SC8815_SetILIMBW(uint8_t NewILIMBW);						//���õ�����·��Ӧ���� ��ֵ����Ϊ 1=5KHz 2=1.25KHz
uint8_t SC8815_GetILIMBW(void);											//��ȡ������·��Ӧ���� ����ֵ 1=5KHz 2=1.25KHz
void SC8815_SetLoop(uint8_t NewLoop);								//���û�·��Ӧģʽ 	��ֵ����Ϊ 1=������·��Ӧ 2=���ƻ�·��Ӧ
uint8_t SC8815_GetLoop(void);												//��ȡ��·��Ӧģʽ 	����ֵ 1=������·��Ӧ 2=���ƻ�·��Ӧ
void SC8815_SetOTGFBMode(uint8_t NewVal);						//����VBUS����ģʽ		
uint8_t SC8815_GetVBUSShort(void);

extern SC8815_HardwareInitTypeDef SC8815_HardwareInitStruct;

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // !SC8815_H

/*****END OF FILE****/
