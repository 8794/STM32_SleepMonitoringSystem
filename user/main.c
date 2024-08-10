/**
	************************************************************
	************************************************************
	************************************************************
	*	文件名： 	main.c
	*
	*	作者： 		张继瑞
	*
	*	日期： 		2017-05-08
	*
	*	版本： 		V1.0
	*
	*	说明： 		接入onenet，上传数据和命令控制
	*
	*	修改记录：	
	************************************************************
	************************************************************
	************************************************************
**/

//单片机头文件
#include "stm32f10x.h"

//网络协议层
#include "onenet.h"

//网络设备
#include "esp8266.h"

//硬件驱动
#include "delay.h"
#include "usart.h"
#include "Bed.h"
#include "led.h"
#include "key.h"
#include "dht11.h"
#include "OLED.h"
//C库
#include <string.h>


#define ESP8266_ONENET_INFO		"AT+CIPSTART=\"TCP\",\"mqtts.heclouds.com\",1883\r\n"


/*
************************************************************
*	函数名称：	Hardware_Init
*
*	函数功能：	硬件初始化
*
*	入口参数：	无
*
*	返回参数：	无
*
*	说明：		初始化单片机功能以及外接设备
************************************************************
*/
void Hardware_Init(void)
{
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//中断控制器分组设置

	Delay_Init();									//systick初始化
	
	Usart3_Init(115200);				//串口3，打印信息用
	
	Bed_Init();										//使用串口1
	
	Usart2_Init(115200);					//串口2，驱动ESP8266用
	
	Key_Init();										//按键初始化
	
	Led_Init();										//LED初始化
	//温湿度初始化
//	while(DHT11_Init())
//	{
//		UsartPrintf(USART_DEBUG, "DHT11 Error \r\n");
//		DelayMs(1000);
//	}
	
	UsartPrintf(USART_DEBUG, " Hardware init OK\r\n");
	
	OLED_Init();
	
}

/*
************************************************************
*	函数名称：	main
*
*	函数功能：	
*
*	入口参数：	无
*
*	返回参数：	0
*
*	说明：		
************************************************************
*/


//数据：温度、湿度、状态、   呼吸率、        心率
uint8_t temp,humi,Status,RespiratoryRate,heartHate;


int main(void)
{
	
	unsigned short timeCount = 0;	//发送间隔变量
	
	unsigned char *dataPtr = NULL;
	
	Hardware_Init();				//初始化外围硬件
	
	ESP8266_Init();					//初始化ESP8266
//	
//	OneNET_RegisterDevice();
//	
	UsartPrintf(USART_DEBUG, "Connect MQTTs Server...\r\n");
	while(ESP8266_SendCmd(ESP8266_ONENET_INFO, "CONNECT"))
		DelayXms(500);
	UsartPrintf(USART_DEBUG, "Connect MQTTs Server Succeed\r\n");
	
	while(OneNet_DevLink())			//接入OneNET
		DelayXms(500);
	
	OneNET_Subscribe();//订阅消息，用于数据上报，即通过软件发送数据给云端来控制硬件

	OLED_ShowString(1, 1, "Status:");
	OLED_ShowString(2, 1, "Rrate:");
	OLED_ShowString(3, 1, "Hhate:");
	OLED_ShowString(4, 1, "temp:");
	OLED_ShowString(4, 9, "humi:");
	while(1)
	{
		
		if(++timeCount >= 100)									//发送间隔0.1s
		{
			DHT11_Read_Data(&temp,&humi);//获取温湿度数据
			UsartPrintf(USART_DEBUG, "temp=%d humi=%d \r\n",temp,humi);
			OLED_ShowNum(4,6,temp,2);
			OLED_ShowNum(4,15,humi,2);
			if (Bed_GetRxFlag() == 1)	//判断是否有睡眠带数据传来
			{
				UsartPrintf(USART_DEBUG, "Status\r\n");
				Status=Bed_RxPacket[62];
				heartHate=Bed_RxPacket[63];
				RespiratoryRate=Bed_RxPacket[64];
				
					//睡眠带数据解析
				Bed_Data(&Status,&heartHate,&RespiratoryRate);
				UsartPrintf(USART_DEBUG, "Status=%d heartHate=%d  RespiratoryRate=%d\r\n",Status,heartHate,RespiratoryRate);
				OLED_ShowNum(1,8,Status,2);
				OLED_ShowNum(2,8,heartHate,2);
				OLED_ShowNum(3,8,RespiratoryRate,2);

			}

			
//			UsartPrintf(USART_DEBUG, "OneNet_SendData\r\n");
			OneNet_SendData();									//发送数据
			
			timeCount = 0;
			ESP8266_Clear();
		}
		
		dataPtr = ESP8266_GetIPD(0);
		if(dataPtr != NULL)
			OneNet_RevPro(dataPtr);
		//UsartPrintf(USART_DEBUG, " Hardware init OK\r\n");
		DelayXms(10);
	
	}

}
