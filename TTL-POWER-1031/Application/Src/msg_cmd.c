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

#define CMD_STR_CNT 6 // 命令参数数量

char uart1_Cmd[Cmd_Length];    //命令缓冲区
uint32_t cmd_Index = 0;  //串口1命令计数指针
// uint8_t uart1_Receive_Data = 0;  //串口1中断接收数据缓存

typedef char *CmdStr[CMD_STR_CNT];
typedef int ascii_handler_fn(CmdStr param, short param_cnt, uint8_t cmd_source);
static ascii_handler_fn
    setVBUS_handler, setIBUS_handler, setIBAT_handler, setIRCOMP_handler, setVBAT_SEL_handler, setCSEL_handler,
    setVCELL_handler, setSW_FREQ_handler, setDeadTime_handler, setFB_Mode_handler, setDITHER_handler,
    setSLEW_SET_handler, setILIM_BW_handler, get_msg_handler, lock_key_handler, start_preset_handler,
    set_switch_handler, set_current_handler, set_current_port_handler, set_current_step_handler,
    set_voltage_handler, set_voltage_step_handler, set_voltage_prot_handler, set_voltage_limit_handler,
    get_fetch_current_handler, get_fetch_voltage_handler, get_fetch_power_handler, get_versions_handler,
    set_preset_handler, save_config_handler, upgrade_app_handler;
typedef struct lookup_table
{
    const char *desc;
    ascii_handler_fn *handler;
} lookup_table_t;
static const lookup_table_t handler_map_static[] = {
    {"setvbus", setVBUS_handler}, {"setibus", setIBUS_handler}, {"setibat", setIBAT_handler}, {"setircomp", setIRCOMP_handler},
    {"setvbatsel", setVBAT_SEL_handler}, {"setcsel", setCSEL_handler}, {"setvcell", setVCELL_handler}, {"setswfreq", setSW_FREQ_handler}, 
    {"setdeadtime", setDeadTime_handler}, {"setfbmode", setFB_Mode_handler}, {"setdither", setDITHER_handler},
    {"setslewset", setSLEW_SET_handler}, {"setilimbw", setILIM_BW_handler}, {"getmsg", get_msg_handler}, {"lockkey", lock_key_handler}, {"startPreset", start_preset_handler},
    {"OUTP", set_switch_handler}, {"CURR", set_current_handler}, {"CURR:PROT", set_current_port_handler}, {"CURR:STEP", set_current_step_handler},
    {"VOLT", set_voltage_handler}, {"VOLT:STEP", set_voltage_step_handler}, {"VOLT:PROT", set_voltage_prot_handler},
    {"VOLT:LIMIT", set_voltage_limit_handler}, {"MEAS:CURR?", get_fetch_current_handler}, {"MEAS:VOLT?", get_fetch_voltage_handler},
    {"MEAS:POW?", get_fetch_power_handler}, {"SYST:VERS?", get_versions_handler}, {"preset", set_preset_handler}, {"save", save_config_handler}, {"upgrade", upgrade_app_handler}
};
const lookup_table_t* handler_map = handler_map_static;


/**
 * @brief 设置 SC8815 在 OTG 反向输出时的输出电压
 * 
 * @param param 命令参数
 * @param param_cnt 命令参数个数
 * @return 1-命令执行成功，0-命令执行失败 
 */
int setVBUS_handler(CmdStr param, short param_cnt, uint8_t cmd_source)
{
    if (param_cnt == 1)
    {
        return 0;
    }
    float value;
    sscanf(param[1], "%f", &value);
    if (value > 36)
    {
        return 0;
    }
    value = value * 1000;
    SC8815_Config.SC8815_VBUS = value;
    SC8815_Config.SC8815_VBUS_Old = value;
    SC8815_SetOutputVoltage(value);
    return 1;
}

/**
 * @brief 设置 SC8815 VBUS 路径上的限流值,双向通用
 * 
 * @param param 命令参数
 * @param param_cnt 命令参数个数
 * @return 1-命令执行成功，0-命令执行失败 
 */
int setIBUS_handler(CmdStr param, short param_cnt, uint8_t cmd_source)
{
    if (param_cnt == 1)
    {
        return 0;
    }
    float value;
    sscanf(param[1], "%f", &value);
    if (value > 6)
    {
        return 0;
    }
    value = value * 1000;
    SC8815_Config.SC8815_IBUS_Limit = value;
    SC8815_Config.SC8815_IBUS_Limit_Old = value;
    SC8815_SetBusCurrentLimit(value);
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
    SC8815_Config.SC8815_Status = SC8815_Standby;
    Application_SC8815_Standby();
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

int set_switch_handler(CmdStr param, short param_cnt, uint8_t cmd_source)
{
    if (param_cnt == 1)
    {
        return 0;
    }
    extern uint8_t key3_press;
    if (strstr(param[1], "ON") != NULL && HAL_GPIO_ReadPin(SC8815_PSTOP_GPIO_Port, SC8815_PSTOP_Pin) == SET)
    {
        // SC8815_Config.SC8815_Status = SC8815_LoadStart;
        Application_SC8815_Run();
		SC8815_SFB_Disable();
		// HAL_Delay(50);
		Application_SoftwareDelay(50);
		SC8815_SFB_Enable();
		SC8815_Config.VOUT_Open_Time = HAL_GetTick();
    }
    else if (strstr(param[1], "OFF") != NULL && HAL_GPIO_ReadPin(SC8815_PSTOP_GPIO_Port, SC8815_PSTOP_Pin) == RESET)
    {
        SC8815_Config.SC8815_Status = SC8815_Standby;
        Application_SC8815_Standby();
    }
    return 1;
}

int set_current_handler(CmdStr param, short param_cnt, uint8_t cmd_source)
{
    if (param_cnt == 1)
    {
        return 0;
    }
    if (strstr(param[1], "UP") != NULL)
    {
        if ((SC8815_Config.SC8815_IBUS_Limit + SC8815_Config.SC8815_IBUS_CMD_Step) > 6000)
        {
            return 0;
        }
        SC8815_Config.SC8815_IBUS_Limit += SC8815_Config.SC8815_IBUS_CMD_Step;
        SC8815_Config.SC8815_IBUS_Limit_Old = SC8815_Config.SC8815_IBUS_Limit;
        SC8815_SetBusCurrentLimit(SC8815_Config.SC8815_IBUS_Limit);
    }
    else
    {
        SC8815_Config.SC8815_Status = SC8815_Standby;
        Application_SC8815_Standby();
        float value;
        sscanf(param[1], "%f", &value);
        if (value > 6)
        {
            return 0;
        }
        value = value * 1000;
        SC8815_Config.SC8815_IBUS_Limit = value;
        SC8815_Config.SC8815_IBUS_Limit_Old = value;
        SC8815_SetBusCurrentLimit(value);
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
    // SC8815_SetBusCurrentLimit(value);
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
    value = value * 1000; 
    SC8815_Config.SC8815_IBUS_CMD_Step = value;
    return 1;
}

int set_voltage_handler(CmdStr param, short param_cnt, uint8_t cmd_source)
{
    if (param_cnt == 1)
    {
        return 0;
    }
    if (strstr(param[1], "UP") != NULL)
    {
        if ((SC8815_Config.SC8815_VBUS + SC8815_Config.SC8815_VBUS_CMD_Step) > 36000)
        {
            return 0;
        }
        SC8815_Config.SC8815_VBUS += SC8815_Config.SC8815_VBUS_CMD_Step;
        SC8815_Config.SC8815_VBUS_Old = SC8815_Config.SC8815_VBUS;
        SC8815_SetOutputVoltage(SC8815_Config.SC8815_VBUS);
    }
    else
    {
        float value;
        sscanf(param[1], "%f", &value);
        if (value > 36)
        {
            return 0;
        }
        value = value * 1000;
        SC8815_Config.SC8815_VBUS = value;
        SC8815_Config.SC8815_VBUS_Old = value;
        SC8815_SetOutputVoltage(value);
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
        usb_printf("%f", App_getIBUS_A());
    }
    else
    {
        printf("%f", App_getIBUS_A());
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
        usb_printf("%f", App_getVBUS_V());
    }
    else
    {
        printf("%f", App_getVBUS_V());
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
        usb_printf("%f", App_getVBUS_V() * App_getIBUS_A());
    }
    else
    {
        printf("%f", App_getVBUS_V() * App_getIBUS_A());
    }
    return 1;
}

int get_versions_handler(CmdStr param, short param_cnt, uint8_t cmd_source)
{
    if (cmd_source)
    {
        CDC_Transmit_FS("1.1.1", strlen("1.1.1"));
    }
    else
    {
        printf("1.1.1");
    }
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
        SC8815_TIM_Work[value].circular = value1;
    }
    else if(param_cnt == 6){
        float value2, value3, value4;
        sscanf(param[3], "%f", &value2);
        value2 *= 1000;
        sscanf(param[4], "%f", &value3);
        value3 *= 1000;
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
        app_config_save_config.upgrade_flag = 1;
        app_config_save();
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
