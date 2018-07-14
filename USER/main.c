#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "adc.h"
#include "beep.h"
#include "key.h"
#include "remote.h"
#include "smog.h"
#include "adc3.h"
#include "timer.h"

int showScreen(void);

int main(void)
{    
	short temp;
	u16 adcx;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);     //初始化延时函数
	uart_init(115200);	 //初始化串口波特率为115200
	BEEP_Init();        //初始化蜂鸣器
	LED_Init();					//初始化LED 
 	LCD_Init();         //液晶初始化
	Adc_Init();         //内部温度传感器ADC初始化 
 	Remote_Init();				//红外接收初始化		 
	Smog_Init();
 	TIM3_Int_Init(5000-1,8400-1);	//定时器时钟84M，分频系数8400，所以84M/8400=10Khz的计数频率，计数5000次为500ms  
	LED1=0;				  	//先点亮绿灯
  showScreen();	
	
	while(1)
	{
		temp=Get_Temprate();	//得到温度值 
		if(temp<0)
		{
			temp=-temp;
			LCD_ShowString(30+10*8,140,16,16,16,"-");	    //显示负号
		}else LCD_ShowString(30+10*8,140,16,16,16," ");	//无符号
		LCD_ShowxNum(30+11*8,140,temp/100,2,16,0);		//显示整数部分
		LCD_ShowxNum(30+14*8,140,temp%100,2,16,0);		//显示小数部分 
		adcx=Smog_Get_Vol();
		LCD_ShowxNum(30+10*8,160,adcx,5,16,0);//显示ADC的值 
	
		if(adcx>=1000){
			BEEP=1;
		}else{
			BEEP=0;
		}
	}
}

int showScreen(void)
{
	POINT_COLOR=RED; 
	LCD_ShowString(30,50,200,16,16,"Explorer STM32F4");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2018/7");	  
	POINT_COLOR=BLUE;//设置字体为蓝色  
	LCD_ShowString(30,140,200,16,16,"TEMPERATE: 00.00C");//先在固定位置显示小数点
	LCD_ShowString(30,140,200,16,16,"TEMPERATE: 00.00C");//先在固定位置显示小数点
	
	LCD_ShowString(30,160,200,16,16,"smokescope:");//先在固定位置显示小数点
	LCD_ShowString(30+16*8,160,200,16,16,"ppm");//先在固定位置显示小数点
	return 0;
}