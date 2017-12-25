#ifndef __LISTBOX_H
#define __LISTBOX_H 
#include "guix.h"  	 			  
#include "scrollbar.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//GUI-�б�� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2012/10/4
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//********************************************************************************
//����˵�� 
//V1.1 20140810
//1,�����Գ���Ŀ�Ĺ�����ʾ����
//2,����ԭ�����ڵ�һЩСbug
//3,�޸Ĺ�����������,ʹ�ø�����
//4,ȥ��LBOX_ITEM_HEIGHT�궨��,���ñ�����ʽ����(��gui_phy�ṹ����������)
//////////////////////////////////////////////////////////////////////////////////

//////listbox�ϵķ��ŵ�Ĭ����ɫ 
#define LBOX_DFT_LBKCOLOR		0XFFFF		//�ڲ�����ɫ
#define LBOX_DFT_LSELCOLOR		0XFFFF		//ѡ��list���������ɫ
#define LBOX_DFT_LSELBKCOLOR	0X001F		//ѡ��list��ı���ɫ
#define LBOX_DFT_LNCOLOR		0X0000		//δѡ�е�list������ɫ
#define LBOX_DFT_RIMCOLOR		0XF800		//�߿���ɫ

////listboxtype
#define LBOX_TYPE_RIM 			0X01		//�б߿�� 
//�������
#define LBOX_SCB_WIDTH 			0X0E		//���������Ϊ15������

    
//list�ṹ��.����ṹ
__packed typedef struct 
{
	void * prevlist;
	void * nextlist;
	u32 id;
	u8 *name;
}_listbox_list;

//listbox�ṹ�嶨��
__packed typedef struct 
{
	u16 top; 				  		//listbox��������
	u16 left;                       //listbox�������
	u16 width; 				  		//���
	u16 height;						//�߶� ����Ϊ12/16�ı���

	u8 type;						//���ͱ����
									//[bit7]:1,��Ҫ������������(������totalitems>itemsperpage);0,����Ҫ������.(��λ������Զ�����)
									//[bit6:0]:����					 
								
	u8 sta;							//listbox״̬,[bit7]:������־;[bit6]:�����Ч�ı�־;[bit5:0]:��һ�ΰ��µı��.	 
	u8 id;							//listbox ��id
	u8 dbclick;					   	//˫��,
									//[7]:0,û��˫��.1,��˫��.
									//[6~0]:0,����.

	u8 font;						//�������� 12/16
	u16 selindex;					//ѡ�е�����
 
	u16 lbkcolor;					//�ڲ�������ɫ
 	u16 lnselcolor; 				//list name ѡ�к����ɫ
  	u16 lnselbkcolor; 				//list name ѡ�к�ı�����ɫ
	u16 lncolor; 					//list name δѡ�е���ɫ
	u16 rimcolor;					//�߿���ɫ
//////////////////////
	u8 *fname;						//��ǰѡ�е�index������
	u16 namelen;					//name��ռ�ĵ���.
	u16 curnamepos;					//��ǰ��ƫ��
	u32 oldtime;					//��һ�θ���ʱ�� 

	_scrollbar_obj * scbv;			//��ֱ������
	_listbox_list *list;			//����	    
}_listbox_obj;


_listbox_obj * listbox_creat(u16 left,u16 top,u16 width,u16 height,u8 type,u8 font);//����listbox
_listbox_list * list_search(_listbox_list *listx,u16 index);//���ұ��Ϊindex��list,��ȡ����ϸ��Ϣ
void listbox_delete(_listbox_obj *listbox_del);	 			//ɾ��listbox
u8 listbox_check(_listbox_obj * listbox,void * in_key);		//���listbox�İ���״̬
u8 listbox_addlist(_listbox_obj * listbox,u8 *name);		//����һ��list
void listbox_draw_list(_listbox_obj *listbox);				//��list
void listbox_draw_listbox(_listbox_obj *listbox);			//�ػ�listbox
void listbox_2click_hook(_listbox_obj *listbox);			//˫�����Ӻ���
#endif





