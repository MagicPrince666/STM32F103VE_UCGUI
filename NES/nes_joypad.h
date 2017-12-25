 
#ifndef _NES_JOYPAD_H_
#define _NES_JOYPAD_H_
#include "joypad.h"


#define JOYPAD_0 	0
#define JOYPAD_1 	1	 
 
typedef struct{
	u8 state;   //状态
	u8  index;	//当前读取位
	u32 value;	//JoyPad 当前值	
}JoyPadType;

/* function ------------------------------------------------------------------*/
void NES_JoyPadInit(void);
void NES_JoyPadReset(void);
void NES_JoyPadDisable(void);
u8 NES_GetJoyPadVlaue(int JoyPadNum);


#endif 













