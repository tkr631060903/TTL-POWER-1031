#include "menu.h"
#include "UART_Debug.h"
#include "Application_SC8815.h"
#include "Application.h"
#include "Application_LCD.h"
#include "string.h"

#define presset_menu_index 10

Key_Index sub_index;    //子页面索引
menu_i32 current_menu_index;    //当前菜单索引
presset_config_set_typeDef presset_config_set;

// 菜单操作表定义
static OP_MENU_PAGE g_opStruct[] =
{
    {MAIN_PAGE, main_page_process},
    {MAIN_MENU_PAGE, main_menu_page_process},
    {VOUT_PAGE, vout_page_process},
    {IOUT_PAGE, iout_page_process},
    {PRESSET_PAGE, presset_page_process},
    {PRESSET_CONFIG_PAGE, presset_config_page_process},
    {PRESSET_CONFIG_SET_PAGE, presset_config_set_page_process},
    {PRESSET_START_PAGE, presset_start_page_process},
    {PRESSET_RUNNING_PAGE, presset_running_page_process},
    {BUZZER_PAGE, buzzer_page_process},
    {TEMP_PAGE, temp_page_process},
    {FASTCH_PAGE, fastch_page_process},
};

/**
 * @brief 跳转到表所对应的页面
 * 
 * @param op 跳转页面索引
 * @param KeyValue 触发键值
 * @return 成功返回 0,失败返回 -1 
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
 * @brief 菜单选择项
 * 
 * @param KeyValue 触发键值
 */
void Menu_Select_Item(menu_u8 KeyValue)
{
    JUMP_Table(current_menu_index, KeyValue);
}

/**
 * @brief 主页面处理
 * 
 * @param KeyValue 触发键值
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
 * @brief 主菜单页面处理
 * 
 * @param KeyValue 触发键值
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
        (sub_index.main_menu_current_index < 4) ? (sub_index.main_menu_current_index++) : (sub_index.main_menu_current_index = 4);
        main_menu_page_ui_process(sub_index.main_menu_current_index);
        break;
    case KEY4_SHORT:
        // 在主菜单页面的时候，短按KEY4进入对应的子页面
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
 * @brief VOUT输出页面处理
 * 
 * @param KeyValue 触发键值
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
        //返回上一级
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
 * @brief IOUT输出页面处理
 * 
 * @param KeyValue 触发键值
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
        //返回上一级
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
 * @brief 预设页面处理
 * 
 * @param KeyValue 触发键值
 */
void presset_page_process(menu_u8 KeyValue)
{
    current_menu_index = PRESSET_PAGE;
    switch (KeyValue)
    {
    case LEFT:
        (sub_index.presset_current_index > 0) ? (sub_index.presset_current_index--) : (sub_index.presset_current_index = 0);
        presset_page_ui_process(sub_index.presset_current_index, KeyValue);
        break;
    case RIGHT:
        (sub_index.presset_current_index < SC8815_TIM_WORK_SIZE - 1) ? (sub_index.presset_current_index++) : (sub_index.presset_current_index = SC8815_TIM_WORK_SIZE - 1);
        presset_page_ui_process(sub_index.presset_current_index, KeyValue);
        break;
    case KEY2_LONG:
        main_page_init();
        break;
    case KEY3_SHORT:
        //返回上一级
        main_menu_Init();
        break;
    case KEY4_SHORT:
        sub_index.presset_config_current_index = 0;
        presset_config_page_ui_process(0);
        break;
    default:
        break;
    }
}

/**
 * @brief 预设参数页面处理
 * 
 * @param KeyValue 触发键值
 */
void presset_config_page_process(menu_u8 KeyValue)
{
    switch (KeyValue)
    {
    case LEFT:
        (sub_index.presset_config_current_index > 0) ? (sub_index.presset_config_current_index--) : (sub_index.presset_config_current_index = 0);
        presset_config_page_ui_process(sub_index.presset_config_current_index);
        break;
    case RIGHT:
        (sub_index.presset_config_current_index < SC8815_TIM_WORK_STEP - 1) ? (sub_index.presset_config_current_index++) : (sub_index.presset_config_current_index = SC8815_TIM_WORK_STEP - 1);
        presset_config_page_ui_process(sub_index.presset_config_current_index);
        break;
    case KEY2_LONG:
        main_page_init();
        break;
    case KEY3_SHORT:
        //返回上一级
        current_menu_index = PRESSET_PAGE;
        presset_page_ui_process(sub_index.presset_current_index, KeyValue);
        break;
    case KEY4_SHORT:
        memset(&presset_config_set, 0, sizeof(presset_config_set_typeDef));
        presset_config_set.set_flag = PRESSET_SET_VOUT;
        presset_config_set.set_setp = 100;
        // extern SC8815_TIM_WorkTypeDef SC8815_TIM_Work[SC8815_TIM_WORK_SIZE];
        // for (size_t i = 0; i < SC8815_TIM_WORK_SIZE; i++)
        // {
        //     presset_config_set.set_vbus[i] = SC8815_TIM_Work[sub_index.presset_current_index].SC8815_VBUS[i];
        //     presset_config_set.set_ibus[i] = SC8815_TIM_Work[sub_index.presset_current_index].SC8815_IBUS_Limit[i];
        //     presset_config_set.set_time[i] = SC8815_TIM_Work[sub_index.presset_current_index].SC8815_TIM_Work_second[i];
        // }
        // presset_config_set.set_circular = SC8815_TIM_Work[sub_index.presset_current_index].circular;
        presset_config_set_page_ui_process(KeyValue);
        break;
    default:
        break;
    }
}

/**
 * @brief 预设参数配置页面处理
 * 
 * @param KeyValue 触发键值
 */
void presset_config_set_page_process(menu_u8 KeyValue)
{
    switch (KeyValue)
    {
    case LEFT:
        set_presset_config(KeyValue);
        presset_config_set_page_ui_process(KeyValue);
        break;
    case RIGHT:
        set_presset_config(KeyValue);
        presset_config_set_page_ui_process(KeyValue);
        break;
    case KEY1_SHORT:
        set_presset_config(KeyValue);
        break;
    case KEY2_SHORT:
        set_presset_config(KeyValue);
        break;
    case KEY2_LONG:
        main_page_init();
        break;
    case KEY3_SHORT:
        presset_config_set.set_flag = PRESSET_SET_CIRCULAR;
        presset_config_set.set_setp = 1;
        presset_config_set_page_ui_process(KeyValue);
        break;
    case KEY4_SHORT:
        if (presset_config_set.set_flag == PRESSET_SET_CIRCULAR)
        {
            set_presset_config(KeyValue);
            current_menu_index = PRESSET_PAGE;
            presset_page_ui_process(sub_index.presset_current_index, KeyValue);
            break;
        }
        if (presset_config_set.current_index == SC8815_TIM_WORK_STEP - 1)
        {
            presset_config_set.set_flag = PRESSET_SET_CIRCULAR;
            presset_config_set.set_setp = 1;
            presset_config_set_page_ui_process(KeyValue);
            break;
        }
        if (presset_config_set.set_flag == PRESSET_SET_TIME)
        {
            presset_config_set.current_index++;
            presset_config_set.set_flag = PRESSET_SET_VOUT;
        }
        else if (presset_config_set.set_flag != PRESSET_SET_CIRCULAR)
        {
            presset_config_set.set_flag++;
        }
        if (presset_config_set.set_flag == PRESSET_SET_VOUT || presset_config_set.set_flag == PRESSET_SET_IOUT)
        {
            presset_config_set.set_setp = 100;
        }
        else if (presset_config_set.set_flag == PRESSET_SET_TIME || presset_config_set.set_flag == PRESSET_SET_CIRCULAR)
        {
            presset_config_set.set_setp = 1;
        }
        presset_config_set_page_ui_process(KeyValue);
        break;
    default:
        break;
    }
}

/**
 * @brief 开启预设页面处理
 * 
 * @param KeyValue 触发键值
 */
void presset_start_page_process(menu_u8 KeyValue)
{
    extern SC8815_TIM_WorkTypeDef SC8815_TIM_Work[SC8815_TIM_WORK_SIZE];
    current_menu_index = PRESSET_START_PAGE;
    extern SC8815_ConfigTypeDef SC8815_Config;
    switch (KeyValue)
    {
    case LEFT:
        (sub_index.presset_current_index > 0) ? (sub_index.presset_current_index--) : (sub_index.presset_current_index = 0);
        presset_page_ui_process(sub_index.presset_current_index, KeyValue);
        break;
    case RIGHT:
        (sub_index.presset_current_index < SC8815_TIM_WORK_SIZE - 1) ? (sub_index.presset_current_index++) : (sub_index.presset_current_index = SC8815_TIM_WORK_SIZE - 1);
        presset_page_ui_process(sub_index.presset_current_index, KeyValue);
        break;
    case KEY2_LONG:
        main_page_init();
        break;
    case KEY3_SHORT:
        //返回上一级
        main_menu_Init();
        break;
    case KEY4_SHORT:
        SC8815_Config.SC8815_Status = SC8815_TIM_WORK;
        SC8815_Config.sc8815_tim_work_step = 0;
        SC8815_Config.sc8815_tim_work_time = SC8815_TIM_WORK_TIME_FAST;
        memcpy(&presset_config_set.set_circular, &SC8815_TIM_Work[sub_index.presset_current_index].circular, sizeof(uint8_t));
        memcpy(&presset_config_set.set_time, &SC8815_TIM_Work[sub_index.presset_current_index].SC8815_TIM_Work_second, sizeof(uint16_t) * SC8815_TIM_WORK_STEP);
        memcpy(&presset_config_set.set_ibus, &SC8815_TIM_Work[sub_index.presset_current_index].SC8815_IBUS_Limit, sizeof(float) * SC8815_TIM_WORK_STEP);
        memcpy(&presset_config_set.set_vbus, &SC8815_TIM_Work[sub_index.presset_current_index].SC8815_VBUS, sizeof(float) * SC8815_TIM_WORK_STEP);
        presset_running_page_process(0);
        break;
    default:
        break;
    }
}

/**
 * @brief 预设运行页面处理
 * 
 * @param KeyValue 触发键值
 */
void presset_running_page_process(menu_u8 KeyValue)
{
    switch (KeyValue)
    {
    case 0:
    case KEY4_SHORT:
        LCD_Clear();
        current_menu_index = PRESSET_RUNNING_PAGE;
        extern SC8815_ConfigTypeDef SC8815_Config;
        char str[10];
        sprintf(str, "vset:%.2fv", presset_config_set.set_vbus[SC8815_Config.sc8815_tim_work_step] / 1000);
        LCD_ShowString(0, 0, (uint8_t *)str, RED, BLACK, 32, 0);
        memset(str, 0, 10);
        sprintf(str, "iset:%.2fA", presset_config_set.set_ibus[SC8815_Config.sc8815_tim_work_step] / 1000);
        LCD_ShowString(0, 33, (uint8_t *)str, RED, BLACK, 32, 0);
        memset(str, 0, 10);
        sprintf(str, "time:%ds", presset_config_set.set_time[SC8815_Config.sc8815_tim_work_step]);
        LCD_ShowString(0, 66, (uint8_t *)str, RED, BLACK, 32, 0);
        memset(str, 0, 10);
        sprintf(str, "circular:%d", presset_config_set.set_circular);
        LCD_ShowString(0, 99, (uint8_t*)str, RED, BLACK, 16, 0);
        break;
    case KEY2_LONG:
        SC8815_SetOutputVoltage(SC8815_Config.SC8815_VBUS);
        SC8815_SetBusCurrentLimit(SC8815_Config.SC8815_IBUS_Limit);
        current_menu_index = MAIN_PAGE;
        SC8815_Config.SC8815_Status = SC8815_Standby;
        Application_SC8815_Standby();
        SC8815_Config.sc8815_tim_work_lcd_flush = tim_work_lcd_main;
        break;
    default:
        break;
    }
}

/**
 * @brief 蜂鸣器页面处理
 * 
 * @param KeyValue 触发键值
 */
void buzzer_page_process(menu_u8 KeyValue)
{
    
}

/**
 * @brief 温度监控页面处理
 * 
 * @param KeyValue 触发键值
 */
void temp_page_process(menu_u8 KeyValue)
{

}

/**
 * @brief 快充设置页面处理
 * 
 * @param KeyValue 触发键值
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
