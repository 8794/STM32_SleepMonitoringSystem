#include "stm32f10x.h"                  // Device header

#include <stdio.h>
#include <stdarg.h>


//uint8_t Serial_TxPacket[8];				//FF 01 02 03 04 FE
//uint8_t Serial_RxPacket[8];
//uint8_t Serial_RxFlag;


//void Uart3_Init(void)
//{
//	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//	
//	GPIO_InitTypeDef GPIO_InitStructure;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
//	
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
//	
//	USART_InitTypeDef USART_InitStructure;
//	USART_InitStructure.USART_BaudRate = 9600;
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
//	USART_InitStructure.USART_Parity = USART_Parity_No;
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//	USART_Init(USART3, &USART_InitStructure);
//	
//	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
////	
////	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
////	
////	NVIC_InitTypeDef NVIC_InitStructure;
////	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
////	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
////	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
////	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
////	NVIC_Init(&NVIC_InitStructure);
//	
//	USART_Cmd(USART3, ENABLE);
//}


void Serial_SendByte(uint8_t Byte)
{
	USART_SendData(USART3, Byte);
	while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
}

void Serial_SendArray(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte(Array[i]);
	}
}

void Serial_SendString(char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)
	{
		Serial_SendByte(String[i]);
	}
}



int fputc(int ch, FILE *f)
{
	Serial_SendByte(ch);
	return ch;
}

void Serial_Printf(char *format, ...)
{
	char String[100];
	va_list arg;
	va_start(arg, format);
	vsprintf(String, format, arg);
	va_end(arg);
	Serial_SendString(String);
}



//播放
void Music_Begin(void)
{
	uint8_t Serial_TxPacket[8]={0x7E ,0xFF, 0x06, 0x0D, 0x00, 0x00, 0x00, 0xEF};
	Serial_SendArray(Serial_TxPacket, 8);

}


//暂停
void Music_Stop(void)
{
	uint8_t Serial_TxPacket[8]={0x7E ,0xFF, 0x06, 0x0E, 0x00, 0x00, 0x00, 0xEF};
	Serial_SendArray(Serial_TxPacket, 8);

}

//下一首
void Music_Next(void)
{
	uint8_t Serial_TxPacket[8]={ 0x7E ,0xFF ,0x06 ,0x01 ,0x00 ,0x00 ,0x00 ,0xEF};
	Serial_SendArray(Serial_TxPacket, 8);

}

//上一首
void Music_Previous(void)
{
	uint8_t Serial_TxPacket[8]={0x7E ,0xFF ,0x06 ,0x02 ,0x00 ,0x00 ,0x00 ,0xEF};
	Serial_SendArray(Serial_TxPacket, 8);

}



// 指定音量为 30 级 
void Music_volume(void)
{
																														//0x1E为十六进制的30
	uint8_t Serial_TxPacket[8]={0x7E ,0xFF ,0x06 ,0x06 ,0x00 ,0x00 ,0x1E ,0xEF};
	Serial_SendArray(Serial_TxPacket, 8);

}

// 循环播放全部 
void loop_AllMusic(void)
{
	uint8_t Serial_TxPacket[8]={ 0x7E ,0xFF ,0x06 ,0x11 ,0x00 ,0x00 ,0x01 ,0xEF};
	Serial_SendArray(Serial_TxPacket, 8);

}


// 指定第一首播放
void Specify_music(void)
{
																															//0x01为指定曲目
	uint8_t Serial_TxPacket[8]={ 0x7E ,0xFF ,0x06 ,0x03 ,0x00 ,0x00 ,0x01 ,0xEF};
	Serial_SendArray(Serial_TxPacket, 8);

}


// 循环播放第一首
void loop_OneMusic(void)
{
																															//0x01为指定曲目
	uint8_t Serial_TxPacket[8]={ 0x7E ,0xFF ,0x06 ,0x08 ,0x00 ,0x00 ,0x01 ,0xEF};
	Serial_SendArray(Serial_TxPacket, 8);

}




