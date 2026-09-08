#include "ws2812.h"
#include "delay.h"


/*
							接线方法						
							GND→GND
							VCC→3.3V/5V
							IN→PA0
							
如果灯的亮度或颜色显示异常，可以检查电源是否为标准电压以及接线是否牢固

*/

void ws2812_init(void)//初始化
{
	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//开启时钟
//	
//	GPIO_InitTypeDef GPIO_InitStructure;//结构体变量
//	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_Out_PP;//推挽输出
//	GPIO_InitStructure.GPIO_Pin= GPIO_Pin_0;//设置端口
//	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;//输出速度
//	GPIO_Init(GPIOA,&GPIO_InitStructure);//初始化
//	GPIO_SetBits(GPIOA,GPIO_Pin_0);
		
}


uint8_t ws_data[200]={0};
void ws2812_rgb(uint8_t ws_i,uint8_t ws_r,uint8_t ws_g,uint8_t ws_b)	
{
	ws_data[(ws_i-1)*3]=ws_g;
	ws_data[(ws_i-1)*3+1]=ws_r;
	ws_data[(ws_i-1)*3+2]=ws_b;
}

void ws2812_rgb_all(uint8_t ws_i,uint8_t ws_r,uint8_t ws_g,uint8_t ws_b)	
{
	static uint8_t rgb_wsi;
	for(rgb_wsi=1;rgb_wsi<=ws_i;rgb_wsi++)
	{
		ws_data[(rgb_wsi-1)*3]=ws_g;
		ws_data[(rgb_wsi-1)*3+1]=ws_r;
		ws_data[(rgb_wsi-1)*3+2]=ws_b;
	}
}

void ws2812_refresh(uint8_t ws_i)
{
	uint8_t ws_ri=0;
	for(;ws_ri<ws_i*3;ws_ri++)
	{
		if((ws_data[ws_ri]&0x80)==0)	send_0();	else send_1();
		if((ws_data[ws_ri]&0x40)==0)	send_0();	else send_1();
		if((ws_data[ws_ri]&0x20)==0)	send_0();	else send_1();
		if((ws_data[ws_ri]&0x10)==0)	send_0();	else send_1();
		if((ws_data[ws_ri]&0x08)==0)	send_0();	else send_1();
		if((ws_data[ws_ri]&0x04)==0)	send_0();	else send_1();
		if((ws_data[ws_ri]&0x02)==0)	send_0();	else send_1();
		if((ws_data[ws_ri]&0x01)==0)	send_0();	else send_1();
	}
	send_res();
}

void ws2812_reset(uint8_t ws_set)
{
	for(int i=0;i<200;i++)
	{
		ws_data[i]=ws_set;
	}
}

void delay_250ns(void)
{
	uint8_t del_t=WS_TIMES;
	while(del_t--)
		__NOP();
}

void send_0(void)
{
	TH;
	delay_250ns();
	TL;
	delay_250ns();
	delay_250ns();
	delay_250ns();
	delay_250ns();
}
void send_1(void)
{
	TH;
	delay_250ns();
	delay_250ns();
	delay_250ns();
	delay_250ns();
	TL;
	delay_250ns();
}
void send_res(void)
{
	TL;
	Delay_us_TIM3(300);
}

