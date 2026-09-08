#include "delay.h"
#include "tim.h"

//在 stm32 内执行任何一条指令是需要消耗时间的。
//实现延时效果可以让 stm32 持续执行一段计数循环，直到其计数到设定的数目后，再让 stm32 退出循环
void delay_us_run(uint16_t us)
{
  for(; us > 0; us--)
  {
    for(uint8_t i = 50; i > 0; i--)
    {
      ;
    }
  }
}

void delay_ms_run(uint16_t ms)
{
  for(; ms > 0; ms--)
  {
    delay_us_run(1000);
  }
}


/**
 * @brief  使用TIM3实现微秒级延时
 * @param  nus: 需要延时的微秒数 (范围: 0 ~ 65535)
 * @retval None
 */
void Delay_us_TIM3(uint16_t nus)
{
    // 确保定时器处于停止状态
    HAL_TIM_Base_Stop(&htim3);
    
    // 设置计数器初始值为0
    __HAL_TIM_SetCounter(&htim3, 0);
    
    // 开启定时器
      HAL_TIM_Base_Start(&htim3);
    
    // 等待计数器值达到nus（每个计数代表1us）
    while (__HAL_TIM_GetCounter(&htim3) < nus);
    
    // 关闭定时器
    HAL_TIM_Base_Stop(&htim3);
}

void Delay_ms_TIM3(uint16_t ms)
{
  for(; ms > 0; ms--)
  {
    Delay_us_TIM3(1000);
  }
}

