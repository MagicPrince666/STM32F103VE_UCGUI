#include "nes_main.h"
//////////////////////////////////////////////////////////////////////////////////	 
//�ο�������:xiaowei061��NESģ�������ٴ˶�xiaowei061��ʾ��л��
//ALIENTEKս��STM32������
//NESģ���� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/10/3
//�汾��V1.0		       								  
////////////////////////////////////////////////////////////////////////////////// 	   

JoyPadType JoyPad[2];

u8 NES_GetJoyPadVlaue(int JoyPadNum)
{
	u8 retval = 0;  
	PS2_JOYPAD_ATT = 0 ;
	if(JoyPadNum==0)
	{		 
		if(JOYPAD_DAT)retval=0;
		else retval=1;	    					 
		JOYPAD_CLK=1;			   //û��һ�����壬�յ�һ������
 		JOYPAD_CLK=0; 
		if(JoyPad[0].index==20)retval=1;//20λ��ʾ��������λ.
		JoyPad[0].index++;  
    } 
	PS2_JOYPAD_ATT = 1 ;
 	return retval;
}	 


void NES_JoyPadReset(void)
{
	JoyPad[0].state = 1;
    JoyPad[0].index = 0;

	JOYPAD_LAT=1;//LOAD һ��
 	JOYPAD_LAT=0;
		
	JoyPad[1].state = 1;
    JoyPad[1].index = 0;
}

void NES_JoyPadInit(void)
{
	JoyPad[0].state = 0;//״̬Ϊ0,��ʾ��ֹ
    JoyPad[0].index = 0;
	JoyPad[0].value = 1 << 20;

	JoyPad[1].state = 0;
    JoyPad[1].index = 0;
	JoyPad[1].value = 1 << 19;
}

void NES_JoyPadDisable(void)
{			  
}












