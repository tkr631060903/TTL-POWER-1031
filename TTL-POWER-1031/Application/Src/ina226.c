#include "ina226.h"
#include "Application_SC8815.h"

static uint16_t INA226_Read2Byte(uint8_t reg_addr)
{
    uint16_t reg_data = 0;
    uint16_t temp = 0;
    i2c_Start();
    i2c_SendByte(INA226_WRITE_ADDR);
    i2c_WaitAck();
    i2c_SendByte(reg_addr);
    i2c_WaitAck();
    i2c_Start();
    i2c_SendByte(INA226_READ_ADDR);
    i2c_WaitAck();
    reg_data = i2c_ReadByte();
    i2c_Ack();
    reg_data = (reg_data << 8) & 0xFF00;
    temp = i2c_ReadByte();
    i2c_NAck();
    i2c_Stop();
    reg_data |= temp;
    return reg_data;
}


static uint8_t INA226_Write2Byte(uint8_t reg_addr, uint16_t reg_data)
{
    uint8_t data_high = (uint8_t)((reg_data & 0xFF00) >> 8);
    uint8_t data_low = (uint8_t)reg_data & 0x00FF;
    i2c_Start();
    i2c_SendByte(INA226_WRITE_ADDR);
    i2c_WaitAck();
    i2c_SendByte(reg_addr);
    i2c_WaitAck();
    i2c_SendByte(data_high);
    i2c_WaitAck();
    i2c_SendByte(data_low);
    i2c_WaitAck();
    i2c_Stop();
    return 1;
}

float INA226_ReadVoltage(void)
{
    float temp = INA226_Read2Byte(Bus_V_Reg) * 1.25;
    if (temp < 0)
        temp = 0;
    return temp;
}

float INA226_ReadCurrent(void)
{
    float temp;
    if (SC8815_Config.SC8815_VBUS >= APP_config.fastCharge_InVoltage * 1000 - 200) {
        temp = (INA226_Read2Byte(Current_Reg) * 0.263) - 9;  //boost
    } else {
        temp = (INA226_Read2Byte(Current_Reg) * 0.264) - 1;   //buck
    }
    if (temp < 0)
        temp = 0;
    return temp;
}

float INA226_ReadPower(void)
{
    return INA226_Read2Byte(Power_Reg) * 0.169 * 25;
}

void INA226_Init(void)
{
    if (INA226_Read2Byte(ID_Reg) != INA226_ID_NUMBER) {
        printf("INA226 Error!\r\n");
        return;
    }
    // INA226_Write2Byte(Config_Reg, 0x4BB7);
    // INA226_Write2Byte(Config_Reg, 0x4527);
    INA226_Write2Byte(Config_Reg, 0x41FF);
    HAL_Delay(5);
    // INA226_Write2Byte(Calib_Reg, 0x17D7);
    INA226_Write2Byte(Calib_Reg, 0x0F6C);
}
