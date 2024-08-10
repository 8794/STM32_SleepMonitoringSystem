#include "stm32f10x.h" // Device header
#include <stdio.h>
#include <string.h>
#include <ctype.h>
uint8_t Bed_TxPacket[4]; // 定义发送数据包数组，数据包格式：FF 01 02 03 04 FE
uint8_t Bed_RxPacket[65]; // 定义接收数据包数组
uint8_t Bed_RxFlag; // 定义接收数据包标志位

// 串口初始化
void Bed_Init(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure; 
	NVIC_InitTypeDef NVIC_InitStructure;
    // 开启时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); // 开启USART1的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  // 开启GPIOA的时钟

    // GPIO初始化
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
			
	
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure); // 将PA9引脚初始化为复用推挽输出

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_Init(GPIOA, &GPIO_InitStructure); // 将PA10引脚初始化为上拉输入

    // USART初始化
    // 定义结构体变量
    USART_InitStructure.USART_BaudRate = 115200; // 波特率
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 硬件流控制，不需要
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; // 模式，发送模式和接收模式均选择
    USART_InitStructure.USART_Parity = USART_Parity_No; // 奇偶校验，不需要
    USART_InitStructure.USART_StopBits = USART_StopBits_1; // 停止位，选择1位
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; // 字长，选择8位
    USART_Init(USART1, &USART_InitStructure); // 将结构体变量交给USART_Init，配置USART1

    // 中断输出配置
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); // 开启串口接收数据的中断

    // NVIC中断分组
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 配置NVIC为分组2

    // NVIC配置
     // 定义结构体变量
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; // 选择配置NVIC的USART1线
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; // 指定NVIC线路使能
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; // 指定NVIC线路的抢占优先级为1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; // 指定NVIC线路的响应优先级为1
    NVIC_Init(&NVIC_InitStructure); // 将结构体变量交给NVIC_Init，配置NVIC外设

    // USART使能
    USART_Cmd(USART1, ENABLE); // 使能USART1，串口开始运行
}



//判断中断标志位
uint8_t Bed_GetRxFlag(void)
{
	if(Bed_RxFlag == 1)
	{
		Bed_RxFlag = 0;
		return 1;
	}
	return 0;
}





//	 数据包解析函数
void Bed_Data(unsigned char *Status, unsigned char *heartHate, unsigned char *RespiratoryRate) {
    char Status_Str[5]; // 确保缓冲区足够大
    char RespiratoryRate_Str[5];
    char heartHate_Str[5];
    int j, i; // 变量声明移到函数开头
    int length, base, decimalValue; // 将循环中使用的变量也移到开头声明

    // 将十六进制值转换为字符串
    snprintf(Status_Str, sizeof(Status_Str), "%02X", *Status);
    snprintf(RespiratoryRate_Str, sizeof(RespiratoryRate_Str), "%02X", *RespiratoryRate);
    snprintf(heartHate_Str, sizeof(heartHate_Str), "%02X", *heartHate);

    for (j = 0; j < 3; j++) {
        decimalValue = 0; // 初始化十进制值
        base = 1; // 初始化基数

        if (j == 0) {
            length = strlen(Status_Str);
            for (i = length - 1; i >= 0; i--) {
                char currentChar = toupper(Status_Str[i]);
                if (currentChar >= '0' && currentChar <= '9') {
                    decimalValue += (currentChar - '0') * base;
                } else if (currentChar >= 'A' && currentChar <= 'F') {
                    decimalValue += (currentChar - 'A' + 10) * base;
                }
                base *= 16;
            }
            *Status = decimalValue;
        } else if (j == 1) {
            length = strlen(RespiratoryRate_Str);
            for (i = length - 1; i >= 0; i--) {
                char currentChar = toupper(RespiratoryRate_Str[i]);
                if (currentChar >= '0' && currentChar <= '9') {
                    decimalValue += (currentChar - '0') * base;
                } else if (currentChar >= 'A' && currentChar <= 'F') {
                    decimalValue += (currentChar - 'A' + 10) * base;
                }
                base *= 16;
            }
            *RespiratoryRate = decimalValue / 10; // 注意这里除以10的操作，确保这是您想要的行为
        } else if (j == 2) {
            length = strlen(heartHate_Str);
            for (i = length - 1; i >= 0; i--) {
                char currentChar = toupper(heartHate_Str[i]);
                if (currentChar >= '0' && currentChar <= '9') {
                    decimalValue += (currentChar - '0') * base;
                } else if (currentChar >= 'A' && currentChar <= 'F') {
                    decimalValue += (currentChar - 'A' + 10) * base;
                }
                base *= 16;
            }
            *heartHate = decimalValue;
        }
    }
}


// USART1中断函数
void USART1_IRQHandler(void) {
    static uint8_t RxState = 0; // 定义表示当前状态机状态的静态变量
    static uint8_t pRxPacket = 0; // 定义表示当前接收数据位置的静态变量
    if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET) // 判断是否是USART1的接收事件触发的中断
    {
        uint8_t RxData = USART_ReceiveData(USART1); // 读取数据寄存器，存放在接收的数据变量

        /* 使用状态机的思路，依次处理数据包的不同部分 */

        /* 当前状态为0，接收数据包包头 */
        if (RxState == 0)
        {
            if (RxData == 0x4F) // 如果数据确实是包头
            {
                RxState = 1; // 置下一个状态
                pRxPacket = 0; // 数据包的位置归零
            }
        }
        /* 当前状态为1，接收数据包数据 */
        else if (RxState == 1)
        {
            Bed_RxPacket[pRxPacket] = RxData; // 将数据存入数据包数组的指定位置
            pRxPacket++; // 数据包的位置自增
            if (pRxPacket >= 65) // 如果收够66个数据
            {
                RxState = 2; // 置下一个状态
            }
        }
        /* 当前状态为2，接收数据包包尾 */
        else if (RxState == 2)
        {
           //if (RxData == 0xFE) // 如果数据确实是包尾部
           // {
                RxState = 0; // 状态归0
                Bed_RxFlag = 1; // 接收数据包标志位置1，成功接收一个数据包
                //Parse_Data(Serial_RxPacket); // 解析数据包
            //}
        }

        USART_ClearITPendingBit(USART1, USART_IT_RXNE); // 清除标志位
    }
}


