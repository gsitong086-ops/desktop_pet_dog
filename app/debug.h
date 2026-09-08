/**
  ******************************************************************************
  * @file    debug.h
  * @author  SZTU OSHA HFH
  * @brief  该文件宏定义了一些调试变量和调试信号
  *
  ******************************************************************************
  * @attention
  *
  * 将该文件include到main.h(main.h被bsp组和nodule组include，因此比较方便)
  * 或者直接include其他需要调试变量和调试信号的源文件中
  *
  * 条件编译指令格式参考：
  * #ifdef ENABLE_FEATURE
  * // 启用特性的代码
  * #else
  * // 禁用特性的代码
  * #endif
  * 
  * 不缩进，与代码块对齐，大多数开源项目（如 Linux 内核、FreeRTOS）采用的风格
  *
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DEBUG_H__
#define __DEBUG_H__

#ifdef __cplusplus
extern "C" {
#endif

/* SZTU OSHA HFH variables macro ---------------------------------------------*/

/* SZTU OSHA HFH debug macro -------------------------------------------------*/
#define DEBUG_HMI_Printf 1
#define DEBUG_Init 1

/* SZTU OSHA HFH Prototypes---------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* __DEBUG_config_h__ */




