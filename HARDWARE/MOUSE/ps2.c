#include "ps2.h" 
#include "usart.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK MiniSTM32������
//PS2 ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2014/3/12
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////	
//PS2������ʱ��Ƶ����10~20Khz(���33K)
//��/�͵�ƽ�ĳ���ʱ��Ϊ25~50us֮��. 

//PS2_Status��ǰ״̬��־
//[7]:���յ�һ������;[6]:У�����;[5:4]:��ǰ������ģʽ;[3:0]:�յ������ݳ���;		   
u8 PS2_Status=CMDMODE; //Ĭ��Ϊ����ģʽ 
u8 PS2_DATA_BUF[16];   //ps2���ݻ�����

//λ������
u8 BIT_Count=0;
//�ж�15~10������
//ÿ11��bit,Ϊ����1���ֽ�
//ÿ������һ����(11λ)��,�豸���ٻ�ȴ�50ms�ٷ�����һ����
//ֻ������겿��,���̲�����ʱδ����
void EXTI15_10_IRQHandler(void)
{	  	 
	static u8 tempdata=0;
	static u8 parity=0;  	   
	if(EXTI->PR&(1<<15))//�ж�15��������Ӧ���ж�
	{
		EXTI->PR=1<<15;  //���LINE15�ϵ��жϱ�־λ
		if(BIT_Count==0)
		{
			parity=0;
			tempdata=0;
		}
		BIT_Count++;    
		if(BIT_Count>1&&BIT_Count<10)//����������
		{	  
			tempdata>>=1;
			if(PS2_SDA)
			{
				tempdata|=0x80;
				parity++;//��¼1�ĸ���
			}   
		}else if(BIT_Count==10)//�õ�У��λ
		{
			if(PS2_SDA)parity|=0x80;//У��λΪ1
		}	   
		if(BIT_Count==11)//���յ�1���ֽڵ�������
		{
 			BIT_Count=parity&0x7f;//ȡ��1�ĸ���	  
			if(((BIT_Count%2==0)&&(parity&0x80))||((BIT_Count%2==1)&&(parity&0x80)==0))//��żУ��OK
			{					    
				//PS2_Status|=1<<7;//��ǵõ�����	   
				BIT_Count=PS2_Status&0x0f;		
				PS2_DATA_BUF[BIT_Count]=tempdata;//��������
				if(BIT_Count<15)PS2_Status++;    //���ݳ��ȼ�1
				BIT_Count=PS2_Status&0x30;	     //�õ�ģʽ	  
				switch(BIT_Count)
				{
					case CMDMODE://����ģʽ��,ÿ�յ�һ���ֽڶ�������������
						PS2_Dis_Data_Report();//��ֹ���ݴ���
						PS2_Status|=1<<7; //��ǵõ�����
						break;
					case KEYBOARD:
						break;
					case MOUSE:
						if(MOUSE_ID==0)//��׼���,3���ֽ�
						{
							if((PS2_Status&0x0f)==3)
							{
								PS2_Status|=1<<7;//��ǵõ�����
								PS2_Dis_Data_Report();//��ֹ���ݴ���
							}
						}else if(MOUSE_ID==3)//��չ���,4���ֽ�
						{
							if((PS2_Status&0x0f)==4)
							{
								PS2_Status|=1<<7;//��ǵõ�����
								PS2_Dis_Data_Report();//��ֹ���ݴ���
							}
						}	 
						break;
				}		   		 
			}else
			{
				PS2_Status|=1<<6;//���У�����
				PS2_Status&=0xf0;//����������ݼ�����
			}
			BIT_Count=0;
		} 	 	  
	}			  
}
//��ֹ���ݴ���
//��ʱ��������,��ֹ���ݴ���	   
void PS2_Dis_Data_Report(void)
{
	PS2_Set_Int(0);   //�ر��ж�
	PS2_SET_SCL_OUT();//����SCLΪ���
	PS2_SCL_OUT=0;    //���ƴ���
}
//ʹ�����ݴ���
//�ͷ�ʱ����		    
void PS2_En_Data_Report(void)
{
	PS2_SET_SCL_IN(); //����SCLΪ����
	PS2_SET_SDA_IN(); //SDA IN
	PS2_SCL_OUT=1;    //����   
	PS2_SDA_OUT=1; 
	PS2_Set_Int(1);   //�����ж�
}

//PS2�ж���������
//en:1������;0���ر�;	 
void PS2_Set_Int(u8 en)
{
	EXTI->PR=1<<15;  //���LINE15�ϵ��жϱ�־λ
	if(en)EXTI->IMR|=1<<15;//������line15�ϵ��ж�
    else EXTI->IMR&=~(1<<15);//����line15�ϵ��ж�   
}
//�ȴ�PS2ʱ����sta״̬�ı�
//sta:1���ȴ���Ϊ1;0���ȴ���Ϊ0;
//����ֵ:0��ʱ���߱����sta;1����ʱ���;	 
u8 Wait_PS2_Scl(u8 sta)
{
	u16 t=0;
	sta=!sta;
	while(PS2_SCL==sta)
	{
		delay_us(1);
		t++;
		if(t>16000)return 1;//ʱ����� (�豸����10ms�ڼ�����״̬)
	}
	return 0;//��������
}
//�ڷ�������/����֮��,�ȴ��豸Ӧ��,�ú���������ȡӦ��
//���صõ���ֵ 
//����0����PS2_Status.6=1��������˴���		  
u8 PS2_Get_Byte(void)
{
	u16 t=0;
	u8 temp=0;
	while(1)//���ȴ�55ms
	{
		t++;
		delay_us(10);
		if(PS2_Status&0x80)//�õ���һ������
		{
			temp=PS2_DATA_BUF[PS2_Status&0x0f-1];
			PS2_Status&=0x70;//��������������յ����ݱ��
			break;	
		}else if(t>5500||PS2_Status&0x40)break;//��ʱ���/���մ���	   
	}
	PS2_En_Data_Report();//ʹ�����ݴ���
	return temp;    
}	    
//����һ�����PS2.
//����ֵ:0���޴���,����,�������
u8 PS2_Send_Cmd(u8 cmd)
{
	u8 i;
	u8 high=0;//��¼1�ĸ���		 
	PS2_Set_Int(0);   //�����ж�
	PS2_SET_SCL_OUT();//����SCLΪ���
	PS2_SET_SDA_OUT();//SDA OUT
	PS2_SCL_OUT=0;//����ʱ����
	delay_us(120);//��������100us
	PS2_SDA_OUT=0;//����������
	delay_us(10);
	PS2_SET_SCL_IN();//�ͷ�ʱ����,����PS2�豸�õ���һ��λ,��ʼλ
	PS2_SCL_OUT=1;
	if(Wait_PS2_Scl(0)==0)//�ȴ�ʱ������
	{									  
		for(i=0;i<8;i++)
		{
			if(cmd&0x01)
			{
			    PS2_SDA_OUT=1;
				high++;
			}else PS2_SDA_OUT=0;   
			cmd>>=1;
			//��Щ�ط�û�м�����,��Ϊ��Щ�ط����������ѭ��
			Wait_PS2_Scl(1);//�ȴ�ʱ������ ����8��λ
			Wait_PS2_Scl(0);//�ȴ�ʱ������
		}
		if((high%2)==0)PS2_SDA_OUT=1;//����У��λ 10
		else PS2_SDA_OUT=0;
		Wait_PS2_Scl(1); //�ȴ�ʱ������ 10λ
		Wait_PS2_Scl(0); //�ȴ�ʱ������
		PS2_SDA_OUT=1;   //����ֹͣλ  11	  
 		Wait_PS2_Scl(1);//�ȴ�ʱ������ 11λ
		PS2_SET_SDA_IN();//SDA in
		Wait_PS2_Scl(0);//�ȴ�ʱ������
		if(PS2_SDA==0)Wait_PS2_Scl(1);//�ȴ�ʱ������ 12λ 
		else 
		{
			PS2_En_Data_Report();
			return 1;//����ʧ��
		}		
	}else 
	{
		PS2_En_Data_Report();
		return 2;//����ʧ��
	}
	PS2_En_Data_Report();
	return 0;    //���ͳɹ�
}
//PS2��ʼ��				 
void PS2_Init(void)
{
	RCC->APB2ENR|=1<<4;    //ʹ��PORTCʱ��
 	GPIOC->CRL&=0XFFFFFF00;	
	GPIOC->CRL|=0X00000088;//PA15���ó�����	
	GPIOC->ODR|=3<<0;	  
 	Ex_NVIC_Config(GPIO_C,0,FTIR);		//��line15ӳ�䵽PA.15���½��ش���.
	MY_NVIC_Init(1,2,EXTI15_10_IRQn,2);	//���䵽�ڶ���,��ռ2,��Ӧ3		 
}

















