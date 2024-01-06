#include "Application_SC8815.h"

void SoftwareDelay(uint8_t ms)
{
	HAL_Delay(ms);
}

void I2C_WriteRegByte(uint8_t SlaveAddress, uint8_t RegAddress, uint8_t ByteData)
{
	uint8_t data[2] = { RegAddress, ByteData };
	HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(&hi2c2, SlaveAddress, data, 2, 100);
	printf("status = %d\r\n", status);
}

uint8_t I2C_ReadRegByte(uint8_t SlaveAddress, uint8_t RegAddress)
{
	uint8_t ReceiveData = 0;
	HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(&hi2c2, SlaveAddress, &RegAddress, 1, 100);
	printf("status = %d\r\n", status);
	status = HAL_I2C_Master_Receive(&hi2c2, SlaveAddress, &ReceiveData, 1, 100);
	printf("status = %d\r\n", status);
	printf("ReceiveData = %x\r\n", ReceiveData);
	return ReceiveData;
}

void SC8815_Test(void)
{
	printf("SC8815_Test\r\n");
	I2C_ReadRegByte(SC8815_ADDR, 0x19);
}
