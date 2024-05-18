/**
 * @file UART_Debug.c
 * @author TanKairong (tkr631060903@gmail.com)
 * @brief uart1调试串口
 * @version 0.1
 * @date 2023-12-27
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "UART_Debug.h"
#include "Application.h"
#include <string.h>
#include "Application_SC8815.h"

char uart1_Cmd[UART_Cmd_Length];    //命令缓冲区
uint8_t cmd_Index = 0;  //串口1命令计数指针
uint8_t uart1_Receive_Data = 0;  //串口1中断接收数据
extern UART_HandleTypeDef huart1;

#define CMD_STR_CNT 5 // 命令参数数量

typedef char *CmdStr[CMD_STR_CNT];
typedef int ascii_handler_fn(CmdStr param, short param_cnt);
static ascii_handler_fn
    setVBUS_handler, setIBUS_handler, setIBAT_handler, setIRCOMP_handler, setVBAT_SEL_handler, setCSEL_handler,
    setVCELL_handler, setSW_FREQ_handler, setDeadTime_handler,
    setFB_Mode_handler, setDITHER_handler, setSLEW_SET_handler, setILIM_BW_handler;
typedef struct lookup_table
{
    const char *desc;
    ascii_handler_fn *handler;
} lookup_table_t;
static const lookup_table_t handler_map_static[] = {
    {"setvbus", setVBUS_handler}, {"setibus", setIBUS_handler}, {"setibat", setIBAT_handler}, {"setircomp", setIRCOMP_handler},
    {"setvbatsel", setVBAT_SEL_handler}, {"setcsel", setCSEL_handler}, {"setvcell", setVCELL_handler}, {"setswfreq", setSW_FREQ_handler}, 
    {"setdeadtime", setDeadTime_handler}, {"setfbmode", setFB_Mode_handler},
    {"setdither", setDITHER_handler}, {"setslewset", setSLEW_SET_handler}, {"setilimbw", setILIM_BW_handler}};
const lookup_table_t *handler_map = handler_map_static;

/**
 *@brief printf重定义
 * 需要在keil5勾选使用微库(Use MicroLIB)，否则STM32正常上电不启动
 * @param ch
 * @param f
 * @return int
 */
int fputc(int ch, FILE *f)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 1000);
    return ch;
}

int setVBUS_handler(CmdStr param, short param_cnt)
{
    if (param_cnt == 1)
    {
        return 0;
    }
    int value;
    sscanf(param[1], "%d", &value);
    SC8815_Config.SC8815_VBUS = value;
    SC8815_Config.SC8815_VBUS_Old = value;
    SC8815_SetOutputVoltage(value);
    return 1;
}

int setIBUS_handler(CmdStr param, short param_cnt)
{
    if (param_cnt == 1)
    {
        return 0;
    }
    int value;
    sscanf(param[1], "%d", &value);
    SC8815_Config.SC8815_IBUS_Limit = value;
    SC8815_Config.SC8815_IBUS_Limit_Old = value;
    SC8815_SetBusCurrentLimit(value);
    return 1;
}

int setIBAT_handler(CmdStr param, short param_cnt)
{
    if (param_cnt == 1)
    {
        return 0;
    }
    Application_SC8815_Standby();
    int value;
    sscanf(param[1], "%d", &value);
    SC8815_Config.SC8815_IBAT_Limit = value;
    SC8815_SetBatteryCurrLimit(value);
    return 1;
}

int setIRCOMP_handler(CmdStr param, short param_cnt)
{
    if (param_cnt == 1)
    {
        return 0;
    }
    Application_SC8815_Standby();
    int value;
    sscanf(param[1], "%x", &value);
    extern SC8815_BatteryConfigTypeDef SC8815_BatteryConfigStruct;
	SC8815_BatteryConfigStruct.IRCOMP = value;
	SC8815_BatteryConfig(&SC8815_BatteryConfigStruct);
    return 1;
}

int setVBAT_SEL_handler(CmdStr param, short param_cnt)
{
    if (param_cnt == 1)
    {
        return 0;
    }
    Application_SC8815_Standby();
    int value;
    sscanf(param[1], "%x", &value);
    extern SC8815_BatteryConfigTypeDef SC8815_BatteryConfigStruct;
	SC8815_BatteryConfigStruct.VBAT_SEL = value;
	SC8815_BatteryConfig(&SC8815_BatteryConfigStruct);
    return 1;
}

int setCSEL_handler(CmdStr param, short param_cnt)
{
    if (param_cnt == 1)
    {
        return 0;
    }
    Application_SC8815_Standby();
    int value;
    sscanf(param[1], "%x", &value);
    SC8815_SetBatteryCell(value);
    return 1;
}

int setVCELL_handler(CmdStr param, short param_cnt)
{
    if (param_cnt == 1)
    {
        return 0;
    }
    Application_SC8815_Standby();
    int value;
    sscanf(param[1], "%x", &value);
    extern SC8815_BatteryConfigTypeDef SC8815_BatteryConfigStruct;
	SC8815_BatteryConfigStruct.VCELL = value;
	SC8815_BatteryConfig(&SC8815_BatteryConfigStruct);
    return 1;
}

int setSW_FREQ_handler(CmdStr param, short param_cnt)
{
    if (param_cnt == 1)
    {
        return 0;
    }
    Application_SC8815_Standby();
    int value;
    sscanf(param[1], "%x", &value);
    SC8815_SetSWFreq(value);
    return 1;
}

int setDeadTime_handler(CmdStr param, short param_cnt)
{
    if (param_cnt == 1)
    {
        return 0;
    }
    Application_SC8815_Standby();
    int value;
    sscanf(param[1], "%x", &value);
    SC8815_SetDeadTime(value);
    return 1;
}

int setFB_Mode_handler(CmdStr param, short param_cnt)
{
    if (param_cnt == 1)
    {
        return 0;
    }
    Application_SC8815_Standby();
    int value;
    sscanf(param[1], "%x", &value);
    SC8815_SetVBUSFBMode(value);
    return 1;
}

int setDITHER_handler(CmdStr param, short param_cnt)
{
    if (param_cnt == 1)
    {
        return 0;
    }
    Application_SC8815_Standby();
    int value;
    sscanf(param[1], "%x", &value);
    extern SC8815_HardwareInitTypeDef SC8815_HardwareInitStruct;
    SC8815_HardwareInitStruct.DITHER = value;
    SC8815_HardwareInit(&SC8815_HardwareInitStruct);
    return 1;
}

int setSLEW_SET_handler(CmdStr param, short param_cnt)
{
    if (param_cnt == 1)
    {
        return 0;
    }
    Application_SC8815_Standby();
    int value;
    sscanf(param[1], "%x", &value);
    extern SC8815_HardwareInitTypeDef SC8815_HardwareInitStruct;
    SC8815_HardwareInitStruct.SLEW_SET = value;
    SC8815_HardwareInit(&SC8815_HardwareInitStruct);
    return 1;
}

int setILIM_BW_handler(CmdStr param, short param_cnt)
{
    if (param_cnt == 1)
    {
        return 0;
    }
    Application_SC8815_Standby();
    int value;
    sscanf(param[1], "%x", &value);
    SC8815_SetILIMBW(value);
    return 1;
}

/**
 * @brief 解析命令字符串
 * 
 * @param input 命令字符串
 * @param delimiter 分隔符
 * @param tokens 解析后的参数数组
 * @param max_tokens 最大参数个数
 * @param param 解析后的参数数组
 * @return 解析参数的个数 
 */
int splitString(char* input, char delimiter, char** tokens, int max_tokens, CmdStr param) {
    int token_count = 0;
    char* token = strtok(input, &delimiter);

    while (token && token_count < max_tokens) {
        tokens[token_count++] = token;
        token = strtok(NULL, &delimiter);
    }
    param[0] = tokens[0];
    param[1] = tokens[1];
    return token_count;
}

int ascii_process(char *cmd)
{
    CmdStr param;
    short param_cnt;
    int ret = -1;
    int i;
    int map_cnt = sizeof(handler_map_static) / sizeof(lookup_table_t);
    char* tokens[10];
    param_cnt = splitString(cmd, ':', tokens, 10, param);
    for (i = 0; i < map_cnt; i++)
    {
        if (!strcasecmp(param[0], handler_map[i].desc))
        {
            ascii_handler_fn *handler = NULL;
            handler = handler_map[i].handler;
            if (handler)
            {
                ret = (*handler)(param, param_cnt);
                break;
            }
        }
    }
    return ret;
}
