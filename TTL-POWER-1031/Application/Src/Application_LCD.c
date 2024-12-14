#include "Application_LCD.h"
#include "Application_ADC.h"
#include "Application_SC8815.h"
#include <string.h>
#include "menu.h"
#include "math.h"

extern Application_Config APP_config;

/**
 * @brief Initializes the main LCD display.
 * 
 * This function is responsible for initializing the LCD display by drawing the necessary lines and shapes
 * to form a grid-like structure, and displays specific text on the LCD. It uses different colors to draw
 * various elements and displays the current settings of the SC8815_Config structure such as voltage and current.
 * 
 * Note: Some lines are uncommented to explain their purpose, but are not executed.
 */
void APP_LCD_main_init(void)
{
    // LCD_DrawPoint(100, 100, LIGHTBLUE);
    // LCD_DrawLine(66, 0, 66, LCD_H, LIGHTBLUE);
    // LCD_DrawLine(0, 0, 0, LCD_H, LIGHTBLUE);
    // LCD_DrawLine(0, 0, LCD_W, 0, LIGHTBLUE);
    // LCD_DrawLine(0, 32, 66, 32, LIGHTBLUE);
    // LCD_DrawLine(0, 67, 66, 67, LIGHTBLUE);
    // LCD_DrawLine(0, 100, 66, 100, LIGHTBLUE);
    // LCD_DrawLine(0, LCD_H - 1, LCD_W, LCD_H - 1, LIGHTBLUE);
    // LCD_DrawLine(LCD_W - 1, 0, LCD_W - 1, LCD_H - 1, LIGHTBLUE);

    LCD_Fill_DMA(66, 0, 67, LCD_H, LIGHTBLUE);  // |
    LCD_Fill_DMA(0, 0, 1, LCD_H, LIGHTBLUE);    // |
    LCD_Fill_DMA(0, 0, LCD_W, 1, LIGHTBLUE);
    LCD_Fill_DMA(0, 32, 66, 33, LIGHTBLUE);
    LCD_Fill_DMA(0, 67, 66, 68, LIGHTBLUE);
    LCD_Fill_DMA(0, 100, 66, 101, LIGHTBLUE);
    LCD_Fill_DMA(0, LCD_H - 1, LCD_W, LCD_H, LIGHTBLUE); // __
    LCD_Fill_DMA(LCD_W - 1, 0, LCD_W, LCD_H, LIGHTBLUE);    // |

    LCD_ShowString(20, 1, "Vset", LIGHTBLUE, BLACK, 16, 0);
    // LCD_ShowString(15, 16, "5.00V", LIGHTBLUE, BLACK, 16, 0);
    LCD_ShowString(20, 34, "Iset", LIGHTBLUE, BLACK, 16, 0);
    // LCD_ShowString(15, 51, "5.00A", LIGHTBLUE, BLACK, 16, 0);
    LCD_ShowString(23, 68, "Vin", LIGHTBLUE, BLACK, 16, 0);
    // LCD_ShowString(15, 84, "12.0V", LIGHTBLUE, BLACK, 16, 0);
    // LCD_ShowString(1, 102, " ON ", BLACK, GREEN, 32, 0);
    // LCD_ShowString(80, 1, "05.10", MAGENTA, BLACK, 48, 0);
    LCD_ShowString(200, 16, "V", MAGENTA, BLACK, 32, 0);
    // LCD_ShowString(80, 44, "04.98", GREEN, BLACK, 48, 0);
    LCD_ShowString(200, 56, "A", GREEN, BLACK, 32, 0);
    // LCD_ShowString(80, 86, "025.4", ORANGE, BLACK, 48, 0);
    LCD_ShowString(200, 100, "W", ORANGE, BLACK, 32, 0);
    // if (HAL_GPIO_ReadPin(SC8815_PSTOP_GPIO_Port, SC8815_PSTOP_Pin) == SET)
    // {
    //     LCD_ShowString(10, 102, "OFF", BLACK, GRAY, 32, 0);
    //     LCD_Fill_DMA(1, 102, 10, LCD_H - 1, GRAY);
    //     LCD_Fill_DMA(56, 102, 66, LCD_H - 1, GRAY);
    // }
    // else {
    //     LCD_ShowString(1, 102, " ON ", BLACK, GREEN, 32, 0);
    // }
    LCD_show_vset();
    LCD_show_iset();
}

static void LCD_show_main_page_status(void)
{
    char str[10] = {0};
    float vbus = App_getVBUS_V();
    // vbus = 10;
    if (vbus >= 0 && vbus < 10) {
        sprintf(str, "0%.2f", vbus);
        LCD_ShowString(80, 1, (const uint8_t*)str, MAGENTA, BLACK, 48, 0);
    } else if (vbus >= 10 && vbus < 40) {
        sprintf(str, "%.2f", vbus);
        LCD_ShowString(80, 1, (const uint8_t*)str, MAGENTA, BLACK, 48, 0);
    } else {
        vbus = 0;
    }

    memset(str, 0, 10);
    float ibus = App_getIBUS_A();
    if (ibus >= 0 && ibus < 10) {
        sprintf(str, "%.3f", ibus);
        LCD_ShowString(80, 44, (const uint8_t*)str, GREEN, BLACK, 48, 0);
    } else {
        ibus = 0;
        sprintf(str, "%.3f", ibus);
        LCD_ShowString(80, 44, (const uint8_t*)str, GREEN, BLACK, 48, 0);
    }

    memset(str, 0, 10);
    float powr = vbus * ibus;
    if (powr >= 0 && powr < 10) {
        sprintf(str, "0%.2f", powr);
        LCD_ShowString(80, 86, (const uint8_t*)str, ORANGE, BLACK, 48, 0);
    } else if (powr >= 10 && powr < 100) {
        sprintf(str, "%.2f", powr);
        LCD_ShowString(80, 86, (const uint8_t*)str, ORANGE, BLACK, 48, 0);
    } else if (powr >= 100 && powr < 105) {
        sprintf(str, "%.1f", powr);
        LCD_ShowString(80, 86, (const uint8_t*)str, ORANGE, BLACK, 48, 0);
    }

    if (HAL_GPIO_ReadPin(SC8815_PSTOP_GPIO_Port, SC8815_PSTOP_Pin) == SET) {
        LCD_ShowString(10, 102, "OFF", BLACK, GRAY, 32, 0);
        LCD_Fill_DMA(1, 102, 10, LCD_H - 1, GRAY);
        LCD_Fill_DMA(56, 102, 66, LCD_H - 1, GRAY);
        LCD_Fill_DMA(220, 2, 236, 18, BLACK);
    } else {
        LCD_ShowString(1, 102, " ON ", BLACK, GREEN, 32, 0);
        if (fabs(vbus - SC8815_Config.SC8815_VBUS) > 0.2) {
            LCD_ShowString(220, 2, "CC", MAGENTA, BLACK, 16, 0);
        } else {
            LCD_ShowString(220, 2, "CV", GREEN, BLACK, 16, 0);
        }
    }
}

/**
 * @brief  Show the main menu interface on the LCD.
 *         This function shows the main menu interface on the LCD, including the
 *         voltage setting, current setting, input voltage, and the status of the
 *         power switch.
 *         This function is called in the main loop.
 */
void APP_LCD_main_show(void)
{
    char str[10] = {0};
    float temp = App_getVBAT_V();
    if (temp >= 0 && temp < 10) {
        sprintf(str, " %.2fV", temp);
    } else if (temp >= 10 && temp < 36) {
        sprintf(str, "%.2fV", temp);
    }
    LCD_ShowString(7, 84, (const uint8_t*)str, LIGHTBLUE, BLACK, 16, 0);

    LCD_show_main_page_status();

    if (SC8815_Config.SC8815_VBUS_Old != SC8815_Config.SC8815_VBUS) {
        LCD_show_vset();
        SC8815_Config.SC8815_VBUS_Old = SC8815_Config.SC8815_VBUS;
    }
    if (SC8815_Config.SC8815_IBUS_Limit_Old != SC8815_Config.SC8815_IBUS_Limit) {
        LCD_show_iset();
        SC8815_Config.SC8815_IBUS_Limit_Old = SC8815_Config.SC8815_IBUS_Limit;
    }
}

void APP_LCD_presset_running_init(void)
{
    LCD_Fill_DMA(66, 0, 67, LCD_H, LIGHTBLUE);  // |
    LCD_Fill_DMA(0, 0, 1, LCD_H, LIGHTBLUE);    // |
    LCD_Fill_DMA(0, 0, LCD_W, 1, LIGHTBLUE);
    LCD_Fill_DMA(0, 33, 66, 34, LIGHTBLUE);
    LCD_Fill_DMA(0, 67, 66, 68, LIGHTBLUE);
    LCD_Fill_DMA(0, 100, 66, 101, LIGHTBLUE);
    LCD_Fill_DMA(0, LCD_H - 1, LCD_W, LCD_H, LIGHTBLUE); // __
    LCD_Fill_DMA(LCD_W - 1, 0, LCD_W, LCD_H, LIGHTBLUE);    // |
    LCD_ShowString(10, 1, "Runing", LIGHTBLUE, BLACK, 16, 0);
    // LCD_ShowString(15, 16, "5.00V", LIGHTBLUE, BLACK, 16, 0);
    LCD_ShowString(20, 34, "Loop", LIGHTBLUE, BLACK, 16, 0);
    // LCD_ShowString(15, 51, "5.00A", LIGHTBLUE, BLACK, 16, 0);
    LCD_ShowString(23, 68, "Key", LIGHTBLUE, BLACK, 16, 0);
    LCD_ShowString(20, 84, "Lock", LIGHTBLUE, BLACK, 16, 0);
    // LCD_ShowString(1, 102, " ON ", BLACK, GREEN, 32, 0);
    // LCD_ShowString(80, 1, "05.10", MAGENTA, BLACK, 48, 0);
    LCD_ShowString(200, 16, "V", MAGENTA, BLACK, 32, 0);
    // LCD_ShowString(80, 44, "04.98", GREEN, BLACK, 48, 0);
    LCD_ShowString(200, 56, "A", GREEN, BLACK, 32, 0);
    // LCD_ShowString(80, 86, "025.4", ORANGE, BLACK, 48, 0);
    LCD_ShowString(200, 100, "W", ORANGE, BLACK, 32, 0);
}

void APP_LCD_presset_running_show(void)
{
    char str[10];
    extern Key_Index sub_index;
    extern presset_config_set_typeDef presset_config_set;
    sprintf(str, ":%d", sub_index.presset_current_index);
    LCD_ShowChinese(9, 17, "Ô¤Éè", LIGHTBLUE, BLACK, 16, 0);
    LCD_ShowString(41, 17, (const uint8_t*)str, LIGHTBLUE, BLACK, 16, 0);

    memset(str, 0, 10);
    sprintf(str, "%d ", presset_config_set.set_circular);
    // LCD_ShowIntNum(15, 51, presset_config_set.set_circular, LIGHTBLUE, BLACK, 16, 0);
    LCD_ShowString(25, 51, (const uint8_t*)str, LIGHTBLUE, BLACK, 16, 0);
    LCD_show_main_page_status();
}

inline void presset_config_set_page_show(void)
{
    char str[10];
    extern presset_config_set_typeDef presset_config_set;
    if (presset_config_set.set_flag == PRESSET_SET_VOUT)
    {
        sprintf(str, "vset:%.2fV ", presset_config_set.set_vbus[presset_config_set.current_index] / 1000);
        LCD_ShowString(0, 0, (uint8_t *)str, BLACK, LIGHTBLUE, 32, 0);
    }
    else {
        sprintf(str, "vset:%.2fV ", presset_config_set.set_vbus[presset_config_set.current_index] / 1000);
        LCD_ShowString(0, 0, (uint8_t *)str, LIGHTBLUE, BLACK, 32, 0);
    }
    

    memset(str, 0, 10);
    if (presset_config_set.set_flag == PRESSET_SET_IOUT)
    {
        sprintf(str, "iset:%.2fA", presset_config_set.set_ibus[presset_config_set.current_index] / 1000);
        LCD_ShowString(0, 33, (uint8_t *)str, BLACK, LIGHTBLUE, 32, 0);
    }
    else {
        sprintf(str, "iset:%.2fA", presset_config_set.set_ibus[presset_config_set.current_index] / 1000);
        LCD_ShowString(0, 33, (uint8_t *)str, LIGHTBLUE, BLACK, 32, 0);
    }

    memset(str, 0, 10);
    if (presset_config_set.set_flag == PRESSET_SET_TIME)
    {
        sprintf(str, "time:%ds", presset_config_set.set_time[presset_config_set.current_index]);
        LCD_ShowString(0, 66, (uint8_t *)str, BLACK, LIGHTBLUE, 32, 0);
    }
    else {
        sprintf(str, "time:%ds", presset_config_set.set_time[presset_config_set.current_index]);
        LCD_ShowString(0, 66, (uint8_t *)str, LIGHTBLUE, BLACK, 32, 0);
    }

    memset(str, 0, 10);
    if (presset_config_set.set_flag == PRESSET_SET_CIRCULAR)
    {
        sprintf(str, "loop:%d", presset_config_set.set_circular);
        LCD_ShowString(0, 99, (uint8_t *)str, BLACK, LIGHTBLUE, 16, 0);
    }
    else {
        sprintf(str, "loop:%d", presset_config_set.set_circular);
        LCD_ShowString(0, 99, (uint8_t *)str, LIGHTBLUE, BLACK, 16, 0);
    }

    memset(str, 0, 10);
    sprintf(str, "step:%d/29", presset_config_set.current_index);
    LCD_ShowString(100, 99, (uint8_t *)str, LIGHTBLUE, BLACK, 16, 0);
}

void LCD_show_vset(void)
{
    char str[10] = {0};
    float temp = SC8815_Config.SC8815_VBUS / 1000;
    if (temp >= 0 && temp < 10) {
        sprintf(str, " %.2fV", temp);
    } else if (temp >= 10 && temp < 100) {
        sprintf(str, "%.2fV", temp);
    }
    LCD_ShowString(7, 16, (const uint8_t*)str, LIGHTBLUE, BLACK, 16, 0);
}

void LCD_show_iset(void)
{
    char str[10] = {0};
    float temp = SC8815_Config.SC8815_IBUS_Limit / 1000;
    sprintf(str, "%.2fA", temp);
    if (temp >= 0 && temp < 10) {
        LCD_ShowString(15, 51, (const uint8_t*)str, LIGHTBLUE, BLACK, 16, 0);
    }
}
