#include "menu.h"
#include "UART_Debug.h"
#include "Application_SC8815.h"
#include "Application.h"
#include "Application_LCD.h"

#define presset_menu_index 10

Key_Index sub_index;    //��ҳ������
menu_i32 current_menu_index;    //��ǰ�˵�����

// �˵���������
static OP_MENU_PAGE g_opStruct[] =
{
    {MAIN_PAGE, main_page_process},
    {MAIN_MENU_PAGE, main_menu_page_process},
    {VOUT_PAGE, vout_page_process},
    {IOUT_PAGE, iout_page_process},
    {PRESSET_PAGE, presset_page_process},
    {BUZZER_PAGE, buzzer_page_process},
    {TEMP_PAGE, temp_page_process},
    {FASTCH_PAGE, fastch_page_process},
};

/**
 * @brief ��ת��������Ӧ��ҳ��
 * 
 * @param op ��תҳ������
 * @param KeyValue ������ֵ
 * @return �ɹ����� 0,ʧ�ܷ��� -1 
 */
static int JUMP_Table(menu_i32 op, menu_u8 KeyValue)
{
    if (op >= sizeof(g_opStruct) / sizeof(g_opStruct[0]) || op < 0)
    {
        printf("unknow operate!\n");
        return -1;
    }
    g_opStruct[op].opfun(KeyValue);
    return 0;
}

/**
 * @brief �˵�ѡ����
 * 
 * @param KeyValue ������ֵ
 */
void Menu_Select_Item(menu_u8 KeyValue)
{
    JUMP_Table(current_menu_index, KeyValue);
}

/**
 * @brief ��ҳ�洦��
 * 
 * @param KeyValue ������ֵ
 */
void main_page_process(menu_u8 KeyValue)
{
    switch (KeyValue)
    {
    case KEY1_SHORT:
        iout_page_ui_process(KeyValue);
        break;   
    case KEY2_LONG:
        main_menu_Init();
        break;
    case KEY2_SHORT:
        vout_page_ui_process(KeyValue);
        break;
    case KEY3_SHORT:
        if (HAL_GPIO_ReadPin(SC8815_PSTOP_GPIO_Port, SC8815_PSTOP_Pin) == RESET)
        {
            SC8815_Config.SC8815_Status = SC8815_Standby;
            Application_SC8815_Standby();
            LCD_ShowString(10, 102, "OFF", BLACK, GRAY, 32, 0);
            LCD_Fill_DMA(1, 102, 10, LCD_H - 1, GRAY);
            LCD_Fill_DMA(56, 102, 66, LCD_H - 1, GRAY);
        }
        else {
            SC8815_Config.SC8815_Status = SC8815_LoadStart;
            LCD_ShowString(1, 102, " ON ", BLACK, GREEN, 32, 0);
        }
        break;
    default:
        break;
    }
}

/**
 * @brief ���˵�ҳ�洦��
 * 
 * @param KeyValue ������ֵ
 */
void main_menu_page_process(menu_u8 KeyValue)
{
    switch (KeyValue)
    {
    case LEFT:
        (sub_index.main_menu_current_index > 0) ? (sub_index.main_menu_current_index--) : (sub_index.main_menu_current_index = 0);
        main_menu_page_ui_process(sub_index.main_menu_current_index);
        break;
    case RIGHT:
        (sub_index.main_menu_current_index < 3) ? (sub_index.main_menu_current_index++) : (sub_index.main_menu_current_index = 3);
        main_menu_page_ui_process(sub_index.main_menu_current_index);
        break;
    case KEY4_SHORT:
        // �����˵�ҳ���ʱ�򣬶̰�KEY4�����Ӧ����ҳ��
        Enter_Page(sub_index.main_menu_current_index, KeyValue);
        break;
    case KEY3_SHORT:
        main_page_init();
        break;
    case KEY2_LONG:
        main_page_init();
        break;
    default:
        break;
    }
}

/**
 * @brief VOUT���ҳ�洦��
 * 
 * @param KeyValue ������ֵ
 */
void vout_page_process(menu_u8 KeyValue)
{
    switch (KeyValue)
    {
    case LEFT:
        set_vout(KeyValue);
        vout_page_ui_process(KeyValue);
        break;
    case RIGHT:
        set_vout(KeyValue);
        vout_page_ui_process(KeyValue);
        break;
    case KEY1_SHORT:
        set_vout(KeyValue);
        break;
    case KEY2_SHORT:
        set_vout(KeyValue);
        break;
    case KEY3_SHORT:
        //������һ��
        set_vout(KeyValue);
        main_page_init();
        break;
    case KEY4_SHORT:
        set_vout(KeyValue);
        main_page_init();
        break;
    default:
        break;
    }
}

/**
 * @brief IOUT���ҳ�洦��
 * 
 * @param KeyValue ������ֵ
 */
void iout_page_process(menu_u8 KeyValue)
{
    switch (KeyValue)
    {
    case LEFT:
        set_iout(KeyValue);
        iout_page_ui_process(KeyValue);
        break;
    case RIGHT:
        set_iout(KeyValue);
        iout_page_ui_process(KeyValue);
        break;
    case KEY1_SHORT:
        set_iout(KeyValue);
        iout_page_ui_process(KeyValue);
        break;
    case KEY2_SHORT:
        set_iout(KeyValue);
        break;
    case KEY3_SHORT:
        //������һ��
        set_iout(KeyValue);
        main_page_init();
        break;
    case KEY4_SHORT:
        set_iout(KeyValue);
        main_page_init();
        break;
    default:
        break;
    }
}

/**
 * @brief Ԥ��ҳ�洦��
 * 
 * @param KeyValue ������ֵ
 */
void presset_page_process(menu_u8 KeyValue)
{
    switch (KeyValue)
    {
    case LEFT:
        (sub_index.presset_current_index > 1) ? (sub_index.presset_current_index--) : (sub_index.presset_current_index = 0);
        presset_page_ui_process(sub_index.presset_current_index);
        break;
    case RIGHT:
        (sub_index.presset_current_index < 10) ? (sub_index.presset_current_index++) : (sub_index.presset_current_index = 10);
        presset_page_ui_process(sub_index.presset_current_index);
        break;
    case KEY3_SHORT:
        //������һ��
        set_iout(KeyValue);
        main_page_init();
        break;
    case KEY4_SHORT:
        set_iout(KeyValue);
        main_page_init();
        break;
    default:
        break;
    }
}

/**
 * @brief ������ҳ�洦��
 * 
 * @param KeyValue ������ֵ
 */
void buzzer_page_process(menu_u8 KeyValue)
{
    
}

/**
 * @brief �¶ȼ��ҳ�洦��
 * 
 * @param KeyValue ������ֵ
 */
void temp_page_process(menu_u8 KeyValue)
{

}

/**
 * @brief �������ҳ�洦��
 * 
 * @param KeyValue ������ֵ
 */
void fastch_page_process(menu_u8 KeyValue)
{
    uint16_t fastch_temp[] = {5, 9, 12, 15, 20};
    int i;
    switch (KeyValue)
    {
    case LEFT:
        for(i = 0; i <= 5; i++)
        {
            if (fastch_temp[i] == sub_index.fastch_current_index)
            {
                break;
            }
        }
        if (i - 1 >= 0)
        {
            fastch_page_ui_process(fastch_temp[i - 1]);
        }
        break;
    case RIGHT:
        for(i = 0; i <= 5; i++)
        {
            if (fastch_temp[i] == sub_index.fastch_current_index)
            {
                break;
            }
        }
        if (i + 1 <= 4)
        {
            fastch_page_ui_process(fastch_temp[i + 1]);
        }
        break;
    case KEY3_SHORT:
        main_menu_page_ui_process(sub_index.main_menu_current_index);
        break;
    case KEY4_SHORT:
        APP_config.fastCharge_InVoltage = sub_index.fastch_current_index;
        set_fastcharge(sub_index.fastch_current_index);
        main_page_init();
        break;
    case KEY2_LONG:
        main_page_init();
        break;
    default:
        break;
    }
}
