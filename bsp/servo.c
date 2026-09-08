/**
  ******************************************************************************
  * @file    servo.c
  * @author  SZTU OSHA 深圳技术大学开源硬件社 [YourName]
  * @brief   舵机控制模块的具体实现，包含舵机初始化、角度设置、校准及测试功能，
  * 通过STM32的TIM1定时器输出PWM信号控制SG90舵机，实现四足机器人腿部动作控制
  * @version 1.0.0
  * @date    [YYYY-MM-DD]
  * @license GPL-3.0-only
  ******************************************************************************
  * @attention
  * 
  * - 舵机测试时需注意：部分安装在机器人上的舵机因机械结构限制无法旋转180度，
  *   测试过程中若发现堵转现象，应立即断电，避免损坏舵机或机械结构
  * - 角度设置时需确保参数在有效范围内（-90°~90°），超出范围将被强制限制
  * - 左前腿（LEG1）和左后腿（LEG3）存在角度方向反转处理，与右前腿（LEG2）、
  *   右后腿（LEG4）方向相反
  ******************************************************************************
  * @implementation
  * - 实现原理: 通过STM32的TIM1定时器输出50Hz（周期20ms）的PWM信号控制SG90舵机，
  *   PWM占空比与舵机角度对应（2.5%~12.5%对应0°~180°）。通过__HAL_TIM_SetCompare函数
  *   修改捕获比较寄存器（CCR）值，实现占空比调节，进而控制舵机角度
  * - 算法描述: 角度与CCR值的转换采用线性映射，公式为CCR = (angle + 90) * 10 + 450
  *   （对应角度-90°~90°到CCR值450~2250的映射）。
  *   左前腿和左后腿通过角度取反实现方向校正，确保机械动作一致性
  * 
  * 注意： | Note:
  * 1. TIM1的预分频系数（PSC=79）和自动重装载值（ARR=18000）是生成50Hz PWM的关键参数，
  * 修改后需重新校准角度与CCR的映射关系
  * 2. 舵机初始化时需确保TIM1已正确配置为PWM输出模式，且相关GPIO引脚已复用为TIM1通道
  * 3. 批量设置角度函数（servo_set_angles）通过调用单个设置函数实现，需注意各舵机的角度方向差异
  ******************************************************************************
  */

/* 包含头文件   | Includes ---------------------------------------------------*/
#include "servo.h"
#include "tim.h"  // 包含定时器相关定义（根据实际工程调整）

/* 私有宏定义   | Private macros ---------------------------------------------*/
// 角度与CCR转换参数
#define ANGLE_TO_CCR_OFFSET 1350.0f  // 0度对应的CCR值
#define ANGLE_TO_CCR_SCALE 10.0f     // 角度到CCR的缩放系数（1度对应10个CCR单位）

/* 私有类型定义 | Private types ----------------------------------------------*/


/* 模块私有变量 | Private variables ------------------------------------------*/
// 舵机角度限位参数（根据机械结构自定义）
static const float LEG1_MIN_ANGLE = -90.0f;
static const float LEG1_MAX_ANGLE = 90.0f;
static const float LEG2_MIN_ANGLE = -90.0f;
static const float LEG2_MAX_ANGLE = 90.0f;
static const float LEG3_MIN_ANGLE = -90.0f;
static const float LEG3_MAX_ANGLE = 90.0f;
static const float LEG4_MIN_ANGLE = -90.0f;
static const float LEG4_MAX_ANGLE = 90.0f;

/* 私有函数原型 | Private function prototypes --------------------------------*/
/**
  * @brief  限制角度在指定范围内
  * @param  angle: 原始角度值
  * @param  min: 最小允许角度
  * @param  max: 最大允许角度
  * @retval 限位后的角度值
  */
static float servo_limit_angle(float angle, float min, float max);

/* 公有函数实现 | Public fhunctions -------------------------------------------*/

/**
  * @brief  初始化舵机控制模块，开启TIM1的PWM输出
  * @param  无
  * @retval 无
  */
void servo_Init(void)
{
  // 启动各舵机对应TIM1通道的PWM输出
  HAL_TIM_PWM_Start(&LEG1_TIM, LEG1_CHANNEL);
  HAL_TIM_PWM_Start(&LEG2_TIM, LEG2_CHANNEL);
  HAL_TIM_PWM_Start(&LEG3_TIM, LEG3_CHANNEL);
  HAL_TIM_PWM_Start(&LEG4_TIM, LEG4_CHANNEL);
}

/**
  * @brief  校准舵机位置，将所有舵机调整到0度（站立位置）
  * @param  无
  * @retval 无
  */
void servo_adjust(void)
{
  // 0度对应CCR值为1350（根据角度映射公式计算）
  __HAL_TIM_SetCompare(&LEG1_TIM, LEG1_CHANNEL, 1350);
  __HAL_TIM_SetCompare(&LEG2_TIM, LEG2_CHANNEL, 1350);
  __HAL_TIM_SetCompare(&LEG3_TIM, LEG3_CHANNEL, 1350);
  __HAL_TIM_SetCompare(&LEG4_TIM, LEG4_CHANNEL, 1350);
}

/**
  * @brief  舵机功能测试，执行从-60度到60度的往复运动
  * @param  无
  * @retval 无
  */
void servo_test(void)
{
   float angle;
  
  // 测试前先回到0度
  servo_adjust();
  HAL_Delay(600);
  
  // 从0度逐步增加到60度（步长1度）
  for (angle = 0; angle <= 60; angle += 0.5)
  {
    servo_set_angles(angle, angle, angle, angle);
    HAL_Delay(20);  // 延时控制转动速度
  }
  HAL_Delay(100);
  // 从60度逐步减小到-60度（步长1度）
  for (angle = 60; angle >= -60; angle -= 0.5)
  {
    servo_set_angles(angle, angle, angle, angle);
    HAL_Delay(20);  // 延时控制转动速度
  }
  HAL_Delay(100);
  // 从-60度逐步增加到0度（步长1度）
  for (angle = -60; angle <= 0; angle +=0.5)
  {
    servo_set_angles(angle, angle, angle, angle);
    HAL_Delay(20);  // 延时控制转动速度
  }
  
  // 测试结束回到0度
  servo_adjust();
  HAL_Delay(600);
}

/**
  * @brief  设置指定舵机的角度
  * @param  LEGx: 舵机编号（LEG1~LEG4）
  * @param  angle: 目标角度（范围：-90~90度）
  * @retval 1: 操作成功；0: 参数无效或舵机编号错误
  */
uint8_t servo_set_angle(uint8_t LEGx, float angle)
{
  uint16_t CCR;
  float limited_angle;
  
  // 根据舵机编号进行角度限位和方向处理
  switch (LEGx)
  {
    case LEG1:
      // 左前腿：角度取反并限位
      limited_angle = servo_limit_angle(-angle, LEG1_MIN_ANGLE, LEG1_MAX_ANGLE);
      CCR = (uint16_t)(limited_angle * ANGLE_TO_CCR_SCALE + ANGLE_TO_CCR_OFFSET);
      __HAL_TIM_SetCompare(&LEG1_TIM, LEG1_CHANNEL, CCR);
      break;
      
    case LEG2:
      // 右前腿：直接限位
      limited_angle = servo_limit_angle(angle, LEG2_MIN_ANGLE, LEG2_MAX_ANGLE);
      CCR = (uint16_t)(limited_angle * ANGLE_TO_CCR_SCALE + ANGLE_TO_CCR_OFFSET);
      __HAL_TIM_SetCompare(&LEG2_TIM, LEG2_CHANNEL, CCR);
      break;
      
    case LEG3:
      // 左后腿：角度取反并限位
      limited_angle = servo_limit_angle(-angle, LEG3_MIN_ANGLE, LEG3_MAX_ANGLE);
      CCR = (uint16_t)(limited_angle * ANGLE_TO_CCR_SCALE + ANGLE_TO_CCR_OFFSET);
      __HAL_TIM_SetCompare(&LEG3_TIM, LEG3_CHANNEL, CCR);
      break;
      
    case LEG4:
      // 右后腿：直接限位
      limited_angle = servo_limit_angle(angle, LEG4_MIN_ANGLE, LEG4_MAX_ANGLE);
      CCR = (uint16_t)(limited_angle * ANGLE_TO_CCR_SCALE + ANGLE_TO_CCR_OFFSET);
      __HAL_TIM_SetCompare(&LEG4_TIM, LEG4_CHANNEL, CCR);
      break;
      
    default:
      return 0;  // 舵机编号无效
  }
  
  return 1;  // 操作成功
}

/**
  * @brief  同时设置四个舵机的角度（批量操作）
  * @param  a1: 左前腿（LEG1）目标角度
  * @param  a2: 右前腿（LEG2）目标角度
  * @param  a3: 左后腿（LEG3）目标角度
  * @param  a4: 右后腿（LEG4）目标角度
  * @retval 无
  */
void servo_set_angles(float a1, float a2, float a3, float a4) {
  servo_set_angle(LEG1, a1);
  servo_set_angle(LEG2, a2);
  servo_set_angle(LEG3, a3);
  servo_set_angle(LEG4, a4);
}

/* 私有函数实现 | Private functions ------------------------------------------*/

/**
  * @brief  限制角度在指定范围内
  * @param  angle: 原始角度值
  * @param  min: 最小允许角度
  * @param  max: 最大允许角度
  * @retval 限位后的角度值
  */
static float servo_limit_angle(float angle, float min, float max)
{
  if (angle < min)
    return min;
  else if (angle > max)
    return max;
  else
    return angle;
}


