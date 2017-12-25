#include "key.h"
#include "delay.h"
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
 	    
//������ʼ������ 
//PA0.15��PC5 ���ó�����
void KEY_Init(void)
{
	RCC->APB2ENR|=1<<2;     //ʹ��PORTAʱ��
	RCC->APB2ENR|=1<<6;     //ʹ��PORTEʱ��
	GPIOA->CRL&=0XFFFFFFF0;	//PA0���ó����룬Ĭ������	  
	GPIOA->CRL|=0X00000008; 
  
	GPIOE->CRL&=0XFF0FF0FF;	//PE2~4���ó�����	  
	GPIOE->CRL|=0X00800800; 				   
	GPIOE->ODR|=1<<5|1<<2;	   	//PE2~4 ����
} 
//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//����ֵ��
//0��û���κΰ�������
//KEY0_PRES��KEY0����
//KEY1_PRES��KEY1����
//WKUP_PRES��WK_UP���� 
//ע��˺�������Ӧ���ȼ�,KEY0>KEY1>WK_UP!!
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//�������ɿ���־
	if(mode)key_up=1;  //֧������		  
	if(key_up&&(KEY0==0||KEY1==0||WK_UP==1))
	{
		delay_ms(10);//ȥ���� 
		key_up=0;
		if(KEY0==0)return KEY0_PRES;
		else if(KEY1==0)return KEY1_PRES;
		else if(WK_UP==1)return WKUP_PRES; 
	}else if(KEY0==1&&KEY1==1&&WK_UP==0)key_up=1; 
	return 0;// �ް�������
}
//����ɨ��KEY0�Ƿ񱻰���
//����ֵ:0,û����;1,������.
u8 KEY0_Scan(void)
{	 
	static u8 key_up=1;//�������ɿ���־ 	  
	if(key_up&&KEY2==0)
	{
		delay_ms(10);//ȥ���� 
		key_up=0;
		if(KEY2==0)return 1;
	}else if(KEY2==1)key_up=1; 	     
	return 0;// �ް�������
}




















