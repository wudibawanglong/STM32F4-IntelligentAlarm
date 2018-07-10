#include "smog.h"
#include "delay.h"
#include "usart.h"
#include "adc3.h"

//初始化烟雾传感器
void Smog_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);//使能PORTF时钟	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;			//PC0 anolog输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;		//模拟输入引脚

	GPIO_Init(GPIOF, &GPIO_InitStructure);	
	Adc3_Init();
}
//读取烟雾传感器的电压值
u16 Smog_Get_Vol(void)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<SMOG_READ_TIMES;t++)
	{
		temp_val+=Get_Adc3(ADC_Channel_5);	//读取ADC值
		delay_ms(5);
	}
	temp_val/=SMOG_READ_TIMES;//得到平均值
	printf("Smog_ADC_Val:%d\r\n", temp_val);
    return (u16)temp_val;
}
//分析从烟雾传感器获取的电压值，通过公式计算出可燃气体的浓度值
//设Rs为传感器的体电阻，其中气体浓度上升，将导致Rs下降。而Rs的下降则会导致，MQ-2的4脚、6脚对地输出的电压增大。
//所以气体浓度增大，其输出的电压也会增大。因Rs在不同气体中有不同的浓度值，所以该函数仅作为参考.
u16 Smog_Trans_Concentration(void)
{
    u16 ulVal = 0;
    u16 temp_val = Smog_Get_Vol();
	u16 Rs;
	Rs = SMOG_PIN46_R*(4096.0/temp_val - 1);
	printf("Smog_Rs_Val:%d\r\n", Rs);
	
	ulVal = 5000+15000.0/4096.0*temp_val;//这里填写公式根据Rs计算气体浓度
	
    return ulVal;
}

