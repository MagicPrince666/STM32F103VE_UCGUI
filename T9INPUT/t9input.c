#include "t9input.h"
#include "atk_ncr.h"
#include "touch.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//T9���뷨 ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2014/2/15
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//*******************************************************************************
//V1.1 20140215
//1,��ӶԸ��ֳߴ�LCD��֧��
//2,����t9_pinyin_process����һ������������bug.
////////////////////////////////////////////////////////////////////////////////// 	   


//12������ť��3���ַ�����
const u8 *t9_mbtnstr_tbl[3][12]=
{   		
{				   
	";","abc","def","��",
	"ghi","jkl","mno","����",
	"pqrs","tuv","wxyz","����", 
},
{				   
	"1","2","3","��",
	"4","5","6",".",
	"7","8","9","0", 
}, 
{				   
	";","ABC","DEF","��",
	"GHI","JKL","MNO","����",
	"PQRS","TUV","WXYZ","����", 
},
};	 
//�����ż�
const u8 *t9_mbtnsign_tbl[T9_SIGNTBL_SIZE][9]=
{
{				   
	",",".","?", 
	"!",":","��", 
	";","��","()", 
},
{				   
	"+","-","*", 
	"/","=","��", 
	"��","%","��", 
}, 
{				   
	"��","��",">", 
	"<","��","��", 
	"��","��","|", 
},
{				   
	"��","^","��", 
	"&","����","����", 
	"{}","@","#",   
},
{				   
	"��","$","��", 
	"~","��","��", 
	"��","��","��",   
},
};							  
//5������ť
const u8 *t9_vbtnstr_tbl[5]={"ƴ��","123","abc","ABC","��д"};

//�������뷨�ؼ� 
//left,top:��ʼ����.
//width,height:���(����Ϊ5�ı���)�͸߶�(height-8,������6�ı���).
//mode:ģʽ
//[7:5]:0,����
//[4:3]:(�������ť״̬)0,���뷨�л�״̬;1,��������״̬;2,������״̬;
//[2:0]:0,ƴ��;1,123;2,abc;3,ABC,4,��д.
_t9_obj * t9_creat(u16 left,u16 top,u16 width,u16 height,u8 mode)
{
	u8 i,t;
	u16 temp;
	u8 sta=0; 
	u16 tbtnw=0;
	u16 tbtnh=0; 
	u8 xoff=0;
	u8 yoff=0; 
 	_t9_obj * t9_crt; 
	if((width%5)||((height-8)%6))return NULL;//�����������.
  	t9_crt=(_t9_obj*)gui_memin_malloc(sizeof(_t9_obj));//�����ڴ�
	gui_memset(t9_crt,0,sizeof(_t9_obj));
	if(t9_crt==NULL)return NULL;//�ڴ���䲻��.
	//12��������
	tbtnw=(width-10)/5;			//�������
	temp=height-(height-8)/6-2;	//5/12����������ʾ�߶�����		
	tbtnh=(temp-7)/3;			//�����ĸ߶� 
	yoff=height-temp+(temp-3*tbtnh-7)/2+2;//���м�  
	t9_crt->ncr_top=top+(height-8)/6+4;			//��дʶ�������TOP����
	t9_crt->ncr_left=left+(width-10)/5;			//��дʶ�������LEFT����
	t9_crt->ncr_width=width-2*(width-10)/5-4;	//��дʶ������Ŀ��
	t9_crt->ncr_height=temp-4;					//��дʶ������ĸ߶� 
 	t=1; 
	for(i=0;i<12;i++)
	{
		t9_crt->mbtn[i]=btn_creat(left+(tbtnw+2)*t,top+yoff,tbtnw,tbtnh,i,0X02);
		if(t==4)
		{
			t=1;
			yoff+=tbtnh+2;//����ƫ��

		}else t++;				 
		if(t9_crt->mbtn[i]==NULL)
		{
			sta=1;//������ڴ�����ʧ��.
			break;
		}
		t9_crt->mbtn[i]->caption=(u8*)t9_mbtnstr_tbl[0][i];
		t9_crt->mbtn[i]->bcfucolor=WHITE;//�ɿ�ʱΪ��ɫ
		t9_crt->mbtn[i]->bcfdcolor=BLACK;//����ʱΪ��ɫ
	}
 	//8�����ְ���
	tbtnw=width/8-1;			//����ѡ��ť�Ŀ��			                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   //�������
	tbtnh=(height-8)/6;			//����ѡ�񰴼��ĸ߶� 
	xoff=(width-(tbtnw+1)*8)/2;	//���м�   
	for(i=0;i<8;i++)
	{
		t9_crt->hbtn[i]=btn_creat(left+(tbtnw+1)*i+xoff,top+1,tbtnw,tbtnh,i+12,0X04);//���ְ�ť,����ɫ��һ
		t9_crt->hbtn[i]->bcfucolor=BLACK;//�ɿ�ʱΪ��ɫ
		t9_crt->hbtn[i]->bcfdcolor=WHITE;//����ʱΪ��ɫ
 		if(t9_crt->hbtn[i]==NULL)
		{
			sta=1;//������ڴ�����ʧ��.
			break;
		}
	}
 	tbtnw=(width-10)/5-2;	//��ఴť�Ŀ��	                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               //�������
	tbtnh=(height-8)/6;	 	//��ఴ���ĸ߶�  
 	yoff=3+(height-8)/6;	//��5������ťռ�õĸ߶�   
	for(i=0;i<5;i++)
	{
		t9_crt->vbtn[i]=btn_creat(left+1,top+i*(tbtnh+1)+yoff,tbtnw,tbtnh,i+20,0X04);//���ְ�ť,����ɫ��һ
  		if(t9_crt->vbtn[i]==NULL)
		{
			sta=1;//������ڴ�����ʧ��.
			break;
		}
		t9_crt->vbtn[i]->caption=(u8*)t9_vbtnstr_tbl[i];
		if(tbtnh>21)t9_crt->vbtn[i]->font=16;	//Ϊ16����
		else t9_crt->vbtn[i]->font=12;			//Ϊ12����
		t9_crt->vbtn[i]->bcfucolor=0XA535;//�ɿ�ʱΪ�Ұ�ɫ
		t9_crt->vbtn[i]->bcfdcolor=0X0000;//����ʱΪ��ɫ
		t9_crt->vbtn[i]->bkctbl[0]=T9_PANEL_BKC;//�ɿ�ʱΪ���뷨�ı���
		t9_crt->vbtn[i]->bkctbl[1]=0XFFFF;//����ʱ����Ϊ��ɫ
	} 			  	 
  	if(sta)//δ����ɹ�
	{
		t9_delete(t9_crt);
		return NULL;
	}  
	t9_crt->top=top;
	t9_crt->left=left;
	t9_crt->width=width;
	t9_crt->height=height;		 
	t9_crt->mode=mode;
	t9_crt->sta=0;					   
	t9_crt->disstr=NULL;					   
	t9_crt->dispagenum=0;					   
	t9_crt->dispagecur=0;					   
	t9_crt->instr[0]=NULL;					   
	t9_crt->outstr[0]=NULL;					   
	t9_set_inputtype(t9_crt,0);//����Ϊƴ�����뷨
	return t9_crt;
}
//ɾ��t9���뷨
//t9_del:Ҫɾ�������뷨.
void t9_delete(_t9_obj * t9_del)
{
	u8 i;
	i=0;
	if(t9_del==NULL)return;//�Ƿ��ĵ�ַ,ֱ���˳�
	while(i<12)
	{
		btn_delete(t9_del->mbtn[i]);		//ɾ��mbtn
		if(i<8)btn_delete(t9_del->hbtn[i]);	//ɾ��hbtn
		if(i<5)btn_delete(t9_del->vbtn[i]);	//ɾ��vbtn
 		i++;
    }
 	gui_memin_free(t9_del);	//�ͷ����뵽���ڴ�
	alientek_ncr_stop();	//�ͷ���дʶ����ڴ�
}
	 	 
//���һ��ʶ��ļ���
void t9_ncr_clr(_t9_obj * t9x)
{
 	t9x->ncr_tcnt=0;
 	t9x->ncr_status=0;//����һ��ʶ��
  	t9x->ncr_timer_old=0;		 
	gui_fill_rectangle(t9x->ncr_left,t9x->ncr_top,t9x->ncr_width,t9x->ncr_height,0XFFFF);//����ɫ 
}

//���	
//t9x:���뷨
//in_key:���밴��ָ��
//����ֵ:δ�õ�			 					   
u8 t9_check(_t9_obj * t9x,void * in_key)
{
	_in_obj *key=(_in_obj*)in_key;
	u8 i;
	static u16 x=0XFFFF,y=0;
	u8 tcnt=0;	 
 	switch(key->intype)
	{
		case IN_TYPE_TOUCH:	//������������
 			if((t9x->mode&0X07)!=0X04)
			{
				for(i=0;i<12;i++)btn_check(t9x->mbtn[i],in_key);//����д���뷨.
			}else  //��дʶ��
			{	
				btn_check(t9x->mbtn[3],in_key);//�����3������
				btn_check(t9x->mbtn[7],in_key); 
				btn_check(t9x->mbtn[11],in_key);
				if((t9x->ncr_top+1)<key->y&&key->y<(t9x->ncr_top+t9x->ncr_height-2)&&(t9x->ncr_left+1)<key->x&&key->x<(t9x->ncr_left+t9x->ncr_width-1))//����д���ڲ�
				{	    
					t9x->ncr_status=0X80;		//�а�������	   
					if(x==0XFFFF)
					{
						x=key->x;
						y=key->y;
					}
					gui_draw_bline(x,y,key->x,key->y,2,RED);//����
					x=key->x;
					y=key->y;   
					if(t9x->ncr_tcnt<200)//��¼���200������켣.
					{
						tcnt=t9x->ncr_tcnt;
						if(tcnt)tcnt--;
						if(t9x->pbuf[tcnt].x!=(key->x-t9x->ncr_left)||t9x->pbuf[tcnt].y!=(key->y-t9x->top))//������,����������겻���,������ɼ�!!!
						{									 
							t9x->pbuf[t9x->ncr_tcnt].x=key->x-t9x->ncr_left;
							t9x->pbuf[t9x->ncr_tcnt].y=key->y-t9x->ncr_top;
	 						t9x->ncr_tcnt++;
						}		   								    
					}
  				}else if(t9x->ncr_status&0x80)//û�а�������,��֮ǰ�а���
				{		
					x=0XFFFF;	 					
					if(t9x->ncr_timer_old!=GUI_TIMER_10MS)
					{
						t9x->ncr_timer_old=GUI_TIMER_10MS;
						t9x->ncr_status++;
						if((t9x->ncr_status&0x7F)>50&&t9x->ncr_tcnt>1)//�ȴ�������500ms
						{
							alientek_ncr(t9x->pbuf,t9x->ncr_tcnt ,6,4,(char*)t9x->instr);
 							t9x->disstr=t9x->instr;//disstrָ��ǰ���°�ť��caption	
 							t9x->dispagenum=0;//�ַ�������ҳ��Ϊ0
							t9x->dispagecur=0;
							t9_hbtn_updc(t9x,t9x->disstr);//���
 							t9_draw_hbtn(t9x);//������ 
  							t9_ncr_clr(t9x);
						}
					}  
				} 
			}
			for(i=0;i<8;i++)
			{	  
 				btn_check(t9x->hbtn[i],in_key);
				if(i<5)btn_check(t9x->vbtn[i],in_key);
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
	t9_process(t9x);//��������Ϣ
	return 0;
}
//��12������ť
//t9x:���뷨
void t9_draw_mbtn(_t9_obj * t9x)
{
	u8 i;
 	for(i=0;i<12;i++)btn_draw(t9x->mbtn[i]);	  
}
//�������8��ѡ��ť
//t9x:���뷨.
void t9_draw_hbtn(_t9_obj * t9x)
{
	u8 i;
 	for(i=0;i<8;i++)btn_draw(t9x->hbtn[i]);	  
}
//�������5��ѡ��ť
//t9x:���뷨.
void t9_draw_vbtn(_t9_obj * t9x)
{
	u8 i;
 	for(i=0;i<5;i++)btn_draw(t9x->vbtn[i]);	 
}
//��t9�������
//t9x:���뷨.
void t9_draw(_t9_obj * t9x)
{
	gui_fill_rectangle(t9x->left,t9x->top,t9x->width,t9x->height,T9_PANEL_BKC);//����ڲ�
	if((t9x->mode&0X07)!=0x4)t9_draw_mbtn(t9x);//����дģʽ
	else//��дʶ�����뷨.
	{
		btn_draw(t9x->mbtn[3]);//ֻ��Ҫ3����ť
		btn_draw(t9x->mbtn[7]);		   
		btn_draw(t9x->mbtn[11]); 
		t9_ncr_clr(t9x);//���һ��ʶ��
	}
	t9_draw_hbtn(t9x);
	t9_draw_vbtn(t9x);
}

//���º���8����ť��caption
//t9x:���뷨.
//str:��ťcaptionժȡ���ַ���.
void t9_hbtn_updc(_t9_obj * t9x,u8* str)
{
	u8 i=0;//		  
	if(t9x->dispagecur>=t9x->dispagenum&&t9x->dispagenum)return;//��ǰҳ���ڻ��ߴ�����ҳ��
 	for(i=0;i<8;i++)t9x->hbtn[i]->caption=NULL;//���֮ǰ��caption	   
	i=0;
	while(*str!=NULL&&str)
	{
		if(*str>0x80)//������,��2���ֽ����
		{
			t9x->disbuf[i][0]=*str++;
			t9x->disbuf[i][1]=*str++;
			t9x->disbuf[i][2]='\0';	    
			t9x->hbtn[i+1]->caption=t9x->disbuf[i]; 	
		}else //Ӣ��
		{
			t9x->disbuf[i][0]=*str++;
			t9x->disbuf[i][1]='\0';		 
			t9x->hbtn[i+1]->caption=t9x->disbuf[i]; 	
		}
		i++;
		if(i>5)break;
	}							 
	t9x->hbtn[0]->caption="��";
	t9x->hbtn[7]->caption="��";	
	if(t9x->dispagecur==0)t9x->hbtn[0]->bcfucolor=0XA535;//ǰ��û��ҳ��
	else t9x->hbtn[0]->bcfucolor=0X0000;//ǰ�滹��ҳ.

	if(t9x->dispagecur==(t9x->dispagenum-1))t9x->hbtn[7]->bcfucolor=0XA535;//�����һҳ��
	else if(t9x->dispagenum>1)t9x->hbtn[7]->bcfucolor=0X0000;//���滹��ҳ.
	else t9x->hbtn[7]->bcfucolor=0XA535;//����û��ҳ��			   
}	   
//�л����뷨
//t9x:t9
//type:0~4;0,ƴ��;1,123;2,abc;3,ABC,4,��д.
void t9_set_inputtype(_t9_obj *t9x,u8 type)
{
	u8 i;
	u8 mx=0;
	if(type>4)return;//�Ƿ���type.
	for(i=0;i<5;i++)
	{
		t9x->vbtn[i]->caption=(u8*)t9_vbtnstr_tbl[i];
 		if(i==type)t9x->vbtn[i]->bcfucolor=0XFFFF;	//��ǰѡ�е�ģʽ,�ɿ�ʱΪ��ɫ
		else t9x->vbtn[i]->bcfucolor=0XA535;		//�ɿ�ʱΪ�Ұ�ɫ 
 	}		
	for(i=0;i<8;i++)t9x->hbtn[i]->caption=NULL;		//��պ����8����ť
	if(type!=4)//������дʶ��ģʽ
	{
		alientek_ncr_stop();//ֹͣ��дʶ��
		if(type==0||type==2)mx=0;
		else if(type==1)mx=1;
		else if(type==3)mx=2;
		for(i=0;i<12;i++)
		{
			t9x->mbtn[i]->caption=(u8*)t9_mbtnstr_tbl[mx][i];//12������ť������
			t9x->mbtn[i]->bcfucolor=0XFFFF;//�ɿ�ʱ��ɫΪ��ɫ
		}
	}else
	{
		alientek_ncr_init();//��ʼ����дʶ��	  
	}
	t9x->instr[0]='\0';
	t9x->disstr='\0';	//������ʾ�ַ�������
	t9x->dispagenum=0;
	t9x->dispagecur=0;
	t9x->mode&=0xE7;	//ȡ�����(�ַ�/ƴ��),�л�Ϊ���뷨�л�״̬
	t9x->mode&=~(0x07);	//�������뷨״̬
	t9x->mode|=type; 	 
}
//����ɿ�ʱ������id
//t9x:���뷨
//����ֵ:0~24,��ȷ��id;
//		 0xff,û����ȷ��id
u8 t9_keyup_scan(_t9_obj *t9x)
{
	u8 i;
	for(i=0;i<12;i++)
	{   

		if((t9x->mbtn[i]->sta&(1<<6))&&(t9x->mbtn[i]->sta&0x03)==0)//�������ɿ���,����Ч
		{
			t9x->mbtn[i]->sta&=~(1<<6);//�����־
			return t9x->mbtn[i]->id;
		}
		if(i<8)if((t9x->hbtn[i]->sta&(1<<6))&&(t9x->hbtn[i]->sta&0x03)==0)//�������ɿ���,����Ч
		{
			t9x->hbtn[i]->sta&=~(1<<6);//�����־
			return t9x->hbtn[i]->id;
		}
		if(i<5)if((t9x->vbtn[i]->sta&(1<<6))&&(t9x->vbtn[i]->sta&0x03)==0)//�������ɿ���,����Ч
		{
			t9x->vbtn[i]->sta&=~(1<<6);//�����־
			return t9x->vbtn[i]->id;
		}
	}
	return 0xff;
}
//������IDת��Ϊ���ַ���
//id:��ť��id.
//����ֵ:��Ӧ������
u8 t9_id2num(u8 id)
{
	if(id<3)return id+1;
	if(id>3&&id<7)return id;
	if(id>7&id<11)return id-1;
	if(id==11)return 0; 
	return 0;//�Ƿ�idֱ�ӷ���0
}
	   
//�����ť��caption
//t9x:���뷨
//btnx:[2],0,�����vbtn;1,���vbtn;
//	   [1],0,�����hbtn;1,���hbtn;
//	   [0],0,�����mbtn;1,���mbtn;
void t9_clrhbtn_caption(_t9_obj *t9x,u8 btnx)
{
	u8 i;
	if(btnx&0x01)for(i=0;i<12;i++)t9x->mbtn[i]->caption=NULL;	//�������ɿ���,����Ч
	if(btnx&0x02)for(i=0;i<8;i++)t9x->hbtn[i]->caption=NULL;	//�������ɿ���,����Ч 
	if(btnx&0x04)for(i=0;i<5;i++)t9x->vbtn[i]->caption=NULL;	//�������ɿ���,����Ч  
}   
//���ر������������
//t9x:���뷨
void t9_load_sign(_t9_obj *t9x)
{
	u8 i=0;
	u8 offset=0;
	if(t9x->signpage>T9_SIGNTBL_SIZE-1)return;//��������
	t9x->mode&=0xE7;
	t9x->mode|=0x10;//���ģʽΪ���������� 
	for(i=0;i<9;i++)//����caption
	{
		if(i==3)offset=1;
		else if(i==6)offset=2;
		t9x->mbtn[i+offset]->caption=(u8*)t9_mbtnsign_tbl[t9x->signpage][i];
	}
	t9x->mbtn[3]->caption="��";
	t9x->mbtn[7]->caption="��";
	t9x->mbtn[11]->caption="����";
	if(t9x->signpage==0)t9x->mbtn[3]->bcfucolor=0XA535;//��ɫ
	else t9x->mbtn[3]->bcfucolor=0XFFFF;
	if(t9x->signpage==T9_SIGNTBL_SIZE-1)t9x->mbtn[7]->bcfucolor=0XA535;//��ɫ
	else t9x->mbtn[7]->bcfucolor=0XFFFF;
}

//��ֱ��ť�����
//t9x:���뷨
//rimcolor:�߿���ɫ.
void t9_drawvbtn_rim(_t9_obj *t9x,u16 rimcolor)
{
	u8 i;
	for(i=0;i<6;i++)gui_draw_hline(t9x->left,t9x->vbtn[0]->top-1+(t9x->vbtn[0]->height+1)*i,t9x->vbtn[0]->width+2,rimcolor);
	gui_draw_vline(t9x->left,t9x->vbtn[0]->top-1,(t9x->vbtn[0]->height+1)*5,rimcolor);
	gui_draw_vline(t9x->vbtn[0]->left+t9x->vbtn[0]->width,t9x->vbtn[0]->top-1,(t9x->vbtn[0]->height+1)*5,rimcolor);
}

//�ַ����봦��
//t9x:���뷨
//id:����id
void t9_sign_process(_t9_obj *t9x,u8 id)
{
	if(id==3||id==7)//��һҳ/��һҳ
	{
		if(id==3&&t9x->signpage)t9x->signpage--;	  
		if(id==7&&(t9x->signpage<T9_SIGNTBL_SIZE-1))t9x->signpage++;
		t9_load_sign(t9x);//
		t9_draw_mbtn(t9x);//�ػ�����ť
		return;
	}else if(id!=11)//���Ƿ���,���������ı�����   
	{
		t9x->outstr[0]='\0';
		strcat((char*)t9x->outstr,(const char*)t9x->mbtn[id]->caption);	//���Ʊ����ŵ�����ַ�������
	}	 
	t9_set_inputtype(t9x,t9x->mode&0x07);//�л�Ϊ���뷨״̬
	t9_draw(t9x);//�ػ� 
}
//�˸���Ĵ���
//t9x:���뷨   
void t9_back_process(_t9_obj *t9x)
{
	u8 cnt=0;
	if(t9x->disstr==0||t9x->disstr[0]==NULL)//�����ַ�Ϊ0,����0X8,��ʾ�˸� 
	{
		t9x->outstr[cnt++]=0x08;//����0X8,�����˸�  
	}else 
	{														    
		t9x->disstr=NULL;				//����ť�ַ�������
	 	t9_clrhbtn_caption(t9x,0x02);	//�������ť			 
		t9_draw_hbtn(t9x);				//�ػ�����ť		   
 	}
	t9x->outstr[cnt]='\0';			//���������
}
//�ո���Ĵ���
//t9x:���뷨   
void t9_space_process(_t9_obj *t9x)
{
	u8 cnt=0;
	if(t9x->disstr!=0&&t9x->disstr[0]!=NULL)//�����ַ���Ϊ0,ȡ�����ַ����ĵ�һ���ַ�/���� 
	{
		t9x->outstr[cnt++]=t9x->disstr[0];
		if(t9x->disstr[0]>0X80)//�Ǻ���
		{
			t9x->outstr[cnt++]=t9x->disstr[1];
 		}
	}else t9x->outstr[cnt++]=' ';	//����ո�
	t9x->outstr[cnt]='\0';			//���������
	t9x->disstr=NULL;				//����ť�ַ�������
 	t9_clrhbtn_caption(t9x,0x02);	//�������ť			 
	t9_draw_hbtn(t9x);				//�ػ�����ť		   
}
//�س����Ĵ���
//t9x:���뷨   
void t9_enter_process(_t9_obj *t9x)
{
	u8 cnt=0;
	if(t9x->disstr!=0&&t9x->disstr[0]!=NULL)//�����ַ���Ϊ0,ȡ�����ַ����ĵ�һ���ַ�/���� 
	{
		t9x->outstr[cnt++]=t9x->disstr[0];
		if(t9x->disstr[0]>0X80)//�Ǻ���
		{
			t9x->outstr[cnt++]=t9x->disstr[1];
 		}
	}else //����س�
	{
		t9x->outstr[cnt++]=0X0D;//�س�
		t9x->outstr[cnt++]=0X0A;//�س�		  
	}				    
	t9x->outstr[cnt]='\0'; 			//���������  
 	t9x->disstr=NULL;				//����ť�ַ�������
	t9_clrhbtn_caption(t9x,0x02);	//�������ť			 
	t9_draw_hbtn(t9x);				//�ػ�����ť		   
}	  

//����ť����
//t9x:���뷨
//id:����id
void t9_hbtn_process(_t9_obj *t9x,u8 id)
{
 	if(id>19||id<12)return ;
	if(t9x->disstr!=NULL)//�����ַ���Ϊ0    
	{
		if(id==12||id==19)//��ǰ/���
		{
			if(t9x->mode&0x03)return ;//ֻ��ƴ�����뷨��Ҫǰ��ƫ��
			else
			{
				if(id==12)//��ǰ
				{	 
					if(t9x->dispagecur)t9x->dispagecur--;//ҳ��
					else return ;//�޷�����ǰ
				}else
				{
					if(t9x->dispagecur<(t9x->dispagenum-1))t9x->dispagecur++;//ҳ��
					else return ;//�޷��ٺ�
				}
			}
		}else
		{		 
			t9x->outstr[0]='\0';
			if(t9x->hbtn[id-12]->caption!=NULL)
			{
				strcpy((char *)t9x->outstr,(const char*)t9x->hbtn[id-12]->caption);//����
			}else return;//�˰�ť��Ч
			t9x->disstr=NULL;		   
			t9x->dispagenum=0;
			t9x->dispagecur=0;	  
			if((t9x->mode&0X07)==0)//ƴ�����뷨��ʱ��
			{
				t9x->instr[0]='\0';//��������ַ���
				t9_drawvbtn_rim(t9x,T9_PANEL_BKC);//��ԭ
				t9_set_inputtype(t9x,0);//�л�Ϊƴ�����뷨
		 		t9_draw_vbtn(t9x);		//�ػ���ֱ��ť 	    
			}
		}  
	}else
	{
		t9x->dispagenum=0;
		t9x->dispagecur=0;	  
	}  
	t9_hbtn_updc(t9x,t9x->disstr+((u16)t9x->dispagecur*12));//���°�ťcaption
	t9_draw_hbtn(t9x);//�ػ�����ť		   
}

//����ť����
//t9x:���뷨
//id:����id:0~24,�����İ���ID. 
void t9_vbtn_process(_t9_obj *t9x,u8 id)
{
	u8 i=0;
	u16 slen=0;
	u8 temp=0;
	temp=(t9x->mode>>3)&0x03;//ȡ�ô�ʱ����ť��״̬
	if(temp!=1)//���Ǻ�������״̬
	{
		t9_set_inputtype(t9x,id-20);//�л����뷨
		t9_draw(t9x);				//�ػ�
		return ;					//�Ǻ�������״̬,ֱ�ӷ���
	}
	//�������ں�������״̬���õ�.
	if(id<24&&id>20)//���м�
	{
		if(t9x->vbtn[id-20]->caption!=NULL)
		{						   
			t9x->disstr=t9x->pymb[(t9x->pypage)*3+id-21]->pymb;//ѡ���µ�ƴ��
		}else return;
	}else if(id==20||id==24)//��/��һҳ
	{
	    if(id==24)//��һҳ
		{
			if(((t9x->pypage+1)*3)<t9x->pynum)t9x->pypage++;//��һҳ
			else return; 
		}else	 //��һҳ
		{
			if(t9x->pypage>0)t9x->pypage--;//��һҳ��λ��
	        else return ;
		} 		 
		t9_clrhbtn_caption(t9x,0x04);//�����ֱ��ť
		temp=t9x->pypage*3; 
		for(i=0;i<3;i++)
		{								
			if((temp+i)>=(t9x->pynum&0X7F))break;//����pynum
 		 	t9x->vbtn[i+1]->caption=t9x->pymb[temp+i]->py;
 			t9x->vbtn[i+1]->bcfucolor=0XFFFF;//��ɫ
			if(i==0)t9x->disstr=t9x->pymb[temp+i]->pymb;//��һ�����ֵ��disstr.	 
		}					    
	}	 
	t9x->vbtn[0]->caption="��";//�������ҳ�ķ���
 	t9x->vbtn[4]->caption="��";
	temp=t9x->pynum/3+((t9x->pynum%3)?1:0);//pynum������ҳ��
	if(t9x->pypage<(temp-1))t9x->vbtn[4]->bcfucolor=0XFFFF;//����һҳ
	else t9x->vbtn[4]->bcfucolor=0XA535;				   //û����һҳ	  
	if(t9x->pypage>0)t9x->vbtn[0]->bcfucolor=0XFFFF;//����һҳ
	else t9x->vbtn[0]->bcfucolor=0XA535;			//û����һҳ
 	t9_draw_vbtn(t9x);						//�ػ���ֱ��ť 	  
	slen=strlen((const char*)t9x->disstr);	//�õ��ַ�������
	slen/=2;							  	//��Ϊ������,ÿ������Ϊ2���ַ�.
	t9x->dispagenum=slen/6+((slen%6)?1:0);	//�õ��ַ�������ҳ��
	t9x->dispagecur=0;					  	//��0��ʼ
	t9_hbtn_updc(t9x,t9x->disstr);			//�����ַ����������8����ť
	t9_draw_hbtn(t9x);
}

//����ƴ������
//t9x:���뷨
//id:����id
void t9_pinyin_process(_t9_obj *t9x,u8 id)
{
	u8 temp;
	u8 str[2];
 	temp=strlen((char*)t9x->instr);//������������
	switch(id)
	{
		case 0://�������л�
			t9x->signpage=0;
			t9_load_sign(t9x);	//װ�ر����Ű�ť
			t9_draw_mbtn(t9x);	//�ػ�����ť
		 	t9_clrhbtn_caption(t9x,0x02);//���ˮƽ��ť��caption����
		 	t9_draw_hbtn(t9x);	//�ػ�ˮƽ��ť  
			return;				//�л�Ϊ������
		case 3://�˸�
			if(temp>1)
			{
				t9x->instr[temp-1]='\0';//�����ַ�����Ϊ��
				temp--;
			}else if(temp==1)//���һ���ַ���. 
			{
				t9_drawvbtn_rim(t9x,T9_PANEL_BKC);//��ԭ
				t9_set_inputtype(t9x,0);//�л�Ϊƴ�����뷨
		 		t9_draw_vbtn(t9x);		//�ػ���ֱ��ť 
		 		t9_draw_hbtn(t9x);		//�ػ�ˮƽ��ť  
				return;
			}else//�˸� 
			{	
				t9_back_process(t9x);	
				return;
			}
			break;
		case 7://�ո�
 			t9_space_process(t9x);	//�ո���
			t9_set_inputtype(t9x,0);//�л�Ϊƴ�����뷨
	 		t9_draw_vbtn(t9x);		//�ػ���ֱ��ť 
	 		t9_draw_hbtn(t9x);		//�ػ�ˮƽ��ť  
			return;	   
		case 11://�س�����
			t9_enter_process(t9x);		
			t9_set_inputtype(t9x,0);//�л�Ϊƴ�����뷨
	 		t9_draw_vbtn(t9x);		//�ػ���ֱ��ť 
	 		t9_draw_hbtn(t9x);		//�ػ�ˮƽ��ť  
			return;
	}	  
	if(temp<6)//�����ַ�����û����
	{
		t9_drawvbtn_rim(t9x,0XA535);//���߿�	  
		t9x->mode&=0xE7;
		t9x->mode|=0x08;//���ģʽΪ�������� 
		if(id!=3)
		{
			str[0]=t9_id2num(id)+'0';
			str[1]='\0';
		}else str[0]='\0';	  
		strcat((char*)t9x->instr,(const char*)str);	//���ת���
		t9x->pynum=get_matched_pymb(t9x->instr,(py_index **)t9x->pymb);//�õ�ƥ���ƴ������
		t9x->pypage=1;//��Ԥ��Ϊ1,��ִ��һ����һҳ����,����page��Ϊ0.
		if(t9x->pynum)
		{
			if(t9x->pynum&0X80)//����ƥ��
			{
				t9x->pynum&=0X7F;//����ƥ��ĸ���
				t9x->instr[t9x->pynum]='\0';//ȥ����Ч���룡	 
				if(t9x->pynum>1)t9x->pynum=get_matched_pymb(t9x->instr,(py_index **)t9x->pymb);//���»�ȡ��ȫƥ���ַ�����
			}  
		}else t9x->pynum=1;//������һ�� 
		if(t9x->pynum&0X80)t9x->pynum=1;
		t9_vbtn_process(t9x,20);
	}	 
}

//t9���뷨����.
//t9x:���뷨
void t9_process(_t9_obj *t9x)
{				   
	u8 id;
 	u16 slen;  
	u8 temp; 
	id=t9_keyup_scan(t9x);
	if(id!=0xff)//��Ч����
	{
		if(id>=20)//���������5����ť��
		{		   
			t9_vbtn_process(t9x,id);//��������ť	   
		}else if(id<=11)//����ť
		{	
			temp=t9x->mode&0X07;
			switch(temp)
			{
				case 0://ƴ�����뷨
					if(((t9x->mode>>3)&0x03)==0X02)//������״̬
					{
						t9_sign_process(t9x,id);
					}else//��������״̬
					{
						t9_pinyin_process(t9x,id);								   
					}
					break;
				case 1://123    
					if(id==3)
					{
						t9_back_process(t9x);
						break;//�˸�.
					}
					else if(id==7)t9x->outstr[0]='.';//С����
				    else t9x->outstr[0]=t9_id2num(id)+'0';
 					t9x->outstr[1]='\0';//���������   
					break;
				case 2://abc
 				case 3://ABC
				case 4://��д
					if(((t9x->mode>>3)&0x03)==0X02)//������״̬
					{
						t9_sign_process(t9x,id);
					}else
					{			    
						switch(id)
						{
							case 0://�л�Ϊ������
	 							t9x->signpage=0;
								t9_load_sign(t9x);	//װ�ر����Ű�ť
								t9_draw_mbtn(t9x);	//�ػ�����ť
							 	t9_clrhbtn_caption(t9x,0x02);//���ˮƽ��ť��caption����
							 	t9_draw_hbtn(t9x);	//�ػ�ˮƽ��ť  
								break;				//�л�Ϊ������
							case 3://�˸�.
					 			t9_back_process(t9x);	//�˸���		 
								break;	   
							case 7://�ո�
					 			t9_space_process(t9x);	//�ո���		 
								break;	   
							case 11://�س���
 					 			t9_enter_process(t9x);	//�س�������		 
								break;
							default://���������ĸ/�������л�
								t9x->disstr=t9x->mbtn[id]->caption;//disstrָ��ǰ���°�ť��caption	
	 							slen=strlen((const char*)t9x->disstr);//�õ��ַ�������
								t9x->dispagenum=slen/6+(slen%6)?1:0;//�õ��ַ�������ҳ��
								t9x->dispagecur=0;
								t9_hbtn_updc(t9x,t9x->disstr);//���
								t9_draw_hbtn(t9x);
								break;

						}	   								 
					}   
					break;	  
			}	
		}else//id ��12~19֮��
		{	
			switch(t9x->mode&0X07)
			{
				case 0://ƴ�����뷨	   
 				case 2://abc   
  				case 3://ABC   
				case 4://��д
				    t9_hbtn_process(t9x,id);//�������ť
					break;
 				case 1://��������
					break;
			}	 
		}
	}
}	





