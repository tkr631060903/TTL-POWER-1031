#include "Application_BUZZER.h"
#include "Application.h"

extern TIM_HandleTypeDef htim1;

/**
 *@brief 打开蜂鸣器time ms后关闭
 * 
 * @param time 打开蜂鸣器时间单位ms
 */
void BUZZER_OPEN(uint32_t time)
{
    if (APP_config.lock_buzzer == 1) {
        HAL_Delay(time);
        return;
    }
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    Application_SoftwareDelay(time);
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
}

/**
 *@brief 打开蜂鸣器
 * 
 */
void BUZZER_ON(void)
{
    if (APP_config.lock_buzzer == 1) {
        return;
    }
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    APP_config.buzzer_delay_ms = 100;
}

/**
 *@brief 关闭蜂鸣器
 * 
 */
void BUZZER_OFF(void)
{
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
}
