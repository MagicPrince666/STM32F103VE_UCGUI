#ifndef __NESPLAY_H
#define __NESPLAY_H 	
#include "common.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//ALIENTEKս��STM32������
//APP-NESģ���� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2014/2/16
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) �������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//*******************************************************************************
//V1.1 20140216
//�����Ը��ֱַ���LCD��֧��.
////////////////////////////////////////////////////////////////////////////////// 	   

extern u8 nesruning ;	//�˳�NES�ı�־
extern u8 frame_cnt;	//ͳ��֡��
 
void nes_clock_set(u8 PLL);		   
void load_nes(u8* path);   
u8 nes_play(void);
#endif






















