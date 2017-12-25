#include "button.h"
#include "piclib.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//GUI-��ť ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2012/10/4
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved	
//********************************************************************************
//����˵�� 
//V1.1 20140722
//1,�޸�btn_draw_picbtn����,֧��ͼƬ��ť�ߴ����ͼƬ�ߴ�,ͼƬ������ʾ
//2,�޸�btn_check����,���Ӷ���Ч�������жϴ���.
//////////////////////////////////////////////////////////////////////////////////


//��ȡ����ɫ
//btnx:��ťָ��
void btn_read_backcolor(_btn_obj * btnx)
{
	u16 x,y,ccnt;
	ccnt=0;
	for(y=btnx->top;y<btnx->top+btnx->height;y++)
	{
		for(x=btnx->left;x<btnx->left+btnx->width;x++)
		{
			btnx->bkctbl[ccnt]=gui_phy.read_point(x,y);//��ȡ��ɫ
			ccnt++;
		}
	}

}
//�ָ�����ɫ
//btnx:��ťָ��
void btn_recover_backcolor(_btn_obj * btnx)
{
	u16 x,y,ccnt;
	ccnt=0;	    
	for(y=btnx->top;y<btnx->top+btnx->height;y++)
	{
		for(x=btnx->left;x<btnx->left+btnx->width;x++)
		{
			gui_phy.draw_point(x,y,btnx->bkctbl[ccnt]);//����	
			ccnt++;
		}
	}  
}

//������ť
//top,left,width,height:����.
//id:����ID
//type:��������
//[7]:0,ģʽA,������һ��״̬,�ɿ���һ��״̬.
//	  1,ģʽB,ÿ����һ��,״̬�ı�һ��.��һ�°���,�ٰ�һ�µ���.
//[6:4]:����
//[3:0]:0,��׼��ť;1,ͼƬ��ť;2,�߽ǰ�ť;3,���ְ�ť(����͸��);4,���ְ�ť(������һ)
_btn_obj * btn_creat(u16 left,u16 top,u16 width,u16 height,u8 id,u8 type)
{
 	_btn_obj * btn_crt;
	if(width<6||height<6)return NULL;//�ߴ粻��̫С		  
	btn_crt=(_btn_obj*)gui_memin_malloc(sizeof(_btn_obj));//�����ڴ�
	if(btn_crt==NULL)return NULL;//�ڴ���䲻��.
	btn_crt->top=top;
	btn_crt->left=left;
	btn_crt->width=width;
	btn_crt->height=height;
	btn_crt->id=id;
	btn_crt->type=type;
	btn_crt->sta=0;
	btn_crt->caption=0;
	btn_crt->font=16;					//Ĭ��Ϊ16����
	btn_crt->arcbtnr=3;					//Ĭ��Բ�ǵİ뾶
	btn_crt->bcfucolor=BTN_DFU_BCFUC;	//Ĭ���ɿ�����ɫ
	btn_crt->bcfdcolor=BTN_DFU_BCFDC;	//Ĭ�ϰ��µ���ɫ
	btn_crt->picbtnpathu=NULL;//Ĭ��·��Ϊ��
	btn_crt->picbtnpathd=NULL;//Ĭ��·��Ϊ��   
	if((type&0X0f)>=BTN_TYPE_ANG)//�߽ǰ�ť/���ְ�ť
	{
		switch(type&0X0f)
		{
			case BTN_TYPE_ANG://�߽ǰ�ť
				btn_crt->bkctbl=(u16*)gui_memin_malloc(4*2);//����8���ֽ�
		 		if(btn_crt->bkctbl==NULL)
				{
					btn_delete(btn_crt);//�ͷ�֮ǰ������ڴ�
					return NULL;		//�ڴ���䲻��.
				}
				btn_crt->bkctbl[0]=ARC_BTN_RIMC;//�߿���ɫ
				btn_crt->bkctbl[1]=ARC_BTN_TP1C;//��һ�е���ɫ				
				btn_crt->bkctbl[2]=ARC_BTN_UPHC;//�ϰ벿����ɫ
				btn_crt->bkctbl[3]=ARC_BTN_DNHC;//�°벿����ɫ				
				break;
			case BTN_TYPE_TEXTA://����͸�������ְ�ť
				btn_crt->bkctbl=(u16*)gui_memin_malloc(width*height*2);//Ҫ���������ı���ɫ��
		 		if(btn_crt->bkctbl==NULL)
				{
					btn_delete(btn_crt);//�ͷ�֮ǰ������ڴ�
					return NULL;		//�ڴ���䲻��.
				}
				btn_read_backcolor(btn_crt);//��ȡȫ������ɫ
				break;
			case BTN_TYPE_TEXTB://������һ�����ְ�ť
				btn_crt->bkctbl=(u16*)gui_memin_malloc(2*2);//����4���ֽ�
		 		if(btn_crt->bkctbl==NULL)
				{
					btn_delete(btn_crt);//�ͷ�֮ǰ������ڴ�
					return NULL;		//�ڴ���䲻��.
				}
				btn_crt->bkctbl[0]=0XFFFF;//Ĭ���ɿ�Ϊ��ɫ
				btn_crt->bkctbl[1]=0X0000;//Ĭ�ϰ���Ϊ��ɫ				
				break;
		} 
	}else btn_crt->bkctbl=NULL;//�Ա�׼��ť��ͼƬ��ť�����Ч.
    return btn_crt;
}
//ɾ����ť
//btn_del:Ҫɾ���İ���
void btn_delete(_btn_obj * btn_del)
{
	if(btn_del==NULL)return;//�Ƿ��ĵ�ַ,ֱ���˳�
	if((btn_del->type&0X0f)==BTN_TYPE_TEXTA)btn_recover_backcolor(btn_del);//�ָ�����ɫ
	gui_memin_free(btn_del->bkctbl);
	gui_memin_free(btn_del);
}
//���	
//btnx:����ָ��
//in_key:���밴��ָ��
//����ֵ:
//0,û��һ����Ч�Ĳ���	
//1,��һ����Ч����		 					   
u8 btn_check(_btn_obj * btnx,void * in_key)
{
	_in_obj *key=(_in_obj*)in_key;
	u8 sta;
	u8 btnok=0;
	if((btnx->sta&0X03)==BTN_INACTIVE)return 0;//��Ч״̬�İ���,ֱ�Ӳ����
	switch(key->intype)
	{
		case IN_TYPE_TOUCH:	//������������
			if(btnx->top<key->y&&key->y<(btnx->top+btnx->height)&&btnx->left<key->x&&key->x<(btnx->left+btnx->width))//�ڰ����ڲ�
			{
				if((btnx->sta&0X80)==0)	//�ϴ�����״̬Ϊ�ɿ�
				{
					btnx->sta|=1<<6;	//��Ǵ˴ΰ�����Ч
				 	if(btnx->type&0X80)	//ģʽB
					{
						sta=btnx->sta&0X03;
						btnx->sta&=~(0X03); 
						if(sta==BTN_PRESS)btnx->sta|=BTN_RELEASE;//�ɿ�
						else btnx->sta|=BTN_PRESS;//����	  
					}else  				//ģʽA
					{
						btnx->sta&=~(0X03);
						btnx->sta|=BTN_PRESS;//����
					}
					btn_draw(btnx);//����ť  
					btnok=1;
 				}
				btnx->sta|=0X80;//��ǰ���		   
			}else 
			{
				btnx->sta&=0X7F;//����ɿ�
				if((btnx->sta&0x03)==BTN_PRESS)		//֮ǰ�а���������
				{
					if(key->ksta&0x01)				//��ʱ�������ǰ���״̬,��Ч!!
					{
	 					btnx->sta=0;					
						btn_draw(btnx);				//�ָ���ť 
					}else if((btnx->type&0X80)==0)	//���������ɿ�
					{
						btnx->sta&=~(0X03);
						btnx->sta|=BTN_RELEASE;	//�ɿ�
						btn_draw(btnx);			//����ť 
						btnok=1;  
					}
				}
			}
			break;
		case IN_TYPE_KEY:	//��������
			break;
		case IN_TYPE_JOYPAD://�ֱ�����
			break;
		case IN_TYPE_MOUSE:	//�������
			break;
		default:
			break;
	}
	return btnok;
}

//����׼��ť
//btnx:��ť
void btn_draw_stdbtn(_btn_obj * btnx)
{
	u8 sta;
	sta=btnx->sta&0x03;
	gui_fill_rectangle(btnx->left+2,btnx->top+2,btnx->width-4,btnx->height-4,STD_BTN_INSC);//����ڲ�
	switch(sta)
	{
		case BTN_RELEASE://�ɿ�
		case BTN_INACTIVE://δ����
			gui_draw_vline(btnx->left,btnx->top,btnx->height,STD_BTN_LTOC);			//������
		 	gui_draw_hline(btnx->left,btnx->top,btnx->width,STD_BTN_LTOC);			//������
			gui_draw_vline(btnx->left+1,btnx->top+1,btnx->height-2,STD_BTN_LTIC);	//������
		 	gui_draw_hline(btnx->left+1,btnx->top+1,btnx->width-2,STD_BTN_LTIC);	//������
		
			gui_draw_vline(btnx->left+btnx->width-1,btnx->top,btnx->height,STD_BTN_RBOC);		//������
		 	gui_draw_hline(btnx->left,btnx->top+btnx->height-1,btnx->width,STD_BTN_RBOC);		//������
			gui_draw_vline(btnx->left+btnx->width-2,btnx->top+1,btnx->height-2,STD_BTN_RBIC);	//������
		 	gui_draw_hline(btnx->left+1,btnx->top+btnx->height-2,btnx->width-2,STD_BTN_RBIC);	//������
 			if(sta==0)gui_show_strmid(btnx->left,btnx->top,btnx->width,btnx->height,btnx->bcfucolor,btnx->font,btnx->caption);//��ʾcaption
			else//δ����
			{
				gui_show_strmid(btnx->left+1,btnx->top+1,btnx->width,btnx->height,STD_BTN_DFRC,btnx->font,btnx->caption);
				gui_show_strmid(btnx->left,btnx->top,btnx->width,btnx->height,STD_BTN_DFLC,btnx->font,btnx->caption);
 			}
			break;
		case BTN_PRESS://����
		  	gui_draw_rectangle(btnx->left,btnx->top,btnx->width,btnx->height,GUI_COLOR_BLACK);		//���
		  	gui_draw_rectangle(btnx->left+1,btnx->top+1,btnx->width-2,btnx->height-2,STD_BTN_RBOC);	//�ڿ�
 			gui_show_strmid(btnx->left+1,btnx->top+1,btnx->width,btnx->height,btnx->bcfdcolor,btnx->font,btnx->caption);//��ʾcaption
			break;
	}	    		
} 
//��ͼƬ��ť(ע��ͼƬ��ť,������뱻���,�����ñ���ɫΪ0X0001,����ÿ�ζ����ȱ�����ͼ)
//btnx:��ť
void btn_draw_picbtn(_btn_obj * btnx)
{
	u8 sta;
 	sta=btnx->sta&0x03;
 	switch(sta)
	{
		case BTN_RELEASE://�ɿ�
		case BTN_INACTIVE://δ����	 
			if(btnx->bcfucolor!=0X0001)
			{
				gui_fill_rectangle(btnx->left,btnx->top,btnx->width,btnx->height,btnx->bcfucolor);//����䱳��ɫ 
			}
 			minibmp_decode(btnx->picbtnpathu,btnx->left,btnx->top,btnx->width,btnx->height,0,0);
 			break;
		case BTN_PRESS://����,�����ڰ���ͼƬʱ,��ʾ���µ�ͼƬ.��������ʱ,���ɿ�ͼƬ����alphablending
			if(btnx->bcfdcolor!=0X0001)
			{
				gui_fill_rectangle(btnx->left,btnx->top,btnx->width,btnx->height,btnx->bcfdcolor);//����䱳��ɫ 
			}
			if(btnx->picbtnpathd!=NULL)
			{
 				minibmp_decode(btnx->picbtnpathd,btnx->left,btnx->top,btnx->width,btnx->height,0,0);
 			}else 
			{
				minibmp_decode(btnx->picbtnpathu,btnx->left,btnx->top,btnx->width,btnx->height,0xffff,0x90);
			}
			break;
	}	    		
}  
//��Բ�ǰ�ť
//btnx:��ť
void btn_draw_arcbtn(_btn_obj * btnx)
{
	u16 angcolor=0,upcolor=0,downcolor=0;	
	u8 sta;
 	if(btnx->arcbtnr>btnx->width/2||btnx->arcbtnr>btnx->height/2)return;//������Χ��
	sta=btnx->sta&0x03;											 			
	switch(sta)
	{
		case BTN_RELEASE://����(�ɿ�)
			angcolor=btnx->bkctbl[1];
			upcolor=btnx->bkctbl[2];
			downcolor=btnx->bkctbl[3];
			break;
		case BTN_PRESS://����
	  		angcolor=gui_alpha_blend565(btnx->bkctbl[2],GUI_COLOR_WHITE,16);
	 		upcolor=angcolor; 
 			downcolor=gui_alpha_blend565(btnx->bkctbl[3],GUI_COLOR_WHITE,16); 
			break;
		case BTN_INACTIVE://���ְ�ťû��δ����״̬
			return;
	} 
  	gui_draw_arcrectangle(btnx->left,btnx->top,btnx->width,btnx->height,btnx->arcbtnr,1,upcolor,downcolor);//���Բ�ǰ�ť
   	gui_draw_arcrectangle(btnx->left,btnx->top,btnx->width,btnx->height,btnx->arcbtnr,0,btnx->bkctbl[0],btnx->bkctbl[0]);//��Բ�Ǳ߿�
	gui_draw_hline (btnx->left+btnx->arcbtnr,btnx->top+1,btnx->width-2*btnx->arcbtnr,angcolor);//��һ��		   
 	//��ʾcaption
	if(sta==0)gui_show_strmid(btnx->left,btnx->top,btnx->width,btnx->height,btnx->bcfucolor,btnx->font,btnx->caption);
	else gui_show_strmid(btnx->left+1,btnx->top+1,btnx->width,btnx->height,btnx->bcfdcolor,btnx->font,btnx->caption);		   
}
//������͸�����ְ�ť
//btnx:��ť
void btn_draw_textabtn(_btn_obj * btnx)
{
	u8 sta;
	sta=btnx->sta&0x03;	  
	switch(sta)
	{
		case BTN_RELEASE://����(�ɿ�)
    		btn_recover_backcolor(btnx);//����ɫ
		    gui_show_strmid(btnx->left,btnx->top,btnx->width,btnx->height,btnx->bcfucolor,btnx->font,btnx->caption);
			break;
		case BTN_PRESS://����   
    		btn_recover_backcolor(btnx);//����ɫ
		    gui_show_strmid(btnx->left+1,btnx->top+1,btnx->width,btnx->height,btnx->bcfdcolor,btnx->font,btnx->caption);
			break;
		case BTN_INACTIVE://���ְ�ťû��δ����״̬
			return;
	} 
}

//��������һ���ְ�ť
//btnx:��ť
void btn_draw_textbbtn(_btn_obj * btnx)
{
	u8 sta;
	sta=btnx->sta&0x03;	  
	switch(sta)
	{
		case BTN_RELEASE://����(�ɿ�)						 
			gui_fill_rectangle(btnx->left,btnx->top,btnx->width,btnx->height,btnx->bkctbl[0]);//��� 
 		    gui_show_strmid(btnx->left,btnx->top,btnx->width,btnx->height,btnx->bcfucolor,btnx->font,btnx->caption);
			break;
		case BTN_PRESS://����   
			gui_fill_rectangle(btnx->left,btnx->top,btnx->width,btnx->height,btnx->bkctbl[1]);//��� 
		    gui_show_strmid(btnx->left+1,btnx->top+1,btnx->width,btnx->height,btnx->bcfdcolor,btnx->font,btnx->caption);
			break;
		case BTN_INACTIVE://���ְ�ťû��δ����״̬
			return;
	} 
}
//����ť
//btnx:��ť
void btn_draw(_btn_obj * btnx) 	
{
	if(btnx==NULL)return;//��Ч,ֱ���˳�
	switch(btnx->type&0x0f)
	{
		case BTN_TYPE_STD://��׼��ť
			btn_draw_stdbtn(btnx);
			break;
		case BTN_TYPE_PIC://ͼƬ��ť
			btn_draw_picbtn(btnx);
			break;
		case BTN_TYPE_ANG://�߽ǰ�ť
			btn_draw_arcbtn(btnx);
			break;
		case BTN_TYPE_TEXTA://���ְ�ť(����͸��)
			btn_draw_textabtn(btnx);
			break;
		case BTN_TYPE_TEXTB://���ְ�ť(������һ)
			btn_draw_textbbtn(btnx);
			break;
 	}
	if(btnx->sta&(1<<6))
	{
		if((btnx->sta&0x03)==BTN_PRESS)btn_press_hook(btnx);//�������µĹ��Ӻ���
		else if((btnx->sta&0x03)==BTN_RELEASE)btn_release_hook(btnx);//�����ɿ�ʱ�Ĺ��Ӻ��� 
	}	
}	

//��ť�ɿ����Ӻ���
//btnx:��ťָ��
void btn_release_hook(_btn_obj *btnx)
{
	//printf("button release %d status:%d\r\n",btnx->id,btnx->sta);
}
//��ť���¹��Ӻ���
//btnx:��ťָ��
void btn_press_hook(_btn_obj *btnx)
{
	//printf("button press %d status:%d\r\n",btnx->id,btnx->sta);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////

void btn_test(u16 x,u16 y,u16 width,u16 height,u8 type,u8 sta,u8 r,u8 *caption,u8 *uppath,u8 *downpath)
{
	_btn_obj* tbtn;
	tbtn=btn_creat(x,y,width,height,0,type);//������ť
	if(tbtn==NULL)return;//����ʧ��.
	tbtn->sta=sta;
	tbtn->caption=caption;//����
	tbtn->font=16;//����
	tbtn->arcbtnr=r; 
	tbtn->picbtnpathu=uppath; 
	tbtn->picbtnpathd=downpath; 
	btn_draw(tbtn);//����ť
 	btn_delete(tbtn);//ɾ����ť	 
} 
 















