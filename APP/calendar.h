#ifndef __CALENDAR_H
#define __CALENDAR_H
#include "sys.h"
#include "includes.h" 	   	 
#include "common.h" 
#include "rtc.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//APP-���� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/7/20
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//*******************************************************************************
//�޸���Ϣ
//��
////////////////////////////////////////////////////////////////////////////////// 	   
 
//���ӽṹ��						   
__packed typedef struct  
{																				   			  
	u8  weekmask;		//������������ bit0~bit6,��������~��6.   
	u8  ringsta;		//����״̬��������־.
						//[7]:0,������;1,������;
						//[6:3]:����
						//[2:0]:������������
	u8  hour;	   		//����Сʱ
	u8  min;			//�������	 
	u8 saveflag;		//�����־,0X0A,�������;����,����δ����	   
}_alarm_obj;

extern _alarm_obj alarm;//���ӽṹ��
extern u8*const calendar_week_table[GUI_LANGUAGE_NUM][7];//�ⲿ��������ڱ�

void calendar_alarm_init(_alarm_obj *alarmx,_calendar_obj *calendarx);
void calendar_alarm_ring(u8 type);
void calendar_get_time(_calendar_obj *calendarx);
void calendar_get_date(_calendar_obj *calendarx);
void calendar_date_refresh(void);
void calendar_read_para(_alarm_obj * alarm);
void calendar_save_para(_alarm_obj * alarm);
u8 calendar_alarm_msg(u16 x,u16 y);
void calendar_circle_clock_drawpanel(u16 x,u16 y,u16 size,u16 d);
void calendar_circle_clock_showtime(u16 x,u16 y,u16 size,u16 d,u8 hour,u8 min,u8 sec);
u8 calendar_play(void);
					    				   
#endif












