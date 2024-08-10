#ifndef __SERIAL_H
#define __SERIAL_H


extern uint8_t Bed_TxPacket[];
extern uint8_t Bed_RxPacket[];
void Bed_Data(unsigned char *Status,unsigned char *heartHate,unsigned char *RespiratoryRate);
	

void Bed_Init(void);

uint8_t Bed_GetRxFlag(void);

#endif
