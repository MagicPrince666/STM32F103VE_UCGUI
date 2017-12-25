#include "appplay.h"
#include "timer.h"
#include "nesplay.h"
#include "ps2play.h"
#include "wifi_common.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//APP-����Ӧ�ó��� ����	   
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

#define APPPLAY_EX_BACKCOLOR	0X0000			//�����ⲿ����ɫ
#define APPPLAY_IN_BACKCOLOR	0X8C51			//�����ڲ�����ɫ
#define APPPLAY_NAME_COLOR		0X001F			//��������ɫ

#define APPPLAY_ALPHA_VAL 		18 				//APPѡ��͸��������
#define APPPLAY_ALPHA_COLOR		WHITE			//APP͸��ɫ
	

//�����
const u8* appplay_caption_tbl[GUI_LANGUAGE_NUM]=
{
"Ӧ������","��������","Applications"
};
//Ӧ�ó������б�
//�����б�Ϊ16��,���ֳ��Ȳ�Ҫ����8���ֽ�(4������)
u8*const appplay_appname_tbl[GUI_LANGUAGE_NUM][16]=
{
{
	"����ң��","PWM����","�����ֱ�","Ӧ�ó���",
	"Ӧ�ó���","Ӧ�ó���","Ӧ�ó���","Ӧ�ó���",
	"NES","Ӧ�ó���","Ӧ�ó���","Ӧ�ó���",
	"Ӧ�ó���","Ӧ�ó���","Ӧ�ó���","Ӧ�ó���",
},
{
	"�t���b��","���ó���","���ó���","���ó���",
	"���ó���","���ó���","���ó���","���ó���",
	"���ó���","���ó���","���ó���","���ó���",
	"���ó���","���ó���","���ó���","���ó���",
},
{
	"REMOTE","APP","APP","APP",
	"APP","APP","APP","APP",
	"APP","APP","APP","APP",   
	"APP","APP","APP","APP",
},
};
 
//appplay��ͼ��·����
u8*const appplay_icospath_tbl[16]=
{
"1:/SYSTEM/APP/APPS/ICOS/remote.bmp",
"1:/SYSTEM/APP/APPS/ICOS/app01.bmp",
"1:/SYSTEM/APP/APPS/ICOS/app02.bmp",
"1:/SYSTEM/APP/APPS/ICOS/app03.bmp",
"1:/SYSTEM/APP/APPS/ICOS/app04.bmp",
"1:/SYSTEM/APP/APPS/ICOS/app05.bmp",
"1:/SYSTEM/APP/APPS/ICOS/app06.bmp",
"1:/SYSTEM/APP/APPS/ICOS/app07.bmp",
"1:/SYSTEM/APP/APPS/ICOS/app08.bmp",
"1:/SYSTEM/APP/APPS/ICOS/app09.bmp",
"1:/SYSTEM/APP/APPS/ICOS/app10.bmp",
"1:/SYSTEM/APP/APPS/ICOS/app11.bmp",
"1:/SYSTEM/APP/APPS/ICOS/app12.bmp",
"1:/SYSTEM/APP/APPS/ICOS/app13.bmp",
"1:/SYSTEM/APP/APPS/ICOS/app14.bmp",
"1:/SYSTEM/APP/APPS/ICOS/app15.bmp",
}; 
///////////////////////////////////////////////////////////////////////////////////
//����ң����Ŀ��Ϣ�б�
u8*const appplay_remote_items_tbl[GUI_LANGUAGE_NUM][3]=
{
{"  ��ֵ:","  ����:","  ����:",},
{"  �Iֵ:","  �Δ�:","  ��̖:",},
{"KEYVAL:","KEYCNT:","SYMBOL:",},
};

//����������
//����ֵ:0,�ɹ�;����,ʧ��;
u8 appplay_load_ui(m_app_dev *appdev) 
{
	u8 i;
	u8 rval=0;
 	_window_obj* twin=0;	//����
	twin=window_creat(3,25,lcddev.width-6,lcddev.height-25-5,0,0X01,16);//��������
   	//twin=window_creat(3,25,234,290,0,0X01,16);//��������
	if(twin)
	{
		twin->captionbkcu=APPPLAY_IN_BACKCOLOR;		//Ĭ��caption�ϲ��ֱ���ɫ
		twin->captionbkcd=APPPLAY_IN_BACKCOLOR;		//Ĭ��caption�²��ֱ���ɫ
		twin->captioncolor=APPPLAY_IN_BACKCOLOR;	//Ĭ��caption����ɫ
	 	twin->windowbkc=APPPLAY_IN_BACKCOLOR;	 	//Ĭ�ϱ���ɫ
	
		gui_fill_rectangle(0,0,lcddev.width,lcddev.height,APPPLAY_EX_BACKCOLOR);//��䱳��ɫ
	  	app_gui_tcbar(0,0,lcddev.width,APP_TEST_TOPBAR_HEIGHT,0x02);			//�·ֽ���	 
 		gui_show_strmid(0,0,lcddev.width,APP_TEST_TOPBAR_HEIGHT,WHITE,16,(u8*)appplay_caption_tbl[gui_phy.language]);//��ʾ����  
		window_draw(twin);
 		for(i=0;i<16;i++)
		{			 
			appdev->icos[i].x=3+5+(i%4)*56;		
			appdev->icos[i].y=25+5+(i/4)*70;	
			appdev->icos[i].width=56;
			appdev->icos[i].height=70;
			appdev->icos[i].path=(u8*)appplay_icospath_tbl[i];
			appdev->icos[i].name=(u8*)appplay_appname_tbl[gui_phy.language][i];
			rval=minibmp_decode(appdev->icos[i].path,appdev->icos[i].x+4,appdev->icos[i].y+4,48,48,0,0);
			if(rval)break;//���������
			gui_show_strmid(appdev->icos[i].x,appdev->icos[i].y+4+48+2,appdev->icos[i].width,12,APPPLAY_NAME_COLOR,12,appdev->icos[i].name);
	 	}
		appdev->selico=0XFF;//Ĭ�ϲ�ѡ���κ�һ��
	}else rval=1;
	window_delete(twin);
	return rval;
}	   
//����ѡ���ĸ�ͼ��
//sel:0~15����ǰҳ��ѡ��ico
void appplay_set_sel(m_app_dev* appdev,u8 sel)
{
	if(sel>=16)return;//�Ƿ�������
	if(appdev->selico<16)
	{
		gui_fill_rectangle(appdev->icos[appdev->selico].x,appdev->icos[appdev->selico].y,appdev->icos[appdev->selico].width,appdev->icos[appdev->selico].height,APPPLAY_IN_BACKCOLOR);//���֮ǰ��ͼƬ
		minibmp_decode(appdev->icos[appdev->selico].path,appdev->icos[appdev->selico].x+4,appdev->icos[appdev->selico].y+4,48,48,0,0);
 		gui_show_strmid(appdev->icos[appdev->selico].x,appdev->icos[appdev->selico].y+4+48+2,appdev->icos[appdev->selico].width,12,APPPLAY_NAME_COLOR,12,appdev->icos[appdev->selico].name);
	};				   
	appdev->selico=sel;
	gui_alphablend_area(appdev->icos[appdev->selico].x,appdev->icos[appdev->selico].y,appdev->icos[appdev->selico].width,appdev->icos[appdev->selico].height,APPPLAY_ALPHA_COLOR,APPPLAY_ALPHA_VAL);
	minibmp_decode(appdev->icos[appdev->selico].path,appdev->icos[appdev->selico].x+4,appdev->icos[appdev->selico].y+4,48,48,0,0);
	gui_show_strmid(appdev->icos[appdev->selico].x,appdev->icos[appdev->selico].y+4+48+2,appdev->icos[appdev->selico].width,12,APPPLAY_NAME_COLOR,12,appdev->icos[appdev->selico].name);
}
//������ɨ��
//����ֵ:0~15,��Ч����.����,��Ч
u8 appplay_tpscan(m_app_dev *appdev)
{
	static u8 firsttpd=0;	//���������־,��ֹһ�ΰ���,��η���
	u8 i=0XFF;
	tp_dev.scan(0);//ɨ��																		 
	if((tp_dev.sta&TP_PRES_DOWN))//�а���������
	{
		if(firsttpd==0)//��һ�δ���?
		{
			firsttpd=1;	  		//����Ѿ������˴˴ΰ���
			for(i=0;i<16;i++)
			{
				if((tp_dev.x[0]>appdev->icos[i].x)&&(tp_dev.x[0]<appdev->icos[i].x+appdev->icos[i].width)
				 &&(tp_dev.y[0]>appdev->icos[i].y)&&(tp_dev.y[0]<appdev->icos[i].y+appdev->icos[i].height))//��������
				{
					break;//�õ�ѡ�еı��	
				}
			}
		}
	}else firsttpd=0;//�ɿ���
	return i;
}

//appӦ��
u8 app_play(void)
{		
	u8 selx=0XFF;
	u8 rval=0;
	m_app_dev *appdev;
	appdev=(m_app_dev*)gui_memin_malloc(sizeof(m_app_dev));
	if(appdev==NULL)rval=1;
	else if(appplay_load_ui(appdev))rval=1;
 	while(rval==0)
	{
		selx=appplay_tpscan(appdev);
		if(selx<16)//����Ч����
		{
			if(selx==appdev->selico)//�ڶ���ѡ�����Ŀ
			{
				//printf("selx:%d\r\n",appdev->selico);
				if(selx==0)//Ŀǰֻ�����˵�һ��Ӧ�ó���,����ң�ؼ���.
				{
					Remote_Play(appdev->icos[selx].name);
 					system_task_return=0;
				}
				if(selx==1)//PWMnes_play();
				{
					PWM_Play(appdev->icos[selx].name);
 					system_task_return=0;
				}
				
				if(selx == 2)
				{
					atk_8266_test();		//����ATK_ESP8266����
					system_task_return=1;
				}
				
				if(selx==3)//PS2�ֱ�
				{
					PS2_Play(appdev->icos[selx].name);
 					system_task_return=0;
				}
				
				if(selx==8)//NES
				{
					nes_play();
 					system_task_return=0;
				}
				
			}else appplay_set_sel(appdev,selx);		 
		}
		if(system_task_return)break;//��Ҫ����
		delay_ms(1000/OS_TICKS_PER_SEC);//��ʱһ��ʱ�ӽ���
 	}
	gui_memin_free(appdev);//�ͷ��ڴ�
	return 0;
}
//app	
u8 PWM_Play(u8* caption)	 
{
	u8 rval=0;
  u8 flag=0;	
  u16 PwmCnt=100;	
	u16 sx=(lcddev.width-120)/2;
	u16 sy=(lcddev.height-80)/2;
	
 	_window_obj* twin=0;	//����
 	twin=window_creat(sx,sy,120,100,0,1<<6|1<<0,16);//��������
	if(twin)
	{	
		twin->caption=caption;					 
	 	twin->windowbkc=APP_WIN_BACK_COLOR;	 								//������ɫ
		window_draw(twin);													//������
	 	app_draw_smooth_line(sx+5,sy+32+1+22,110,1,0Xb1ffc4,0X1600b1);		//������
	 	app_draw_smooth_line(sx+5,sy+32+1+22+22,110,1,0Xb1ffc4,0X1600b1);	//������
		BACK_COLOR=APP_WIN_BACK_COLOR;
 		//��ʾ��Ŀ��Ϣ
		gui_show_ptstr(sx+8,sy+32+1+3,sx+8+56,sy+32+1+3+16,0,BLACK,16,"PWM:",1);
		gui_show_ptstr(sx+8,sy+32+1+3+22,sx+8+56,sy+32+1+3+22+16,0,BLACK,16,"Speed:",1);
		
 	}else rval=1;
	if(rval==0)
	{
		BACK_COLOR=APP_WIN_BACK_COLOR;	//����ɫΪ������ɫ
		POINT_COLOR=RED;				//��ɫ����
		TIM1_PWM_Init(2000,719);
		while(1)
		{	   
			if(system_task_return)break;//��Ҫ����
			TIM1->CCR1 = PwmCnt;	 	 			   
			LCD_ShowNum(sx+8+56,sy+32+1+3,PwmCnt,3,16);	//��ʾ��������
			
      if(flag==0)			PwmCnt++;
      else 		PwmCnt--;
			
      if(PwmCnt>200)
			{
				TIM1->CCR1 = PwmCnt = 200;	
				flag =1;
				delay_ms(40);
			}	
      if(PwmCnt<100)
			{
				TIM1->CCR1 = PwmCnt = 100;	
				flag =0;
				delay_ms(40);
			}			
			delay_ms(40); 
			LED1=!LED1;
		}
	}
	window_delete(twin);
	return rval;

} 

///////////////////////////////////////////////////////////////////////////////////
//����ң��app	
u8 Remote_Play(u8* caption)	 
{
	u8 rval=0;
	u8 key;		 
	u16 sx=(lcddev.width-120)/2;
	u16 sy=(lcddev.height-100)/2;
	
 	_window_obj* twin=0;	//����
 	twin=window_creat(sx,sy,120,100,0,1<<6|1<<0,16);//��������
	if(twin)
	{	
		twin->caption=caption;					 
	 	twin->windowbkc=APP_WIN_BACK_COLOR;	 								//������ɫ
		window_draw(twin);													//������
	 	app_draw_smooth_line(sx+5,sy+32+1+22,110,1,0Xb1ffc4,0X1600b1);		//������
	 	app_draw_smooth_line(sx+5,sy+32+1+22+22,110,1,0Xb1ffc4,0X1600b1);	//������
		BACK_COLOR=APP_WIN_BACK_COLOR;
 		//��ʾ��Ŀ��Ϣ
		gui_show_ptstr(sx+8,sy+32+1+3,sx+8+56,sy+32+1+3+16,0,BLACK,16,(u8*)appplay_remote_items_tbl[gui_phy.language][0],1);
		gui_show_ptstr(sx+8,sy+32+1+3+22,sx+8+56,sy+32+1+3+22+16,0,BLACK,16,(u8*)appplay_remote_items_tbl[gui_phy.language][1],1);
		gui_show_ptstr(sx+8,sy+32+1+3+44,sx+8+56,sy+32+1+3+44+16,0,BLACK,16,(u8*)appplay_remote_items_tbl[gui_phy.language][2],1);
 	}else rval=1;
	if(rval==0)
	{
		BACK_COLOR=APP_WIN_BACK_COLOR;	//����ɫΪ������ɫ
		POINT_COLOR=RED;				//��ɫ����
		Remote_Init();//��ʼ���������
		while(1)
		{	  
			key=Remote_Scan();		 
			if(system_task_return)break;//��Ҫ����
			if(key)
			{				 	 			   
				LCD_ShowNum(sx+8+56,sy+32+1+3,key,3,16);		//��ʾ��ֵ
				LCD_ShowNum(sx+8+56,sy+32+1+3+22,RmtCnt,3,16);	//��ʾ��������
 				switch(key)
				{			    
					case 162: 
						LCD_ShowString(sx+8+56,sy+32+1+3+44,240,320,16,"POWER");	  
						break;
					case 98:
						LCD_ShowString(sx+8+56,sy+32+1+3+44,240,320,16,"UP    ");
						break;
					case 2:
						LCD_ShowString(sx+8+56,sy+32+1+3+44,240,320,16,"PLAY  ");
						break;
					case 226:
						LCD_ShowString(sx+8+56,sy+32+1+3+44,240,320,16,"ALIEN ");
						break;
					case 194:
						LCD_ShowString(sx+8+56,sy+32+1+3+44,240,320,16,"RIGHT ");
						break;
					case 34:
						LCD_ShowString(sx+8+56,sy+32+1+3+44,240,320,16,"LEFT  ");
						break;
					case 56:
						LCD_ShowString(sx+8+56,sy+32+1+3+44,240,320,16,"8     ");
						break;
					case 224:
						LCD_ShowString(sx+8+56,sy+32+1+3+44,240,320,16,"VOL-  ");
						break;
					case 168:
						LCD_ShowString(sx+8+56,sy+32+1+3+44,240,320,16,"DOWN  ");
						break;
					case 144:
						LCD_ShowString(sx+8+56,sy+32+1+3+44,240,320,16,"VOL+  ");
						break;
					case 104:
						LCD_ShowString(sx+8+56,sy+32+1+3+44,240,320,16,"1     ");
						break;
					case 152:
						LCD_ShowString(sx+8+56,sy+32+1+3+44,240,320,16,"2     ");
						break;
					case 176:
						LCD_ShowString(sx+8+56,sy+32+1+3+44,240,320,16,"3     ");
						break;
					case 48:
						LCD_ShowString(sx+8+56,sy+32+1+3+44,240,320,16,"4     ");
						break;
					case 24:
						LCD_ShowString(sx+8+56,sy+32+1+3+44,240,320,16,"5     ");
						break;
					case 122:
						LCD_ShowString(sx+8+56,sy+32+1+3+44,240,320,16,"6     ");
						break;
					case 16:
						LCD_ShowString(sx+8+56,sy+32+1+3+44,240,320,16,"7     ");
						break;
					case 90:
						LCD_ShowString(sx+8+56,sy+32+1+3+44,240,320,16,"9     ");
						break;
					case 66:
						LCD_ShowString(sx+8+56,sy+32+1+3+44,240,320,16,"0     ");
						break;
					case 82:
						LCD_ShowString(sx+8+56,sy+32+1+3+44,240,320,16,"DELETE");
						break;	    
				}
			} 
			delay_ms(10); 
		}
	}
	TIM5->CR1&=~(1<<0);//�رն�ʱ��5
	window_delete(twin);
	return rval;

} 












