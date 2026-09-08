#include "Myit.h"
#include "bsp_LED.h"
#include "OLED.h"
#include "HMI.h"

extern TIM_HandleTypeDef htim4;
extern volatile uint8_t Myit_f;
volatile uint8_t touch_EXTI_f = 0;
extern volatile uint8_t touch_f;

extern uint8_t touch_level_last ;
extern uint8_t touch_level_now ;
extern uint16_t time_cnt;
void Myit_Init(void)
{
  HAL_TIM_Base_Start_IT(&MYIT_1_TIM);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if(htim == &MYIT_1_TIM)
  {
    Myit_f = 1;
    MYIT_1_FLAG = 1;
//    if(touch_EXTI_f)
//    {
//      touch_EXTI_f = 0;
//      touch_level_now = HAL_GPIO_ReadPin(touch_GPIO_Port, touch_Pin);
////      if(touch_level_now != touch_level_last)
////      {
////        touch_time ++;
////        
////        touch_level_last = touch_level_now;
////      }else{
////        touch_time = 0;
////        touch_level_last = HAL_GPIO_ReadPin(touch_GPIO_Port, touch_Pin);
////      }
//    }
  }
}


/**
  * 函数功能: 按键外部中断回调函数
  * 输入参数: GPIO_Pin：中断引脚
  * 返 回 值: 无
  * 说    明: 无
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin ==touch_Pin)
  {
//    LED_Toggle();
    if(HAL_GPIO_ReadPin(touch_GPIO_Port, touch_Pin))
    {
      uint8_t hex_data = 0xBB;
      HMI_send_hex_values(&hex_data, 1);
      OLED_ShowImage(0, 0, 128, 64, BMP6);
      OLED_Update();
      time_cnt = 0;
    }
    touch_EXTI_f = 1;
  }
}

