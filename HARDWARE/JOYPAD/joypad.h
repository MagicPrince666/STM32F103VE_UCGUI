#ifndef __JOYPAD_H
#define __JOYPAD_H	 
#include "sys.h"
 
//手柄连接引脚
#define JOYPAD_CLK PCout(2)  	//时钟		  PC0
#define JOYPAD_LAT PCout(1)  	//锁存     	PC1
#define JOYPAD_DAT PCin(0)	 	//数据     	PC2    

void JOYPAD_Init(void);			//初始化	
u8 JOYPAD_Read(void);			//读取键值	 	


#include "sys.h"
#include "delay.h"
#include "usart.h"

#define PS2_JOYPAD_DATA  PCin(0)
#define PS2_JOYPAD_CMND PCout(1)
#define PS2_JOYPAD_CLOCK PCout(2)
#define PS2_JOYPAD_ATT PCout(3)
#define PS2_JOYPAD_ACK PCin(4)

#define PS2_JOYPAD_CMND_START  0X01  //起始命令
#define PS2_JOYPAD_CMND_DEMAND 0X42 //数据请求
#define PS2_JOYPAD_CMND_NOP   0X00   //idle

void PS2_Wireless_JOYPAD_Init(void);
u16 PS2_Wireless_JOYPAD_DATA(void);

u16 key_scan(void);
void psout(void);
void psinout(void);

#endif
















