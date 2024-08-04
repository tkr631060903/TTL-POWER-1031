#include "msg_cmd.h"
#include "Application.h"
#include <string.h>
#include "Application_SC8815.h"
#include <stdio.h>
#include "Application_Callback.h"

#define CMD_STR_CNT 5 // �����������

char uart1_Cmd[Cmd_Length];    //�������
char USB_COM_Cmd[Cmd_Length];    //�������
uint8_t cmd_Index = 0;  //����1�������ָ��
uint8_t uart1_Receive_Data = 0;  //����1�жϽ������ݻ���

typedef char *CmdStr[CMD_STR_CNT];
typedef int ascii_handler_fn(CmdStr param, short param_cnt);
static ascii_handler_fn
    setVBUS_handler, setIBUS_handler, setIBAT_handler, setIRCOMP_handler, setVBAT_SEL_handler, setCSEL_handler,
    setVCELL_handler, setSW_FREQ_handler, setDeadTime_handler,
    setFB_Mode_handler, setDITHER_handler, setSLEW_SET_handler, setILIM_BW_handler, get_msg_handler;
typedef struct lookup_table
{
    const char *desc;
    ascii_handler_fn *handler;
} lookup_table_t;
static const lookup_table_t handler_map_static[] = {
    {"setvbus", setVBUS_handler}, {"setibus", setIBUS_handler}, {"setibat", setIBAT_handler}, {"setircomp", setIRCOMP_handler},
    {"setvbatsel", setVBAT_SEL_handler}, {"setcsel", setCSEL_handler}, {"setvcell", setVCELL_handler}, {"setswfreq", setSW_FREQ_handler}, 
    {"setdeadtime", setDeadTime_handler}, {"setfbmode", setFB_Mode_handler},
    {"setdither", setDITHER_handler}, {"setslewset", setSLEW_SET_handler}, {"setilimbw", setILIM_BW_handler}, {"getmsg", get_msg_handler}};
const lookup_table_t *handler_map = handler_map_static;


/**
 * @brief ���� SC8815 �� OTG �������ʱ�������ѹ
 * 
 * @param param �������
 * @param param_cnt �����������
 * @return 1-����ִ�гɹ���0-����ִ��ʧ�� 
 */
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

/**
 * @brief ���� SC8815 VBUS ·���ϵ�����ֵ,˫��ͨ��
 * 
 * @param param �������
 * @param param_cnt �����������
 * @return 1-����ִ�гɹ���0-����ִ��ʧ�� 
 */
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

/**
 * @brief ���� SC8815 ���·���ϵ�����ֵ,˫��ͨ��
 * 
 * @param param �������
 * @param param_cnt �����������
 * @return 1-����ִ�гɹ���0-����ִ��ʧ�� 
 */
int setIBAT_handler(CmdStr param, short param_cnt)
{
    if (param_cnt == 1)
    {
        return 0;
    }
    SC8815_Config.SC8815_Status = SC8815_Standby;
    Application_SC8815_Standby();
    int value;
    sscanf(param[1], "%d", &value);
    SC8815_Config.SC8815_IBAT_Limit = value;
    SC8815_SetBatteryCurrLimit(value);
    return 1;
}

/**
 * @brief ���� SC8815 ����������
 * 
 * @param param �������
 * @param param_cnt �����������
 * @return 1-����ִ�гɹ���0-����ִ��ʧ�� 
 */
int setIRCOMP_handler(CmdStr param, short param_cnt)
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
 * @brief ���� SC8815 ��ط���ģʽ
 * 
 * @param param �������
 * @param param_cnt �����������
 * @return 1-����ִ�гɹ���0-����ִ��ʧ�� 
 */
int setVBAT_SEL_handler(CmdStr param, short param_cnt)
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
 * @brief ���� SC8815 ��ظ�������
 * 
 * @param param �������
 * @param param_cnt �����������
 * @return 1-����ִ�гɹ���0-����ִ��ʧ�� 
 */
int setCSEL_handler(CmdStr param, short param_cnt)
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
 * @brief ���� SC8815 ��ص�ѹ����
 * 
 * @param param �������
 * @param param_cnt �����������
 * @return 1-����ִ�гɹ���0-����ִ��ʧ�� 
 */
int setVCELL_handler(CmdStr param, short param_cnt)
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
 * @brief ���� SC8815 �����л�Ƶ��
 * 
 * @param param �������
 * @param param_cnt �����������
 * @return 1-����ִ�гɹ���0-����ִ��ʧ�� 
 */
int setSW_FREQ_handler(CmdStr param, short param_cnt)
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
    SC8815_HardwareInit(&SC8815_HardwareInitStruct);
    // SC8815_SetSWFreq(value);
    return 1;
}

/**
 * @brief ���� SC8815 ����ʱ��
 * 
 * @param param �������
 * @param param_cnt �����������
 * @return 1-����ִ�гɹ���0-����ִ��ʧ�� 
 */
int setDeadTime_handler(CmdStr param, short param_cnt)
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
 * @brief ���� SC8815 VBUS��ѹ����ģʽ
 * 
 * @param param �������
 * @param param_cnt �����������
 * @return 1-����ִ�гɹ���0-����ִ��ʧ�� 
 */
int setFB_Mode_handler(CmdStr param, short param_cnt)
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
 * @brief �򿪻�ر� SC8815 Ƶ�ʶ�������
 * 
 * @param param �������
 * @param param_cnt �����������
 * @return 1-����ִ�гɹ���0-����ִ��ʧ�� 
 */
int setDITHER_handler(CmdStr param, short param_cnt)
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
 * @brief ���� SC8815 ����ŵ�ģʽ��VBUS��̬�仯������
 * 
 * @param param �������
 * @param param_cnt �����������
 * @return 1-����ִ�гɹ���0-����ִ��ʧ�� 
 */
int setSLEW_SET_handler(CmdStr param, short param_cnt)
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
 * @brief ���� SC8815 ������·��Ӧ����
 * 
 * @param param �������
 * @param param_cnt �����������
 * @return 1-����ִ�гɹ���0-����ִ��ʧ�� 
 */
int setILIM_BW_handler(CmdStr param, short param_cnt)
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
 * @brief �����ϱ�����
 * 
 * @param param �������
 * @param param_cnt �����������
 * @return 1-����ִ�гɹ���0-����ִ��ʧ�� 
 */
int get_msg_handler(CmdStr param, short param_cnt)
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

/**
 * @brief ���������ַ���
 * 
 * @param input �����ַ���
 * @param delimiter �ָ���
 * @param tokens ������Ĳ�������
 * @param max_tokens ����������
 * @param param ������Ĳ�������
 * @return ���������ĸ��� 
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

/**
 * @brief ASCII�������
 * 
 * @param cmd �����ַ���
 * @return 1-�ɹ� 0-ʧ��
 */
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
