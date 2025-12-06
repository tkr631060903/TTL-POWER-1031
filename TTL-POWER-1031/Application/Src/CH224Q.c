#include "CH224Q.h"
#include "Application_ADC.h"
#include "Application_SC8815.h"
#include <math.h>

void Fixed_req(uint8_t vol)//fixed请求,设置输入电压
{
    switch (vol) {
    case 5:
        if (I2C_ReadRegByte((0x23 << 1), 0x0A) != 0)
            I2C_WriteRegByte((0x23 << 1), 0x0a, 0);
        printf("Req_ 5V... \r\n");
        break;
    case 9:
        if (I2C_ReadRegByte((0x23 << 1), 0x0A) != 1)
            I2C_WriteRegByte((0x23 << 1), 0x0a, 1);
        printf("Req_ 9V...  \r\n");
        break;
    case 12:
        if (I2C_ReadRegByte((0x23 << 1), 0x0A) != 2)
            I2C_WriteRegByte((0x23 << 1), 0x0a, 2);
        printf("Req_12V...  \r\n");
        break;
    case 15:
        if (I2C_ReadRegByte((0x23 << 1), 0x0A) != 3)
            I2C_WriteRegByte((0x23 << 1), 0x0a, 3);
        printf("Req_15V...  \r\n");
        break;
    case 20:
        if (I2C_ReadRegByte((0x23 << 1), 0x0A) != 4)
            I2C_WriteRegByte((0x23 << 1), 0x0a, 4);
        printf("Req_20V...  \r\n");
        break;
    case 28:
        if (I2C_ReadRegByte((0x23 << 1), 0x0A) != 5)
            I2C_WriteRegByte((0x23 << 1), 0x0a, 5);
        printf("Req_28V...  \r\n");
        break;//注意请求进入EPR时尽量等待1S左右 再进行其他操作
    default:
        printf("Req_error...  \r\n");
        break;//Fixed仅可请求5V 9V 12V 15V 20V 28V
    }
}

uint8_t get_ch224q_status(void)
{
    uint8_t status = I2C_ReadRegByte((0x23 << 1), 0x09);
    if (status == 0 || status == 0xFF) return ACTICATE_NONE;

    // if (status == 0x01) {
    //     return ACTICATE_BC;
    // } else if (status == 0x02) {
    //     return ACTICATE_QC2;
    // } else if (status == 0x04) {
    //     return ACTICATE_QC3;
    // } else if (status == 0x08) {
    //     return ACTICATE_PD;
    // } else if (status == 0x10) {
    //     return ACTICATE_EPR;
    // }

    return ACTICATE_TRUE;
}

/**
 *@brief 获取PD激活时最大输入电流
 * 
 * @return uint16_t 单位mA
 */
uint16_t get_PD_current(void)
{
    uint8_t status = 0, current = 0;
    status = I2C_ReadRegByte((0x23 << 1), 0x09);
    if (status & 0x08) {
        current = I2C_ReadRegByte((0x23 << 1), 0x50);
    }
    return current * 50;
}
