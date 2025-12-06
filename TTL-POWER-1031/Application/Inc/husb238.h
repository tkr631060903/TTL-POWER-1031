#ifndef __HUSB238_H__
#define __HUSB238_H__

#include "Application.h"
#include <stdbool.h>

#define HUSB238_I2CAddress 0x08

/// @brief HUSB238 寄存器地址
enum HUSB238_reg_addr
{
    Reg_PD_STATUS0 = 0x00,
    Reg_PD_STATUS1,
    Reg_SRC_PDO_5V,
    Reg_SRC_PDO_9V,
    Reg_SRC_PDO_12V,
    Reg_SRC_PDO_15V,
    Reg_SRC_PDO_18V,
    Reg_SRC_PDO_20V,
    Reg_SRC_PDO_SEL,
    Reg_GO_COMMAND,
};

/// @brief 寄存器电流对应值
typedef enum
{
    PD_0_5A = 0x00,
    PD_0_7A,
    PD_1_0A,
    PD_1_25A,
    PD_1_5A,
    PD_1_75A,
    PD_2A,
    PD_2_25A,
    PD_2_5A,
    PD_2_75A,
    PD_3A,
    PD_3_25A,
    PD_3_5A,
    PD_4A,
    PD_4_5A,
    PD_5A,
} HUSB238_CURRENT_e;

/// @brief 写入 SRC_PDO(0x08) 设置输出电压用到
typedef enum
{
    Not_sel = 0x0,
    PDO_5V = 0x1,
    PDO_9V = 0x2,
    PDO_12V = 0x3,
    PDO_15V = 0x8,
    PDO_18V = 0x9,
    PDO_20V = 0xa,
} HUSB238_SELECT_Voltage_e;

/// @brief 读取 STATUS0(0x00) 状态寄存器值用到
typedef enum
{
    Voltage_unknown = 0x0,
    Voltage_5V,
    Voltage_9V,
    Voltage_12V,
    Voltage_15V,
    Voltage_18V,
    Voltage_20V,
} HUSB238_Voltage_e;

/// @brief 命令
enum HUSB238_CMD {
    Request_PDO = 0x01,
    Get_SRC_Cap = 0x04,
    Hard_Reset = 0x10,
};

typedef union
{
    struct
    {
        HUSB238_CURRENT_e PD_SRC_CURRENT : 4;
        HUSB238_Voltage_e PD_SRC_VOLTAGE : 4;
    } bit;
    uint8_t all;
} HUSB238_Reg_PD_STATUS0;

typedef union
{
    struct
    {
        uint8_t CURRENT_5V : 2;
        uint8_t VOLTAGE_5V : 1;
        uint8_t PD_RESPONSE : 3;
        uint8_t ATTACH : 1;
        uint8_t CC_DIR : 1;
    } bit;
    uint8_t all;
} HUSB238_Reg_PD_STATUS1;

typedef union
{
    struct
    {
        HUSB238_CURRENT_e SRC_CURRENT : 4;
        uint8_t RESERVED : 3;
        uint8_t SRC_DETECTED : 1;
    } bit;
    uint8_t all;
} HUSB238_Reg_SRC_PDO;

typedef union
{
    struct
    {
        uint8_t RESERVED : 4;
        HUSB238_SELECT_Voltage_e PDO_SELECT : 4;
    } bit;
    uint8_t all;
} HUSB238_Reg_SRC_PDO_SEL;

typedef struct
{
    HUSB238_Reg_PD_STATUS0 PD_STATUS0;
    HUSB238_Reg_PD_STATUS1 PD_STATUS1;
    HUSB238_Reg_SRC_PDO SRC_PDO_5V;
    HUSB238_Reg_SRC_PDO SRC_PDO_9V;
    HUSB238_Reg_SRC_PDO SRC_PDO_12V;
    HUSB238_Reg_SRC_PDO SRC_PDO_15V;
    HUSB238_Reg_SRC_PDO SRC_PDO_18V;
    HUSB238_Reg_SRC_PDO SRC_PDO_20V;
    HUSB238_Reg_SRC_PDO_SEL SRC_PDO;
    uint8_t GO_COMMAND;
} HUSB238_reg_t;

typedef struct
{
    bool detected;
    float current;
    uint16_t voltage;
} HUSB238_Capability_t;

/// @brief regs 寄存器值 转化可输出能力列表
/// @param regs 大小为 10 
/// @param pdoList 大小为 6 ！
unsigned char HUSB238_ExtractCap(HUSB238_Capability_t* pdoList);

/// @brief 获取当前PD输出能力
/// @param voltage 
/// @param current 
void HUSB238_GetCapabilities(uint16_t* voltage, float* current);
void HUSB238_GetCurrentMode(HUSB238_Voltage_e* voltage, HUSB238_CURRENT_e* current);
void HUSB238_SelVoltage(HUSB238_SELECT_Voltage_e voltage);

/// @brief 读取到的电压寄存器值转化电压值
/// @param voltage 从 STATUS0(0x00) 读取到的电压值
/// @return 电压值
uint16_t HUSB238_Voltage2PDO(HUSB238_Voltage_e voltage);

/// @brief 转化寄存器电流对应值
/// @param c 被转化的电流寄存器值
/// @return 返回电流值
float to_current(HUSB238_CURRENT_e c);

/// @brief 硬复位
void HUSB238_HardReset(void);

// 读取全部数据
void HUSB238_ReadAllReg(uint8_t* regs);
void husb238_init(void);
void HUSB238_Refresh_Voltage(void);

#endif /* __HUSB238_H */
