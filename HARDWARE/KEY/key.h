#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK Mini STM32������
//�������� ��������		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2014/3/18
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved			
//********************************************************************************
//V1.1�޸�˵�� 20140318
//����KEY0_Scan����
//////////////////////////////////////////////////////////////////////////////////	 

#define KEY0_PRES	1		//KEY0����
#define KEY1_PRES	2		//KEY1����
#define WKUP_PRES	3		//WK_UP����

#define KEY0 PEin(5)   	//PE4
#define KEY1 PEin(2)	//PE3 
#define KEY2 PEin(5)	//PD12
#define WK_UP PAin(0)	//PA0  WK_UP
	 
void KEY_Init(void);		//IO��ʼ��
u8 KEY_Scan(u8 mode);		//����ɨ�躯��
u8 KEY0_Scan(void);			//����ɨ��KEY0����
#endif
