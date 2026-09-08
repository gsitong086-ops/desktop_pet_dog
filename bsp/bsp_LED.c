#include "bsp_LED.h"

void  LED_On(void)
{
  HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET);
}

void  LED_Off(void)
{
  HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_RESET);
}

void  LED_Toggle(void)
{                             
  HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
}

