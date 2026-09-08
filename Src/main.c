/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "debug.h"
#include "servo.h"
#include "face.h"
#include "bsp_LED.h"
#include "bsp_uart.h"
#include "OLED.h"
#include "motion.h"
#include "delay.h"
#include "ws2812.h"
#include "HMI.h"
#include "BlueTooth.h"
#include "Myit.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */


/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
volatile uint8_t Myit_f = 0;
volatile uint8_t touch_f = 0;
uint8_t touch_level_last = 0;
uint8_t touch_level_now = 0;

// 要发送的十六进制数值
uint8_t hex_data[] = {0xAA};
size_t data_length = sizeof(hex_data) / sizeof(hex_data[0]);

Face_HandleTypeDef hface;

// 创建运动控制器实例
Motion_HandleTypeDef hMotion;

uint8_t dog_light_f = 1;
uint16_t time_cnt = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void adc_light_process(void);
void Myit_process(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_ADC1_Init();
  MX_SPI2_Init();
  MX_TIM1_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
  
//  __disable_irq();
  HAL_Delay(500);
  
  Face_Init(&hface, NULL);
  
  HAL_ADC_Stop(&hadc1);
  HAL_ADCEx_Calibration_Start(&hadc1);
  HAL_Delay(20);
  HAL_ADC_Start(&hadc1);
  
  OLED_Clear();
  OLED_ShowImage(22, 0, 86, 64, OSHAImg);
  OLED_Update();
  HAL_Delay(800);
  OLED_Clear();
  OLED_ShowImage(38,10,80,80,rosydoedian);
  OLED_Update();
  HAL_Delay(800);
  
  hex_data[0] = 0x99;
  HMI_send_hex_values(hex_data, data_length);
  
  servo_Init();
  HMI_Init();
  BLUETOOTH_Init();
  Myit_Init();
  
//  __enable_irq();

  // 初始化运动控制器
  motion_init(&hMotion);
  
  // 设置运动速度为70（范围0-100）
  motion_set_speed(&hMotion, 70);
  
  // 演示基本姿态切换
  motion_set_pose(&hMotion, POSE_STAND);    // 站立
//  HAL_Delay(1000);                          // 等待1秒
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    BLUETOOTH_process_information();
    HMI_process_information();
    
    Myit_process();
    
    adc_light_process();
    
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void face_test(void)
{
    OLED_ShowImage(0, 0, 128, 64, BMP1);
    OLED_Update();
    HAL_Delay(500);
    OLED_ShowImage(0, 0, 128, 64, BMP2);
    OLED_Update();
    HAL_Delay(500);
    OLED_ShowImage(0, 0, 128, 64, BMP3);
    OLED_Update();
    HAL_Delay(500);
    OLED_ShowImage(0, 0, 128, 64, BMP4);
    OLED_Update();
    HAL_Delay(500);
    OLED_ShowImage(0, 0, 128, 64, BMP5);
    OLED_Update();
    HAL_Delay(500);
    OLED_ShowImage(0, 0, 128, 64, BMP6);
    OLED_Update();
    HAL_Delay(500);
    OLED_ShowImage(0, 0, 128, 64, BMP7);
    OLED_Update();
    HAL_Delay(500);
    OLED_ShowImage(0, 0, 128, 64, BMPaa);
    OLED_Update();
    HAL_Delay(500);
    OLED_ShowImage(0, 0, 128, 64, BMPbb);
    OLED_Update();
    HAL_Delay(500);
    OLED_ShowImage(0, 0, 128, 64, BMPcc);
    OLED_Update();
    HAL_Delay(500);
    OLED_ShowImage(0, 0, 128, 64, BMPdd);
    OLED_Update();
    HAL_Delay(500);
    OLED_ShowImage(0, 0, 128, 64, BMPee);
    OLED_Update();
    HAL_Delay(500);
}
void adc_light_process(void){
    HAL_ADC_PollForConversion(&hadc1, 20);
    printf("LDR ADC : %d,\n", HAL_ADC_GetValue(&hadc1));
    if(HAL_ADC_GetValue(&hadc1) <= 150 && dog_light_f)
    {
      dog_light_f = 0;
      hex_data[0] = 0xAA;
      HMI_send_hex_values(hex_data, data_length);
    }else if(HAL_ADC_GetValue(&hadc1) >= 1000 && !dog_light_f){
      hex_data[0] = 0xDD;
      HMI_send_hex_values(hex_data, data_length);
      OLED_ShowImage(0, 0, 128, 64, BMPcc);
      OLED_Update();
      time_cnt = 0;
      dog_light_f = 1;
    }
}
void Myit_process(void){
    if(Myit_f)
    {
      LED_Toggle();
      Myit_f = 0;
      time_cnt++;
      if(time_cnt >=120)
      {
        time_cnt = 0;
        OLED_ShowImage(0, 0, 128, 64, BMP1);
        OLED_Update();
      }
    }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
