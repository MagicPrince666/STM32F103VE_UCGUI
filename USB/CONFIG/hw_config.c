#include "usb_lib.h" 
#include "mass_mal.h"
#include "usb_desc.h"
#include "usb_pwr.h"
#include "usb_lib.h"
#include "usb_istr.h"
#include "led.h"
#include "usart.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKMiniSTM32������V3
//USB-hw_config ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/1/29
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
 
 
 
 /////////////////////////////////////////////////////////////////////////////////
//��USB����ͨ�ò��ִ���,ST����USB����,�˲��ִ��붼���Թ���.
//�˲��ִ���һ�㲻��Ҫ�޸�!

//USB�����жϷ�����
void USBWakeUp_IRQHandler(void) 
{
	EXTI->PR|=1<<18;//���USB�����жϹ���λ
} 

//USB�жϴ�����
void USB_LP_CAN1_RX0_IRQHandler(void) 
{
	USB_Istr();
} 

//USBʱ�����ú���,USBclk=48Mhz@HCLK=72Mhz
void Set_USBClock(void)
{
 	RCC->CFGR&=~(1<<22); //USBclk=PLLclk/1.5=48Mhz	    
	RCC->APB1ENR|=1<<23; //USBʱ��ʹ��					 
} 

//USB����͹���ģʽ
//��USB����suspendģʽʱ,MCU����͹���ģʽ
//��������ӵ͹��Ĵ���(�����ʱ�ӵ�)
void Enter_LowPowerMode(void)
{
 	printf("usb enter low power mode\r\n");
	bDeviceState=SUSPENDED;
} 

//USB�˳��͹���ģʽ
//�û��������������ش���(������������ʱ�ӵ�)
void Leave_LowPowerMode(void)
{
	DEVICE_INFO *pInfo=&Device_Info;
	printf("leave low power mode\r\n"); 
	if (pInfo->Current_Configuration!=0)bDeviceState=CONFIGURED; 
	else bDeviceState = ATTACHED; 
} 

//USB�ж�����
void USB_Interrupts_Config(void)
{ 
	EXTI->IMR|=1<<18;	//������18�ϵ��ж�
 	EXTI->RTSR|=1<<18;	//line 18���¼��������ش���	 
	MY_NVIC_Init(1,0,USB_LP_CAN1_RX0_IRQn,2);	//��2�����ȼ���֮ 
	MY_NVIC_Init(0,0,USBWakeUp_IRQn,2);     	//��2�����ȼ����	 	 
}	

//USB�ӿ�����(����1.5K��������,ս��V3����Ҫ����,������)
//NewState:DISABLE,������
//         ENABLE,����
void USB_Cable_Config (FunctionalState NewState)
{ 
	if (NewState!=DISABLE)printf("usb pull up enable\r\n"); 
	else printf("usb pull up disable\r\n"); 
}

//USBʹ������/����
//enable:0,�Ͽ�
//       1,��������	   
void USB_Port_Set(u8 enable)
{
	RCC->APB2ENR|=1<<2;    //ʹ��PORTAʱ��	   	 
	if(enable)_SetCNTR(_GetCNTR()&(~(1<<1)));//�˳��ϵ�ģʽ
	else
	{	  
		_SetCNTR(_GetCNTR()|(1<<1));  // �ϵ�ģʽ
		GPIOA->CRH&=0XFFF00FFF;
		GPIOA->CRH|=0X00033000;
		PAout(12)=0;	    		  
	}
}  

//��ȡSTM32��ΨһID
//����USB������Ϣ
void Get_SerialNum(void)
{
	u32 Device_Serial0, Device_Serial1, Device_Serial2;
	Device_Serial0 = *(u32*)(0x1FFFF7E8);
	Device_Serial1 = *(u32*)(0x1FFFF7EC);
	Device_Serial2 = *(u32*)(0x1FFFF7F0);
	Device_Serial0 += Device_Serial2;
	if (Device_Serial0 != 0)
	{
		IntToUnicode (Device_Serial0,&MASS_StringSerial[2] , 8);
		IntToUnicode (Device_Serial1,&MASS_StringSerial[18], 4);
	}
} 

//��32λ��ֵת����unicode.
//value,Ҫת����ֵ(32bit)
//pbuf:�洢��ַ
//len:Ҫת���ĳ���
void IntToUnicode (u32 value , u8 *pbuf , u8 len)
{
	u8 idx = 0;
	for( idx = 0 ; idx < len ; idx ++)
	{
		if( ((value >> 28)) < 0xA )
		{
			pbuf[ 2* idx] = (value >> 28) + '0';
		}
		else
		{
			pbuf[2* idx] = (value >> 28) + 'A' - 10; 
		} 
		value = value << 4; 
		pbuf[ 2* idx + 1] = 0;
	}
}
/////////////////////////////////////////////////////////////////////////////////

//���USB���ڽ��ж�/д����
void Led_RW_ON(void)
{
	LED1=0;
}
 
//���USB��/д�����Ѿ����
void Led_RW_OFF(void)
{
	LED1=1;
}
//��ʾUSB��δ�������
void USB_NotConfigured_LED(void)
{
	printf("USB NotConfigured\r\n");
}




















