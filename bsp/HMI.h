/**
  ******************************************************************************
  * @file    HMI.h
  * @author  SZTU OSHA
  * @brief   语音人机交互模块
  *
  ******************************************************************************
  * @attention
  *
  * 语音人机交互模块连接到UART2
  * 使用普通发送模式
  * 使用串口空闲中断+DMA的接收模式 这样可以实现不定长指令的接收
  *
  * 在语音人机交互模块对应串口的中断处理函数USART2_IRQHandler中，
  * 写清除空闲标志位的代码，如下：
  * if(__HAL_UART_GET_FLAG(&huart2, UART_FLAG_IDLE) != RESET){
  *   __HAL_UART_CLEAR_IDLEFLAG(&huart2);
  *   HMI_RxFlag = 1;
  * }
  *
  ******************************************************************************
  * @CubeMXConfiguration

  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HMI_H__
#define __HMI_H__

#ifdef __cplusplus
extern "C" {
#endif

/* SZTU OSHA Includes-----------------------------------------------------*/
#include "main.h"
#include "stdio.h"
#include "string.h"


/* SZTU OSHA instructions macro-------------------------------------------*/
#define HMI_UART huart2                         //语音人机交互模块连接的串口
#define HMI_DMA hdma_usart2_rx                 //串口对应的接收DMA
#define HMI_DMA_LENGTH 100                              //接收DMA的大小

#define HMI_TO_STM32_HEADER 0x55                //HMI发送给STM32的包头(可选)


/* SZTU OSHA variables----------------------------------------------------*/
extern volatile uint8_t HMI_RxFlag;

/* SZTU OSHA Prototypes---------------------------------------------------*/
void HMI_Init(void);
HAL_StatusTypeDef HMI_send_hex_values(const uint8_t *data, size_t length);
void HMI_process_information(void);

#ifdef __cplusplus
}
#endif

#endif /* __HMI_H__ */




