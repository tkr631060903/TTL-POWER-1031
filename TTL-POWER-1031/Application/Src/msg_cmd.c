#include "msg_cmd.h"
#include "Application.h"
#include <string.h>
#include "Application_SC8815.h"
#include <stdio.h>
#include "Application_Callback.h"
#include "usart.h"
#include "usbd_cdc_if.h"
#include "Application_ADC.h"
#include "menu.h"
#include "stmflash.h"

#define CMD_STR_CNT 6 // 命令参数数量

char uart1_Cmd[Cmd_Length];    //命令缓冲区
uint32_t cmd_Index = 0;  //串口1命令计数指针
// uint8_t uart1_Receive_Data = 0;  //串口1中断接收数据缓存

typedef char *CmdStr[CMD_STR_CNT];
typedef int ascii_handler_fn(CmdStr param, short param_cnt, uint8_t cmd_source);
static ascii_handler_fn
    setIBAT_handler, setIRCOMP_handler, setVBAT_SEL_handler, setCSEL_handler, calibration_ibus_handler, get_temperature_handler,
    setVCELL_handler, setSW_FREQ_handler, setDeadTime_handler, setFB_Mode_handler, setDITHER_handler,
    setSLEW_SET_handler, setILIM_BW_handler, get_msg_handler, lock_key_handler, start_preset_handler,
    set_switch_handler, set_current_handler, set_current_port_handler, set_current_step_handler,
    set_voltage_handler, set_voltage_step_handler, set_voltage_prot_handler, set_voltage_limit_handler,
    get_fetch_current_handler, get_fetch_voltage_handler, get_fetch_power_handler, get_versions_handler,
    set_preset_handler, save_config_handler, upgrade_app_handler, set_power_limit_handler, set_name_handler, 
    set_key_handler, PDP_search_handler, set_SFB_handler;
typedef struct lookup_table
{
    const char *desc;
    ascii_handler_fn *handler;
} lookup_table_t;
static const lookup_table_t handler_map_static[] = {
    {"setibat", setIBAT_handler}, {"setircomp", setIRCOMP_handler}, {"calibus", calibration_ibus_handler}, {"gettemp", get_temperature_handler},
    {"setvbatsel", setVBAT_SEL_handler}, {"setcsel", setCSEL_handler}, {"setvcell", setVCELL_handler}, {"setswfreq", setSW_FREQ_handler}, 
    {"setdeadtime", setDeadTime_handler}, {"setfbmode", setFB_Mode_handler}, {"setdither", setDITHER_handler},
    {"setslewset", setSLEW_SET_handler}, {"setilimbw", setILIM_BW_handler}, {"getmsg", get_msg_handler}, {"lockkey", lock_key_handler}, {"startPreset", start_preset_handler},
    {"OUTP", set_switch_handler}, {"CURR", set_current_handler}, {"CURR:PROT", set_current_port_handler}, {"CURR:STEP", set_current_step_handler},
    {"VOLT", set_voltage_handler}, {"VOLT:STEP", set_voltage_step_handler}, {"VOLT:PROT", set_voltage_prot_handler},
    {"VOLT:LIMIT", set_voltage_limit_handler}, {"MEAS:CURR?", get_fetch_current_handler}, {"MEAS:VOLT?", get_fetch_voltage_handler}, {"SYST:VERS?\r\n", get_versions_handler},
    {"MEAS:POW?", get_fetch_power_handler}, {"SYST:VERS?", get_versions_handler}, {"preset", set_preset_handler}, {"save", save_config_handler}, {"upgrade", upgrade_app_handler},
    {"setpower", set_power_limit_handler}, {"setname", set_name_handler}, {"setkey", set_key_handler}, {"PDPsearch", PDP_search_handler}, {"setSFB", set_SFB_handler}
};
const lookup_table_t* handler_map = handler_map_static;

int set_SFB_handler(CmdStr param, short param_cnt, uint8_t cmd_source)
{
    if (param_cnt == 1) {
        return 0;
    }
    SC8815_Config.SC8815_Status = SC8815_Standby;
    Application_SC8815_Standby();
    int value = 0;
    sscanf(param[1], "%d", &value);
    if (value) {
        app_config_save_config.SC8815_SFB = SCHWI_SFB_Enable;
        SC8815_SFB_Enable();
        usb_printf("SFB Enable succeed");
    } else {
        app_config_save_config.SC8815_SFB = SCHWI_SFB_Disable;
        SC8815_SFB_Disable();
        usb_printf("SFB Disable succeed");
    }
    app_config_save();
    return 1;
}

/**
 * @brief 设置 SC8815 电池路径上的限流值,双向通用
 * 
 * @param param 命令参数
 * @param param_cnt 命令参数个数
 * @return 1-命令执行成功，0-命令执行失败 
 */
int setIBAT_handler(CmdStr param, short param_cnt, uint8_t cmd_source)
{
    if (param_cnt == 1)
    {
        return 0;
    }
    SC8815_Config.SC8815_Status = SC8815_Standby;
    Application_SC8815_Standby();
    float value;
    sscanf(param[1], "%f", &value);
    value = value * 1000; 
    SC8815_Config.SC8815_IBAT_Limit = value;
    SC8815_SetBatteryCurrLimit(value);
    return 1;
}

/**
 * @brief 配置 SC8815 电池内阻参数
 * 
 * @param param 命令参数
 * @param param_cnt 命令参数个数
 * @return 1-命令执行成功，0-命令执行失败 
 */
int setIRCOMP_handler(CmdStr param, short param_cnt, uint8_t cmd_source)
{
    if (param_cnt == 1)
    {
        return 0;
    }
    SC8815_Config.SC8815_Status = SC8815_Standby;
    Application_SC8815_Standby();
    int value;
    sscanf(param[1], "%x", &value);
    extern SC8815_BatteryConfigTypeDef SC8815_BatteryConfigStruct;
	SC8815_BatteryConfigStruct.IRCOMP = value;
	SC8815_BatteryConfig(&SC8815_BatteryConfigStruct);
    return 1;
}

/**
 * @brief 配置 SC8815 电池反馈模式
 * 
 * @param param 命令参数
 * @param param_cnt 命令参数个数
 * @return 1-命令执行成功，0-命令执行失败 
 */
int setVBAT_SEL_handler(CmdStr param, short param_cnt, uint8_t cmd_source)
{
    if (param_cnt == 1)
    {
        return 0;
    }
    SC8815_Config.SC8815_Status = SC8815_Standby;
    Application_SC8815_Standby();
    int value;
    sscanf(param[1], "%x", &value);
    extern SC8815_BatteryConfigTypeDef SC8815_BatteryConfigStruct;
	SC8815_BatteryConfigStruct.VBAT_SEL = value;
	SC8815_BatteryConfig(&SC8815_BatteryConfigStruct);
    return 1;
}

/**
 * @brief 配置 SC8815 电池个数参数
 * 
 * @param param 命令参数
 * @param param_cnt 命令参数个数
 * @return 1-命令执行成功，0-命令执行失败 
 */
int setCSEL_handler(CmdStr param, short param_cnt, uint8_t cmd_source)
{
    if (param_cnt == 1)
    {
        return 0;
    }
    SC8815_Config.SC8815_Status = SC8815_Standby;
    Application_SC8815_Standby();
    int value;
    sscanf(param[1], "%d", &value);
    SC8815_SetBatteryCell(value);
    return 1;
}

/**
 * @brief 配置 SC8815 电池电压参数
 * 
 * @param param 命令参数
 * @param param_cnt 命令参数个数
 * @return 1-命令执行成功，0-命令执行失败 
 */
int setVCELL_handler(CmdStr param, short param_cnt, uint8_t cmd_source)
{
    if (param_cnt == 1)
    {
        return 0;
    }
    SC8815_Config.SC8815_Status = SC8815_Standby;
    Application_SC8815_Standby();
    int value;
    sscanf(param[1], "%x", &value);
    extern SC8815_BatteryConfigTypeDef SC8815_BatteryConfigStruct;
	SC8815_BatteryConfigStruct.VCELL = value;
	SC8815_BatteryConfig(&SC8815_BatteryConfigStruct);
    return 1;
}

/**
 * @brief 设置 SC8815 开关切换频率
 * 
 * @param param 命令参数
 * @param param_cnt 命令参数个数
 * @return 1-命令执行成功，0-命令执行失败 
 */
int setSW_FREQ_handler(CmdStr param, short param_cnt, uint8_t cmd_source)
{
    if (param_cnt == 1)
    {
        return 0;
    }
    SC8815_Config.SC8815_Status = SC8815_Standby;
    Application_SC8815_Standby();
    int value;
    sscanf(param[1], "%x", &value);
    extern SC8815_HardwareInitTypeDef SC8815_HardwareInitStruct;
    SC8815_HardwareInitStruct.SW_FREQ = value;
    SC8815_SetSWFreq(value);
    SC8815_OTG_Enable();
    // SC8815_Config.SC8815_Status = SC8815_LoadStart;
    // Application_SC8815_loadStart();
    switch (value) {
        case 1: { value = SCHWI_FREQ_150KHz;app_config_save_config.SW_FREQ = SCHWI_FREQ_150KHz;break; }
        case 2: { value = SCHWI_FREQ_300KHz_1;app_config_save_config.SW_FREQ = SCHWI_FREQ_300KHz_1;break; }
        case 3: { value = SCHWI_FREQ_300KHz_2;app_config_save_config.SW_FREQ = SCHWI_FREQ_300KHz_2;break; }
        case 4: { value = SCHWI_FREQ_450KHz;app_config_save_config.SW_FREQ = SCHWI_FREQ_450KHz;break; }
        default:break;
    }
    app_config_save();
    return 1;
}

/**
 * @brief 设置 SC8815 死区时间
 * 
 * @param param 命令参数
 * @param param_cnt 命令参数个数
 * @return 1-命令执行成功，0-命令执行失败 
 */
int setDeadTime_handler(CmdStr param, short param_cnt, uint8_t cmd_source)
{
    if (param_cnt == 1)
    {
        return 0;
    }
    SC8815_Config.SC8815_Status = SC8815_Standby;
    Application_SC8815_Standby();
    int value;
    sscanf(param[1], "%d", &value);
    SC8815_SetDeadTime(value);
    return 1;
}

/**
 * @brief 设置 SC8815 VBUS电压反馈模式
 * 
 * @param param 命令参数
 * @param param_cnt 命令参数个数
 * @return 1-命令执行成功，0-命令执行失败 
 */
int setFB_Mode_handler(CmdStr param, short param_cnt, uint8_t cmd_source)
{
    if (param_cnt == 1)
    {
        return 0;
    }
    int value;
    sscanf(param[1], "%d", &value);
    SC8815_SetVBUSFBMode(value);
    return 1;
}

/**
 * @brief 打开或关闭 SC8815 频率抖动功能
 * 
 * @param param 命令参数
 * @param param_cnt 命令参数个数
 * @return 1-命令执行成功，0-命令执行失败 
 */
int setDITHER_handler(CmdStr param, short param_cnt, uint8_t cmd_source)
{
    if (param_cnt == 1)
    {
        return 0;
    }
    SC8815_Config.SC8815_Status = SC8815_Standby;
    Application_SC8815_Standby();
    int value;
    sscanf(param[1], "%x", &value);
    extern SC8815_HardwareInitTypeDef SC8815_HardwareInitStruct;
    SC8815_HardwareInitStruct.DITHER = value;
    SC8815_HardwareInit(&SC8815_HardwareInitStruct);
    return 1;
}

/**
 * @brief 设置 SC8815 反向放电模式下VBUS动态变化的速率
 * 
 * @param param 命令参数
 * @param param_cnt 命令参数个数
 * @return 1-命令执行成功，0-命令执行失败 
 */
int setSLEW_SET_handler(CmdStr param, short param_cnt, uint8_t cmd_source)
{
    if (param_cnt == 1)
    {
        return 0;
    }
    SC8815_Config.SC8815_Status = SC8815_Standby;
    Application_SC8815_Standby();
    int value;
    sscanf(param[1], "%x", &value);
    extern SC8815_HardwareInitTypeDef SC8815_HardwareInitStruct;
    SC8815_HardwareInitStruct.SLEW_SET = value;
    SC8815_HardwareInit(&SC8815_HardwareInitStruct);
    return 1;
}

/**
 * @brief 设置 SC8815 电流环路响应带宽
 * 
 * @param param 命令参数
 * @param param_cnt 命令参数个数
 * @return 1-命令执行成功，0-命令执行失败 
 */
int setILIM_BW_handler(CmdStr param, short param_cnt, uint8_t cmd_source)
{
    if (param_cnt == 1)
    {
        return 0;
    }
    SC8815_Config.SC8815_Status = SC8815_Standby;
    Application_SC8815_Standby();
    int value;
    sscanf(param[1], "%d", &value);
    SC8815_SetILIMBW(value);
    return 1;
}

/**
 * @brief 订阅上报数据
 * 
 * @param param 命令参数
 * @param param_cnt 命令参数个数
 * @return 1-命令执行成功，0-命令执行失败 
 */
int get_msg_handler(CmdStr param, short param_cnt, uint8_t cmd_source)
{
    if (param_cnt == 1)
    {
        return 0;
    }
    extern Application_Config APP_config;
    int value;
    sscanf(param[1], "%d", &value);
    APP_config.msg_get_time = value;
    return 1;
}

int lock_key_handler(CmdStr param, short param_cnt, uint8_t cmd_source)
{
    if (param_cnt == 1)
    {
        return 0;
    }
    int value;
    sscanf(param[1], "%d", &value);
    APP_config.lock_key = value;
    return 1;
}

int set_key_handler(CmdStr param, short param_cnt, uint8_t cmd_source)
{
    if (param_cnt == 1) {
        return 0;
    }
    int value;
    sscanf(param[1], "%d", &value);
    if (value == 112233) {
        uint32_t CpuID = GetCPU_ID();
        STMFLASH_Write(APP_SET_KEY_ADDR, (uint16_t*)&CpuID, sizeof(uint32_t) >> 1);
        __set_FAULTMASK(1); //关闭所有中断
        NVIC_SystemReset(); //进行软件复位
    }
    return 1;
}

int set_switch_handler(CmdStr param, short param_cnt, uint8_t cmd_source)
{
    if (param_cnt == 1)
    {
        return 0;
    }
    extern uint8_t key3_press;
    if (strstr(param[1], "ON") != NULL && HAL_GPIO_ReadPin(SC8815_PSTOP_GPIO_Port, SC8815_PSTOP_Pin) == SET)
    {
        SC8815_Config.SC8815_Status = SC8815_LoadStart;
        Application_SC8815_loadStart();
    }
    else if (strstr(param[1], "OFF") != NULL && HAL_GPIO_ReadPin(SC8815_PSTOP_GPIO_Port, SC8815_PSTOP_Pin) == RESET)
    {
        SC8815_Config.SC8815_Status = SC8815_Standby;
        Application_SC8815_Standby();
    }
    return 1;
}

/**
 * @brief 设置 SC8815 VBUS 路径上的限流值,双向通用
 * 
 * @param param 命令参数
 * @param param_cnt 命令参数个数
 * @return 1-命令执行成功，0-命令执行失败 
 */
int set_current_handler(CmdStr param, short param_cnt, uint8_t cmd_source)
{
    if (param_cnt == 1)
    {
        return 0;
    }
    if (strstr(param[1], "UP") != NULL)
    {
        SC8815_Config.SC8815_IBUS_Limit += SC8815_Config.SC8815_IBUS_CMD_Step;
        if (SC8815_Config.SC8815_IBUS_Limit > SC8815_IBUS_MAX)
            SC8815_Config.SC8815_IBUS_Limit = SC8815_IBUS_MAX;
        App_SC8815_SetBusCurrentLimit(SC8815_Config.SC8815_IBUS_Limit);
    }
    else
    {
//        SC8815_Config.SC8815_Status = SC8815_Standby;
//        Application_SC8815_Standby();
        float value;
        sscanf(param[1], "%f", &value);
        if (value > SC8815_IBUS_MAX / 1000)
        {
            return 0;
        } else if (value < SC8815_IBUS_MIN / 1000) {
            return 0;
        }
        value = value * 1000;
        SC8815_Config.SC8815_IBUS_Limit = value;
        App_SC8815_SetBusCurrentLimit(value);
    }
    return 1;
}

int set_current_port_handler(CmdStr param, short param_cnt, uint8_t cmd_source)
{
    if (param_cnt == 1)
    {
        return 0;
    }
    // float value;
    // sscanf(param[1], "%f", &value);
    // value = value * 1000; 
    // SC8815_Config.SC8815_IBUS_Limit = value;
    // SC8815_Config.SC8815_IBUS_Limit_Old = value;
    // App_SC8815_SetBusCurrentLimit(value);
    return 1;
}

int set_current_step_handler(CmdStr param, short param_cnt, uint8_t cmd_source)
{
    if (param_cnt == 1)
    {
        return 0;
    }
    float value;
    sscanf(param[1], "%f", &value);
    if (value < 0)
        return 0;
    value = value * 1000;
    SC8815_Config.SC8815_IBUS_CMD_Step = value;
    return 1;
}

/**
 * @brief 设置 SC8815 在 OTG 反向输出时的输出电压
 * 
 * @param param 命令参数
 * @param param_cnt 命令参数个数
 * @return 1-命令执行成功，0-命令执行失败 
 */
int set_voltage_handler(CmdStr param, short param_cnt, uint8_t cmd_source)
{
    if (param_cnt == 1)
    {
        return 0;
    }
    if (strstr(param[1], "UP") != NULL)
    {
        SC8815_Config.SC8815_VBUS += SC8815_Config.SC8815_VBUS_CMD_Step;
        if (SC8815_Config.SC8815_VBUS > SC8815_VBUS_MAX)
            SC8815_Config.SC8815_VBUS = SC8815_VBUS_MAX;
        App_SC8815_SetOutputVoltage(SC8815_Config.SC8815_VBUS);
    }
    else
    {
        float value;
        sscanf(param[1], "%f", &value);
        if (value > SC8815_VBUS_MAX / 1000)
        {
            return 0;
        } else if (value < SC8815_VBUS_MIN / 1000) {
            return 0;
        }
        value = value * 1000;
        SC8815_Config.SC8815_VBUS = value;
        App_SC8815_SetOutputVoltage(value);
    }
    return 1;
}

int set_voltage_step_handler(CmdStr param, short param_cnt, uint8_t cmd_source)
{
    if (param_cnt == 1)
    {
        return 0;
    }
    float value;
    sscanf(param[1], "%f", &value);
    value = value * 1000; 
    SC8815_Config.SC8815_VBUS_CMD_Step = value;
    return 1;
}

int set_voltage_prot_handler(CmdStr param, short param_cnt, uint8_t cmd_source)
{
    if (param_cnt == 1)
    {
        return 0;
    }
    return 1;
}

int set_voltage_limit_handler(CmdStr param, short param_cnt, uint8_t cmd_source)
{
    if (param_cnt == 1)
    {
        return 0;
    }
    return 1;
}

int get_fetch_current_handler(CmdStr param, short param_cnt, uint8_t cmd_source)
{
    // uint32_t value[1];
    // // sscanf(param[1], "%d", &value);
    // value[0] = htonl(App_getIBUS_mA());
    // if (cmd_source)
    // {
    //     CDC_Transmit_FS((uint8_t*)&value, sizeof(uint32_t));
    // }
    // else
    // {
    //     HAL_UART_Transmit(&huart1, (uint8_t*)&value, sizeof(uint32_t), 1000);
    // }
    if (cmd_source)
    {
        usb_printf("%.4f\r\n", App_getIBUS_A());
    }
    else
    {
        printf("%.4f\r\n", App_getIBUS_A());
    }
    return 1;
}

int get_fetch_voltage_handler(CmdStr param, short param_cnt, uint8_t cmd_source)
{
    // uint32_t value[1];
    // // sscanf(param[1], "%d", &value);
    // value[0] = htonl(App_getVBUS_mV());
    // if (cmd_source)
    // {
    //     CDC_Transmit_FS((uint8_t*)&value, sizeof(uint32_t));
    // }
    // else
    // {
    //     HAL_UART_Transmit(&huart1, (uint8_t*)&value, sizeof(uint32_t), 1000);
    // }
    if (cmd_source)
    {
        usb_printf("%.4f\r\n", App_getVBUS_V());
    }
    else
    {
        printf("%.4f\r\n", App_getVBUS_V());
    }
    return 1;
}

int get_fetch_power_handler(CmdStr param, short param_cnt, uint8_t cmd_source)
{
    // uint32_t value[1];
    // // sscanf(param[1], "%d", &value);
    // value[0] = htonl((App_getVBUS_mV() / 1000) * (App_getIBUS_mA() / 1000));
    // if (cmd_source)
    // {
    //     CDC_Transmit_FS((uint8_t*)&value, sizeof(uint32_t));
    // }
    // else
    // {
    //     HAL_UART_Transmit(&huart1, (uint8_t*)&value, sizeof(uint32_t), 1000);
    // }
    if (cmd_source)
    {
        usb_printf("%.4f\r\n", (App_getVBUS_V() * App_getIBUS_A()));
    }
    else
    {
        printf("%.4f\r\n", (App_getVBUS_V() * App_getIBUS_A()));
    }
    return 1;
}

int get_versions_handler(CmdStr param, short param_cnt, uint8_t cmd_source)
{
    if (cmd_source)
    {
        CDC_Transmit_FS("1.1.0\r\n", strlen("1.1.0\r\n"));
    }
    else
    {
        printf("1.1.0\r\n");
    }
    return 1;
}

int set_power_limit_handler(CmdStr param, short param_cnt, uint8_t cmd_source)
{
    if (param_cnt == 1) {
        return 0;
    }
    int value;
    sscanf(param[1], "%d", &value);
    if (value > 255) {
        return 0;
    }
    set_sc8815_power(value);
    return 1;
}

int set_preset_handler(CmdStr param, short param_cnt, uint8_t cmd_source)
{
    if (param_cnt < 4)
    {
        return 0;
    }
    int value, value1;
    sscanf(param[1], "%d", &value);
    sscanf(param[2], "%d", &value1);
    if (strstr(param[2], "30") != NULL && param_cnt == 4)
    {
        int value2;
        sscanf(param[3], "%d", &value2);
        SC8815_TIM_Work[value].circular = value2;
    }
    else if (param_cnt == 6) {
        if (value > 9 || value1 > 29)
            return 0;
        float value2, value3, value4;
        sscanf(param[3], "%f", &value2);
        value2 *= 1000;
        sscanf(param[4], "%f", &value3);
        value3 *= 1000;
        if (value2 > SC8815_VBUS_MAX || value2 < SC8815_VBUS_MIN || value3 > SC8815_IBUS_MAX || value3 < SC8815_IBUS_MIN)
            return 0;
        sscanf(param[5], "%f", &value4);
        SC8815_TIM_Work[value].SC8815_VBUS[value1] = value2;
        SC8815_TIM_Work[value].SC8815_IBUS_Limit[value1] = value3;
        SC8815_TIM_Work[value].SC8815_TIM_Work_second[value1] = value4;
    }
    return 1;
}

int save_config_handler(CmdStr param, short param_cnt, uint8_t cmd_source)
{
    if (param_cnt == 1)
    {
        return 0;
    }
    if (strstr(param[1], "preset") != NULL)
    {
        SC8815_Preset_Save();
        return 1;
    }
    return 0;
}

int upgrade_app_handler(CmdStr param, short param_cnt, uint8_t cmd_source)
{
    if (param_cnt == 1)
    {
        return 0;
    }
    if (strstr(param[1], "app") != NULL)
    {
        set_app_upgrade_flag(1);
        if (cmd_source)
        {
            usb_printf("ok");
        }
        else
        {
            printf("ok");
        }
        Application_SoftwareDelay(10);
        __set_FAULTMASK(1); //关闭所有中断
        NVIC_SystemReset(); //进行软件复位
        return 1;
    }
    return 0;
}

int start_preset_handler(CmdStr param, short param_cnt, uint8_t cmd_source)
{
    if (param_cnt == 1)
    {
        return 0;
    }
    int value;
    sscanf(param[1], "%d", &value);
    extern presset_config_set_typeDef presset_config_set;
    extern SC8815_TIM_WorkTypeDef SC8815_TIM_Work[SC8815_TIM_WORK_SIZE];
    if (value > SC8815_TIM_WORK_SIZE - 1) {
        return 0;
    }
    SC8815_Config.sc8815_tim_work_time = SC8815_TIM_WORK_TIME_FAST;
    SC8815_Config.sc8815_tim_work_step = 0;
    memcpy(&presset_config_set.set_circular, &SC8815_TIM_Work[value].circular, sizeof(uint16_t));
    memcpy(&presset_config_set.set_time, &SC8815_TIM_Work[value].SC8815_TIM_Work_second, sizeof(uint16_t) * SC8815_TIM_WORK_STEP);
    memcpy(&presset_config_set.set_ibus, &SC8815_TIM_Work[value].SC8815_IBUS_Limit, sizeof(float) * SC8815_TIM_WORK_STEP);
    memcpy(&presset_config_set.set_vbus, &SC8815_TIM_Work[value].SC8815_VBUS, sizeof(float) * SC8815_TIM_WORK_STEP);
    APP_config.lock_key = 1;
    SC8815_Config.sc8815_tim_work_lcd_flush = tim_work_lcd_cmd;
    return 1;
}

int set_name_handler(CmdStr param, short param_cnt, uint8_t cmd_source)
{
    if (param_cnt == 1) {
        return 0;
    }
    if (strlen(param[1]) > 10) {
        return 0;
    }
    memset(APP_config.device_name, 0, sizeof(APP_config.device_name));
    memcpy(APP_config.device_name, param[1], sizeof(APP_config.device_name));
    memcpy(app_config_save_config.device_name, APP_config.device_name, sizeof(APP_config.device_name));
    app_config_save();
    return 1;
}

int PDP_search_handler(CmdStr param, short param_cnt, uint8_t cmd_source)
{
    if (param_cnt == 1) {
        return 0;
    }
    char str[24];
    sprintf(str, "info%s", APP_config.device_name);
    //memcpy(str + 4 + sizeof(APP_config.device_name), "1.1.0", sizeof("1.1.0"));
    if (cmd_source) {
        usb_printf("%s", str);
    } 
    // else {
    //     printf("%s", str);
    // }
    return 1;
}

int calibration_ibus_handler(CmdStr param, short param_cnt, uint8_t cmd_source)
{
    if (param_cnt == 1) {
        return 0;
    }
    int value;
    sscanf(param[1], "%d", &value);
    SC8815_Config.sc8815_calibration_flag = value;
    return 1;
}

int get_temperature_handler(CmdStr param, short param_cnt, uint8_t cmd_source)
{
    if (param_cnt == 1) {
        return 0;
    }
    if (cmd_source) {
        usb_printf("%.1f", App_getTemp());
    } else {
        printf("%.1f", App_getTemp());
    }
    return 1;
}


void remove_crlf(char *str) {
    size_t len = strlen(str);
    if (len >= 2 && str[len - 2] == '\r' && str[len - 1] == '\n') {
        str[len - 2] = '\0';
    } else if (len >= 1 && (str[len - 1] == '\r' || str[len - 1] == '\n')) {
        str[len - 1] = '\0';
    }
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
    for (size_t i = 0; i < CMD_STR_CNT; i++)
    {
        param[i] = tokens[i];
    }
    return token_count;
}

/**
 * @brief ASCII命令处理函数
 * 
 * @param cmd 命令字符串
 * @param cmd_source 命令源头，0-UART，1-USB
 * @return 1-成功 0-失败
 */
int ascii_process(char *cmd, uint8_t cmd_source)
{
    CmdStr param;
    short param_cnt;
    int ret = -1;
    int i;
    int map_cnt = sizeof(handler_map_static) / sizeof(lookup_table_t);
    char* tokens[10];
    // param_cnt = splitString(cmd, ':', tokens, 10, param);
    remove_crlf(cmd);
    param_cnt = splitString(cmd, ' ', tokens, 10, param);
    for (i = 0; i < map_cnt; i++)
    {
        if (!strcasecmp(param[0], handler_map[i].desc))
        {
            ascii_handler_fn *handler = NULL;
            handler = handler_map[i].handler;
            if (handler)
            {
                ret = (*handler)(param, param_cnt, cmd_source);
                break;
            }
        }
    }
    return ret;
}
