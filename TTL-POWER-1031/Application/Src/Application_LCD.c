#include "Application_LCD.h"
#include "Application_ADC.h"
#include "Application_SC8815.h"
#include <string.h>
#include "menu.h"
#include "math.h"

extern Application_Config APP_config;
extern const unsigned char gImage_left1[512];
extern const unsigned char gImage_left2[512];
extern const unsigned char gImage_right1[512];
extern const unsigned char gImage_right2[512];
extern const unsigned char gImage_smallleft1[512];
extern const unsigned char gImage_smallleft2[512];
extern const unsigned char gImage_smallright1[512];
extern const unsigned char gImage_smallright2[512];

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
    // LCD_Fill_DMA(66, 0, 72, LCD_H, WHITE);
    LCD_Fill_DMA(80, 0, LCD_W, LCD_H, MAIN_PAGE_COLOR);
    LCD_ShowPicture(80, 0, 16, 16, gImage_left1);
    LCD_ShowPicture(80, LCD_H - 16, 16, 16, gImage_left2);
    LCD_ShowPicture(LCD_W - 16, 0, 16, 16, gImage_right1);
    LCD_ShowPicture(LCD_W - 16, LCD_H - 16, 16, 16, gImage_right2);
    LCD_Fill_DMA(0, 0, 78, 32, MAIN_PAGE_COLOR);
    LCD_ShowPicture(0, 0, 16, 16, gImage_smallleft1);
    LCD_ShowPicture(0, 16, 16, 16, gImage_smallleft2);
    LCD_ShowPicture(62, 0, 16, 16, gImage_smallright1);
    LCD_ShowPicture(62, 16, 16, 16, gImage_smallright2);
    LCD_Fill_DMA(0, 34, 78, 66, MAIN_PAGE_COLOR);
    LCD_ShowPicture(0, 34, 16, 16, gImage_smallleft1);
    LCD_ShowPicture(0, 50, 16, 16, gImage_smallleft2);
    LCD_ShowPicture(62, 34, 16, 16, gImage_smallright1);
    LCD_ShowPicture(62, 50, 16, 16, gImage_smallright2);
    LCD_Fill_DMA(0, 68, 78, 100, MAIN_PAGE_COLOR);
    LCD_ShowPicture(0, 68, 16, 16, gImage_smallleft1);
    LCD_ShowPicture(0, 84, 16, 16, gImage_smallleft2);
    LCD_ShowPicture(62, 68, 16, 16, gImage_smallright1);
    LCD_ShowPicture(62, 84, 16, 16, gImage_smallright2);
    LCD_ShowString(25, 0, "Vset", WHITE, MAIN_PAGE_COLOR, 16, 0);
    LCD_ShowString(25, 34, "Iset", WHITE, MAIN_PAGE_COLOR, 16, 0);
    LCD_ShowString(214, 16, "V", GREEN, MAIN_PAGE_COLOR, 32, 0);
    LCD_ShowString(214, 56, "A", MAGENTA, MAIN_PAGE_COLOR, 32, 0);
    LCD_ShowString(214, 100, "W", ORANGE, MAIN_PAGE_COLOR, 32, 0);
    LCD_show_vset();
    LCD_show_iset();

    LCD_ShowString(28, 68, "Vin", WHITE, MAIN_PAGE_COLOR, 16, 0);
    float temp = App_getVBAT_V();
    char str[10] = { 0 };
    if (temp >= 0 && temp < 9.990) {
        sprintf(str, " %.2fV", temp);
    } else if (temp >= 9.990 && temp < 36) {
        sprintf(str, "%.2fV", temp);
    }
    LCD_ShowString(12, 84, (const uint8_t*)str, WHITE, MAIN_PAGE_COLOR, 16, 0);
}

static void LCD_show_main_page_status(void)
{
    char str[16] = { 0 };
    float vbus = App_getVBUS_V();
    extern presset_config_set_typeDef presset_config_set;
    // vbus = 10;
    if (vbus >= 0 && vbus < 9.990) {
        sprintf(str, "0%.2f", vbus);
        if (strlen((char*)str) == 5 && (char*)str[5] == '\0') {
            LCD_ShowString(92, 1, (const uint8_t*)str, GREEN, MAIN_PAGE_COLOR, 48, 0);
        }
    } else if (vbus >= 9.990 && vbus < 40) {
        sprintf(str, "%.2f", vbus);
        if (strlen((char*)str) == 5 && (char*)str[5] == '\0') {
            LCD_ShowString(92, 1, (const uint8_t*)str, GREEN, MAIN_PAGE_COLOR, 48, 0);
        }
    } else {
        vbus = 0;
    }

    memset(str, 0, sizeof(str));
    float ibus = App_getIBUS_A();
    if (ibus >= 0 && ibus < 9.990) {
        sprintf(str, "%.3f", ibus);
        if (strlen((char*)str) == 5 && (char*)str[5] == '\0') {
            LCD_ShowString(92, 44, (const uint8_t*)str, MAGENTA, MAIN_PAGE_COLOR, 48, 0);
        }
    } else {
        ibus = 0;
        sprintf(str, "%.3f", ibus);
        if (strlen((char*)str) == 5 && (char*)str[5] == '\0') {
            LCD_ShowString(92, 44, (const uint8_t*)str, MAGENTA, MAIN_PAGE_COLOR, 48, 0);
        }
    }

    memset(str, 0, sizeof(str));
    float powr = vbus * ibus;
    if (powr >= 0 && powr < 9.990) {
        sprintf(str, "0%.2f", powr);
        if (strlen((char*)str) == 5 && (char*)str[5] == '\0') {
            LCD_ShowString(92, 86, (const uint8_t*)str, ORANGE, MAIN_PAGE_COLOR, 48, 0);
        }
    } else if (powr >= 9.990 && powr < 99.90) {
        sprintf(str, "%.2f", powr);
        if (strlen((char*)str) == 5 && (char*)str[5] == '\0') {
            LCD_ShowString(92, 86, (const uint8_t*)str, ORANGE, MAIN_PAGE_COLOR, 48, 0);
        }
    } else if (powr >= 99.90 && powr < get_sc8815_power()) {
        sprintf(str, "%.1f", powr);
        if (strlen((char*)str) == 5 && (char*)str[5] == '\0') {
            LCD_ShowString(92, 86, (const uint8_t*)str, ORANGE, MAIN_PAGE_COLOR, 48, 0);
        }
    }

    if (HAL_GPIO_ReadPin(SC8815_PSTOP_GPIO_Port, SC8815_PSTOP_Pin) == SET) {
        // LCD_Fill_DMA(0, 102, 78, LCD_H - 1, MAIN_PAGE_COLOR);
        // LCD_Fill_DMA(0, 118, 15, LCD_H - 17, MAIN_PAGE_COLOR);
        // LCD_Fill_DMA(63, 118, 78, LCD_H - 17, MAIN_PAGE_COLOR);
        LCD_ShowPicture(0, 102, 16, 16, gImage_smallleft1);
        LCD_ShowPicture(0, 118, 16, 16, gImage_smallleft2);
        LCD_ShowPicture(62, 102, 16, 16, gImage_smallright1);
        LCD_ShowPicture(62, 118, 16, 16, gImage_smallright2);
        LCD_ShowString(15, 102, "OFF", WHITE, MAIN_PAGE_COLOR, 32, 0);
    } else if (HAL_GPIO_ReadPin(SC8815_PSTOP_GPIO_Port, SC8815_PSTOP_Pin) == RESET) {
        if (SC8815_Config.SC8815_Status == SC8815_TIM_WORK) {
            vbus = presset_config_set.set_vbus[SC8815_Config.sc8815_tim_work_step] - vbus * 1000;
        } else {
            vbus = SC8815_Config.SC8815_VBUS - vbus * 1000;
        }
        if (vbus >= 200 && HAL_GetTick() - SC8815_Config.VOUT_Open_Time > 500) {
            // LCD_Fill_DMA(0, 102, 78, LCD_H - 1, MAGENTA);
            LCD_Fill_DMA(16, 102, 25, LCD_H - 1, MAGENTA);
            LCD_Fill_DMA(56, 102, 62, LCD_H - 1, MAGENTA);
            LCD_show_on_off(0, 102, 16, 16, gImage_smallleft1, MAGENTA);
            LCD_show_on_off(0, 118, 16, 16, gImage_smallleft2, MAGENTA);
            LCD_show_on_off(62, 102, 16, 16, gImage_smallright1, MAGENTA);
            LCD_show_on_off(62, 118, 16, 16, gImage_smallright2, MAGENTA);
            LCD_ShowString(24, 102, "CC", MAIN_PAGE_COLOR, MAGENTA, 32, 0);
        } else {
            // LCD_Fill_DMA(0, 102, 78, LCD_H - 1, GREEN);
            LCD_Fill_DMA(16, 102, 25, LCD_H - 1, GREEN);
            LCD_Fill_DMA(56, 102, 62, LCD_H - 1, GREEN);
            LCD_show_on_off(0, 102, 16, 16, gImage_smallleft1, GREEN);
            LCD_show_on_off(0, 118, 16, 16, gImage_smallleft2, GREEN);
            LCD_show_on_off(62, 102, 16, 16, gImage_smallright1, GREEN);
            LCD_show_on_off(62, 118, 16, 16, gImage_smallright2, GREEN);
            LCD_ShowString(24, 102, "CV", MAIN_PAGE_COLOR, GREEN, 32, 0);
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
    static uint32_t vbat_temp_time = 0;
    static uint8_t vbat_temp_flag = 0;
    char str[10] = { 0 };
    float temp = App_getVBAT_V();
    if (HAL_GetTick() - vbat_temp_time > 3000) {
        if (vbat_temp_flag == 0) {
            LCD_Fill_DMA(25, 68, 60, 84, MAIN_PAGE_COLOR);
            LCD_ShowString(28, 68, "Vin", WHITE, MAIN_PAGE_COLOR, 16, 0);
            LCD_Fill_DMA(12, 84, 60, 100, MAIN_PAGE_COLOR);
            if (temp >= 0 && temp < 9.990) {
                sprintf(str, " %.2fV", temp);
            } else if (temp >= 9.990 && temp < 36) {
                sprintf(str, "%.2fV", temp);
            }
            LCD_ShowString(12, 84, (const uint8_t*)str, WHITE, MAIN_PAGE_COLOR, 16, 0);
            vbat_temp_flag = 1;
        } else if (vbat_temp_flag == 1) {
            temp = App_getTemp();
            LCD_ShowString(25, 68, "Temp", WHITE, MAIN_PAGE_COLOR, 16, 0);
            LCD_Fill_DMA(12, 84, 60, 100, MAIN_PAGE_COLOR);
            if (temp >= 0 && temp < 9.990) {
                sprintf(str, " %.1fC ", temp);
                LCD_ShowString(12, 84, (const uint8_t*)str, WHITE, MAIN_PAGE_COLOR, 16, 0);
            } else if (temp >= 9.990 && temp < 99.90) {
                sprintf(str, "%.1fC ", temp);
                LCD_ShowString(20, 84, (const uint8_t*)str, WHITE, MAIN_PAGE_COLOR, 16, 0);
            }
            vbat_temp_flag = 0;
        }
        vbat_temp_time = HAL_GetTick();
    }

    LCD_show_main_page_status();
    if (SC8815_Config.SC8815_VBUS != SC8815_Config.SC8815_VBUS_Old) {
        LCD_show_vset();
        SC8815_Config.SC8815_VBUS_Old = SC8815_Config.SC8815_VBUS;
    }
    if (SC8815_Config.SC8815_IBUS_Limit != SC8815_Config.SC8815_IBUS_Limit_Old) {
        LCD_show_iset();
        SC8815_Config.SC8815_IBUS_Limit_Old = SC8815_Config.SC8815_IBUS_Limit;
    }
}

void APP_LCD_presset_running_init(void)
{
    LCD_Fill_DMA(80, 0, LCD_W, LCD_H, MAIN_PAGE_COLOR);
    LCD_ShowPicture(80, 0, 16, 16, gImage_left1);
    LCD_ShowPicture(80, LCD_H - 16, 16, 16, gImage_left2);
    LCD_ShowPicture(LCD_W - 16, 0, 16, 16, gImage_right1);
    LCD_ShowPicture(LCD_W - 16, LCD_H - 16, 16, 16, gImage_right2);
    LCD_Fill_DMA(0, 0, 78, 32, MAIN_PAGE_COLOR);
    LCD_ShowPicture(0, 0, 16, 16, gImage_smallleft1);
    LCD_ShowPicture(0, 16, 16, 16, gImage_smallleft2);
    LCD_ShowPicture(62, 0, 16, 16, gImage_smallright1);
    LCD_ShowPicture(62, 16, 16, 16, gImage_smallright2);
    LCD_Fill_DMA(0, 34, 78, 66, MAIN_PAGE_COLOR);
    LCD_ShowPicture(0, 34, 16, 16, gImage_smallleft1);
    LCD_ShowPicture(0, 50, 16, 16, gImage_smallleft2);
    LCD_ShowPicture(62, 34, 16, 16, gImage_smallright1);
    LCD_ShowPicture(62, 50, 16, 16, gImage_smallright2);
    LCD_Fill_DMA(0, 68, 78, 100, MAIN_PAGE_COLOR);
    LCD_ShowPicture(0, 68, 16, 16, gImage_smallleft1);
    LCD_ShowPicture(0, 84, 16, 16, gImage_smallleft2);
    LCD_ShowPicture(62, 68, 16, 16, gImage_smallright1);
    LCD_ShowPicture(62, 84, 16, 16, gImage_smallright2);
    LCD_ShowString(214, 16, "V", GREEN, MAIN_PAGE_COLOR, 32, 0);
    LCD_ShowString(214, 56, "A", MAGENTA, MAIN_PAGE_COLOR, 32, 0);
    LCD_ShowString(214, 100, "W", ORANGE, MAIN_PAGE_COLOR, 32, 0);
    LCD_ShowString(15, 0, "Runing", WHITE, MAIN_PAGE_COLOR, 16, 0);
    LCD_ShowString(25, 34, "Loop", WHITE, MAIN_PAGE_COLOR, 16, 0);
    LCD_ShowString(28, 68, "Key", WHITE, MAIN_PAGE_COLOR, 16, 0);
    LCD_ShowString(25, 84, "Lock", WHITE, MAIN_PAGE_COLOR, 16, 0);
}

void APP_LCD_presset_running_show(void)
{
    char str[10];
    extern Key_Index sub_index;
    extern presset_config_set_typeDef presset_config_set;
    static uint32_t key_temp_time = 0;
    static uint8_t key_temp_flag = 0;
    float temp = 0;
    sprintf(str, ":%d", sub_index.presset_current_index);
#ifdef ENABLE_EN_FONT
    LCD_ShowString(14, 16, "Preset", WHITE, MAIN_PAGE_COLOR, 16, 0);
    LCD_ShowString(96, 16, (const uint8_t*)str, WHITE, MAIN_PAGE_COLOR, 16, 0);
#else
    LCD_ShowChinese(14, 16, "预设", WHITE, MAIN_PAGE_COLOR, 16, 0);
    LCD_ShowString(46, 16, (const uint8_t*)str, WHITE, MAIN_PAGE_COLOR, 16, 0);
#endif

    memset(str, 0, 10);
    sprintf(str, "%d ", presset_config_set.set_circular);
    // LCD_ShowIntNum(15, 51, presset_config_set.set_circular, WHITE, BLACK, 16, 0);
    LCD_ShowString(27, 50, (const uint8_t*)str, WHITE, MAIN_PAGE_COLOR, 16, 0);
    if (HAL_GetTick() - key_temp_time > 3000) {
        if (key_temp_flag == 0) {
            LCD_Fill_DMA(25, 68, 60, 84, MAIN_PAGE_COLOR);
            LCD_ShowString(28, 68, "Key", WHITE, MAIN_PAGE_COLOR, 16, 0);
            LCD_Fill_DMA(12, 84, 60, 100, MAIN_PAGE_COLOR);
            LCD_ShowString(25, 84, "Lock", WHITE, MAIN_PAGE_COLOR, 16, 0);
            key_temp_flag = 1;
        } else if (key_temp_flag == 1) {
            temp = App_getTemp();
            LCD_ShowString(25, 68, "Temp", WHITE, MAIN_PAGE_COLOR, 16, 0);
            LCD_Fill_DMA(12, 84, 60, 100, MAIN_PAGE_COLOR);
            if (temp >= 0 && temp < 9.990) {
                sprintf(str, " %.1fC ", temp);
                LCD_ShowString(12, 84, (const uint8_t*)str, WHITE, MAIN_PAGE_COLOR, 16, 0);
            } else if (temp >= 9.990 && temp < 99.90) {
                sprintf(str, "%.1fC ", temp);
                LCD_ShowString(20, 84, (const uint8_t*)str, WHITE, MAIN_PAGE_COLOR, 16, 0);
            }
            key_temp_flag = 0;
        }
        key_temp_time = HAL_GetTick();
    }
    LCD_show_main_page_status();
}

void presset_config_set_page_show(void)
{
    char str[10];
    int number = 0, length = 0, digits[5] = { 0 };
    extern presset_config_set_typeDef presset_config_set;
    if (presset_config_set.set_flag == PRESSET_SET_VOUT) {
        LCD_Fill_DMA(80, 0, LCD_W, 32, RED);
        number = presset_config_set.set_vbus[presset_config_set.current_index];
        if (number >= 0 && number < 10000) {
            sprintf(str, "Vset:0%.2fV ", presset_config_set.set_vbus[presset_config_set.current_index] / 1000);
        } else {
            sprintf(str, "Vset:%.2fV ", presset_config_set.set_vbus[presset_config_set.current_index] / 1000);
        }
        LCD_ShowString(0, 0, (uint8_t*)str, WHITE, RED, 32, 0);
        if (presset_config_set.set_vbus[presset_config_set.current_index] >= presset_config_set.set_setp) {
            // Take the tens, thousands, and hundreds values and store them in variate digits
            while (number > 0) {
                number /= 10;
                length++;
            }
            number = presset_config_set.set_vbus[presset_config_set.current_index];
            for (int i = length - 1; i >= 0; i--) {
                digits[i] = number / (int)pow(10, i);
                number %= (int)pow(10, i);
            }
        }
        if (presset_config_set.set_setp == 100) {
            LCD_ShowIntNum(80 + 16 * 3, 0, digits[2], 1, RED, WHITE, 32);
        } else if (presset_config_set.set_setp == 1000) {
            LCD_ShowIntNum(80 + 16 * 1, 0, digits[3], 1, RED, WHITE, 32);
        } else if (presset_config_set.set_setp == 10000) {
            LCD_ShowIntNum(80, 0, digits[4], 1, RED, WHITE, 32);
        }
    } else {
        LCD_Fill_DMA(80, 0, LCD_W, 32, BLACK);
        number = presset_config_set.set_vbus[presset_config_set.current_index];
        if (number >= 0 && number < 10000) {
            sprintf(str, "Vset:0%.2fV ", presset_config_set.set_vbus[presset_config_set.current_index] / 1000);
        } else {
            sprintf(str, "Vset:%.2fV ", presset_config_set.set_vbus[presset_config_set.current_index] / 1000);
        }
        LCD_ShowString(0, 0, (uint8_t*)str, WHITE, BLACK, 32, 0);
    }

    memset(str, 0, 10);
    if (presset_config_set.set_flag == PRESSET_SET_IOUT) {
        LCD_Fill_DMA(80, 34, LCD_W, 65, RED);
        sprintf(str, "Iset:%.2fA", presset_config_set.set_ibus[presset_config_set.current_index] / 1000);
        LCD_ShowString(0, 34, (uint8_t*)str, WHITE, RED, 32, 0);
        if (presset_config_set.set_ibus[presset_config_set.current_index] >= presset_config_set.set_setp) {
            // Take the tens, thousands, and hundreds values and store them in variate digits
            number = presset_config_set.set_ibus[presset_config_set.current_index];
            while (number > 0) {
                number /= 10;
                length++;
            }
            number = presset_config_set.set_ibus[presset_config_set.current_index];
            for (int i = length - 1; i >= 0; i--) {
                digits[i] = number / (int)pow(10, i);
                number %= (int)pow(10, i);
            }
        }
        if (presset_config_set.set_setp == 100) {
            LCD_ShowIntNum(80 + 16 * 2, 34, digits[2], 1, RED, WHITE, 32);
        } else if (presset_config_set.set_setp == 1000) {
            LCD_ShowIntNum(80, 34, digits[3], 1, RED, WHITE, 32);
        }
    } else {
        LCD_Fill_DMA(80, 34, LCD_W, 65, BLACK);
        sprintf(str, "Iset:%.2fA", presset_config_set.set_ibus[presset_config_set.current_index] / 1000);
        LCD_ShowString(0, 34, (uint8_t*)str, WHITE, BLACK, 32, 0);
    }

    memset(str, 0, 10);
    if (presset_config_set.set_flag == PRESSET_SET_TIME) {
        LCD_Fill_DMA(80, 68, LCD_W, 98, RED);
        sprintf(str, "Time:%ds", presset_config_set.set_time[presset_config_set.current_index]);
        LCD_ShowString(0, 68, (uint8_t*)str, WHITE, RED, 32, 0);
        if (presset_config_set.set_time[presset_config_set.current_index] >= presset_config_set.set_setp) {
            // Take the tens, thousands, and hundreds values and store them in variate digits
            number = presset_config_set.set_time[presset_config_set.current_index];
            while (number > 0) {
                number /= 10;
                length++;
            }
            number = presset_config_set.set_time[presset_config_set.current_index];
            for (int i = length - 1; i >= 0; i--) {
                digits[i] = number / (int)pow(10, i);
                number %= (int)pow(10, i);
            }
        }
        number = presset_config_set.set_time[presset_config_set.current_index];
        if (presset_config_set.set_setp == 1) {
            if (number >= 10000) {
                LCD_ShowIntNum(80 + 16 * 4, 68, digits[0], 1, RED, WHITE, 32);
            } else if (number >= 1000) {
                LCD_ShowIntNum(80 + 16 * 3, 68, digits[0], 1, RED, WHITE, 32);
            } else if (number >= 100) {
                LCD_ShowIntNum(80 + 16 * 2, 68, digits[0], 1, RED, WHITE, 32);
            } else if (number >= 10) {
                LCD_ShowIntNum(80 + 16, 68, digits[0], 1, RED, WHITE, 32);
            } else {
                LCD_ShowIntNum(80, 68, digits[0], 1, RED, WHITE, 32);
            }
        } else if (presset_config_set.set_setp == 10) {
            if (number >= 10000) {
                LCD_ShowIntNum(80 + 16 * 3, 68, digits[1], 1, RED, WHITE, 32);
            } else if (number >= 1000) {
                LCD_ShowIntNum(80 + 16 * 2, 68, digits[1], 1, RED, WHITE, 32);
            } else if (number >= 100) {
                LCD_ShowIntNum(80 + 16, 68, digits[1], 1, RED, WHITE, 32);
            } else if (number >= 10) {
                LCD_ShowIntNum(80, 68, digits[1], 1, RED, WHITE, 32);
            } else {
                sprintf(str, "Time:0%ds", presset_config_set.set_time[presset_config_set.current_index]);
                LCD_ShowString(0, 68, (uint8_t*)str, WHITE, RED, 32, 0);
                LCD_ShowIntNum(80, 68, digits[1], 1, RED, WHITE, 32);
            }
        } else if (presset_config_set.set_setp == 100) {
            if (number >= 10000) {
                LCD_ShowIntNum(80 + 16 * 2, 68, digits[2], 1, RED, WHITE, 32);
            } else if (number >= 1000) {
                LCD_ShowIntNum(80 + 16 * 1, 68, digits[2], 1, RED, WHITE, 32);
            } else if (number >= 100) {
                LCD_ShowIntNum(80, 68, digits[2], 1, RED, WHITE, 32);
            } else if (number >= 10) {
                sprintf(str, "Time:0%ds", presset_config_set.set_time[presset_config_set.current_index]);
                LCD_ShowString(0, 68, (uint8_t*)str, WHITE, RED, 32, 0);
                LCD_ShowIntNum(80, 68, digits[2], 1, RED, WHITE, 32);
            } else {
                sprintf(str, "Time:00%ds", presset_config_set.set_time[presset_config_set.current_index]);
                LCD_ShowString(0, 68, (uint8_t*)str, WHITE, RED, 32, 0);
                LCD_ShowIntNum(80, 68, digits[2], 1, RED, WHITE, 32);
            }
        } else if (presset_config_set.set_setp == 1000) {
            if (number >= 10000) {
                LCD_ShowIntNum(80 + 16, 68, digits[3], 1, RED, WHITE, 32);
            } else if (number >= 1000) {
                LCD_ShowIntNum(80, 68, digits[3], 1, RED, WHITE, 32);
            } else if (number >= 100) {
                sprintf(str, "Time:0%ds", presset_config_set.set_time[presset_config_set.current_index]);
                LCD_ShowString(0, 68, (uint8_t*)str, WHITE, RED, 32, 0);
                LCD_ShowIntNum(80, 68, digits[3], 1, RED, WHITE, 32);
            } else if (number >= 10) {
                sprintf(str, "Time:00%ds", presset_config_set.set_time[presset_config_set.current_index]);
                LCD_ShowString(0, 68, (uint8_t*)str, WHITE, RED, 32, 0);
                LCD_ShowIntNum(80, 68, digits[3], 1, RED, WHITE, 32);
            } else {
                sprintf(str, "Time:000%ds", presset_config_set.set_time[presset_config_set.current_index]);
                LCD_ShowString(0, 68, (uint8_t*)str, WHITE, RED, 32, 0);
                LCD_ShowIntNum(80, 68, digits[3], 1, RED, WHITE, 32);
            }
        }
    } else {
        LCD_Fill_DMA(80, 68, LCD_W, 100, BLACK);
        sprintf(str, "Time:%ds", presset_config_set.set_time[presset_config_set.current_index]);
        LCD_ShowString(0, 68, (uint8_t*)str, WHITE, BLACK, 32, 0);
    }

    memset(str, 0, 10);
    if (presset_config_set.set_flag == PRESSET_SET_CIRCULAR) {
        LCD_Fill_DMA(80, 102, LCD_W, 134, RED);
        sprintf(str, "Loop:%d", presset_config_set.set_circular);
        LCD_ShowString(0, 102, (uint8_t*)str, WHITE, RED, 32, 0);
        if (presset_config_set.set_circular >= presset_config_set.set_setp) {
            // Take the tens, thousands, and hundreds values and store them in variate digits
            number = presset_config_set.set_circular;
            while (number > 0) {
                number /= 10;
                length++;
            }
            number = presset_config_set.set_circular;
            for (int i = length - 1; i >= 0; i--) {
                digits[i] = number / (int)pow(10, i);
                number %= (int)pow(10, i);
            }
        }
        number = presset_config_set.set_circular;
        if (presset_config_set.set_setp == 1) {
            if (number >= 10000) {
                LCD_ShowIntNum(80 + 16 * 4, 102, digits[0], 1, RED, WHITE, 32);
            } else if (number >= 1000) {
                LCD_ShowIntNum(80 + 16 * 3, 102, digits[0], 1, RED, WHITE, 32);
            } else if (number >= 100) {
                LCD_ShowIntNum(80 + 16 * 2, 102, digits[0], 1, RED, WHITE, 32);
            } else if (number >= 10) {
                LCD_ShowIntNum(80 + 16, 102, digits[0], 1, RED, WHITE, 32);
            } else {
                LCD_ShowIntNum(80, 102, digits[0], 1, RED, WHITE, 32);
            }
        } else if (presset_config_set.set_setp == 10) {
            if (number >= 10000) {
                LCD_ShowIntNum(80 + 16 * 3, 102, digits[1], 1, RED, WHITE, 32);
            } else if (number >= 1000) {
                LCD_ShowIntNum(80 + 16 * 2, 102, digits[1], 1, RED, WHITE, 32);
            } else if (number >= 100) {
                LCD_ShowIntNum(80 + 16, 102, digits[1], 1, RED, WHITE, 32);
            } else if (number >= 10) {
                LCD_ShowIntNum(80, 102, digits[1], 1, RED, WHITE, 32);
            } else {
                sprintf(str, "Loop:0%d", presset_config_set.set_circular);
                LCD_ShowString(0, 102, (uint8_t*)str, WHITE, RED, 32, 0);
                LCD_ShowIntNum(80, 102, digits[1], 1, RED, WHITE, 32);
            }
        } else if (presset_config_set.set_setp == 100) {
            if (number >= 10000) {
                LCD_ShowIntNum(80 + 16 * 2, 102, digits[2], 1, RED, WHITE, 32);
            } else if (number >= 1000) {
                LCD_ShowIntNum(80 + 16 * 1, 102, digits[2], 1, RED, WHITE, 32);
            } else if (number >= 100) {
                LCD_ShowIntNum(80, 102, digits[2], 1, RED, WHITE, 32);
            } else if (number >= 10) {
                sprintf(str, "Loop:0%d", presset_config_set.set_circular);
                LCD_ShowString(0, 102, (uint8_t*)str, WHITE, RED, 32, 0);
                LCD_ShowIntNum(80, 102, digits[2], 1, RED, WHITE, 32);
            } else {
                sprintf(str, "Loop:00%d", presset_config_set.set_circular);
                LCD_ShowString(0, 102, (uint8_t*)str, WHITE, RED, 32, 0);
                LCD_ShowIntNum(80, 102, digits[2], 1, RED, WHITE, 32);
            }
        } else if (presset_config_set.set_setp == 1000) {
            if (number >= 10000) {
                LCD_ShowIntNum(80 + 16, 102, digits[3], 1, RED, WHITE, 32);
            } else if (number >= 1000) {
                LCD_ShowIntNum(80, 102, digits[3], 1, RED, WHITE, 32);
            } else if (number >= 100) {
                sprintf(str, "Loop:0%d", presset_config_set.set_circular);
                LCD_ShowString(0, 102, (uint8_t*)str, WHITE, RED, 32, 0);
                LCD_ShowIntNum(80, 102, digits[3], 1, RED, WHITE, 32);
            } else if (number >= 10) {
                sprintf(str, "Loop:00%d", presset_config_set.set_circular);
                LCD_ShowString(0, 102, (uint8_t*)str, WHITE, RED, 32, 0);
                LCD_ShowIntNum(80, 102, digits[3], 1, RED, WHITE, 32);
            } else {
                sprintf(str, "Loop:000%d", presset_config_set.set_circular);
                LCD_ShowString(0, 102, (uint8_t*)str, WHITE, RED, 32, 0);
                LCD_ShowIntNum(80, 102, digits[3], 1, RED, WHITE, 32);
            }
        }
        memset(str, 0, 10);
        if (presset_config_set.current_index < 10) {
            sprintf(str, "Step:%d/29 ", presset_config_set.current_index);
        } else {
            sprintf(str, "Step:%d/29", presset_config_set.current_index);
        }
        LCD_ShowString(160, 118, (uint8_t*)str, WHITE, RED, 16, 0);
    } else {
        LCD_Fill_DMA(80, 102, LCD_W, 134, BLACK);
        sprintf(str, "Loop:%d", presset_config_set.set_circular);
        LCD_ShowString(0, 102, (uint8_t*)str, WHITE, BLACK, 32, 0);
        memset(str, 0, 10);
        if (presset_config_set.current_index < 10) {
            sprintf(str, "Step:%d/29 ", presset_config_set.current_index);
        } else {
            sprintf(str, "Step:%d/29", presset_config_set.current_index);
        }
        LCD_ShowString(160, 118, (uint8_t*)str, WHITE, BLACK, 16, 0);
    }
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
    LCD_ShowString(12, 16, (const uint8_t*)str, WHITE, MAIN_PAGE_COLOR, 16, 0);
}

void LCD_show_iset(void)
{
    char str[10] = {0};
    float temp = SC8815_Config.SC8815_IBUS_Limit / 1000;
    sprintf(str, "%.2fA", temp);
    if (temp >= 0 && temp < 10) {
        LCD_ShowString(20, 50, (const uint8_t*)str, WHITE, MAIN_PAGE_COLOR, 16, 0);
    }
}

void LCD_show_icon(uint16_t x, uint16_t y, uint16_t length, uint16_t width, const uint8_t pic[], uint16_t bg_color)
{
    uint8_t color;
    uint16_t i, j;
    uint32_t k = 0;
    isData = 0;
    LCD_Address_Set(x, y, x + length - 1, y + width - 1);
    isData = 1;
    for (i = 0;i < length;i++) {
        for (j = 0;j < width;j++) {
            if (pic[k * 2] == 0) {
                color = bg_color >> 8 & 0xff;
            } else {
                color = pic[k * 2];
            }
            LCD_WR_DATA8(color);
            if (pic[k * 2 + 1] == 0) {
                color = bg_color & 0xff;
            } else {
                color = pic[k * 2 + 1];
            }
            LCD_WR_DATA8(color);
            k++;
        }
    }
}

void LCD_show_on_off(uint16_t x, uint16_t y, uint16_t length, uint16_t width, const uint8_t pic[], uint16_t bg_color)
{
    uint8_t color;
    uint16_t i, j;
    uint32_t k = 0;
    isData = 0;
    LCD_Address_Set(x, y, x + length - 1, y + width - 1);
    isData = 1;
    for (i = 0;i < length;i++) {
        for (j = 0;j < width;j++) {
            if (pic[k * 2] != 0) {
                color = bg_color >> 8 & 0xff;
            } else {
                color = pic[k * 2];
            }
            LCD_WR_DATA8(color);
            if (pic[k * 2 + 1] != 0) {
                color = bg_color & 0xff;
            } else {
                color = pic[k * 2 + 1];
            }
            LCD_WR_DATA8(color);
            k++;
        }
    }
}

void LCD_fill_line(uint8_t line, uint16_t color)
{
    switch (line) {
    case 0:
        LCD_Fill_DMA(0, 0, LCD_W, 32, color);
        break;
    default:
        break;
    }
}

void LCD_on_menu_line(uint8_t line, uint8_t* image, uint8_t* text)
{
    uint16_t y, icon_x = 15;
#ifdef ENABLE_EN_FONT
    icon_x = 0;
#endif 
    uint8_t str[2];
    LCD_Fill_DMA(0, 32, LCD_W, 34, GRAY2);
    LCD_Fill_DMA(0, 66, LCD_W, 68, GRAY2);
    LCD_Fill_DMA(0, 100, LCD_W, 102, GRAY2);
    switch (line) {
    case 0:
        LCD_Fill_DMA(0, 0, LCD_W, 32, RED);
        LCD_show_icon(icon_x, 0, 32, 32, image, RED);
        y = 0;
        break;
    case 1:
        LCD_Fill_DMA(0, 34, LCD_W, 66, RED);
        LCD_show_icon(icon_x, 34, 32, 32, image, RED);
        y = 34;
        break;
    case 2:
        LCD_Fill_DMA(0, 68, LCD_W, 100, RED);
        LCD_show_icon(icon_x, 68, 32, 32, image, RED);
        y = 68;
        break;
    case 3:
        LCD_Fill_DMA(0, 102, LCD_W, 135, RED);
        LCD_show_icon(icon_x, 102, 32, 32, image, RED);
        y = 102;
        break;
    default:
        break;
    }
    // 处理中文字符
    for (int i = 0; text[i] != '\0'; i++) {
        if (text[i] >= 0x80 && text[i + 1] >= 0x80) {
            // 如果是GBK2312中文字符
            str[0] = text[i];
            str[1] = text[i + 1];
            LCD_ShowChinese(80 + i * 16, y, str, WHITE, RED, 32, 0);
            i++; // 跳过第二个字节
        } else {
            // 如果是ASCII字符
            str[0] = text[i];
            str[1] = 0;
#ifdef ENABLE_EN_FONT
            LCD_ShowString(40 + i * 16, y, str, WHITE, RED, 32, 0);
#else
            LCD_ShowString(80 + i * 16, y, str, WHITE, RED, 32, 0);
#endif            
        }
    }
}

void LCD_off_menu_line(uint8_t line, uint8_t* image, uint8_t* text)
{
    uint16_t y, icon_x = 15;
#ifdef ENABLE_EN_FONT
    icon_x = 0;
#endif 
    uint8_t str[2];
    switch (line) {
    case 0:
        LCD_Fill_DMA(0, 0, LCD_W, 32, BLACK);
        LCD_ShowPicture(icon_x, 0, 32, 32, image);
        y = 0;
        break;
    case 1:
        LCD_Fill_DMA(0, 34, LCD_W, 66, BLACK);
        LCD_ShowPicture(icon_x, 34, 32, 32, image);
        y = 34;
        break;
    case 2:
        LCD_Fill_DMA(0, 68, LCD_W, 100, BLACK);
        LCD_ShowPicture(icon_x, 68, 32, 32, image);
        y = 68;
        break;
    case 3:
        LCD_Fill_DMA(0, 102, LCD_W, 135, BLACK);
        LCD_ShowPicture(icon_x, 102, 32, 32, image);
        y = 102;
        break;
    default:
        break;
    }
    // 处理中文字符
    for (int i = 0; text[i] != '\0'; i++) {
        if (text[i] >= 0x80 && text[i + 1] >= 0x80) {
            // 如果是GBK2312中文字符
            str[0] = text[i];
            str[1] = text[i + 1];
            LCD_ShowChinese(80 + i * 16, y, str, WHITE, BLACK, 32, 0);
            i++; // 跳过第二个字节
        } else {
            // 如果是ASCII字符
            str[0] = text[i];
            str[1] = 0;
#ifdef ENABLE_EN_FONT
            LCD_ShowString(40 + i * 16, y, str, WHITE, BLACK, 32, 0);
#else
            LCD_ShowString(80 + i * 16, y, str, WHITE, BLACK, 32, 0);
#endif             
        }
    }
}
