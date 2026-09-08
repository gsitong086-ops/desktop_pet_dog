#include "Servo.h"
//#include "BlueTooth.h"

	/*舵机位置
	    1        2
	
	

	
	    3        4
	*/
	
	//舵机充当腿，舵机为90度时表示的是腿向前进的方向甩，0度站立
//Number of repetitions of action
#define Chongfunumber 2  //动作重复次数、前进后退左转右转
#define SwingRepeatnumber 3  //摇摆重复次数
#define HelloRepeatnumber 4  //打招呼重复次数

uint16_t PAnumbers=Chongfunumber;//动作重复次数

uint16_t TiaoTurn=0;
uint16_t TiaoTurn2=0;

uint16_t AllLed=1;  //开启灯光
uint16_t BreatheLed=0;//开启呼吸灯
uint16_t Sustainedmove=0;//持续运动


uint16_t Action_Mode=0;
uint16_t SpeedDelay=200;
uint16_t SwingDelay=6;
uint16_t Face_Mode=0;


void Action_relaxed_getdowm(void)
{
	servo_set_angle(LEG1,70);
	servo_set_angle(LEG2,70);
	HAL_Delay(80);
	servo_set_angle(LEG3,70);
	servo_set_angle(LEG4,70);
}

void Action_upright(void)//站立
{
	servo_set_angle(LEG1,0);
	servo_set_angle(LEG2,0);
	HAL_Delay(80);
	servo_set_angle(LEG3,0);
	servo_set_angle(LEG4,0);
	
}

void Action_upright2(void)//站立
{
	servo_set_angle(LEG3,0);
	servo_set_angle(LEG4,0);
	HAL_Delay(80);
	servo_set_angle(LEG1,0);
	servo_set_angle(LEG2,0);
	
}


void Action_getdowm(void)//趴下,腿朝后
{
	servo_set_angle(LEG1,-70);
	servo_set_angle(LEG2,-70);
	HAL_Delay(80);
	servo_set_angle(LEG3,-70);
	servo_set_angle(LEG4,-70);
	
}

void Action_sit(void)//坐下
{
	servo_set_angle(LEG1,0);
	servo_set_angle(LEG2,0);
	HAL_Delay(80);
	servo_set_angle(LEG3,70);
	servo_set_angle(LEG4,70);
	
//	if(WeiBa==1)
//	{
//		Action_Mode=9;
//	}
}



void Action_advance(void)//前进
{

	while(Action_Mode==4)
	{
		PAnumbers=Chongfunumber;
			while((PAnumbers || Sustainedmove)&& Action_Mode==4)
			{
				servo_set_angle(LEG2,45);	
				servo_set_angle(LEG3,45);
				HAL_Delay(SpeedDelay);
				if(Action_Mode!=4)break;
				servo_set_angle(LEG1,135);	
				servo_set_angle(LEG4,135);
				HAL_Delay(SpeedDelay);
				if(Action_Mode!=4)break;
				servo_set_angle(LEG2,90);	
				servo_set_angle(LEG3,90);
				HAL_Delay(SpeedDelay);
				if(Action_Mode!=4)break;
				servo_set_angle(LEG1,90);	
				servo_set_angle(LEG4,90);
				HAL_Delay(SpeedDelay);
				if(Action_Mode!=4)break;
		
				servo_set_angle(LEG1,45);
				servo_set_angle(LEG4,45);
				HAL_Delay(SpeedDelay);
				if(Action_Mode!=4)break;
				servo_set_angle(LEG2,135);
				servo_set_angle(LEG3,135);
				HAL_Delay(SpeedDelay);
				if(Action_Mode!=4)break;
				servo_set_angle(LEG1,90);
				servo_set_angle(LEG4,90);
				HAL_Delay(SpeedDelay);
				if(Action_Mode!=4)break;
				servo_set_angle(LEG2,90);	
				servo_set_angle(LEG3,90);
				HAL_Delay(SpeedDelay);
				if(Action_Mode!=4)break;
				
				PAnumbers--;
			}
			if(Sustainedmove!=1 && Action_Mode==4)
				Action_Mode=2;
	}
}

void Action_back(void)//后退
{
	while(Action_Mode==5)
	{
		PAnumbers=Chongfunumber;
		while((PAnumbers || Sustainedmove) && Action_Mode==5 )
		{
			servo_set_angle2(135);	
			servo_set_angle3(135);
			HAL_Delay(SpeedDelay);
			if(Action_Mode!=5)break;
			servo_set_angle(LEG1,45);	
			servo_set_angle4(45);
			HAL_Delay(SpeedDelay);
			if(Action_Mode!=5)break;
			servo_set_angle2(90);	
			servo_set_angle3(90);
			HAL_Delay(SpeedDelay);
			if(Action_Mode!=5)break;
			servo_set_angle(LEG1,90);	
			servo_set_angle4(90);
			HAL_Delay(SpeedDelay);
			if(Action_Mode!=5)break;
			
			servo_set_angle(LEG1,135);	
			servo_set_angle4(135);
			HAL_Delay(SpeedDelay);
			if(Action_Mode!=5)break;
			servo_set_angle2(45);	
			servo_set_angle3(45);
			HAL_Delay(SpeedDelay);
			if(Action_Mode!=5)break;
			servo_set_angle(LEG1,90);	
			servo_set_angle4(90);
			HAL_Delay(SpeedDelay);
			if(Action_Mode!=5)break;
			servo_set_angle2(90);	
			servo_set_angle3(90);
			HAL_Delay(SpeedDelay);
			if(Action_Mode!=5)break;
			
			PAnumbers--;
		}
		if(Sustainedmove!=1 && Action_Mode==5)
		Action_Mode=2;

		
	}
}

void Action_Lrotation(void)//向左旋转
{
	while(Action_Mode==6)
	{
		PAnumbers=Chongfunumber;
		PAnumbers=PAnumbers+Chongfunumber;
		while((PAnumbers || Sustainedmove) && Action_Mode==6)
		{
			servo_set_angle2(45);
			servo_set_angle3(135);
			HAL_Delay(SpeedDelay);
			if(Action_Mode!=6)break;
			servo_set_angle(LEG1,45);
			servo_set_angle4(135);
			HAL_Delay(SpeedDelay);
			if(Action_Mode!=6)break;
			servo_set_angle2(90);
			servo_set_angle3(90);	
			HAL_Delay(SpeedDelay);
			if(Action_Mode!=6)break;
			servo_set_angle(LEG1,90);
			servo_set_angle4(90);	
			HAL_Delay(SpeedDelay);
			if(Action_Mode!=6)break;
			
			PAnumbers--;
		}
		if(Sustainedmove!=1 && Action_Mode==6)
		Action_Mode=2;
	}

}


void Action_Rrotation(void)//向右旋转
{
	while(Action_Mode==7)
	{
		PAnumbers=Chongfunumber;
		PAnumbers=PAnumbers+Chongfunumber;
		while((PAnumbers || Sustainedmove)  && Action_Mode==7)
		{
			servo_set_angle(LEG1,45);
			servo_set_angle4(135);
			HAL_Delay(SpeedDelay);
			if(Action_Mode!=7)break;
			servo_set_angle2(45);
			servo_set_angle3(135);
			HAL_Delay(SpeedDelay);
			if(Action_Mode!=7)break;
			servo_set_angle(LEG1,90);
			servo_set_angle4(90);	
			HAL_Delay(SpeedDelay);
			if(Action_Mode!=7)break;
			servo_set_angle2(90);
			servo_set_angle3(90);	
			HAL_Delay(SpeedDelay);
			if(Action_Mode!=7)break;
			
			PAnumbers--;
		}
		if(Sustainedmove!=1 && Action_Mode==7)
		Action_Mode=2;
	}

}

void Action_Swing(void)//摇摆
{
	uint16_t SwingNumber=SwingRepeatnumber;
	while(SwingNumber && Action_Mode==8)
	{
		for(uint8_t i=30;i<150;i++)
		{
			servo_set_angle(LEG1,i);
			servo_set_angle2(i);
			servo_set_angle3(i);
			servo_set_angle4(i);
			HAL_Delay(SwingDelay);
			if(Action_Mode!=8)break;
		}
		if(Action_Mode!=8)break;
		for(uint8_t i=150;i>30;i--)
		{
			servo_set_angle(LEG1,i);
			servo_set_angle2(i);
			servo_set_angle3(i);
			servo_set_angle4(i);
			HAL_Delay(SwingDelay);
			if(Action_Mode!=8)break;
		}
		if(Action_Mode!=8)break;
		
		SwingNumber--;
	}
		for(uint8_t i=30;i<90;i++)
		{
			servo_set_angle(LEG1,i);
			servo_set_angle2(i);
			servo_set_angle3(i);
			servo_set_angle4(i);
			HAL_Delay(SwingDelay);
			if(Action_Mode!=8)break;
		}
		if(Action_Mode==8)
		Action_Mode=2;
}


void Action_SwingTail(void)//摇尾巴
{
	uint16_t SwingTailNumber=3;
	HAL_Delay(60);
	while(SwingTailNumber && Action_Mode==9)
	{
		for(uint8_t i=30;i<150;i++)
		{
			Wservo_set_angle(i);
			HAL_Delay(SwingDelay);
			if(Action_Mode!=9)break;
		}
		if(Action_Mode!=9)break;
		for(uint8_t i=150;i>30;i--)
		{
			Wservo_set_angle(i);
			HAL_Delay(SwingDelay);
			if(Action_Mode!=9)break;
		}
		if(Action_Mode!=9)break;
		SwingTailNumber--;
	}
	HAL_Delay(60);
}

void Action_JumpU(void)//向前跳
{
	if(TiaoTurn==0)
	{
		servo_set_angle(LEG1,140);
		servo_set_angle4(35);
		HAL_Delay(SpeedDelay);
		
		servo_set_angle2(140);
		servo_set_angle3(35);
		HAL_Delay(SpeedDelay+80);
		
		Action_Mode=2;
		TiaoTurn=1;
	}
	else
	{
		servo_set_angle2(140);
		servo_set_angle3(35);
		HAL_Delay(SpeedDelay);
		
		servo_set_angle(LEG1,140);
		servo_set_angle4(35);
		HAL_Delay(SpeedDelay+80);
		
		Action_Mode=2;
		TiaoTurn=0;
	}
}

void Action_JumpD(void)//向后跳
{
	if(TiaoTurn2==0){
		servo_set_angle4(35);
		servo_set_angle(LEG1,140);
		HAL_Delay(SpeedDelay);
	
		servo_set_angle3(35);
		servo_set_angle2(140);
		HAL_Delay(SpeedDelay);
	
		Action_Mode=12;
		TiaoTurn2=1;
	}
	else
	{
		servo_set_angle3(35);
		servo_set_angle2(140);
		HAL_Delay(SpeedDelay);
	
		servo_set_angle4(35);
		servo_set_angle(LEG1,140);
		HAL_Delay(SpeedDelay);
	
		Action_Mode=12;
		TiaoTurn2=0;
	}
}

void Action_Hello(void)
{
	uint16_t HelloNumber=HelloRepeatnumber;
	
	servo_set_angle3(20);
	servo_set_angle4(45);
	HAL_Delay(80);
	servo_set_angle(LEG1,90);
	while(HelloNumber && Action_Mode==13)
	{
		if(Action_Mode!=13)break;
		for(int i=0;i<=45;i++)
		{
			if(Action_Mode!=13)break;
			servo_set_angle2(i);
			HAL_Delay(SwingDelay);
		}
		for(int i=45;i>0;i--)
		{
			if(Action_Mode!=13)break;
			servo_set_angle2(i);
			HAL_Delay(SwingDelay);
		}
		if(Action_Mode!=13)break;
		
		HelloNumber--;
	}
	if(Action_Mode==13)
	Action_Mode=2;
}

void Action_stretch(void)//伸懒腰
{
	servo_set_angle3(90);
	servo_set_angle4(90);
	HAL_Delay(80);
	for(int i=90;i>10;i--)
	{
		servo_set_angle(LEG1,i);
		servo_set_angle2(i);
		if(Action_Mode!=14)break;
		HAL_Delay(15);
	}
	for(int i=10;i<90;i++)
	{
		servo_set_angle(LEG1,i);
		servo_set_angle2(i);
		if(Action_Mode!=14)break;
		HAL_Delay(15);
	}
	for(int i=90;i<170;i++)
	{
		servo_set_angle3(i);
		servo_set_angle4(i);
		if(Action_Mode!=14)break;
		HAL_Delay(15);
	}
	
	for(int i=170;i>90;i--)
	{
		servo_set_angle3(i);
		servo_set_angle4(i);
		if(Action_Mode!=14)break;
		HAL_Delay(15);
	}
	if(Action_Mode==14)
	Action_Mode=15;
}

void Action_Lstretch(void)//后腿拉伸
{
	int breakvalue=1;
	int temp=3;
	while(breakvalue)
	{
		servo_set_angle(LEG1,90);
		servo_set_angle2(20);
		HAL_Delay(60);
		servo_set_angle4(110);
		for(int i=90;i<180;i++)
		{
			if(Action_Mode!=15)break;
			servo_set_angle3(i);
			HAL_Delay(6);
		}
		while(temp && Action_Mode==15)
		{
			for(int i=180;i>150;i--)
			{
				if(Action_Mode!=15)break;
				servo_set_angle3(i);
				HAL_Delay(15);
			}
			temp--;
		}
		if(Action_Mode!=15)break;
		HAL_Delay(100);
		servo_set_angle(LEG1,90);
		servo_set_angle2(90);
		if(Action_Mode!=15)break;
		HAL_Delay(80);
		servo_set_angle3(90);
		servo_set_angle4(90);
		HAL_Delay(100);
		if(Action_Mode!=15)break;
		
		temp=3;
		
		servo_set_angle2(90);
		servo_set_angle(LEG1,20);
		if(Action_Mode!=15)break;
		HAL_Delay(60);
		servo_set_angle3(110);
		for(int i=90;i<180;i++)
		{
			if(Action_Mode!=15)break;
			servo_set_angle4(i);
			HAL_Delay(6);
		}
		while(temp && Action_Mode==15)
		{
			for(int i=180;i>150;i--)
			{
				if(Action_Mode!=15)break;
				servo_set_angle4(i);
				HAL_Delay(15);
			}
			temp--;
		}
		if(Action_Mode==15)
		Action_Mode=2;
		
		
		breakvalue=0;
	}
}
