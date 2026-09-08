/**
  ******************************************************************************
  * @file    bsp_uart.h
  * @author  SZTU OSHA HFH
  * @brief   This file contains all the function prototypes for
  *          the bsp_uart.c file
  ******************************************************************************
  * @attention
  *
  * 用UART1来做printf的底层实现,用来串口调试
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_UART_H__
#define __BSP_UART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* SZTU OSHA Includes-----------------------------------------------------*/
#include "main.h"
#include "stdio.h"
#include "string.h"

/* SZTU OSHA Private defines----------------------------------------------*/
extern UART_HandleTypeDef huart1;

/* SZTU OSHA Prototypes---------------------------------------------------*/


#ifdef __cplusplus
}
#endif

#endif /* __bsp_uart_H__ */



