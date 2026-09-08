/**
  ******************************************************************************
  * @file    servo.h
  * @author  SZTU OSHA 深圳技术大学开源硬件社 [YourName]
  * @brief   舵机控制模块，用于初始化和控制四足机器人腿部舵机 
  * @version 1.0.0
  * @date    [YYYY-MM-DD]
  * @license GPL-3.0-only
  ******************************************************************************
  * @attention
  * 
  * - 角度与控制信号PWM波关系
  * sg90舵机（180度款）要求控制信号PWM波的周期为20ms，即50Hz的PWM波
  * 当前配置:  TIM1主频:72000000 / TIM1分配系数:80 / 重装值:18000 = 50Hz
  * 角度与占空比的对应：(0 ~ 180) 对应 (2.5% ~ 12.5%)
  * 对应的CCR  (18000*2.5% ~ 18000*12.5%)  即：(450 ~ 2250)
  *
  * - CCR与角度对应关系
  * SG90 舵机一般有 0-90°、0-180°、0-360° 等角度版本，以我们使用的 180° 版本为例，
  * 很多 SG90 舵机的位置等级有 1024 个，若有效角度范围为 180度，
  * 其控制的角度精度为 180 ÷ 1024 ≈ 0.18°，对应的脉宽控制精度约为 2 微秒。
  * 我们使用CCR值中的 1800细分 去覆盖 舵机本身的 1024细分。
  *
  * - 实际使用的角度范围
  * 角度(0 ~ 180) 对应 CCR(450 ~ 2250)
  * 实际使用中，引入负角度，把小狗的腿向前定义为正，反之为负，此时站立对应0度
  * 因此 角度(-90 ~ 90) 对应 CCR(450 ~ 2250)
  *
  *
  * - 舵机位置
  *   LEG1      LEG2
  *   左前      右前
  *
  *
  *
  *   LEG3      LEG4
  *   左后      右后
  * 
  ******************************************************************************
  * @CubeMXConfiguration
  * - 外设配置：TIM1 配置为PWM输出模式，周期20ms(50Hz)
  *             TIM6 配置为10ms周期定时器
  * - 引脚分配：
  *   - GPIOA_Pin8: TIM1_CH1 (LEG1舵机控制)
  *   - GPIOA_Pin9: TIM1_CH2 (LEG2舵机控制)
  *   - GPIOA_Pin10: TIM1_CH3 (LEG3舵机控制)
  *   - GPIOB_Pin13: TIM1_CH4 (LEG4舵机控制)
  *   - GPIOA_Pin5: 推挽输出，高速
  * - 时钟配置：TIM1时钟源为72MHz 
  *             TIM6时钟源为84MHz
  * 

  ******************************************************************************
  * @usage
  * 1. 初始化: servo_Init() 初始化舵机控制定时器和GPIO
  * 2. 设置单舵机角度: servo_set_angle(LEGx, angle) 设置指定舵机角度
  * 3. 设置多舵机角度: servo_set_angles(a1, a2, a3, a4) 同时设置四个舵机角度
  * 4. 校准舵机: servo_adjust() 执行舵机校准
  * 5. 测试舵机: servo_test() 运行舵机测试程序
  * 
  * 注意： | Note:
  * 1. 参数修改需在适当状态下进行
  * 2. 角度范围应限制在-90°到90°之间
  ******************************************************************************
  * @compatibility
  * - yyy.h
  ******************************************************************************
  */

/* 防止递归包含保护 | Prevent recursive inclusion */
#ifndef __SERVO_H__
#define __SERVO_H__

#ifdef __cplusplus
extern "C" {
#endif

/* 包含头文件   | Includes ----------------------------------------------------*/
#include "main.h"

/* 宏定义       | Macros ------------------------------------------------------*/

#define LEG1 1
#define LEG1_TIM htim1
#define LEG1_CHANNEL TIM_CHANNEL_1
#define LEG2 2
#define LEG2_TIM htim1
#define LEG2_CHANNEL TIM_CHANNEL_2
#define LEG3 3
#define LEG3_TIM htim1
#define LEG3_CHANNEL TIM_CHANNEL_3
#define LEG4 4
#define LEG4_TIM htim1
#define LEG4_CHANNEL TIM_CHANNEL_4

/* 状态码定义   | Status code definitions -------------------------------------*/
// 与HAL库状态码兼容（便于混合使用）
#define SERVO_OK                HAL_OK         // 操作成功
#define SERVO_ERROR             HAL_ERROR      // 操作失败
#define SERVO_BUSY              HAL_BUSY       // 模块忙
#define SERVO_TIMEOUT           HAL_TIMEOUT    // 操作超时
#define SERVO_INVALID_PARAM     0x04U          // 参数无效（自定义状态码）

/* 类型定义     | Types -------------------------------------------------------*/


/* 公有函数声明 | Public function prototypes ----------------------------------*/
/**
  * @brief  初始化舵机控制模块
  * @param  无
  * @retval 无
  */
void servo_Init(void);

/**
  * @brief  校准舵机位置
  * @param  无
  * @retval 无
  */
void servo_adjust(void);

/**
  * @brief  测试舵机功能
  * @param  无
  * @retval 无
  */
void servo_test(void);

/**
  * @brief  设置指定舵机的角度
  * @param  LEGx: 舵机编号(LEG1-LEG4)
  * @param  angle: 目标角度(-90到90度)
  * @retval SERVO_OK: 操作成功; SERVO_INVALID_PARAM: 参数无效
  */
uint8_t servo_set_angle(uint8_t LEGx, float angle);

/**
  * @brief  同时设置四个舵机的角度
  * @param  a1: 第一个舵机角度
  * @param  a2: 第二个舵机角度
  * @param  a3: 第三个舵机角度
  * @param  a4: 第四个舵机角度
  * @retval 无
  */
void servo_set_angles(float a1, float a2, float a3, float a4) ;

#ifdef __cplusplus
}
#endif

#endif /* __SERVO_H__ */


