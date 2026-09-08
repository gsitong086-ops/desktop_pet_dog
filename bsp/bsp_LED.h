/**
  ******************************************************************************
  * @file    bsp_LED.h
  * @author  SZTU OSHA 深圳技术大学开源硬件社 | HFH KRM LZS
  * @brief   
  * @version 1.0.0
  * @date    2025-10-1
  * @license GPL-3.0-only
  ******************************************************************************
  * @attention
  * 
  * STM32F103C8T6上只有一个蓝色LED供用户使用，，高电平点亮，连接在PC13
  * 
  ******************************************************************************
  * @CubeMXConfiguration
  * - TIM6: 84MHz/83/999 → 10ms周期
  * - GPIOA_Pin5: 推挽输出，高速
  * - 中断: TIM6_DAC_IRQn (抢占=3，子=0)
  * 
  * 注意：
  * 1. 修改TIM6参数需同步调整业务逻辑
  ******************************************************************************
  * @usage
  * 1. Init() 初始化
  * 2. Set()  调用
  * 
  * 注意：
  * 1. 参数修改需在适当状态下进行
  ******************************************************************************
  * @compatibility
  * - STM32CubeMX v6.8.1+
  * - HAL库 v1.18.0+
  * 
  * 依赖：
  * - yyy.h
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_LED_H__
#define __BSP_LED_H__

#ifdef __cplusplus
extern "C" {
#endif

/* SZTU OSHA  Includes--------------------------------------------------------*/
#include "main.h"


/* SZTU OSHA  Macros----------------------------------------------------------*/
#define LED_PORT GPIOC
#define LED_PIN GPIO_PIN_13
#define LED_LIGHTEN_LEVEL GPIO_PIN_SET

/* SZTU OSHA  Types-----------------------------------------------------------*/


/* SZTU OSHA  Constants-------------------------------------------------------*/


/* SZTU OSHA  Variables-------------------------------------------------------*/


/* SZTU OSHA  Function Prototypes --------------------------------------------*/
void  LED_On(void);
void  LED_Off(void);
void  LED_Toggle(void);



#ifdef __cplusplus
}
#endif

#endif /* __BSP_LED_H__ */






