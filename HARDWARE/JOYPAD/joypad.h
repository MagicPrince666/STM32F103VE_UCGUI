#ifndef __JOYPAD_H
#define __JOYPAD_H	 
#include "sys.h"
 
//�ֱ���������
#define JOYPAD_CLK PCout(2)  	//ʱ��		  PC0
#define JOYPAD_LAT PCout(1)  	//����     	PC1
#define JOYPAD_DAT PCin(0)	 	//����     	PC2    

void JOYPAD_Init(void);			//��ʼ��	
u8 JOYPAD_Read(void);			//��ȡ��ֵ	 	


#include "sys.h"
#include "delay.h"
#include "usart.h"

#define PS2_JOYPAD_DATA  PCin(0)
#define PS2_JOYPAD_CMND PCout(1)
#define PS2_JOYPAD_CLOCK PCout(2)
#define PS2_JOYPAD_ATT PCout(3)
#define PS2_JOYPAD_ACK PCin(4)

#define PS2_JOYPAD_CMND_START  0X01  //��ʼ����
#define PS2_JOYPAD_CMND_DEMAND 0X42 //��������
#define PS2_JOYPAD_CMND_NOP   0X00   //idle

void PS2_Wireless_JOYPAD_Init(void);
u16 PS2_Wireless_JOYPAD_DATA(void);

u16 key_scan(void);
void psout(void);
void psinout(void);

#endif
















