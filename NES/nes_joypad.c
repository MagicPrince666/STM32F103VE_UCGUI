#include "nes_main.h"
//////////////////////////////////////////////////////////////////////////////////	 
//参考自网友:xiaowei061的NES模拟器，再此对xiaowei061表示感谢！
//ALIENTEK战舰STM32开发板
//NES模拟器 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/10/3
//版本：V1.0		       								  
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
		JOYPAD_CLK=1;			   //没给一次脉冲，收到一个数据
 		JOYPAD_CLK=0; 
		if(JoyPad[0].index==20)retval=1;//20位表示控制器在位.
		JoyPad[0].index++;  
    } 
	PS2_JOYPAD_ATT = 1 ;
 	return retval;
}	 


void NES_JoyPadReset(void)
{
	JoyPad[0].state = 1;
    JoyPad[0].index = 0;

	JOYPAD_LAT=1;//LOAD 一下
 	JOYPAD_LAT=0;
		
	JoyPad[1].state = 1;
    JoyPad[1].index = 0;
}

void NES_JoyPadInit(void)
{
	JoyPad[0].state = 0;//状态为0,表示禁止
    JoyPad[0].index = 0;
	JoyPad[0].value = 1 << 20;

	JoyPad[1].state = 0;
    JoyPad[1].index = 0;
	JoyPad[1].value = 1 << 19;
}

void NES_JoyPadDisable(void)
{			  
}












