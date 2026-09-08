/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "debug.h"
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
#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC
#define TX2_ASRPRO_Pin GPIO_PIN_2
#define TX2_ASRPRO_GPIO_Port GPIOA
#define RX2_ASRPRO_Pin GPIO_PIN_3
#define RX2_ASRPRO_GPIO_Port GPIOA
#define light_Pin GPIO_PIN_5
#define light_GPIO_Port GPIOA
#define touch_Pin GPIO_PIN_7
#define touch_GPIO_Port GPIOA
#define touch_EXTI_IRQn EXTI9_5_IRQn
#define TX3_Bluetooth_Pin GPIO_PIN_10
#define TX3_Bluetooth_GPIO_Port GPIOB
#define RX3_Bluetooth_Pin GPIO_PIN_11
#define RX3_Bluetooth_GPIO_Port GPIOB
#define servo1_Pin GPIO_PIN_8
#define servo1_GPIO_Port GPIOA
#define servo2_Pin GPIO_PIN_9
#define servo2_GPIO_Port GPIOA
#define servo3_Pin GPIO_PIN_10
#define servo3_GPIO_Port GPIOA
#define servo4_Pin GPIO_PIN_11
#define servo4_GPIO_Port GPIOA
#define flow_LED_Pin GPIO_PIN_12
#define flow_LED_GPIO_Port GPIOA
#define TX1_Debug_Pin GPIO_PIN_6
#define TX1_Debug_GPIO_Port GPIOB
#define RX1_Debug_Pin GPIO_PIN_7
#define RX1_Debug_GPIO_Port GPIOB
#define My_OLED_SDA_Pin GPIO_PIN_8
#define My_OLED_SDA_GPIO_Port GPIOB
#define My_OLED_SCL_Pin GPIO_PIN_9
#define My_OLED_SCL_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
