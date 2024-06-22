#include "menu.h"
#include "UART_Debug.h"
#include "Application_SC8815.h"
#include "Application.h"

Key_Index sub_index;
menu_i32 current_menu_index;

// 菜单操作表定义
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

// 跳转到表所对应的页面
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

// 菜单选择项
void Menu_Select_Item(menu_u8 KeyValue)
{
    JUMP_Table(current_menu_index, KeyValue);
}

void main_page_process(menu_u8 KeyValue)
{
    switch (KeyValue)
    {
    case KEY1_SHORT:
        current_menu_index = IOUT_PAGE;
        iout_page_ui_process(KeyValue);
        break;   
    case KEY2_LONG:
        main_menu_Init();
        break;
    case KEY2_SHORT:
        current_menu_index = VOUT_PAGE;
        vout_page_ui_process(KeyValue);
        break;
    case KEY3_SHORT:
        if (HAL_GPIO_ReadPin(SC8815_PSTOP_GPIO_Port, SC8815_PSTOP_Pin) == RESET)
        {
            SC8815_Config.SC8815_Status = SC8815_Standby;
            Application_SC8815_Standby();
        }
        else {
            SC8815_Config.SC8815_Status = SC8815_LoadStart;
        }
        break;
    default:
        break;
    }
}

// 主菜单页面处理
void main_menu_page_process(menu_u8 KeyValue)
{
    current_menu_index = MAIN_MENU_PAGE;
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

void vout_page_process(menu_u8 KeyValue)
{
    current_menu_index = VOUT_PAGE;
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

void iout_page_process(menu_u8 KeyValue)
{
    current_menu_index = IOUT_PAGE;
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

void presset_page_process(menu_u8 KeyValue)
{
}

void buzzer_page_process(menu_u8 KeyValue)
{
    
}

void temp_page_process(menu_u8 KeyValue)
{

}

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
