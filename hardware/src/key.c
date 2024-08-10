//����ͷ�ļ�
#include "key.h"

//Ӳ������
#include "stm32f10x.h"                  // Device header
#include "led.h"
#include "delay.h"

/*
************************************************************
*	�������ƣ�	Key_Init
*
*	�������ܣ�	����IO��ʼ��
*
*	��ڲ�����	��
*
*	���ز�����	��
*
*	˵����		SW2-PD2		SW3-PC11	SW4-PC12	SW5-PC13	
*				����Ϊ�͵�ƽ		�ͷ�Ϊ�ߵ�ƽ
************************************************************
*/
void Key_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;
	GPIO_InitTypeDef gpio_initstruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//��GPIOA��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	gpio_initstruct.GPIO_Mode = GPIO_Mode_IPU;									//����Ϊ��������ģʽ
	gpio_initstruct.GPIO_Pin = GPIO_Pin_1;			//��ʼ��Pin1
	gpio_initstruct.GPIO_Speed = GPIO_Speed_50MHz;								//���ص����Ƶ��
	GPIO_Init(GPIOA, &gpio_initstruct);											//��ʼ��GPIOC
	
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
















