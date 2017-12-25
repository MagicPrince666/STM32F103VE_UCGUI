#ifndef __SETTINGS_H
#define __SETTINGS_H
#include "sys.h"
#include "includes.h" 	   	 
#include "common.h"    
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
 
//list�ṹ��.����ṹ
__packed typedef struct 
{
	u8 syslanguage;			//Ĭ��ϵͳ����
 	u8 lcdbklight;			//LED�������� 10~110.10�����;110��������
	u8 picmode;				//ͼƬ���ģʽ:0,˳��ѭ������;1,�������.
	u8 audiomode;			//��Ƶ����ģʽ:0,˳��ѭ������;1,�������;2,����ѭ������.
	u8 videomode;			//��Ƶ����ģʽ:0,˳��ѭ������;1,�������;2,����ѭ������.
	u8 saveflag;			//�����־,0X0A,�������;����,����δ����	   
}_system_setings;

extern _system_setings systemset;	//��settings.c�������� 

u8 sysset_time_set(u16 x,u16 y,u8 *hour,u8 *min,u8*caption);
u8 sysset_date_set(u16 x,u16 y,u16 *year,u8 *month,u8 *date,u8*caption);
//void sysset_wm8978_vol3d_show(u16 x,u16 y,u8 mask,u8 val);
//void sysset_wm8978_eq_show(u16 x,u16 y,u8 mask,u8 eqx,u8 val);
//u8 sysset_wm8978_vol3d_set(u16 x,u16 y,_wm8978_obj *wmset,u8* caption);
//u8 sysset_wm8978_eq_set(u16 x,u16 y,_wm8978_obj *wmset,u8 eqx,u8* caption);
u8 sysset_bklight_set(u16 x,u16 y,u8* caption,u16 *bkval);
u8 sysset_system_update_cpymsg(u8*pname,u8 pct,u8 mode);
void sysset_system_update(u8*caption,u16 sx,u16 sy);
void sysset_system_info(u16 x,u16 y,u8*caption);
void sysset_system_status(u16 x,u16 y,u8* caption);
void sysset_system_about(u16 x,u16 y,u8*caption);
u8* set_search_caption(const u8 *mcaption);	 
void sysset_read_para(_system_setings * sysset);
void sysset_save_para(_system_setings * sysset);

u8 sysset_play(void);
										   
#endif












