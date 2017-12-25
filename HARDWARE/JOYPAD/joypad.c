#include  "joypad.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//��Ϸ�ֱ����� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/12
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

//��ʼ���ֱ��ӿ�.	 
void JOYPAD_Init(void)
{
 	RCC->APB2ENR|=1<<4;//��ʹ������PORTCʱ��    	
	GPIOC->CRL&=0XFFF00000;
	GPIOC->CRL|=0X00083833;     
	GPIOC->ODR|=0x1f<<0;  
}

//��ȡ�ֱ�����ֵ.
//FC�ֱ����������ʽ:
//ÿ��һ������,���һλ����,���˳��:
//A->B->SELECT->START->UP->DOWN->LEFT->RIGHT.
//�ܹ�8λ,������C��ť���ֱ�,����C��ʵ�͵���A+Bͬʱ����.
//������0,�ɿ���1.
//����ֵ:
//[0]:��
//[1]:��
//[2]:��
//[3]:��
//[4]:Start
//[5]:Select
//[6]:B
//[7]:A
u8 JOYPAD_Read(void)
{
	u8 temp=0;
	u8 t;
	JOYPAD_LAT=1;					//���浱ǰ״̬
 	JOYPAD_LAT=0;
	for(t=0;t<8;t++)
	{
		temp<<=1;	 
		if(JOYPAD_DAT)temp|=0x01;	//LOAD֮�󣬾͵õ���һ������
		JOYPAD_CLK=1;			   	//ÿ��һ�����壬�յ�һ������
 		JOYPAD_CLK=0;	
	}
	return temp;
}



u8 PS2_Wireless_JOYPAD_WriteRead(u8 cmd)
{
	u8 i=0;
	u8 value=0;
	u8 CMD=cmd;
	for(i=0;i<8;i++)
	{
		PS2_JOYPAD_CLOCK=1;
		if(CMD&0x01)PS2_JOYPAD_CMND=1;
		else PS2_JOYPAD_CMND=0;
		CMD>>=1;
		delay_us(10);
		PS2_JOYPAD_CLOCK=0;
		delay_us(15);
		value>>=1;
		if(PS2_JOYPAD_DATA)value|=0x80; //0000 0000
	}
	PS2_JOYPAD_CLOCK=1;
	return value;
}

//���Ӧ����
//��Ӧ���źŷ���0����Ӧ���źŷ���1
u8 PS2_Wireless_JOYPAD_ACK(void)
{
	u8 i=0;
	PS2_JOYPAD_CLOCK=1;
	delay_us(5);
	PS2_JOYPAD_CLOCK=0;
	while(i<100)
	{
		if(0==PS2_JOYPAD_ACK)break;
		i++;
	}
	PS2_JOYPAD_CLOCK=1;
	return i<100?0:1;
}

//��ȡ���ݺ���
//�ɹ�����0��ʧ�ܷ���1
u16 PS2_Wireless_JOYPAD_DATA(void)
{
	u8 PS2_JOYPAD_ID=0;
	u8 table[3]={0};
	//LEFT DOWN RGHT UP STRT X X SLCT       ������ �� Բ�������� R1 L1 R2 L2
	u16 PS2_VALUE=0XFFFF;

	PS2_JOYPAD_ATT=0;//Ƭѡʹ��
	delay_us(10);

	PS2_Wireless_JOYPAD_WriteRead(PS2_JOYPAD_CMND_START);
	if(1==PS2_Wireless_JOYPAD_ACK())return 1;
	//�ֱ�ID�ţ��ҵ��������ֱ�����0X41
	PS2_JOYPAD_ID=PS2_Wireless_JOYPAD_WriteRead(PS2_JOYPAD_CMND_DEMAND);
	printf("%d\n",PS2_JOYPAD_ID);
	if(1==PS2_Wireless_JOYPAD_ACK())return 2;	
	table[0]=PS2_Wireless_JOYPAD_WriteRead(PS2_JOYPAD_CMND_NOP);//0x5a
	if(1==PS2_Wireless_JOYPAD_ACK())return 3;
	table[1]=PS2_Wireless_JOYPAD_WriteRead(PS2_JOYPAD_CMND_NOP);//data1
	if(1==PS2_Wireless_JOYPAD_ACK())return 4;	
	table[2]=PS2_Wireless_JOYPAD_WriteRead(PS2_JOYPAD_CMND_NOP);//data2

	PS2_VALUE=(table[1]<<8)|table[2];

	delay_us(10);
	PS2_JOYPAD_ATT=1;

	return PS2_VALUE;
}


u8 t=0;
u8  HAND;                       
u8  keybuf0;  //�ֱ���������洢��Ԫ
u8  keybuf1;
u8  RES[6]; 

u16 key_scan(void)//��ɨ��
{
    t=0;
    PS2_JOYPAD_ATT=0;         //�������ֱ�������ATT
    HAND=PS2_JOYPAD_CMND_START;     //�������Ϳ�ʼ����
    psinout();     //0xff
    delay_us(10);
	
    HAND=PS2_JOYPAD_CMND_DEMAND;     //��������������������
    psinout();     //0x41���ֱ���������Ӧ���ź�
    delay_us(5);
    psout();       //0x5A
    delay_us(5);
    psout(); //keybuf0(��0xff)  //�ֱ����ذ��������һ�ֽ�
    delay_us(5);
    psout(); //keybuf1(��0xff)  //�ֱ����ذ�������ڶ��ֽ�
    keybuf0=RES[3];              
    keybuf1=RES[4];
	
    delay_us(10);
    PS2_JOYPAD_ATT=1;
	
	  return RES[3]<<8|RES[4];
}

void psout(void)//���������ӳ���            
{
    int j,k;
    unsigned char duf=0;
    j=1;
    for(k=0;k<=7;k++)       //��λ���� 
    {
       PS2_JOYPAD_CLOCK = 1;
       delay_us(10);
       PS2_JOYPAD_CLOCK = 0;
       delay_us(15);
       if(PS2_JOYPAD_DATA == 1)
         duf=duf+j;
       j=j*2;
       PS2_JOYPAD_CLOCK = 1;
     }
    RES[t++]=duf;
}

void psinout(void)//�ֱ������ӳ���
{
    u8 buf,duf=0;
    u8 i,j=1;
    buf=HAND;
    for(i=0;i<=7;i++)     //��λ����     
    {
     PS2_JOYPAD_CLOCK=1;
     delay_us(10);
     if(buf&0x01)
        PS2_JOYPAD_CMND = 1;
     else
        PS2_JOYPAD_CMND = 0;
     buf=buf>>1;
     PS2_JOYPAD_CLOCK = 0;
     delay_us(15);
     if(PS2_JOYPAD_DATA == 1)
        duf=duf+j;
     j=j*2;
     PS2_JOYPAD_CLOCK = 1;
    }
    RES[t++]=duf;
}



