/**
  ******************************************************************************
  * @file    Myit.h
  * @author  SZTU OSHA 深圳技术大学开源硬件社  XXX 某某某
  * @brief   
  * @version 1.0.0
  * @date    2025-10-1
  * @license GPL-3.0-only
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  ******************************************************************************
  * @CubeMXConfiguration

  ******************************************************************************
  * @usage

  ******************************************************************************
  * @compatibility

  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MYIT_H__
#define __MYIT_H__

#ifdef __cplusplus
extern "C" {
#endif

/* SZTU OSHA  Includes--------------------------------------------------------*/
#include "main.h"


/* SZTU OSHA  Macros----------------------------------------------------------*/
#define MYIT_1_TIM htim4
#define MYIT_1_FLAG Myit_f

/* SZTU OSHA  Types-----------------------------------------------------------*/


/* SZTU OSHA  Constants-------------------------------------------------------*/


/* SZTU OSHA  Variables-------------------------------------------------------*/


/* SZTU OSHA  Function Prototypes --------------------------------------------*/
void Myit_Init(void);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

#ifdef __cplusplus
}
#endif

#endif /* __MYIT_H__ */






