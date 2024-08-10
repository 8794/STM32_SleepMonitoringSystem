#ifndef __MP3_H__
#define __MP3_H__

#include <stdio.h>

//void Uart3_Init(void);
void Serial_SendByte(uint8_t Byte);
void Serial_SendArray(uint8_t *Array, uint16_t Length);
void Serial_SendString(char *String);
void Serial_Printf(char *format, ...);
void Serial_SendPacket(void);


//播放
void Music_Begin(void);
//暂停
void Music_Stop(void);
//下一首
void Music_Next(void);
//上一首
void Music_Previous(void);
// 指定音量为 30 级 
void Music_volume(void);
// 循环播放全部 
void Music_Allloop(void);
// 循环播放第一首
void loop_OneMusic(void);
// 指定第一首播放
void Specify_music(void);

#endif
