#ifndef __MP3_H__
#define __MP3_H__

#include <stdio.h>

//void Uart3_Init(void);
void Serial_SendByte(uint8_t Byte);
void Serial_SendArray(uint8_t *Array, uint16_t Length);
void Serial_SendString(char *String);
void Serial_Printf(char *format, ...);
void Serial_SendPacket(void);


//����
void Music_Begin(void);
//��ͣ
void Music_Stop(void);
//��һ��
void Music_Next(void);
//��һ��
void Music_Previous(void);
// ָ������Ϊ 30 �� 
void Music_volume(void);
// ѭ������ȫ�� 
void Music_Allloop(void);
// ѭ�����ŵ�һ��
void loop_OneMusic(void);
// ָ����һ�ײ���
void Specify_music(void);

#endif
