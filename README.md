[README.md](https://github.com/user-attachments/files/31962154/README.md)
/**
  ******************************************************************************
  * @file    README.md
  * @author  SZTU OSHA HFH
  * @brief   介绍工程配置、文件结构、工程结构
  *
  ******************************************************************************
  */
  
## 1 如何新建工程
    利用模板工程文件夹 LCKFB_STM32F407VET6_V0.0
    复制粘贴模板工程文件夹 
    修改文件夹名字为新工程名
    在文件夹目录下找到project，在project下修改CubeMx的工程名为新工程名，CubeMx工程名后缀为.ioc
    删除project下的MDK-ARM文件夹
    打开新的CubeMx工程
    直接点生成工程代码，并打开工程
    按下面的提示继续配置

## 2 工程配置：
    'Options for Target'控件中:
        'Target'选项卡:
            'ARM Compiler' 使用 version 6 编译器
            勾选 '[Y] Use MicroLIB'
            'Floating Point Hardware'选 'Single precision'
        'C/C++(AC6)'选项卡:
            'Preprocessor Symbols' 的Define中 添加 'ARM_MATH_LOOPUNROLL' 宏，宏之间英文逗号分隔
            'Optimization' 优化选 '-Ofast'
            'Include Paths' 添加 头文件路径
            'Misc Controls'添加 -Wno-excess-initializers 屏蔽中文字符警告 initializer-string for char array is too long [-Wexcess-initializers]
        'Debug'选项卡:
            [Y] Use  'xxx Debugger' 'Settings'中
                'Flash Download'
                    勾选 '[Y] Reset and Run'
                'Packs'
                    取消勾选 '[N] Enable'
    'Run-timer Environment'(RTE)控件中:
        'CMSIS'中添加 DSP 源码
            勾选[Y] 'DSP'和'CORE'

## 3 文件结构：
    app：      主要存放应用层代码
    bsp：      存放和底层相关的支持包。
    module：   主要存放各种软件模块，比如软件定时器，PID,FIFO,状态机等。
    project:   存放工程文件。
    README.md: 文档
    

## 4 工程结构：
    添加组：
        |-app
           |_debug_config.h
        |-bsp
        |-module
        |_doc
           |_README.md
           
## 5 头文件
    #include "arm_math.h"
            
            
## 6头文件模板 (xxx.h)
/**
  ******************************************************************************
  * @file    xxx.h
  * @author  SZTU OSHA 深圳技术大学开源硬件社 [YourName]
  * @brief   [模块功能简要描述] Brief description of module function
  * @version 1.0.0
  * @date    [YYYY-MM-DD]
  * @license GPL-3.0-only
  ******************************************************************************
  * @attention
  * 
  * 版权所有 (c) [YYYY] 深圳技术大学开源硬件社
  * Copyright (c) [YYYY] SZTU OSHA
  * 
  * 根据GPL-3.0许可证授权，请确保遵守许可条款
  * Licensed under GPL-3.0, please ensure compliance with the license terms
  * 
  ******************************************************************************
  * @CubeMXConfiguration
  * - 外设配置：[外设名称] 配置说明 | Peripheral Configuration: [Peripheral] description
  * - 引脚分配：
  *   - [引脚号]: [功能描述] | Pin Assignment:
  *   - [Pin]: [Function description]
  * - 时钟配置：[时钟源] @ [频率] | Clock Configuration: [Clock source] @ [Frequency]
  * 
  * 注意： | Note:
  * 1. [重要配置注意事项] | [Important configuration notes]
  ******************************************************************************
  * @usage
  * 1. 初始化: XXX_Init() | Initialization: XXX_Init()
  * 2. 设置参数: XXX_SetParameter() | Set parameters: XXX_SetParameter()
  * 3. 执行操作: XXX_Execute() | Execute operation: XXX_Execute()
  * 
  * 注意： | Note:
  * 1. [使用时的关键注意事项] | [Key considerations when using]
  ******************************************************************************
  * @compatibility
  * - STM32CubeMX v6.8.1+
  * - HAL库 v1.18.0+ | HAL Library v1.18.0+
  * 
  * 依赖： | Dependencies:
  * - stm32fxx_hal.h
  * - yyy.h
  ******************************************************************************
  */

/* 防止递归包含保护 | Prevent recursive inclusion */
#ifndef __XXX_H__
#define __XXX_H__

#ifdef __cplusplus
extern "C" {
#endif

/* 包含头文件 | Includes ----------------------------------------------------------------*/
#include "main.h"
#include <stdbool.h>

/* 宏定义 | Macros -------------------------------------------------------------------*/
#define XXX_MAX_VALUE     (1000)  // 模块最大值 | Module maximum value
#define XXX_TIMEOUT       (0xFFFF)// 操作超时时间 | Operation timeout

/* 状态码定义 | Status code definitions */
#define XXX_OK            (0x00)  // 操作成功 | Operation successful
#define XXX_ERROR         (0x01)  // 操作失败 | Operation failed
#define XXX_BUSY          (0x02)  // 模块忙 | Module busy

/* 类型定义 | Types -----------------------------------------------------------------*/
typedef enum {
  XXX_MODE_LOW_POWER = 0, // 低功耗模式 | Low power mode
  XXX_MODE_NORMAL,        // 普通模式 | Normal mode
  XXX_MODE_HIGH_PERF      // 高性能模式 | High performance mode
} XXX_OperatingMode_t;

typedef struct {
  uint32_t    param1;     // 参数1 | Parameter 1
  uint16_t    param2;     // 参数2 | Parameter 2
  bool        enableFeature; // 使能特性 | Enable feature
} XXX_Config_t;

/* 函数声明 | Function prototypes ---------------------------------------------------*/
uint8_t XXX_Init(XXX_HandleTypeDef *hxxx);  // 初始化模块 | Initialize module
uint8_t XXX_SetMode(XXX_OperatingMode_t mode); // 设置模式 | Set mode
void    XXX_ProcessData(uint8_t *input, uint8_t *output, uint16_t size); // 处理数据 | Process data
bool    XXX_CheckStatus(void); // 检查状态 | Check status

#ifdef __cplusplus
}
#endif

#endif /* __XXX_H__ */

## 7 源文件模板 (xxx.c)
/**
  ******************************************************************************
  * @file    xxx.c
  * @author  SZTU OSHA 深圳技术大学开源硬件社 [YourName]
  * @brief   [模块功能详细描述] Detailed description of module function
  * @version 1.0.0
  * @date    [YYYY-MM-DD]
  * @license GPL-3.0-only
  ******************************************************************************
  * @attention
  * 
  * 版权所有 (c) [YYYY] 深圳技术大学开源硬件社
  * Copyright (c) [YYYY] SZTU OSHA
  * 
  * 根据GPL-3.0许可证授权，请确保遵守许可条款
  * Licensed under GPL-3.0, please ensure compliance with the license terms
  * 
  ******************************************************************************
  * @implementation
  * - 实现原理: [技术实现原理说明] | Implementation principle: [Description]
  * - 算法描述: [使用的算法说明] | Algorithm description: [Description]
  * 
  * 注意： | Note:
  * 1. [核心实现注意事项] | [Core implementation notes]
  ******************************************************************************
  */

/* 包含头文件 | Includes ----------------------------------------------------------------*/
#include "xxx.h"
#include "yyy.h"

/* 私有宏定义 | Private macros ---------------------------------------------------------------*/
#define INTERNAL_BUFFER_SIZE  (64)  // 内部缓冲区大小 | Internal buffer size

/* 私有类型定义 | Private types -------------------------------------------------------------*/
typedef struct {
  XXX_Config_t config;   // 配置参数 | Configuration parameters
  bool         initialized; // 初始化标志 | Initialization flag
  uint32_t     lastTick; // 上次记录的时间戳 | Last recorded timestamp
} XXX_Context_t;

/* 模块私有变量 | Private variables -------------------------------------------------------------*/
static XXX_Context_t xxxContext = {
  .config = {
    .param1 = 0,
    .param2 = XXX_MAX_VALUE,
    .enableFeature = true
  },
  .initialized = false,
  .lastTick = 0
};

/* 私有函数原型 | Private function prototypes ---------------------------------------------*/
static void _XXX_ResetHardware(XXX_HandleTypeDef *hxxx); // 硬件复位 | Hardware reset
static uint8_t _XXX_ValidateConfig(XXX_Config_t *config); // 配置验证 | Configuration validation

/* 公有函数实现 | Public functions -------------------------------------------------------------*/

/**
  * @brief  初始化XXX模块 | Initialize XXX module
  * @param  hxxx: XXX模块句柄指针 | Handle pointer of XXX module
  * @retval 状态码 (XXX_OK/XXX_ERROR) | Status code (XXX_OK/XXX_ERROR)
  */
uint8_t XXX_Init(XXX_HandleTypeDef *hxxx)
{
  /* 参数检查 | Parameter check */
  if(hxxx == NULL) {
    return XXX_ERROR;
  }
  
  /* 硬件复位 | Hardware reset */
  _XXX_ResetHardware(hxxx);
  
  /* 配置验证 | Configuration validation */
  if(_XXX_ValidateConfig(&xxxContext.config) != XXX_OK) {
    return XXX_ERROR;
  }
  
  /* 初始化完成标志 | Set initialization complete flag */
  xxxContext.initialized = true;
  xxxContext.lastTick = HAL_GetTick(); // 记录当前时间 | Record current time
  
  return XXX_OK;
}

/**
  * @brief  设置工作模式 | Set operating mode
  * @param  mode: 选择的工作模式 | Selected operating mode
  * @retval 状态码 | Status code
  */
uint8_t XXX_SetMode(XXX_OperatingMode_t mode)
{
  /* 模式有效性检查 | Mode validity check */
  if(mode > XXX_MODE_HIGH_PERF) {
    return XXX_ERROR;
  }
  
  /* [实际模式切换实现] | [Actual mode switching implementation] */
  
  return XXX_OK;
}

/* 私有函数实现 | Private functions -------------------------------------------------------------*/

/**
  * @brief  内部硬件复位函数 | Internal hardware reset function
  * @param  hxxx: 模块句柄 | Module handle
  * @retval None
  */
static void _XXX_ResetHardware(XXX_HandleTypeDef *hxxx)
{
  /* 复位硬件寄存器 | Reset hardware registers */
  hxxx->Instance->CR = 0x00;
  hxxx->Instance->CFGR = 0x00000000;
  
  /* 等待复位完成 | Wait for reset completion */
  while((hxxx->Instance->SR & 0x01) != 0);
}

/**
  * @brief  配置参数验证 | Configuration parameter validation
  * @param  config: 配置结构体指针 | Pointer to configuration structure
  * @retval 验证结果 | Validation result
  */
static uint8_t _XXX_ValidateConfig(XXX_Config_t *config)
{
  if(config->param2 > XXX_MAX_VALUE) {
    return XXX_ERROR;
  }
  
  /* [其他参数验证逻辑] | [Other parameter validation logic] */
  
  return XXX_OK;
}
