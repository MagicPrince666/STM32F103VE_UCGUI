#include "common.h"
#include "mainui.h"
#include "appplay.h"
#include "settings.h"
#include "calendar.h"	 
#include "paint.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//APPͨ�� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2014/2/16
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//*******************************************************************************
//V1.1 20140216
//�����Ը��ֱַ���LCD��֧��.
////////////////////////////////////////////////////////////////////////////////// 	   

//ģʽѡ���б�Ĵ�������
u8* const APP_MODESEL_CAPTION_TBL[GUI_LANGUAGE_NUM]=
{
"ģʽѡ��","ģʽ�x��","Mode Select",
}; 
//��ʾ��Ϣ�Ĵ�������
u8*const APP_REMIND_CAPTION_TBL[GUI_LANGUAGE_NUM]=
{
"��ʾ��Ϣ","��ʾ��Ϣ","Remind",	 
};
//���ѱ���Ĵ�������
 u8*const APP_SAVE_CAPTION_TBL[GUI_LANGUAGE_NUM]=
{
"�����ļ�?","�����ļ�?","Save?",
}; 
//�����ļ�ʧ��,��ʾ�Ƿ����SD��? ,200�Ŀ��
u8*const APP_CREAT_ERR_MSG_TBL[GUI_LANGUAGE_NUM]=
{					 	 
"����ʧ��,���SD��!","����ʧ��,�z��SD��!","Failed,Check SD!",
};  
//app��Ҫ���ܽ������
u8*const APP_MFUNS_CAPTION_TBL[APP_FUNS_NUM][GUI_LANGUAGE_NUM]=
{
	{"����ͼ��","��ӈD��","EBOOK",},
	{"�������","���a���","PHOTOS",},
	{"USB����","USB�B��","USB",},
 	{"Ӧ������","��������","APPLICATIONS",},
 	{"","","",},
	{"ϵͳ����","ϵ�y�O��","SYSTEM SET",},
	{"","","",},
	{"���ߴ���","�o������","WIRELESS",},
	{"���±�","ӛ�±�","NOTEPAD",},  
};

u8*const  APP_OK_PIC="1:/SYSTEM/APP/COMMON/ok.bmp";				//ȷ��ͼ��
u8*const  APP_CANCEL_PIC="1:/SYSTEM/APP/COMMON/cancel.bmp";		//ȡ��ͼ��
u8*const  APP_UNSELECT_PIC="1:/SYSTEM/APP/COMMON/unselect.bmp";	//δѡ��ͼ��
u8*const  APP_SELECT_PIC="1:/SYSTEM/APP/COMMON/select.bmp";		//ѡ��ͼ��
 
//PC2LCD2002����ȡģ����:����ʽ,˳��(��λ��ǰ),����.C51��ʽ.																		    
//��������:
//���������:ASCII��+��(' '+95)
//��ͨ����:ASCII��
u8*const APP_ASCII_S6030="1:/SYSTEM/APP/COMMON/fonts60.fon";	//���������60*30����������·�� 
u8*const APP_ASCII_5427="1:/SYSTEM/APP/COMMON/font54.fon";		//��ͨ����54*27����������·�� 
u8*const APP_ASCII_3618="1:/SYSTEM/APP/COMMON/font36.fon";		//��ͨ����36*18����������·��
u8*const APP_ASCII_2814="1:/SYSTEM/APP/COMMON/font28.fon";		//��ͨ����28*14����������·�� 


u8* asc2_s6030=0;	//���������60*30���������
u8* asc2_5427=0;	//��ͨ����54*27���������
u8* asc2_3618=0;	//��ͨ����36*18���������
u8* asc2_2814=0;	//��ͨ����28*14���������




//ALIENTEK logo ͼ��(18*24��С)
//PCtoLCD2002ȡģ��ʽ:����,����ʽ,˳��
const u8 APP_ALIENTEK_ICO1824[]=
{
0x03,0xF0,0x00,0x0F,0xF8,0x00,0x1F,0xFC,0x00,0x3F,0xFE,0x00,0x7F,0xFF,0x00,0x7F,
0xFF,0x00,0xFF,0xFF,0x80,0xFF,0xFF,0x80,0xFF,0xFF,0x80,0xFF,0xFF,0x80,0xFF,0xFF,
0x80,0xFF,0xFF,0x80,0xCF,0xF9,0x80,0xC7,0xF1,0x80,0x43,0xE1,0x00,0x61,0xC3,0x00,
0x31,0xC6,0x00,0x39,0xCE,0x00,0x1F,0xFC,0x00,0x0F,0xF8,0x00,0x07,0xF0,0x00,0x03,
0xE0,0x00,0x01,0xC0,0x00,0x00,0x80,0x00
};
//ALIENTEK logo ͼ��(24*32��С)
//PCtoLCD2002ȡģ��ʽ:����,����ʽ,˳��
const u8 APP_ALIENTEK_ICO2432[]=
{
0x00,0x7F,0x00,0x01,0xFF,0xC0,0x03,0xFF,0xE0,0x07,0xFF,0xF0,0x0F,0xFF,0xF8,0x1F,
0xFF,0xFC,0x1F,0xFF,0xFC,0x3F,0xFF,0xFE,0x3F,0xFF,0xFE,0x7F,0xFF,0xFF,0x7F,0xFF,
0xFF,0x7F,0xFF,0xFF,0x7F,0xFF,0xFF,0x7F,0xFF,0xFF,0x7F,0xFF,0xFF,0x7F,0xFF,0xFF,
0x73,0xFF,0xE7,0x70,0xFF,0x87,0x30,0xFF,0x86,0x30,0x7F,0x06,0x38,0x3E,0x0E,0x1C,
0x3E,0x1C,0x1E,0x1C,0x3C,0x0F,0x1C,0x78,0x07,0xFF,0xF0,0x07,0xFF,0xF0,0x03,0xFF,
0xE0,0x01,0xFF,0xC0,0x00,0xFF,0x80,0x00,0x7F,0x00,0x00,0x3E,0x00,0x00,0x1C,0x00,
};
//ALIENTEK logo ͼ��(36*48��С)
//PCtoLCD2002ȡģ��ʽ:����,����ʽ,˳��
const u8 APP_ALIENTEK_ICO3648[]=
{
0x00,0x01,0xFC,0x00,0x00,0x00,0x0F,0xFF,0x80,0x00,0x00,0x1F,0xFF,0xC0,0x00,0x00,
0x7F,0xFF,0xF0,0x00,0x00,0xFF,0xFF,0xF8,0x00,0x01,0xFF,0xFF,0xFC,0x00,0x03,0xFF,
0xFF,0xFE,0x00,0x07,0xFF,0xFF,0xFF,0x00,0x07,0xFF,0xFF,0xFF,0x80,0x0F,0xFF,0xFF,
0xFF,0x80,0x0F,0xFF,0xFF,0xFF,0xC0,0x1F,0xFF,0xFF,0xFF,0xC0,0x1F,0xFF,0xFF,0xFF,
0xC0,0x1F,0xFF,0xFF,0xFF,0xC0,0x1F,0xFF,0xFF,0xFF,0xE0,0x1F,0xFF,0xFF,0xFF,0xE0,
0x3F,0xFF,0xFF,0xFF,0xE0,0x3F,0xFF,0xFF,0xFF,0xE0,0x3F,0xFF,0xFF,0xFF,0xE0,0x3F,
0xFF,0xFF,0xFF,0xE0,0x3F,0xFF,0xFF,0xFF,0xE0,0x3F,0xFF,0xFF,0xFF,0xE0,0x3F,0xFF,
0xFF,0xFF,0xE0,0x3F,0xFF,0xFF,0xFF,0xE0,0x3C,0xFF,0xFF,0xFD,0xE0,0x38,0x3F,0xFF,
0xF0,0xE0,0x18,0x1F,0xFF,0xC0,0xE0,0x1C,0x0F,0xFF,0x80,0xC0,0x1C,0x07,0xFF,0x01,
0xC0,0x0C,0x03,0xFF,0x01,0xC0,0x0E,0x03,0xFE,0x03,0x80,0x07,0x01,0xFC,0x07,0x80,
0x07,0x81,0xFC,0x07,0x00,0x03,0xC0,0xFC,0x0F,0x00,0x03,0xE0,0xF8,0x3E,0x00,0x01,
0xF0,0xF8,0x7E,0x00,0x01,0xFE,0xFD,0xFC,0x00,0x00,0xFF,0xFF,0xFC,0x00,0x00,0x7F,
0xFF,0xF8,0x00,0x00,0x3F,0xFF,0xF0,0x00,0x00,0x3F,0xFF,0xE0,0x00,0x00,0x1F,0xFF,
0xC0,0x00,0x00,0x0F,0xFF,0x80,0x00,0x00,0x07,0xFF,0x00,0x00,0x00,0x03,0xFE,0x00,
0x00,0x00,0x01,0xFC,0x00,0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};
////////////////////////////////α����������취////////////////////////////////
u32 random_seed=1;
void app_srand(u32 seed)
{
	random_seed=seed;
}
//��ȡα�����
//���Բ���0~RANDOM_MAX-1�������
//seed:����
//max:���ֵ	  		  
//����ֵ:0~(max-1)�е�һ��ֵ 		
u32 app_get_rand(u32 max)
{			    	    
	random_seed=random_seed*22695477+1;
	return (random_seed)%max; 
}  
/////////////////////////////////////////////////////////////////////////////////
//����LCD�ֱ���
//mode:0,��ԭʼ�ֱ���(��ʵ�ʷֱ���)
//     1,ǿ������Ϊ240*320�ֱ���.
void app_set_lcdsize(u8 mode)
{
	if(mode)
	{
		LCD_Clear(BLACK);
		if(lcddev.id==0X6804||lcddev.id==0X5310||lcddev.id==0X5510) 	//ǿ��������Ļ�ֱ���Ϊ320*240.��֧��3.5�����
		{
			lcddev.width=240;
			lcddev.height=320;     
			LCD_Scan_Dir(L2R_U2D);//ʹ������Ч
		}
	}else
	{
		if(lcddev.id==0X6804||lcddev.id==0X5310)//3.5��,�ֱ���Ϊ320*480
		{
			lcddev.width=320;
			lcddev.height=480;  
		}else if(lcddev.id==0X5510)				//4.3��,�ֱ���Ϊ480*800
		{
			lcddev.width=480;
			lcddev.height=800; 
		}else
		{
			lcddev.width=240;
			lcddev.height=320; 
		}
		LCD_Scan_Dir(L2R_U2D);//ʹ������Ч
	}
	gui_phy.lcdwidth=lcddev.width;
	gui_phy.lcdheight=lcddev.height; 
}
//��ȡ����ɫ
//x,y,width,height:����ɫ��ȡ��Χ
//ctbl:����ɫ���ָ��
void app_read_bkcolor(u16 x,u16 y,u16 width,u16 height,u16 *ctbl)
{
	u16 x0,y0,ccnt;
	ccnt=0;
	for(y0=y;y0<y+height;y0++)
	{
		for(x0=x;x0<x+width;x0++)
		{
			ctbl[ccnt]=gui_phy.read_point(x0,y0);//��ȡ��ɫ
			ccnt++;
		}
	}
}  
//�ָ�����ɫ
//x,y,width,height:����ɫ��ԭ��Χ
//ctbl:����ɫ���ָ��
void app_recover_bkcolor(u16 x,u16 y,u16 width,u16 height,u16 *ctbl)
{
	u16 x0,y0,ccnt;
	ccnt=0;
	for(y0=y;y0<y+height;y0++)
	{
		for(x0=x;x0<x+width;x0++)
		{
			gui_phy.draw_point(x0,y0,ctbl[ccnt]);//��ȡ��ɫ
			ccnt++;
		}
	}
}
//2ɫ��
//x,y,width,height:���꼰�ߴ�.
//mode:	���÷ֽ���
//	    [3]:�ұ߷ֽ���
//		[2]:��߷ֽ���
//		[1]:�±߷ֽ���
//		[0]:�ϱ߷ֽ���
void app_gui_tcbar(u16 x,u16 y,u16 width,u16 height,u8 mode)
{
 	u16 halfheight=height/2;
 	gui_fill_rectangle(x,y,width,halfheight,LIGHTBLUE);  			//���ײ���ɫ(ǳ��ɫ)	
 	gui_fill_rectangle(x,y+halfheight,width,halfheight,GRAYBLUE); 	//���ײ���ɫ(����ɫ)
	if(mode&0x01)gui_draw_hline(x,y,width,DARKBLUE);
	if(mode&0x02)gui_draw_hline(x,y+height-1,width,DARKBLUE);
	if(mode&0x04)gui_draw_vline(x,y,height,DARKBLUE);
	if(mode&0x08)gui_draw_vline(x+width-1,y,width,DARKBLUE);
} 
//�õ����ֵ�λ��
//num:����
//dir:0,�Ӹ�λ����λ.1,�ӵ�λ����λ.
//����ֵ:���ֵ�λ��.(���Ϊ10λ)
//ע:0,��λ��Ϊ0λ.
u8 app_get_numlen(long long num,u8 dir)
{
#define MAX_NUM_LENTH		10		//�������ֳ���
	u8 i=0,j;
	u8 temp=0;  
	if(dir==0)//�Ӹߵ���
	{
		i=MAX_NUM_LENTH-1;
		while(1)
		{
			temp=(num/gui_pow(10,i))%10;
			if(temp||i==0)break;
			i--;
		}
	}else	//�ӵ͵���
	{
		j=0;
 		while(1)
		{
			if(num%10)
			{
				i=app_get_numlen(num,0);    
				return i;
			}
			if(j==(MAX_NUM_LENTH-1))break;	 
			num/=10;
			j++;
		}
	}
	if(i)return i+1;
	else if(temp)return 1;
	else return 0;	
}
 
//��ʾ��ɫͼ��
//x,y,width,height:���꼰�ߴ�.
//icosbase:����λ��
//color:�������ɫ.
//bkcolor:����ɫ
void app_show_mono_icos(u16 x,u16 y,u8 width,u8 height,u8 *icosbase,u16 color,u16 bkcolor)
{
	u16 rsize;
	u8 i,j;
	u8 temp;
	u8 t=0;
	u16 x0=x;//����x��λ��
	rsize=width/8+((width%8)?1:0);//ÿ�е��ֽ���
 	for(i=0;i<rsize*height;i++)
	{
		temp=icosbase[i];
		for(j=0;j<8;j++)
		{
	        if(temp&0x80)gui_phy.draw_point(x,y,color);  
			else gui_phy.draw_point(x,y,bkcolor);  	 
			temp<<=1;
			x++;
			t++;			//��ȼ�����
			if(t==width)
			{
				t=0;
				x=x0;
				y++;  
				break;
			}
		}
	}				
} 
//��ʾһ��������(֧�ָ���)
//ע��:����������Ǵ��ҵ�����ʾ��.
//x,y:��ʼ��ʾ������
//num:����	   
//flen:С����λ��.0,û��С������;1,��һλС��;2,����λС��;�����Դ�����.
//clen:Ҫ�����λ��(���ǰһ�εĺۼ�)
//font:����
//color:������ɫ.
//bkcolor:����ɫ
void app_show_float(u16 x,u16 y,long long num,u8 flen,u8 clen,u8 font,u16 color,u16 bkcolor)
{			   
	u8 offpos=0;
	u8 ilen=0;						//�������ֳ��Ⱥ�С�����ֵĳ���
	u8 negative=0;					//�������
	u16 maxlen=(u16)clen*(font/2);	//����ĳ���
	gui_fill_rectangle(x-maxlen,y,maxlen,font,bkcolor);//���֮ǰ�ĺۼ�
	if(num<0) 
	{
		num=-num;
  		negative=1;
	}
	ilen=app_get_numlen(num,0);	//�õ���λ��
	gui_phy.back_color=bkcolor;
	if(flen)
	{
 		gui_show_num(x-(font/2)*flen,y,flen,color,font,num,0X80);//��ʾС������
		gui_show_ptchar(x-(font/2)*(flen+1),y,gui_phy.lcdwidth,gui_phy.lcdheight,0,color,font,'.',0);//��ʾС����
		offpos=2+flen;
	} 
	if(ilen<=flen)ilen=1;
 	else 
	{
		offpos=ilen+1;
		ilen=ilen-flen;	//�õ��������ֵĳ���.
	}
	num=num/gui_pow(10,flen);//�õ���������
	gui_show_num(x-(font/2)*offpos,y,ilen,color,font,num,0X80);	//���0��ʾ 
	if(negative)gui_show_ptchar(x-(font/2)*(offpos+1),y,gui_phy.lcdwidth,gui_phy.lcdheight,0,color,font,'-',0);//��ʾ����		 
} 			  
					   
//topname:�����ʱ��Ҫ��ʾ������		 				 
//mode:
//[0]:0,����ʾ�Ϸ�ɫ��;1,��ʾ�Ϸ�ɫ��
//[1]:0,����ʾ�·�ɫ��;1,��ʾ�·�ɫ��
//[2]:0,����ʾ����;1,��ʾ����
//[3~7]:����
//����ֵ:��	 						  
void app_filebrower(u8 *topname,u8 mode)
{		
  	if(mode&0X01)app_gui_tcbar(0,0,lcddev.width,gui_phy.tbheight,0x02);								//�·ֽ���
	if(mode&0X02)app_gui_tcbar(0,lcddev.height-gui_phy.tbheight,lcddev.width,gui_phy.tbheight,0x01);//�Ϸֽ���
	if(mode&0X04)gui_show_strmid(0,0,lcddev.width,gui_phy.tbheight,WHITE,gui_phy.tbfsize,topname);	  
}  
//��һ�������м���ʾ����
//x,y,width,height:����
//num:Ҫ��ʾ������
//len:λ��
//size:����ߴ�
//ptcolor,bkcolor:������ɫ�Լ�����ɫ   
void app_show_nummid(u16 x,u16 y,u16 width,u16 height,u32 num,u8 len,u8 size,u16 ptcolor,u16 bkcolor)
{
	u16 numlen;
	u8 xoff,yoff;
	numlen=(size/2)*len;//���ֳ���
	if(numlen>width||size>height)return;
	xoff=(width-numlen)/2;
	yoff=(height-size)/2;
	POINT_COLOR=ptcolor;
	BACK_COLOR=bkcolor;
	LCD_ShowxNum(x+xoff,y+yoff,num,len,size,0X80);//��ʾ�������
}
//��һ��ƽ�����ɵĲ�ɫ��(�����)
//���м�Ϊ���,����չ��
//x,y,width,height:����������ߴ�
//sergb,mrgb:��ֹ��ɫ���м���ɫ
void app_draw_smooth_line(u16 x,u16 y,u16 width,u16 height,u32 sergb,u32 mrgb)
{	  
	gui_draw_smooth_rectangle(x,y,width/2,height,sergb,mrgb);	   		//ǰ��ν���
	gui_draw_smooth_rectangle(x+width/2,y,width/2,height,mrgb,sergb);   //���ν���
}      

//�жϴ�������ǰֵ�ǲ�����ĳ��������
//tp:������
//x,y,width,height:����ͳߴ� 
//����ֵ:0,����������;1,��������.
u8 app_tp_is_in_area(_m_tp_dev *tp,u16 x,u16 y,u16 width,u16 height)
{						 	 
	if(tp->x[0]<=(x+width)&&tp->x[0]>=x&&tp->y[0]<=(y+height)&&tp->y[0]>=y)return 1;
	else return 0;							 	
}
//��ʾ��Ŀ
//x,y,itemwidth,itemheight:��Ŀ���꼰�ߴ�
//name:��Ŀ����
//icopath:ͼ��·��
void app_show_items(u16 x,u16 y,u16 itemwidth,u16 itemheight,u8*name,u8*icopath,u16 color,u16 bkcolor)
{
  	gui_fill_rectangle(x,y,itemwidth,itemheight,bkcolor);	//��䱳��ɫ
	gui_show_ptstr(x+5,y+(itemheight-16)/2,x+itemwidth-10-APP_ITEM_ICO_SIZE-5,y+itemheight,0,color,16,name,1);	//��ʾ��Ŀ����
	if(icopath)minibmp_decode(icopath,x+itemwidth-10-APP_ITEM_ICO_SIZE,y+(itemheight-APP_ITEM_ICO_SIZE)/2,APP_ITEM_ICO_SIZE,APP_ITEM_ICO_SIZE,0,0);			//����APP_ITEM_ICO_SIZE*APP_ITEM_ICO_SIZE��bmpͼƬ
}		 
//��õ�ǰ��Ŀ��ͼ��·��
//mode:0,��ѡģʽ;1,��ѡģʽ
//selpath,unselpath:ѡ�кͷ�ѡ�е�ͼ��·��
//selx:ѡ��ͼ����(��ѡģʽ)/��Чͼ������(��ѡģʽ)
//index:��ǰ��Ŀ���
u8 * app_get_icopath(u8 mode,u8 *selpath,u8 *unselpath,u8 selx,u8 index)
{
	u8 *icopath=0;
	if(mode)//��ѡģʽ
	{
		if(selx&(1<<index))icopath=selpath;	//��ѡ�е���Ŀ
		else icopath=unselpath;			  	//�Ƿ�ѡ�е���Ŀ
	}else
	{
		if(selx==index)icopath=selpath;		//��ѡ�е���Ŀ
		else icopath=unselpath;			  	//�Ƿ�ѡ�е���Ŀ
	}
	return icopath;
}
//��Ŀѡ��
//x,y,width,height:����ߴ�(width��СΪ150,height��СΪ72)
//items[]:��Ŀ���ּ�
//itemsize:����Ŀ��(��󲻳���8��)
//selx:���.��ѡģʽʱ,��Ӧ�����ѡ�����.��ѡģʽʱ,��Ӧѡ�����Ŀ.
//mode:
//[7]:0,��OK��ť;1,��OK��ť
//[6]:0,����ȡ����ɫ;1,��ȡ����ɫ
//[5]:0,��ѡģʽ;1,��ѡģʽ
//[4]:0,������ͼ��;1,����ͼ��
//[3:0]:����
//caption:��������	  
//����ֵ:0,ok;����,ȡ�����ߴ���.
u8 app_items_sel(u16 x,u16 y,u16 width,u16 height,u8 *items[],u8 itemsize,u8 *selx,u8 mode,u8*caption) 
{
	u8 rval=0,res;
	u8 selsta=0;	//ѡ��״̬Ϊ0,
					//[7]:����Ƿ��Ѿ���¼��һ�ΰ��µ���Ŀ;
					//[6:4]:����
	                //[3:0]:��һ�ΰ��µ���Ŀ
	u16 i;

	u8 temp;
	u16 itemheight=0;		//ÿ����Ŀ�ĸ߶�
	u16 itemwidth=0;		//ÿ����Ŀ�Ŀ��
	u8* unselpath=0;		//δѡ�е�ͼ���·��
	u8* selpath=0;			//ѡ��ͼ���·��
	u8* icopath=0;

 	_window_obj* twin=0;	//����
 	_btn_obj * okbtn=0;		//ȷ����ť
 	_btn_obj * cancelbtn=0; //ȡ����ť

	if(itemsize>8||itemsize<1)return 0xff;	//��Ŀ������
	if(width<150||height<72)return 0xff; 	//�ߴ����
	
	itemheight=(height-72)/itemsize-1;	//�õ�ÿ����Ŀ�ĸ߶�
	itemwidth=width-10;					//ÿ����Ŀ�Ŀ��

 	twin=window_creat(x,y,width,height,0,1|(1<<5)|((1<<6)&mode),16);//��������
  	if(mode&(1<<7))
	{
   		temp=(width-APP_ITEM_BTN1_WIDTH*2)/3;
		okbtn=btn_creat(x+temp,y+height-APP_ITEM_BTN_HEIGHT-5,APP_ITEM_BTN1_WIDTH,APP_ITEM_BTN_HEIGHT,0,0x02);							//����OK��ť
		cancelbtn=btn_creat(x+APP_ITEM_BTN1_WIDTH+temp*2,y+height-APP_ITEM_BTN_HEIGHT-5,APP_ITEM_BTN1_WIDTH,APP_ITEM_BTN_HEIGHT,0,0x02);//����CANCEL��ť
		if(twin==NULL||okbtn==NULL||cancelbtn==NULL)rval=1;
		else
		{
	 		okbtn->caption=(u8*)GUI_OK_CAPTION_TBL[gui_phy.language];//ȷ��
			okbtn->bkctbl[0]=0X8452;//�߿���ɫ
			okbtn->bkctbl[1]=0XAD97;//��һ�е���ɫ				
			okbtn->bkctbl[2]=0XAD97;//�ϰ벿����ɫ
			okbtn->bkctbl[3]=0X8452;//�°벿����ɫ
		}
	}else 
	{
   		temp=(width-APP_ITEM_BTN2_WIDTH)/2;
		cancelbtn=btn_creat(x+temp,y+height-APP_ITEM_BTN_HEIGHT-5,APP_ITEM_BTN2_WIDTH,APP_ITEM_BTN_HEIGHT,0,0x02);	//����CANCEL��ť
		if(twin==NULL||cancelbtn==NULL)rval=1;
	}
 	if(rval==0)//֮ǰ�Ĳ�������
	{
 		twin->caption=caption;
		twin->windowbkc=APP_WIN_BACK_COLOR;	     
 		cancelbtn->caption=(u8*)GUI_CANCEL_CAPTION_TBL[gui_phy.language];//ȡ��
 		cancelbtn->bkctbl[0]=0X8452;//�߿���ɫ
		cancelbtn->bkctbl[1]=0XAD97;//��һ�е���ɫ				
		cancelbtn->bkctbl[2]=0XAD97;//�ϰ벿����ɫ
		cancelbtn->bkctbl[3]=0X8452;//�°벿����ɫ

		if(mode&(1<<4))//��Ҫ����ͼ��
		{
  			if(mode&(1<<5))//��ѡģʽ
			{
				unselpath=(u8*)APP_CANCEL_PIC;		//δѡ�е�ͼ���·��
				selpath=(u8*)APP_OK_PIC;			//ѡ��ͼ���·��
			}else		   //��ѡģʽ
			{
				unselpath=(u8*)APP_UNSELECT_PIC;	//δѡ�е�ͼ���·��
				selpath=(u8*)APP_SELECT_PIC;		//ѡ��ͼ���·��
			}
		}
		window_draw(twin);				//��������
		btn_draw(cancelbtn);			//����ť
	    if(mode&(1<<7))btn_draw(okbtn);	//����ť
		for(i=0;i<itemsize;i++)
		{
			icopath=app_get_icopath(mode&(1<<5),selpath,unselpath,*selx,i); //�õ�ͼ��·��
			app_show_items(x+5,y+32+i*(itemheight+1),itemwidth,itemheight,items[i],icopath,BLACK,twin->windowbkc);//��ʾ���е���Ŀ
			if((i+1)!=itemsize)app_draw_smooth_line(x+5,y+32+(i+1)*(itemheight+1)-1,itemwidth,1,0Xb1ffc4,0X1600b1);//������
 		}
		while(rval==0)
		{
			tp_dev.scan(0);    
			in_obj.get_key(&tp_dev,IN_TYPE_TOUCH);	//�õ�������ֵ   
			delay_ms(1000/OS_TICKS_PER_SEC);		//��ʱһ��ʱ�ӽ���
			if(system_task_return){rval=1;break;};	//��Ҫ����	
			if(mode&(1<<7))
			{
				res=btn_check(okbtn,&in_obj);		//ȷ�ϰ�ť���
				if(res)
				{
					if((okbtn->sta&0X80)==0)//����Ч����
					{
						rval=0XFF;
						break;//ȷ�ϰ�ť
					}
				}
			}   
			res=btn_check(cancelbtn,&in_obj);		//ȡ����ť���
			if(res)
			{
				if((cancelbtn->sta&0X80)==0)//����Ч����
				{
					rval=1;
					break;//ȡ����ť	 
				}
			}
			temp=0XFF;//�����,���Ϊ0XFF,���ɿ���ʱ��,˵���ǲ�����Ч�����ڵ�.�����0XFF,���ʾTP�ɿ���ʱ��,������Ч������.
			for(i=0;i<itemsize;i++)
			{
				if(tp_dev.sta&TP_PRES_DOWN)//������������
				{
				 	if(app_tp_is_in_area(&tp_dev,x+5,y+32+i*(itemheight+1),itemwidth,itemheight))//�ж�ĳ��ʱ��,��������ֵ�ǲ�����ĳ��������
					{ 
						if((selsta&0X80)==0)//��û�а��¹�
						{
							icopath=app_get_icopath(mode&(1<<5),selpath,unselpath,*selx,i); //�õ�ͼ��·��
							app_show_items(x+5,y+32+i*(itemheight+1),itemwidth,itemheight,items[i],icopath,BLACK,APP_ITEM_SEL_BKCOLOR);//��ѡ��Ŀ
							selsta=i;		//��¼��һ�ΰ��µ���Ŀ
							selsta|=0X80;	//����Ѿ����¹���
						}
						break;		
					}
				}else //���������ɿ���
				{
				 	if(app_tp_is_in_area(&tp_dev,x+5,y+32+i*(itemheight+1),itemwidth,itemheight))//�ж�ĳ��ʱ��,��������ֵ�ǲ�����ĳ��������
					{ 
						temp=i;	   
						break;
					}
				}
			}
			if((selsta&0X80)&&(tp_dev.sta&TP_PRES_DOWN)==0)//�а��¹�,�Ұ����ɿ���
			{
				if((selsta&0X0F)==temp)//�ɿ�֮ǰ������Ҳ���ڰ���ʱ��������.
				{
					if(mode&(1<<5))//��ѡģʽ,ִ��ȡ������
					{
						if((*selx)&(1<<temp))*selx&=~(1<<temp);
						else *selx|=1<<temp;
					}else//��ѡģʽ
					{																					  
						app_show_items(x+5,y+32+(*selx)*(itemheight+1),itemwidth,itemheight,items[*selx],unselpath,BLACK,twin->windowbkc);//ȡ��֮ǰѡ�����Ŀ
						*selx=temp;
					}
				}else temp=selsta&0X0F;//�õ���ʱ���µ���Ŀ��
 				icopath=app_get_icopath(mode&(1<<5),selpath,unselpath,*selx,temp); //�õ�ͼ��·��
				app_show_items(x+5,y+32+temp*(itemheight+1),itemwidth,itemheight,items[temp],icopath,BLACK,twin->windowbkc);//��ѡ��Ŀ
				selsta=0;//ȡ��
			}
 		}
 	}
	window_delete(twin);
	btn_delete(okbtn);
	btn_delete(cancelbtn);
	system_task_return=0;
	if(rval==0XFF)return 0;
	return rval;
} 

//�½�ѡ���
//sel:��ǰѡ�е���Ŀ
//top:��ǰ�������Ŀ
//caption:ѡ�������
//items[]:��Ŀ���ּ�
//itemsize:����Ŀ��
//����ֵ:[7]:0,���µ��Ƿ��ذ���,���߷����˴���;1,������ȷ�ϰ���,����˫����ѡ�е���Ŀ.
//     [6:0]:0,��ʾ�޴���;����,�������.
u8 app_listbox_select(u8 *sel,u8 *top,u8 * caption,u8 *items[],u8 itemsize)
{
	u8  res;
	u8 rval=0;			//����ֵ	  
  	u16 i;	    						   
 
 	_btn_obj* rbtn=0;			//���ذ�ť�ؼ�
 	_btn_obj* okbtn=0;			//ȷ�ϰ�ť�ؼ�
	_listbox_obj * tlistbox;	//listbox 
	
	if(*sel>=itemsize||*top>=itemsize)return 2;//��������/�����Ƿ�
 	app_filebrower(caption,0X07);				//��ʾ����  
   	tlistbox=listbox_creat(0,gui_phy.tbheight,lcddev.width,lcddev.height-gui_phy.tbheight*2,1,gui_phy.tbfsize);//����һ��filelistbox
	if(tlistbox==NULL)rval=1;					//�����ڴ�ʧ��.
	else	//�����Ŀ
	{
		for(i=0;i<itemsize;i++)
		{
			res=listbox_addlist(tlistbox,items[i]);
			if(res)
			{
				rval=1;
				break;
			}
		}
	}
	if(rval==0)//�ɹ��������Ŀ
	{
		tlistbox->scbv->topitem=*top;
		tlistbox->selindex=*sel;
		listbox_draw_listbox(tlistbox);
		rbtn=btn_creat(lcddev.width-2*gui_phy.tbfsize-8-1,lcddev.height-gui_phy.tbheight,2*gui_phy.tbfsize+8,gui_phy.tbheight-1,0,0x03);//�������ְ�ť
 		okbtn=btn_creat(0,lcddev.height-gui_phy.tbheight,2*gui_phy.tbfsize+8,gui_phy.tbheight-1,0,0x03);//����ȷ�����ְ�ť
		if(rbtn==NULL||okbtn==NULL)rval=1;		//û���㹻�ڴ湻����
		else
		{
		 	rbtn->caption=(u8*)GUI_BACK_CAPTION_TBL[gui_phy.language];	//����
			rbtn->font=gui_phy.tbfsize;//�����µ������С	 	 
			rbtn->bcfdcolor=WHITE;	//����ʱ����ɫ
			rbtn->bcfucolor=WHITE;	//�ɿ�ʱ����ɫ
			btn_draw(rbtn);//����ť
	
			
		 	okbtn->caption=(u8*)GUI_OK_CAPTION_TBL[gui_phy.language];	//����
		 	okbtn->font=gui_phy.tbfsize;//�����µ������С	 
			okbtn->bcfdcolor=WHITE;	//����ʱ����ɫ
			okbtn->bcfucolor=WHITE;	//�ɿ�ʱ����ɫ
			btn_draw(okbtn);//����ť
		}	   
 	}  
   	while(rval==0)
	{
		tp_dev.scan(0);    
		in_obj.get_key(&tp_dev,IN_TYPE_TOUCH);	//�õ�������ֵ   
		delay_ms(1000/OS_TICKS_PER_SEC);		//��ʱһ��ʱ�ӽ���
 		if(system_task_return)break;			//TPAD����
		res=btn_check(rbtn,&in_obj);		    //���ذ�ť���
		if(res)if(((rbtn->sta&0X80)==0))break;	//�˳� 
		res=btn_check(okbtn,&in_obj);//ȷ�ϰ�ť���
		if(res)
		{
			if(((okbtn->sta&0X80)==0))//��ť״̬�ı���
			{ 
				*top=tlistbox->scbv->topitem;//��¼�˳�ʱѡ�����Ŀ
				*sel=tlistbox->selindex;
				rval|=1<<7;//���˫��������								 				   			   
  			}	 
		}   
		listbox_check(tlistbox,&in_obj);	//ɨ�� 
		if(tlistbox->dbclick==0X80)//˫����
		{	  
			*top=tlistbox->scbv->topitem;//��¼�˳�ʱѡ�����Ŀ
			*sel=tlistbox->selindex;
			rval|=1<<7;//���˫��������								 				   			   
		}
	}	
	listbox_delete(tlistbox);		//ɾ��listbox
	btn_delete(okbtn);				//ɾ����ť	  	 
	btn_delete(rbtn);				//ɾ����ť 
	return rval; 
}
//������·����Ҫ���޸�
//des:Ŀ�껺����
//diskx:�µĴ��̱��
//src:ԭ·��
void app_system_file_chgdisk(u8 *des,u8 *diskx,u8 *src)
{ 
	//static u8 t=0;
	strcpy((char*)des,(const char*)src);
	des[0]=diskx[0]; 
	//printf("path[%d]:%s\r\n",t++,des);	//��ӡ�ļ���
}
//ϵͳ�ܹ���Ҫ��ͼ��/ͼƬ/ϵͳ�ļ���141��,����SPB����ռ20��.����121��ΪAPP������ռ��
//�⻹������SYSTEM/FONT�ļ����ڵ������ļ�.
//141���ļ��ܴ�С:1,033,804 �ֽ�(0.98MB)
//3�������ļ��ܴ�С:1,514,984 �ֽ�(1.44MB)
//������е�ϵͳ�ļ��Ƿ�����
//diskx:����·��."0",SD��;"1",SPI FLASH
//����ֵ:0,����
//    ����,�������
u8 app_system_file_check(u8* diskx)
{
	u8 i;
	u8 rval=0;
	u8 res=0;
	FIL *f_check;
	u8 *path;
	f_check=(FIL *)gui_memin_malloc(sizeof(FIL));	//����FIL�ֽڵ��ڴ����� 
	path=gui_memin_malloc(200);						//Ϊpath�����ڴ� 
	if(!f_check||!path)rval=0XFF;//����ʧ��
	while(rval==0)
	{	
		//������ͼ����,9��. 
 		for(i=0;i<9;i++)
		{
			app_system_file_chgdisk(path,diskx,(u8*)mui_icos_path_tbl[i]); 
	 		res=f_open(f_check,(const TCHAR*)path,FA_READ);//ֻ����ʽ���ļ�
			if(res)break;		//��ʧ��   
			f_close(f_check);    //�ر��ļ�  
		}
		rval+=i;
		if(i<9){rval++;break;}   
		//COMMON�ļ����,5��
		app_system_file_chgdisk(path,diskx,APP_OK_PIC); 
		res=f_open(f_check,(const TCHAR*)path,FA_READ);//ֻ����ʽ���ļ�
		if(res)break;			//��ʧ��      
    	f_close(f_check);		//�ر��ļ�     
		rval++;		  
		app_system_file_chgdisk(path,diskx,APP_CANCEL_PIC); 
		res=f_open(f_check,(const TCHAR*)path,FA_READ);//ֻ����ʽ���ļ�
		if(res)break;			//��ʧ��      
    	f_close(f_check);		//�ر��ļ�     
		rval++;		  
		app_system_file_chgdisk(path,diskx,APP_UNSELECT_PIC); 
		res=f_open(f_check,(const TCHAR*)path,FA_READ);//ֻ����ʽ���ļ�
		if(res)break;			//��ʧ��      
    	f_close(f_check);		//�ر��ļ�     
		rval++;		  
		app_system_file_chgdisk(path,diskx,APP_SELECT_PIC); 
		res=f_open(f_check,(const TCHAR*)path,FA_READ);//ֻ����ʽ���ļ�
		if(res)break;			//��ʧ��      
    	f_close(f_check);		//�ر��ļ�     
		rval++;		  
		app_system_file_chgdisk(path,diskx,APP_ASCII_S6030); 
		res=f_open(f_check,(const TCHAR*)path,FA_READ);//ֻ����ʽ���ļ�
		if(res)break;			//��ʧ��      
    	f_close(f_check);		//�ر��ļ�     
		rval++;	      
		app_system_file_chgdisk(path,diskx,APP_ASCII_5427); 
		res=f_open(f_check,(const TCHAR*)path,FA_READ);//ֻ����ʽ���ļ�
		if(res)break;			//��ʧ��      
    	f_close(f_check);		//�ر��ļ�     
		rval++;	      
		app_system_file_chgdisk(path,diskx,APP_ASCII_3618); 
		res=f_open(f_check,(const TCHAR*)path,FA_READ);//ֻ����ʽ���ļ�
		if(res)break;			//��ʧ��      
    	f_close(f_check);		//�ر��ļ�     
		rval++;	      
		app_system_file_chgdisk(path,diskx,APP_ASCII_2814); 
		res=f_open(f_check,(const TCHAR*)path,FA_READ);//ֻ����ʽ���ļ�
		if(res)break;			//��ʧ��      
    	f_close(f_check);		//�ر��ļ�     
		rval++;	      
		//���Ӧ�ó���16��ͼ��
   		for(i=0;i<16;i++)
		{
			app_system_file_chgdisk(path,diskx,(u8*)appplay_icospath_tbl[i]); 
	 		res=f_open(f_check,(const TCHAR*)path,FA_READ);//ֻ����ʽ���ļ�
			if(res)break;		//��ʧ��       
			f_close(f_check);    //�ر��ļ�  
		}
		rval+=i;
		if(i<16)break;
		printf("\r\ntotal system files:%d\r\n",rval);
		rval=0;
		break;    
	}
	gui_memin_free(f_check);//�ͷ��ڴ� 
	gui_memin_free(path);	//�ͷ��ڴ�
	return rval; 
}

////////////////////////////////////////////////////////////////////////////////////////////
//һ�´��������ʵ��SYSTEM�ļ��и���
u8*const  APP_SYSTEM_APPPATH="0:/SYSTEM/APP";		//APP�ļ���·��
u8*const  APP_SYSTEM_FONTPATH="0:/SYSTEM/FONT";		//FONT�ļ���·��
u8*const  APP_SYSTEM_SYSICOPATH="0:/SYSTEM/SYSICO";	//SYSICO�ļ���·��
u8*const  APP_SYSTEM_DSTPATH="1:/SYSTEM";			//ϵͳ�ļ�Ŀ��·��

//�ļ�������Ϣ��ʾ���� 
//�ļ�������Ϣ��ʾ���������
static u16 cpdmsg_x;
static u16 cpdmsg_y;
static u8  cpdmsg_fsize;	


//ϵͳ������ʱ��,������ʾ���½���
//*pname:�����ļ�����
//pct:�ٷֱ�
//mode:ģʽ
//[0]:�����ļ���
//[1]:���°ٷֱ�pct
//[2]:�����ļ���
//[3~7]:����
//����ֵ:0,����;
//       1,��������
u8 app_boot_cpdmsg(u8*pname,u8 pct,u8 mode)
{													 
	if(mode&0X01)
	{
		LCD_Fill(cpdmsg_x+9*(cpdmsg_fsize/2),cpdmsg_y,cpdmsg_x+(9+16)*(cpdmsg_fsize/2),cpdmsg_y+cpdmsg_fsize,BLACK);	//����ɫ
		LCD_ShowString(cpdmsg_x+9*(cpdmsg_fsize/2),cpdmsg_y,16*(cpdmsg_fsize/2),cpdmsg_fsize,cpdmsg_fsize,pname);	//��ʾ�ļ���,�16���ַ����
		printf("\r\nCopy File:%s\r\n",pname);  
	}
	if(mode&0X04)printf("Copy Folder:%s\r\n",pname);
	if(mode&0X02)//���°ٷֱ�
	{
		LCD_ShowString(cpdmsg_x+(9+16+1+3)*(cpdmsg_fsize/2),cpdmsg_y,lcddev.width,lcddev.height,cpdmsg_fsize,"%");		
 		LCD_ShowNum(cpdmsg_x+(9+16+1)*(cpdmsg_fsize/2),cpdmsg_y,pct,3,cpdmsg_fsize);//��ʾ��ֵ
		printf("File Copyed:%d\r\n",pct);
	}
	return 0;	
}
//����app_boot_cpdmsg����ʾ����
//x,y:����.
//fisze:�����С
void app_boot_cpdmsg_set(u16 x,u16 y,u8 fsize)
{
	cpdmsg_x=x;
	cpdmsg_y=y;
	cpdmsg_fsize=fsize;
}
//ϵͳ����
//����ֵ:0,����
//    ����,����,0XFFǿ���˳���
u8 app_system_update(u8(*fcpymsg)(u8*pname,u8 pct,u8 mode))
{
	u32 totsize=0;
	u32 cpdsize=0;
	u8 res=0;
	totsize+=exf_fdsize((u8*)APP_SYSTEM_APPPATH);//�õ������ļ��е��ܴ�С
	totsize+=exf_fdsize((u8*)APP_SYSTEM_FONTPATH);
	totsize+=exf_fdsize((u8*)APP_SYSTEM_SYSICOPATH);
	printf("totsize:%d\r\n",totsize);
	f_mkdir((const TCHAR *)APP_SYSTEM_DSTPATH);//ǿ�ƴ���Ŀ���ļ���
 	res=exf_fdcopy(fcpymsg,(u8*)APP_SYSTEM_APPPATH,(u8*)APP_SYSTEM_DSTPATH,&totsize,&cpdsize,1);
	if(res)
	{
		if(res!=0xff)return 1;	//��һ�׶θ��Ƴ���
		else return 0XFF;		//ǿ���˳�
	}
	res=exf_fdcopy(fcpymsg,(u8*)APP_SYSTEM_FONTPATH,(u8*)APP_SYSTEM_DSTPATH,&totsize,&cpdsize,1);
	if(res)
	{
		if(res!=0xff)return 1;	//�ڶ��׶θ��Ƴ���
		else return 0XFF;		//ǿ���˳�
	}
 	res=exf_fdcopy(fcpymsg,(u8*)APP_SYSTEM_SYSICOPATH,(u8*)APP_SYSTEM_DSTPATH,&totsize,&cpdsize,1);
	if(res)
	{
		if(res!=0xff)return 1;	//�����׶θ��Ƴ���
		else return 0XFF;		//ǿ���˳�
	}
	return 0;
}
//�õ�STM32�����к�
//sn0,sn1,sn2:3���������к�
void app_getstm32_sn(u32 *sn0,u32 *sn1,u32 *sn2)
{
	*sn0=*(vu32*)(0x1FFFF7E8);
	*sn1=*(vu32*)(0x1FFFF7EC);
	*sn2=*(vu32*)(0x1FFFF7F0);    
}
//��ӡSN
void app_usmart_getsn(void)
{
	u32 sn0,sn1,sn2;
	app_getstm32_sn(&sn0,&sn1,&sn2);
	printf("\r\nSerial Number:%X%X%X\r\n",sn0,sn1,sn2);
}
//�õ��汾��Ϣ
//buf:�汾������(������Ҫ6���ֽ�,�籣��:V1.00)
//ver:�汾�궨��
//len:�汾λ��.1,����ֻ��1��λ�İ汾,V1;2,����2��λ�汾,V1.0;3,����3��λ�汾,V1.00;
void app_get_version(u8*buf,u32 ver,u8 len)
{
	u8 i=0;
	buf[i++]='V';
	if(len==1)
	{
		buf[i++]=ver%10+'0';
	}else if(len==2)
	{ 	
		buf[i++]=(ver/10)%10+'0';
		buf[i++]='.';
		buf[i++]=ver%10+'0';
	}else
	{
		buf[i++]=(ver/100)%10+'0';
		buf[i++]='.';
		buf[i++]=(ver/10)%10+'0';
		buf[i++]=ver%10+'0';	 
	}
	buf[i]=0;//���������	 			   
}

//��ʼ������ϵͳ����
//����ϵͳ����,��������,���������ݵ�
u8 app_system_parameter_init(void)
{
	sysset_read_para(&systemset);		//��ȡϵͳ������Ϣ 
	calendar_read_para(&alarm);			//��ȡ������Ϣ 
	if(systemset.saveflag!=0X0A&&systemset.syslanguage>2)//֮ǰû�б����/���Դ���
	{
		systemset.syslanguage=0;		//Ĭ��Ϊ�������� 
		systemset.saveflag=0X0A;  		//���ñ�����
		sysset_save_para(&systemset);	//����ϵͳ������Ϣ 
 	} 
	if(alarm.saveflag!=0X0A)  			//֮ǰû�б����
	{
		alarm.weekmask=0;				//������������Ϊ0
		alarm.ringsta=0;				//����״̬��������־Ϊ0
		alarm.hour=0;					//����ʱ������Ϊ00:00
		alarm.min=0;					  
 		alarm.saveflag=0X0A;  			//���ñ�����
		calendar_save_para(&alarm);		//��������������Ϣ 
 	}  
	f_mkdir("0:TEXT");		//ǿ�ƴ����ļ���,�����±���
   	f_mkdir("0:PAINT");		//ǿ�ƴ����ļ���,����ͼ��
 	gui_phy.language=systemset.syslanguage;			//��������
 	return 0;	
} 












