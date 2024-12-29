#include "husb238.h"
#include <string.h>
#include "Application_SC8815.h"
#include "Application_LCD.h"
#include "Application_SC8815.h"
#include "menu_ui.h"

// ��ѹֵ
uint16_t votlage_Table[] = { 0, 5, 9, 12, 15, 18, 20 };
// ����ֵ
float current_Table[] = { 0.5f, 0.7f, 1.0f, 1.25f, 1.5f, 1.75f, 2.0f, 2.25f, 2.5f, 2.75f, 3.0f, 3.25f, 3.5f, 4.0f, 4.5f, 5.0f };

/// @brief I2C ��ȡ����Ĵ���ֵ
/// @param slaveAddr �ӻ���ַ
/// @param startAddress ��ʼ��ַ
/// @param nBytesRead �ֽ���
/// @param data ��ȡ����
/// @return 
int HUSB238_I2CReadBytes(uint8_t slaveAddr, uint8_t startAddress, uint16_t nBytesRead, uint8_t* data)
{
    if (nBytesRead == 1) {
        *data = I2C_ReadRegByte(slaveAddr << 1, startAddress);
    } else {
        do {
            *data++ = I2C_ReadRegByte(slaveAddr << 1, startAddress++);
        } while (nBytesRead-- > 0);
    }
    return 1;
}

/// @brief I2Cд��Ĵ���ֵ
/// @param slaveAddr �ӻ���ַ
/// @param writeAddress д��Ĵ�����ַ
/// @param data д�������
/// @return 
int HUSB238_WriteReg(uint8_t slaveAddr, uint8_t writeAddress, uint8_t data)
{
    I2C_WriteRegByte(slaveAddr << 1, writeAddress, data);
    uint8_t dataCheck = 0;
    HUSB238_I2CReadBytes(slaveAddr, writeAddress, 1, &dataCheck);
    if (dataCheck != data)
        return -2;
    return 0;
}


/// @brief ����Get_SRC_Cap����
void HUSB238_RefreshSrcCap(void)
{
    uint8_t cmd = Get_SRC_Cap;
    HUSB238_WriteReg(HUSB238_I2CAddress, Reg_GO_COMMAND, cmd);
}


/// @brief ��ȡ HUSB238 ȫ���Ĵ�������
/// @param regs ��СΪ 10 �� uint8_t ��������
void HUSB238_ReadAllReg(uint8_t* regs)
{
    memset(regs, 0, 10);
    HUSB238_I2CReadBytes(HUSB238_I2CAddress, 0x00, 10, (uint8_t*)regs);
}


// @brief regs �Ĵ���ֵ ת������������б�
// @param regs ��СΪ 10 
// @param pdoList ��СΪ 6 ��
void HUSB238_ExtractCap(HUSB238_Capability_t* pdoList)
{
    uint8_t regs[10];

    if (!pdoList) {
        return;
    }
    HUSB238_ReadAllReg(regs);
    HUSB238_Reg_SRC_PDO* reg;
    HUSB238_Capability_t cap;
    for (int i = 0; i < 6; i++) {
        reg = (HUSB238_Reg_SRC_PDO*)(regs + i + 2);
        // memcpy(&reg, &regs[i + 2], sizeof(uint8_t));
        // reg = static_cast<HUSB238_Reg_SRC_PDO>(regs[i + 2]);
        if (reg->bit.SRC_DETECTED) {
            HUSB238_CURRENT_e current = reg->bit.SRC_CURRENT;
            cap.detected = true;
            cap.current = to_current(current);
        } else {
            cap.detected = false;
        }
        cap.voltage = HUSB238_Voltage2PDO((HUSB238_Voltage_e)(i + 1));
        pdoList[i] = cap;
    }
}


/// @brief ��ȡ��ǰPD�������
/// @param voltage ��ѹ ��λ 'V'
/// @param current ���� ��λ 'A'
void HUSB238_GetCapabilities(uint16_t* voltage, float* current)
{
    HUSB238_Voltage_e voltage_e;
    HUSB238_CURRENT_e current_e;

    HUSB238_RefreshSrcCap();
    HAL_Delay(100);
    HUSB238_GetCurrentMode(&voltage_e, &current_e);
    *voltage = HUSB238_Voltage2PDO(voltage_e);
    if (*voltage != 0) {
        *current = to_current(current_e);
    } else {
        *current = 0;
    }
}


/// @brief ����PDO���
void HUSB238_RequestPDO(void)
{
    uint8_t cmd = Request_PDO;
    HUSB238_WriteReg(HUSB238_I2CAddress, Reg_GO_COMMAND, cmd);
}

/// @brief Ӳ��λ
void HUSB238_HardReset(void)
{
    uint8_t cmd = Hard_Reset;
    HUSB238_WriteReg(HUSB238_I2CAddress, Reg_GO_COMMAND, cmd);
}

/// @brief ����PD��ѹֵ
/// @param pdo ���õĵ�ѹ��HUSB238_SELECT_Voltage_e��
void HUSB238_SelVoltage(HUSB238_SELECT_Voltage_e pdo)
{
    HUSB238_Reg_SRC_PDO_SEL targetPDO;
    targetPDO.all = 0xA;
    targetPDO.bit.PDO_SELECT = pdo;
    HUSB238_WriteReg(HUSB238_I2CAddress, Reg_SRC_PDO_SEL, targetPDO.all);
    HUSB238_RequestPDO();
}

/// @brief ��ȡPD��ǰ״̬�Ĵ���ֵ
/// @param voltage ��ѹ״ֵ̬
/// @param current ����״ֵ̬
void HUSB238_GetCurrentMode(HUSB238_Voltage_e* voltage, HUSB238_CURRENT_e* current)
{
    uint8_t reag[2] = { 0 };
    HUSB238_Reg_PD_STATUS0 status0;
    HUSB238_Reg_PD_STATUS1 status1;
    HUSB238_I2CReadBytes(HUSB238_I2CAddress, 0x00, 2, (uint8_t*)&reag);
    ((uint8_t*)&status0)[0] = reag[0];
    ((uint8_t*)&status1)[0] = reag[1];
    *current = status0.bit.PD_SRC_CURRENT;
    *voltage = status0.bit.PD_SRC_VOLTAGE;
}


/// @brief ��ȡ���ĵ�ѹ�Ĵ���ֵת����ѹֵ
/// @param voltage �� STATUS0(0x00) ��ȡ���ĵ�ѹֵ
/// @return ��ѹֵ
uint16_t HUSB238_Voltage2PDO(HUSB238_Voltage_e voltage)
{
    return votlage_Table[voltage];
}

/// @brief ת���Ĵ���������Ӧֵ
/// @param c ��ת���ĵ����Ĵ���ֵ
/// @return ���ص���ֵ
float to_current(HUSB238_CURRENT_e c)
{
    uint8_t i = c & 0xf;
    if (i <= PD_5A) {
        return current_Table[i];
    } else {
        return 0.0f;
    }
}

void husb238_init(void)
{
    // PD оƬ��ر���
    uint8_t index = 0;
    // uint16_t PD_Voltage = 0;
    // float PD_Current = 0;
    float PD_Power = 0;
    char str[16];
    HUSB238_SELECT_Voltage_e votlage[6] = { PDO_5V, PDO_9V, PDO_12V, PDO_15V, PDO_18V, PDO_20V };
    HUSB238_Capability_t PDCapabilities[6];
    HUSB238_ExtractCap(PDCapabilities);
    // printf("\n��������б�:\n");
    for (int i = 0; i < 6; i++) {
        if (PDCapabilities[i].detected == true && PDCapabilities[i].voltage * PDCapabilities[i].current >= PD_Power) {
            // printf("| �����ѹ: %dV | �������: %.2fA\n", PDCapabilities[i].voltage, PDCapabilities[i].current);
            PD_Power = PDCapabilities[i].voltage * PDCapabilities[i].current;
            index = i;
        }
    }
    // printf("\n");
    if (PD_Power > 0) {
        //HUSB238_GetCapabilities(&PD_Voltage, &PD_Current);
        LCD_ShowChinese(20, 32, "�����������", LIGHTBLUE, BLACK, 32, 0);
        sprintf(str, "%dV %.1fA %.1fW", PDCapabilities[index].voltage, PDCapabilities[index].current, PD_Power);
        LCD_ShowString(6, 80, (const uint8_t*)str, LIGHTBLUE, BLACK, 32, 0);
        APP_config.fastCharge_InVoltage = PDCapabilities[index].voltage;
        APP_config.fastCharge_InCurrent = PDCapabilities[index].current;
        SC8815_Config.SC8815_IBAT_Limit = PDCapabilities[index].current * 1000;
        HAL_Delay(1000);
        HUSB238_SelVoltage(votlage[index]);
    } else {
        SC8815_Config.SC8815_VBUS_IBUS_Step = 1000;
        LCD_ShowChinese(20, 32, "�����������", LIGHTBLUE, BLACK, 32, 0);
        DC_limit_page_ui_process(KEY2_SHORT);
    }
}
