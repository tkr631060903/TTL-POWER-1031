/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED1_Pin GPIO_PIN_14
#define LED1_GPIO_Port GPIOC
#define USART_TX_Debug_Pin GPIO_PIN_2
#define USART_TX_Debug_GPIO_Port GPIOA
#define USART_RX_Debug_Pin GPIO_PIN_3
#define USART_RX_Debug_GPIO_Port GPIOA
#define KEY3_Pin GPIO_PIN_4
#define KEY3_GPIO_Port GPIOA
#define KEY3_EXTI_IRQn EXTI4_IRQn
#define KEY2_Pin GPIO_PIN_5
#define KEY2_GPIO_Port GPIOA
#define KEY2_EXTI_IRQn EXTI9_5_IRQn
#define KEY1_Pin GPIO_PIN_6
#define KEY1_GPIO_Port GPIOA
#define KEY1_EXTI_IRQn EXTI9_5_IRQn
#define VDD_OUT_Pin GPIO_PIN_7
#define VDD_OUT_GPIO_Port GPIOA
#define VDD_IN_Pin GPIO_PIN_0
#define VDD_IN_GPIO_Port GPIOB
#define SC8815_I2C_SCL_Pin GPIO_PIN_10
#define SC8815_I2C_SCL_GPIO_Port GPIOB
#define SC8815_I2C_SDL_Pin GPIO_PIN_11
#define SC8815_I2C_SDL_GPIO_Port GPIOB
#define Rotar_L_Pin GPIO_PIN_8
#define Rotar_L_GPIO_Port GPIOA
#define Rotar_L_EXTI_IRQn EXTI9_5_IRQn
#define Rotar_R_Pin GPIO_PIN_9
#define Rotar_R_GPIO_Port GPIOA
#define KEY4_Pin GPIO_PIN_10
#define KEY4_GPIO_Port GPIOA
#define SC8815_INT_Pin GPIO_PIN_15
#define SC8815_INT_GPIO_Port GPIOA
#define SC8815_PSTOP_Pin GPIO_PIN_3
#define SC8815_PSTOP_GPIO_Port GPIOB
#define SC8815_CE_Pin GPIO_PIN_4
#define SC8815_CE_GPIO_Port GPIOB
#define CH224K_CFG1_Pin GPIO_PIN_5
#define CH224K_CFG1_GPIO_Port GPIOB
#define CH224K_CFG2_Pin GPIO_PIN_6
#define CH224K_CFG2_GPIO_Port GPIOB
#define CH224K_CFG3_Pin GPIO_PIN_7
#define CH224K_CFG3_GPIO_Port GPIOB
#define POWER_RELEASE_Pin GPIO_PIN_8
#define POWER_RELEASE_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#define KEY4_Pin GPIO_PIN_10
#define KEY4_GPIO_Port GPIOA
#define KEY4_EXTI_IRQn EXTI15_10_IRQn
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
