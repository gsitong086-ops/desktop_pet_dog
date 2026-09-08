/**
  ******************************************************************************
  * @file    BlueTooth.h
  * @author  SZTU OSHA 深圳技术大学开源硬件社 [YourName]
  * @brief   蓝牙模块驱动接口定义
  *          包含蓝牙模块初始化、数据发送和接收处理的函数声明及配置宏定义
  * @version 1.0.0
  * @date    [YYYY-MM-DD]
  * @license GPL-3.0-only
  ******************************************************************************
  * @attention
  * 
  * 在对应串口的中断处理函数中，需添加清除空闲标志位的代码，示例如下：
  * if(__HAL_UART_GET_FLAG(&BLUETOOTH_UART, UART_FLAG_IDLE) != RESET){
  *   __HAL_UART_CLEAR_IDLEFLAG(&BLUETOOTH_UART);
  *   BLUETOOTH_RxFlag = 1;  // 置位接收标志位，用于主循环处理
  * }
  * 注：中断函数名称应为USART3_IRQHandler（与使用的USART3对应）
  * 
  ******************************************************************************
  * @CubeMXConfiguration
  * - 外设配置：USART3 配置为异步通信模式，开启DMA接收功能
  *             波特率：115200bps，数据位：8位，停止位：1位，校验位：无
  *             使能接收中断（IDLE中断），DMA模式为循环接收
  * - 引脚分配：
  *   - PB10: USART3_RX (蓝牙模块接收引脚，连接STM32的发送端)
  *   - PB11: USART3_TX (蓝牙模块发送引脚，连接STM32的接收端)
  *   - DMA1_Channel3: USART3_RX DMA通道（根据芯片型号确认，此处为示例）
  * - 时钟配置：APB1 外设时钟 @ 36MHz（USART3挂载于APB1总线）
  * 
  * 注意：
  * 1. 需启用UART空闲中断 (IDLE Interrupt) 以检测数据包结束
  * 2. DMA配置为循环模式 (Circular Mode)，缓冲区大小为BLUETOOTH_DMA_LENGTH
  * 3. 接收缓冲区大小需根据最大数据包长度调整，避免溢出
  * 4. 引脚功能需在CubeMX中正确配置为USART3复用功能
  ******************************************************************************
  * @usage
  * 1. 系统初始化时调用 BLUETOOTH_Init() 完成硬件初始化（建议在main函数中HAL_Init后调用）
  * 2. 通过 BLUETOOTH_send_information() 发送指令，参数为INS1-INS3（需提前定义指令功能）
  * 3. 在主循环中周期性调用 BLUETOOTH_process_information() 处理接收数据
  * 
  * 注意：
  * 1. 发送指令前需确保蓝牙模块已上电并完成初始化（通常需等待模块就绪）
  * 2. 处理函数的调用周期应小于最短数据包间隔，避免接收缓冲区覆盖
  * 3. 调试时可通过DEBUG_BLUETOOTH_Printf宏开启信息打印（在.c文件中定义）
  * 4. 多任务环境下需注意对共享变量（如接收缓冲区）的互斥访问
  ******************************************************************************
  * @compatibility
  * - STM32CubeMX v6.8.1+
  * - HAL库 v1.18.0+ 
  * - 适用于STM32F1/F4/L4等系列芯片（需根据具体型号调整引脚和DMA通道）
  * 
  * 依赖：
  * - stm32fxx_hal.h（芯片对应的HAL库头文件）
  * - main.h（包含外设句柄声明，如huart3、hdma_usart3_rx）
  * - stdio.h（调试打印用）
  * - string.h（字符串操作函数用）
  ******************************************************************************
  */

/* 防止递归包含保护 | Prevent recursive inclusion */
#ifndef __BLUETOOTH_H__
#define __BLUETOOTH_H__

#ifdef __cplusplus
extern "C" {
#endif

/* 包含头文件 | Includes -------------------------------------------------------*/
#include "main.h"
#include "stdio.h"
#include "string.h"

/* 宏定义 | Macros -------------------------------------------------------------*/
#define BLUETOOTH_UART        huart3                // 蓝牙连接的UART句柄（与CubeMX生成的句柄名一致）
#define BLUETOOTH_DMA         hdma_usart3_rx        // 蓝牙接收DMA句柄（与CubeMX生成的句柄名一致）
#define BLUETOOTH_DMA_LENGTH  100                   // DMA接收缓冲区大小（需根据实际需求调整）
#define BLUETOOTH_TO_STM32_HEADER 0xAA              // 蓝牙到STM32的数据包头部标识（协议约定）

// 指令定义 - 对应发送给蓝牙模块的控制指令
#define INS1  0x01                                  // 指令1：例如"查询设备状态"
#define INS2  0x02                                  // 指令2：例如"启动数据传输"
#define INS3  0x03                                  // 指令3：例如"停止数据传输"

/* 状态码定义 | Status code definitions */
#define BLUETOOTH_OK          0x00                  // 操作成功
#define BLUETOOTH_ERROR       0x01                  // 操作失败（如发送超时）
#define BLUETOOTH_TIMEOUT     0x02                  // 超时错误（未收到响应）
#define BLUETOOTH_INVALID     0x03                  // 无效指令（指令值不在合法范围内）

/* 类型定义 | Types -------------------------------------------------------------*/
/**
 * @brief 蓝牙模块状态枚举
 * @note 可用于应用层判断模块当前工作状态
 */
typedef enum {
    BLUETOOTH_DISCONNECTED,    // 未连接状态（未检测到蓝牙连接）
    BLUETOOTH_CONNECTED,       // 已连接状态（蓝牙链路建立成功）
    BLUETOOTH_DATA_RECEIVED    // 数据接收状态（收到新的数据包未处理）
} BLUETOOTH_StateTypeDef;

/* 外部变量声明 | External variables -------------------------------------------*/
extern volatile uint8_t BLUETOOTH_RxFlag;           // 蓝牙接收完成标志位（中断中置位，主循环中检测）
extern uint8_t BlueTooth_TxBuffer[50];              // 蓝牙发送缓冲区（大小根据最大发送数据包定义）
extern uint8_t BlueTooth_RxBuffer[BLUETOOTH_DMA_LENGTH]; // 蓝牙接收缓冲区（由DMA直接写入）
extern uint8_t BlueTooth_RxCount;                   // 蓝牙接收数据长度（实际收到的字节数）

/* 函数声明 | Function prototypes -----------------------------------------------*/

/**
  * @brief  初始化蓝牙模块通信接口
  * @details 配置UART空闲中断，启动DMA循环接收模式，初始化接收缓冲区
  * @note 需在调用此函数前确保USART3和DMA已通过CubeMX正确配置并生成句柄
  * @retval 无
  */
void BLUETOOTH_Init(void);

/**
  * @brief  向蓝牙模块发送指定指令
  * @param  instruction 指令类型，取值为INS1-INS3（定义在本头文件中）
  * @note 指令的具体发送格式需在函数内部实现（如添加包头、校验等）
  * @retval 无
  */
void BLUETOOTH_send_information(uint8_t instruction);

/**
  * @brief  处理蓝牙模块接收的数据
  * @details 检查BLUETOOTH_RxFlag，若置位则解析RxBuffer中的数据，执行对应逻辑
  *          处理完成后需重置标志位并重启DMA接收
  * @note 建议在主循环中高频调用，确保数据及时处理
  * @retval 无
  */
void BLUETOOTH_process_information(void);

#ifdef __cplusplus
}
#endif

#endif /* __BLUETOOTH_H__ */

