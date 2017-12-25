#include "spi.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//Mini STM32������
//SPI ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2014/3/9 
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  


//������SPIģ��ĳ�ʼ�����룬���ó�����ģʽ������SD Card/W25Q64/24L01 						  
//SPI�ڳ�ʼ��
//�������Ƕ�SPI1�ĳ�ʼ��
void SPI2_Init(void)
{	 
	RCC->APB2ENR|=1<<3;  	//PORTBʱ��ʹ�� 	 
	RCC->APB1ENR|=1<<14;   	//SPI2ʱ��ʹ�� 
	//����ֻ���SPI�ڳ�ʼ��
	GPIOB->CRH&=0X000FFFFF; 
	GPIOB->CRH|=0XBBB00000;	//PB13/14/15���� 	    
	GPIOB->ODR|=0X7<<13;   	//PB13/14/15����
	SPI2->CR1|=0<<10;		//ȫ˫��ģʽ	
	SPI2->CR1|=1<<9; 		//���nss����
	SPI2->CR1|=1<<8;  

	SPI2->CR1|=1<<2; 		//SPI����
	SPI2->CR1|=0<<11;		//8bit���ݸ�ʽ	
	SPI2->CR1|=1<<1; 		//����ģʽ��SCKΪ1 CPOL=1
	SPI2->CR1|=1<<0; 		//���ݲ����ӵڶ���ʱ����ؿ�ʼ,CPHA=1  
	//��SPI2����APB1������.ʱ��Ƶ�����Ϊ36M.
	SPI2->CR1|=3<<3; 		//Fsck=Fpclk1/256
	SPI2->CR1|=0<<7; 		//MSBfirst   
	SPI2->CR1|=1<<6; 		//SPI�豸ʹ��
	SPI2_ReadWriteByte(0xff);//��������	 
}   
//SPI1 �ٶ����ú���
//SpeedSet:0~7
//SPI�ٶ�=fAPB2/2^(SpeedSet+1)
//APB2ʱ��һ��Ϊ72Mhz
void SPI2_SetSpeed(u8 SpeedSet)
{
	SpeedSet&=0X07;			//���Ʒ�Χ
	SPI2->CR1&=0XFFC7; 
	SPI2->CR1|=SpeedSet<<3;	//����SPI2�ٶ�  
	SPI2->CR1|=1<<6; 		//SPI�豸ʹ��	
} 
//SPI1 ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPI2_ReadWriteByte(u8 TxData)
{		
	u16 retry=0;				 
	while((SPI2->SR&1<<1)==0)		//�ȴ���������	
	{
		retry++;
		if(retry>=0XFFFE)return 0; 	//��ʱ�˳�
	}			  
	SPI2->DR=TxData;	 	  		//����һ��byte 
	retry=0;
	while((SPI2->SR&1<<0)==0) 		//�ȴ�������һ��byte  
	{
		retry++;
		if(retry>=0XFFFE)return 0;	//��ʱ�˳�
	}	  						    
	return SPI2->DR;          		//�����յ�������			    
}































