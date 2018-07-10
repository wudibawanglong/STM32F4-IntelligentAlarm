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

void remoteFunction(void);
void keyFunction(void);
int showScreen(void);

u8 screen=0;
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
		keyFunction();
		remoteFunction();
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

//按键
void keyFunction() 
{
	u8 key=KEY_Scan(0);		//得到键值
	if(key)
	{						   
		switch(key)
		{				 
			case WKUP_PRES:
				if(screen==0){
					GPIO_SetBits(GPIOB,GPIO_Pin_15);   //关闭屏幕
					screen=1;
				}else {
					GPIO_ResetBits(GPIOB,GPIO_Pin_15);
					screen=0;
				}
				break;
			case KEY1_PRES:	//控制LED0翻转	 
				if(LED0==0)
					GPIO_SetBits(GPIOF,GPIO_Pin_9);	   //LED0对应引脚GPIOF.0拉高，灭  等同LED0=1;
				else GPIO_ResetBits(GPIOF,GPIO_Pin_9);  //LED0对应引脚GPIOF.9拉低，亮  等同LED0=0;
				break; 
			case KEY2_PRES:	//控制蜂鸣器
				if(BEEP==0)
					GPIO_SetBits(GPIOF,GPIO_Pin_8);   //BEEP引脚拉高， 等同BEEP=1;	
				else GPIO_ResetBits(GPIOF,GPIO_Pin_8); //BEEP引脚拉低， 等同BEEP=0;			
				break;
			default:break;
		}
	}else delay_ms(10); 
}

//红外
void remoteFunction(void)
{	
	u8 key=Remote_Scan();	
	if(key)
	{  
		switch(key)
		{		    
			case 226:
				if(screen==0){
					GPIO_SetBits(GPIOB,GPIO_Pin_15);   //关闭屏幕
					screen=1;
				}else {
					GPIO_ResetBits(GPIOB,GPIO_Pin_15);
					screen=0;
				}
				break;	    
			case 104:
				if(LED0==0)
					LED0=1;	   //LED0对应引脚GPIOF.0拉高，灭
				else LED0=0;  //LED0对应引脚GPIOF.9拉低，亮
				break;		  
			case 152:
				if(BEEP==0)
					BEEP=1;   //BEEP引脚拉高	
				else BEEP=0; //BEEP引脚拉低
				break;	   	 
			default:break;
		}
	}
	delay_ms(10);
}