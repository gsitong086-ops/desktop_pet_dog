#ifndef __DELAY_H
#define __DELAY_H

#include "main.h"

// 函数声明
void delay_us_run(uint16_t us);
void delay_ms_run(uint16_t ms);
void Delay_us_TIM3(uint16_t nus);
void Delay_ms_TIM3(uint16_t ms);
  
#endif // __DELAY_H
