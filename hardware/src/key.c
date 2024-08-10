//按键头文件
#include "key.h"

//硬件驱动
#include "stm32f10x.h"                  // Device header
#include "led.h"
#include "delay.h"

/*
************************************************************
*	函数名称：	Key_Init
*
*	函数功能：	按键IO初始化
*
*	入口参数：	无
*
*	返回参数：	无
*
*	说明：		SW2-PD2		SW3-PC11	SW4-PC12	SW5-PC13	
*				按下为低电平		释放为高电平
************************************************************
*/
void Key_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;
	GPIO_InitTypeDef gpio_initstruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//打开GPIOA的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	gpio_initstruct.GPIO_Mode = GPIO_Mode_IPU;									//设置为上拉输入模式
	gpio_initstruct.GPIO_Pin = GPIO_Pin_1;			//初始化Pin1
	gpio_initstruct.GPIO_Speed = GPIO_Speed_50MHz;								//承载的最大频率
	GPIO_Init(GPIOA, &gpio_initstruct);											//初始化GPIOC
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource1);
	
	
	EXTI_InitStruct.EXTI_Line=EXTI_Line1;
	EXTI_InitStruct.EXTI_LineCmd=ENABLE;
	EXTI_InitStruct.EXTI_Mode= EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStruct);
	
	
	
	NVIC_InitStruct.NVIC_IRQChannel=EXTI1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=2;
	NVIC_Init(&NVIC_InitStruct);

}

void EXTI1_IRQHandler(void)
{
	DelayXms(10);
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==0)
	{
		if(led_info.Led_Status==LED_ON)
		{
			Led_Set(LED_OFF);
		}
		else
			Led_Set(LED_ON);
	}
	EXTI_ClearITPendingBit(EXTI_Line1);
}
















