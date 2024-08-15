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

uint16_t ADC_Value[4];
float VBUS_Value_Buff[10];
float IBUS_Value_Buff[10];
uint8_t ADC_Value_count = 0;
uint8_t ADC_Value_timestamp = 0;

/**
 *@brief 使用ADC获取VBUS电压
 *
 * @return VBUS单位mV
 */
float App_getVBUS_mV(void)
{
    // return (12 * ((float)ADC_Value[0] * SAMPLING_RATE)) * 1000;
    return ((12.023 * ((float)ADC_Value[0] * SAMPLING_RATE)) * 1000 - 22.312);
}

/**
 *@brief 使用ADC获取VBUS电压
 *
 * @return VBUS单位V
 */
float App_getVBUS_V(void)
{
    // return 12 * ((float)ADC_Value[0] * SAMPLING_RATE);
    return 12.023 * ((float)ADC_Value[0] * SAMPLING_RATE) - 0.022312;
}

/**
 *@brief 使用ADC获取温度传感器电压
 *
 * @return VBUS单位mV
 */
float App_getTemp_mV(void)
{
    return ((float)ADC_Value[1] * SAMPLING_RATE) * 1000;
}

/**
 *@brief 使用ADC获取温度传感器电压
 *
 * @return VBUS单位V
 */
float App_getTemp_V(void)
{
    return ((float)ADC_Value[1] * SAMPLING_RATE);   //ADC=1.22V=3K=45℃ ADC=0.93V=2K=55℃  ADC=0.79V=1.6K=75℃
}

/**
 *@brief 使用ADC获取VBUS电流
 *
 * @return 单位mA
 */
float App_getIBUS_mA(void)
{
    // return (((float)ADC_Value[2] * SAMPLING_RATE) / 0.5 * 1000) * 0.932;
    return (((float)ADC_Value[2] * SAMPLING_RATE) / 0.5 * 1000);
}

/**
 *@brief 使用ADC获取VBUS电流
 *
 * @return 单位A
 */
float App_getIBUS_A(void)
{
    // return (((float)ADC_Value[2] * SAMPLING_RATE) / 0.5) * 0.932;
    return (((float)ADC_Value[2] * SAMPLING_RATE) / 0.5);
}

/**
 *@brief 使用ADC获取VBAT电压
 *
 * @return VBAT单位mV
 */
float App_getVBAT_mV(void)
{
    return (12 * ((float)ADC_Value[3] * SAMPLING_RATE)) * 1000;
}

/**
 *@brief 使用ADC获取VBAT电压
 *
 * @return VBAT单位V
 */
float App_getVBAT_V(void)
{
    return 12 * ((float)ADC_Value[3] * SAMPLING_RATE);
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
        res += VBUS_Value_Buff[i];
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
        res += IBUS_Value_Buff[i];
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
