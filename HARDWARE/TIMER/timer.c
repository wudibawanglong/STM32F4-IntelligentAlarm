#include "timer.h"
#include "led.h"
#include "delay.h"
#include "led.h"
#include "lcd.h"
#include "beep.h"
#include "key.h"
#include "remote.h"

void remoteFunction(void);
void keyFunction(void);

//通用定时器3中断初始化
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器3!
void TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///使能TIM3时钟
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//初始化TIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	TIM_Cmd(TIM3,ENABLE); //使能定时器3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

u8 screen=0;
//定时器3中断服务函数
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //溢出中断
	{
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //清除中断标志位
		keyFunction();
		remoteFunction();
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //清除中断标志位
}



//按键
void keyFunction() 
{
	u8 key=KEY_Scan(0);		//得到键值
	if(key)
	{						   
		switch(key)
		{				 
			case KEY0_PRES:
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
	}
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
}