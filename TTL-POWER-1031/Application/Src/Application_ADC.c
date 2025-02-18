/**
 *@file Application_ADC.c
 * @author TanKairong (tkr631060903@gmail.com)
 * @brief ADC驱动
 * @version 0.1
 * @date 2024-01-31
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "Application_ADC.h"
#include "ina226.h"
#include "Application_SC8815.h"
#include "menu.h"

#define REFLESH_VBUS_IBUS_TIME 50
#define WAIT_PRESSET (presset_config_set.set_time[SC8815_Config.sc8815_tim_work_step] != 0 && \
                    (presset_config_set.set_time[SC8815_Config.sc8815_tim_work_step] * 1000) - SC8815_Config.sc8815_tim_work_time <= 5)

uint16_t ADC_Value[4];
extern presset_config_set_typeDef presset_config_set;
static float ina226_voltage = 0;
static float ina226_current = 0;
static uint32_t reflesh_vbus_ibus_time = 0;

void reflesh_VBUS_IBUS(void)
{
    if (HAL_GetTick() - reflesh_vbus_ibus_time >= REFLESH_VBUS_IBUS_TIME) {
        ina226_voltage = INA226_ReadVoltage();
        ina226_current = INA226_ReadCurrent();
        reflesh_vbus_ibus_time = HAL_GetTick();
    }
}

/**
 *@brief 获取VBUS电压
 *
 * @return VBUS单位mV
 */
float App_getVBUS_mV(void)
{
    // return (12 * ((float)ADC_Value[0] * SAMPLING_RATE)) * 1000;
    // while (WAIT_PRESSET || get_i2c_mutex());
    // ina226_voltage = INA226_ReadVoltage();
    // if (ina226_voltage < 0) {
    //     ina226_voltage = 0;
    // }
    return ina226_voltage;
}

/**
 *@brief 获取VBUS电压
 *
 * @return VBUS单位V
 */
float App_getVBUS_V(void)
{
    // return 12 * ((float)ADC_Value[0] * SAMPLING_RATE);
    // while (WAIT_PRESSET || get_i2c_mutex());
    // ina226_voltage = INA226_ReadVoltage();
    // if (ina226_voltage < 0) {
    //     ina226_voltage = 0;
    // }
    return App_getVBUS_mV() / 1000;
}

/**
 *@brief 使用ADC获取温度传感器电压
 *
 * @return VBUS单位mV
 */
float App_getTemp_mV(void)
{
    return App_getTemp_V() * 1000;
}

/**
 *@brief 使用ADC获取温度传感器电压
 *
 * @return VBUS单位V
 */
float App_getTemp_V(void)
{
    return 1.02 * ((float)ADC_Value[1] * SAMPLING_RATE);
}

/**
 * @brief 获取温度分两段获取-20℃~34℃和34~95℃
 *
 * @return float 
 */
float App_getTemp(void)
{
    if (App_getTemp_V() >= 1.893) {
        // 小于等于34℃
        return -21.058 * (ADC_Value[1] * SAMPLING_RATE * ADC_Value[1] * SAMPLING_RATE * ADC_Value[1] * SAMPLING_RATE) + 141.66 * ADC_Value[1] * SAMPLING_RATE * ADC_Value[1] * SAMPLING_RATE - 347.4 * ADC_Value[1] * SAMPLING_RATE + 327.04;
    } else {
        return -13.187 * (ADC_Value[1] * SAMPLING_RATE * ADC_Value[1] * SAMPLING_RATE * ADC_Value[1] * SAMPLING_RATE) + 61.641 * ADC_Value[1] * SAMPLING_RATE * ADC_Value[1] * SAMPLING_RATE - 125.76 * ADC_Value[1] * SAMPLING_RATE + 140.18;
    }
}

/**
 *@brief 获取VBUS电流
 *
 * @return 单位mA
 */
float App_getIBUS_mA(void)
{
    // return (((float)ADC_Value[2] * SAMPLING_RATE) / 0.5 * 1000);
    // while (WAIT_PRESSET || get_i2c_mutex());
    // ina226_current = INA226_ReadCurrent();
    // if (ina226_current < 0) {
    //     ina226_current = 0;
    // }
    return ina226_current;
}

/**
 *@brief 获取VBUS电流
 *
 * @return 单位A
 */
float App_getIBUS_A(void)
{
    // return (((float)ADC_Value[2] * SAMPLING_RATE) / 0.5);
    // while (WAIT_PRESSET || get_i2c_mutex());
    // ina226_current = INA226_ReadCurrent();
    // if (ina226_current < 0) {
    //     ina226_current = 0;
    // }
    return App_getIBUS_mA() / 1000;
}

/**
 *@brief 使用ADC获取VBAT电压
 *
 * @return VBAT单位mV
 */
float App_getVBAT_mV(void)
{
    return App_getVBAT_V() * 1000;
}

/**
 *@brief 使用ADC获取VBAT电压
 *
 * @return VBAT单位V
 */
float App_getVBAT_V(void)
{
    // return 11.59 * ((float)ADC_Value[3] * SAMPLING_RATE);
    return 11.799 * ((float)ADC_Value[3] * SAMPLING_RATE) - 0.1701;
}

/**
 * @brief 冒泡排序法
 * 
 * @param arr 待排序数组
 * @param size 数组长度
 */
void BubbleSort(float *arr, int size)
{
    int i, j, tmp;
    for (i = 0; i < size - 1; i++)
    {
        for (j = 0; j < size - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
}

/**
 *@brief 使用ADC获取VBUS电压并做平均
 *
 * @return VBUS单位mV
 */
float App_getVBUS_average_mV(void)
{
    // 可考虑在主循环添加一个持续ADC采样的功能，存在一个固定的环形buff里，然后再此函数做平均计算即可
    float res_arr[10], res = 0;
    for (int i = 0; i < 10; i++)
    {
        res_arr[i] = App_getVBUS_mV();
    }
    BubbleSort(res_arr, 10);
    for (int i = 1; i < 9; i++)
    {
        res += res_arr[i];
    }
    return res / 8;
}

/**
 *@brief 使用ADC获取VBUS电压并做平均
 *
 * @return VBUS单位V
 */
float App_getVBUS_average_V(void)
{
    // float res_arr[10], res = 0;
    // for (int i = 0; i < 10; i++)
    // {
    //     res_arr[i] = App_getVBUS_V();
    // }
    // BubbleSort(res_arr, 10);
    // for (int i = 1; i < 9; i++)
    // {
    //     res += res_arr[i];
    // }
    // return res / 8;
    float res = 0;
    for (int i = 0; i < 10; i++)
    {
        // res += VBUS_Value_Buff[i];
    }
    return res / 10;
}

/**
 *@brief 使用ADC获取VBUS电流并做平均
 *
 * @return 单位mA
 */
float App_getIBUS_average_mA(void)
{
    float res_arr[10], res = 0;
    for (int i = 0; i < 10; i++)
    {
        res_arr[i] = App_getIBUS_mA();
    }
    BubbleSort(res_arr, 10);
    for (int i = 1; i < 9; i++)
    {
        res += res_arr[i];
    }
    return res / 8;
}

/**
 *@brief 使用ADC获取VBUS电流并做平均
 *
 * @return 单位A
 */
float App_getIBUS_average_A(void)
{
    // float res_arr[10], res = 0;
    // for (int i = 0; i < 10; i++)
    // {
    //     res_arr[i] = App_getIBUS_A();
    // }
    // BubbleSort(res_arr, 10);
    // for (int i = 1; i < 9; i++)
    // {
    //     res += res_arr[i];
    // }
    // return res / 8;
    float res = 0;
    for (int i = 0; i < 10; i++)
    {
        // res += IBUS_Value_Buff[i];
    }
    return res / 10;
}

/**
 *@brief 使用ADC获取VBAT电压并做平均
 *
 * @return VBAT单位mV
 */
float App_getVBAT_average_mV(void)
{
    float res_arr[10], res = 0;
    for (int i = 0; i < 10; i++)
    {
        res_arr[i] = App_getVBAT_mV();
    }
    BubbleSort(res_arr, 10);
    for (int i = 1; i < 9; i++)
    {
        res += res_arr[i];
    }
    return res / 8;
}

/**
 *@brief 使用ADC获取VBAT电压并做平均
 *
 * @return VBAT单位V
 */
float App_getVBAT_average_V(void)
{
    float res_arr[10], res = 0;
    for (int i = 0; i < 10; i++)
    {
        res_arr[i] = App_getVBAT_V();
    }
    BubbleSort(res_arr, 10);
    for (int i = 1; i < 9; i++)
    {
        res += res_arr[i];
    }
    return res / 8;
}
