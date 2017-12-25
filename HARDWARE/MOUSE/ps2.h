#ifndef __PS2_H
#define __PS2_H	 
#include "delay.h"	   
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//ALIENTEK MiniSTM32������
//PS2 ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2014/3/12
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) �������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////	

//�����ӿڶ���
//PS2���� 		  				    
#define PS2_SCL PCin(1)			  //PC1
#define PS2_SDA PCin(0)				//PC0
//PS2���
#define PS2_SCL_OUT PCout(1)		//PC1
#define PS2_SDA_OUT PCout(0)		//PC0

//����PS2_SCL�������״̬.		  
#define PS2_SET_SCL_IN()  {GPIOC->CRL&=0XFFFFFF0F;GPIOC->CRL|=0X00000080;}
#define PS2_SET_SCL_OUT() {GPIOC->CRL&=0XFFFFFF0F;GPIOC->CRL|=0X00000030;}	  
//����PS2_SDA�������״̬.		  
#define PS2_SET_SDA_IN()  {GPIOC->CRL&=0XFFFFFFF0;GPIOC->CRL|=0X00000008;}
#define PS2_SET_SDA_OUT() {GPIOC->CRL&=0XFFFFFFF0;GPIOC->CRL|=0X00000003;} 

#define MOUSE    0X20 //���ģʽ
#define KEYBOARD 0X10 //����ģʽ
#define CMDMODE  0X00 //��������
//PS2_Status��ǰ״̬��־
//[5:4]:��ǰ������ģʽ;[7]:���յ�һ������
//[6]:У�����;[3:0]:�յ������ݳ���;	 
extern u8 PS2_Status;       //����Ϊ����ģʽ
extern u8 PS2_DATA_BUF[16]; //ps2���ݻ�����
extern u8 MOUSE_ID;

void PS2_Init(void);
u8 PS2_Send_Cmd(u8 cmd);
void PS2_Set_Int(u8 en);
u8 PS2_Get_Byte(void);
void PS2_En_Data_Report(void);  
void PS2_Dis_Data_Report(void);		  				    
#endif




















