#ifndef _NES_MAIN_H_
#define _NES_MAIN_H_
#include "sys.h"    
#include <stdio.h>
#include <string.h>   
#include "6502.h"
#include "PPU.h"
#include "nes_joypad.h"	 
#include "nes_rom.h"
#include "guix.h"
//////////////////////////////////////////////////////////////////////////////////	 
//�ο�������:xiaowei061��NESģ�������ٴ˶�xiaowei061��ʾ��л��
//ALIENTEKս��STM32������
//NESģ���� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/10/3
//�汾��V1.0		       								  
////////////////////////////////////////////////////////////////////////////////// 	   
#define NES_TRUE 	1
#define NES_FALSE	0

							 										  
typedef struct
{
	char filetype[4]; 	//�ַ�����NES^Z������ʶ��.NES�ļ� 		 
	u8 romnum;			//16kB ROM����Ŀ 						 
	u8 vromnum;			//8kB VROM����Ŀ				 
	u8 romfeature;		//D0��1����ֱ����0��ˮƽ���� 
						// D1��1���е�ؼ��䣬SRAM��ַ$6000-$7FFF
						// D2��1����$7000-$71FF��һ��512�ֽڵ�trainer 
						// D3��1��4��ĻVRAM���� 
						//  D4��D7��ROM Mapper���4� 	  
	u8 rommappernum;	// D0��D3��������������0��׼����Ϊ��Mapper��^_^��
						// D4��D7��ROM Mapper�ĸ�4λ 		    
	//u8 reserve[8];	// ������������0 					    
	//OM���������У��������trainer������512�ֽڰ���ROM��֮ǰ 
	//VROM��, �������� 
}NesHeader;

//����NES�������ʾλ��
typedef struct
{ 
	u16 sx;			//nes������ʼ��X����
	u16 sy;			//nes������ʼ��X���� 
}nes_disp_info;																		 

extern nes_disp_info nes_display_info;

u8 nes_main(void);
void NesFrameCycle(void);
void NES_ReadJoyPad(u8 JoyPadNum);


//PPUʹ��
extern u8 *NameTable;			//2K�ı���
extern u16	*Buffer_scanline;	//����ʾ����,���±�Խ�����Ϊ7����ʾ�� 7 ~ 263  0~7 263~270 Ϊ��ֹ�����
//CPUʹ��
extern u8 *ram6502;  			//RAM  2K�ֽ�,��malloc����

u8 nes_mem_creat(void);		//����nes���������RAM.
void nes_mem_delete(void);	//ɾ��nes����ʱ�����RAM	
										 
#endif











