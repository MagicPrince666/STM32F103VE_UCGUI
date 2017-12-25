#include "dma.h"
#include "lcd.h"
#include "delay.h"
#include "malloc.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//��������� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2014/2/22
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   
  
#define MUI_BACK_COLOR		LIGHTBLUE		//MUIѡ��ͼ���ɫ
#define MUI_FONT_COLOR 		BLUE 			//MUI������ɫ

#define MUI_EX_BACKCOLOR	0X0000			//�����ⲿ����ɫ
#define MUI_IN_BACKCOLOR	0xffff//0X8C51			//�����ڲ�����ɫ

////////////////////////////////////////////////////////////////////////////////////////////
//��ͼ��/ͼƬ·��
extern  u8 *const mui_icos_path_tbl[9]; //ͼ��·����
////////////////////////////////////////////////////////////////////////////////////////////
//ÿ��ͼ��Ĳ�����Ϣ
__packed typedef struct _m_mui_icos
{										    
	u16 x;			//ͼ�����꼰�ߴ�
	u16 y;
	u8 width;
	u8 height; 
	u8 * path;		//ͼ��·��
	u8 * name;		//ͼ������
}m_mui_icos;

//������ ������
typedef struct _m_mui_dev
{					
	u16 tpx;			//���������һ�ε�X����
	u16 tpy;			//���������һ�ε�Y����
	u8 status;			//��ǰѡ��״̬.
						//bit7:0,û����Ч����;1,����Ч����.
						//bit6~5:����
						//bit4~0:0~8,��ѡ�е�ͼ����;0XF,û��ѡ���κ�ͼ��  
	m_mui_icos icos[9];	//1ҳ,��9��ͼ��
}m_mui_dev;
extern m_mui_dev muidev;

void mui_init(u8 mode);
void mui_load_icos(void);
void mui_language_set(u8 language);
void mui_set_sel(u8 sel); 
u8 mui_touch_chk(void);
























 




