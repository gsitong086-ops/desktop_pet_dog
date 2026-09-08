#ifndef WS2812_h__
#define WS2812_h__
#include "main.h"                  // Device header

//端口修改处
#define WS_TIMES 1//和芯片主频有关，stm32f103用1，多试下，或者有条件的用示波器看下 
#define TH	HAL_GPIO_WritePin(flow_LED_GPIO_Port, flow_LED_Pin, GPIO_PIN_SET);
#define TL	HAL_GPIO_WritePin(flow_LED_GPIO_Port, flow_LED_Pin, GPIO_PIN_RESET);

//RGB--定义了几个标准的rgb值 
#define WS_DARK 	0,0,0            //灭
#define WS_WHITE 	255,255,255		 //白色
#define WS_RED 		255,0,0          //红色
#define WS_GREEN 	0,255,0          //绿色
#define WS_BLUE 	0,0,255          //蓝色
#define WS_YELLO 	255,255,0        //黄色
#define WS_PURPLE   255,0,255        //紫色
#define WS_CYAN 	0,255,255        //青色

//extern uint8_t ws_data[];如果你需要在其他地方用到这个数组，就不注释 

void ws2812_init(void);//WS2812初始化
void ws2812_rgb(uint8_t ws_i,uint8_t ws_r,uint8_t ws_g,uint8_t ws_b);//灯位置，颜色
void ws2812_rgb_all(uint8_t ws_i,uint8_t ws_r,uint8_t ws_g,uint8_t ws_b);//几个灯，颜色
void ws2812_refresh(uint8_t ws_i);//刷新led数据（用几个灯就刷新几个）
void ws2812_reset(uint8_t ws_set);//ws2812_reset(0);//把所有值清零


void delay_250ns(void);
void send_0(void);
void send_1(void);
void send_res(void);

#endif 
