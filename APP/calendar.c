#include "calendar.h" 	      						  
#include "stdio.h"
#include "settings.h"  
#include "ds18b20.h"
#include "stmflash.h"
#include "math.h"
#include "rtc.h" 
#include "led.h" 
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
 
_alarm_obj alarm;		//���ӽṹ��
_calendar_obj calendar;	//�����ṹ��

static u16 TIME_TOPY;		//	120
static u16 OTHER_TOPY;		//	200 	
 
u8*const calendar_week_table[GUI_LANGUAGE_NUM][7]=
{
{"������","����һ","���ڶ�","������","������","������","������"},
{"������","����һ","���ڶ�","������","������","������","������"},
{"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"},
};
//���ӱ���
u8*const calendar_alarm_caption_table[GUI_LANGUAGE_NUM]=
{
"����","�[�","ALARM",
};
//���찴ť
u8*const calendar_alarm_realarm_table[GUI_LANGUAGE_NUM]=
{
"����","���","REALARM",
}; 
u8*const calendar_loading_str[GUI_LANGUAGE_NUM][3]=
{
{
	"���ڼ���,���Ժ�...",
	"δ��⵽DS18B20!",
	"�����ڲ��¶ȴ�����...",
},
{
	"���ڼ���,���Ժ�...",
	"δ��⵽DS18B20!",
	"�����ڲ��¶ȴ�����...",
},
{
	"Loading...",
	"DS18B20 Check Failed!",
	"Start Internal Sensor...",
},
};	 
//���³�ʼ������		    
//alarmx:���ӽṹ��
//calendarx:�����ṹ��
void calendar_alarm_init(_alarm_obj *alarmx,_calendar_obj *calendarx) 
{	  
	u8 temp;
	RTC_Get(); 
	if(calendarx->week==7)temp=1<<0;
	else temp=1<<calendarx->week; 
	if(alarmx->weekmask&temp)		//��Ҫ����
	{  
		printf("alarm:%d-%d-%d %d:%d\r\n",calendarx->w_year,calendarx->w_month,calendarx->w_date,alarmx->hour,alarmx->min);
		RTC_Alarm_Set(calendarx->w_year,calendarx->w_month,calendarx->w_date,alarmx->hour,alarmx->min,0);//��������ʱ��	 
	}   
} 
//����������
//type:��������	   
//0,��.
//1,��.��.
//2,��.��.��
//4,��.��.��.��
void calendar_alarm_ring(u8 type)
{
	u8 i;	 
	for(i=0;i<(type+1);i++)
	{
		LED1=0;
		delay_ms(100);
		LED1=1;
		delay_ms(100);
	}	 
}  
//���ݵ�ǰ������,����������.
void calendar_date_refresh(void) 
{
 	u8 weekn;   //�ܼĴ�
	u16 offx=(lcddev.width-240)/2;
 	//��ʾ����������
	POINT_COLOR=BRED; 
	BACK_COLOR=BLACK; 
	LCD_ShowxNum(offx+5,OTHER_TOPY+9,(calendar.w_year/100)%100,2,16,0);//��ʾ��  20/19  
	LCD_ShowxNum(offx+21,OTHER_TOPY+9,calendar.w_year%100,2,16,0);     //��ʾ��  
	LCD_ShowString(offx+37,OTHER_TOPY+9,lcddev.width,lcddev.height,16,"-"); //"-"
	LCD_ShowxNum(offx+45,OTHER_TOPY+9,calendar.w_month,2,16,0X80);     //��ʾ��
	LCD_ShowString(offx+61,OTHER_TOPY+9,lcddev.width,lcddev.height,16,"-"); //"-"
	LCD_ShowxNum(offx+69,OTHER_TOPY+9,calendar.w_date,2,16,0X80);      //��ʾ��	  
	//��ʾ�ܼ�?
	POINT_COLOR=RED;
    weekn=calendar.week;
	Show_Str(5+offx,OTHER_TOPY+35,lcddev.width,lcddev.height,(u8 *)calendar_week_table[gui_phy.language][weekn],16,0); //��ʾ�ܼ�?	
													 
}
//�������ݱ�����:SYSTEM_PARA_SAVE_BASE+sizeof(_system_setings)+sizeof(_vs10xx_obj)
//��ȡ����������Ϣ
//alarm:������Ϣ 
void calendar_read_para(_alarm_obj * alarm)
{
	STMFLASH_Read(SYSTEM_PARA_SAVE_BASE+sizeof(_system_setings),(u16*)alarm,sizeof(_alarm_obj));
}
//д������������Ϣ
//alarm:������Ϣ 
void calendar_save_para(_alarm_obj * alarm)
{
  	OS_CPU_SR cpu_sr=0;
	alarm->ringsta&=0X7F;	//������λ
	OS_ENTER_CRITICAL();	//�����ٽ���(�޷����жϴ��) 
	STMFLASH_Write(SYSTEM_PARA_SAVE_BASE+sizeof(_system_setings),(u16*)alarm,sizeof(_alarm_obj));
	OS_EXIT_CRITICAL();		//�˳��ٽ���(���Ա��жϴ��)
} 

//���崦��(�ߴ�:44*20)
//x,y:����
//����ֵ,������
u8 calendar_alarm_msg(u16 x,u16 y)
{
	u8 rval=0;  			  	
  	u16 *lcdbuf=0;							//LCD�Դ� 
   	lcdbuf=(u16*)gui_memin_malloc(44*20*2);	//�����ڴ� 
 	if(lcdbuf)								//����ɹ�
	{
		app_read_bkcolor(x,y,44,20,lcdbuf);	//��ȡ����ɫ
		gui_fill_rectangle(x,y,44,20,LIGHTBLUE);
		gui_draw_rectangle(x,y,44,20,BROWN);
		gui_show_num(x+2,y+2,2,RED,16,alarm.hour,0X81); 
 		gui_show_ptchar(x+2+16,y+2,x+2+16+8,y+2+16,0,RED,16,':',1);
		gui_show_num(x+2+24,y+2,2,RED,16,alarm.min,0X81); 
  		OSTaskSuspend(6); //����������
		while(rval==0)
		{
			tp_dev.scan(0);
			if(tp_dev.sta&TP_PRES_DOWN)//������������
			{
				if(app_tp_is_in_area(&tp_dev,x,y,44,20))//�ж�ĳ��ʱ��,��������ֵ�ǲ�����ĳ��������
				{
					rval=0XFF;//ȡ��
					break;
				} 
			 }
			delay_ms(5);  
    		if(system_task_return)break;			//��Ҫ����	 
		}	 
		app_recover_bkcolor(x,y,44,20,lcdbuf);	//��ȡ����ɫ
	}else rval=1;
	system_task_return=0;
	alarm.ringsta&=~(1<<7);	//ȡ������   
	calendar_alarm_init((_alarm_obj*)&alarm,&calendar);	//���³�ʼ������
	gui_memin_free(lcdbuf); 
	OSTaskResume(6); 		//�ָ�������
	system_task_return=0;
	return rval;
} 
//��Բ��ָ�����
//x,y:�������ĵ�
//size:���̴�С(ֱ��)
//d:���̷ָ�,���ӵĸ߶�
void calendar_circle_clock_drawpanel(u16 x,u16 y,u16 size,u16 d)
{
	u16 r=size/2;//�õ��뾶 
	u16 sx=x-r;
	u16 sy=y-r;
	u16 px0,px1;
	u16 py0,py1; 
	u16 i; 
	gui_fill_circle(x,y,r,WHITE);		//����Ȧ
	gui_fill_circle(x,y,r-4,BLACK);		//����Ȧ
	for(i=0;i<60;i++)//�����Ӹ�
	{ 
		px0=sx+r+(r-4)*sin((app_pi/30)*i); 
		py0=sy+r-(r-4)*cos((app_pi/30)*i); 
		px1=sx+r+(r-d)*sin((app_pi/30)*i); 
		py1=sy+r-(r-d)*cos((app_pi/30)*i);  
		gui_draw_bline1(px0,py0,px1,py1,0,WHITE);		
	}
	for(i=0;i<12;i++)//��Сʱ��
	{ 
		px0=sx+r+(r-5)*sin((app_pi/6)*i); 
		py0=sy+r-(r-5)*cos((app_pi/6)*i); 
		px1=sx+r+(r-d)*sin((app_pi/6)*i); 
		py1=sy+r-(r-d)*cos((app_pi/6)*i);  
		gui_draw_bline1(px0,py0,px1,py1,2,YELLOW);		
	}
	for(i=0;i<4;i++)//��3Сʱ��
	{ 
		px0=sx+r+(r-5)*sin((app_pi/2)*i); 
		py0=sy+r-(r-5)*cos((app_pi/2)*i); 
		px1=sx+r+(r-d-3)*sin((app_pi/2)*i); 
		py1=sy+r-(r-d-3)*cos((app_pi/2)*i);  
		gui_draw_bline1(px0,py0,px1,py1,2,YELLOW);		
	}
	gui_fill_circle(x,y,d/2,WHITE);		//������Ȧ
}
//��ʾʱ��
//x,y:�������ĵ�
//size:���̴�С(ֱ��)
//d:���̷ָ�,���ӵĸ߶�
//hour:ʱ��
//min:����
//sec:����
void calendar_circle_clock_showtime(u16 x,u16 y,u16 size,u16 d,u8 hour,u8 min,u8 sec)
{
	static u8 oldhour=0;	//���һ�ν���ú�����ʱ������Ϣ
	static u8 oldmin=0;
	static u8 oldsec=0;
	float temp;
	u16 r=size/2;//�õ��뾶 
	u16 sx=x-r;
	u16 sy=y-r;
	u16 px0,px1;
	u16 py0,py1;  
	u8 r1; 
	if(hour>11)hour-=12;
///////////////////////////////////////////////
	//���Сʱ
	r1=d/2+4;
	//�����һ�ε�����
	temp=(float)oldmin/60;
	temp+=oldhour;
	px0=sx+r+(r-3*d-7)*sin((app_pi/6)*temp); 
	py0=sy+r-(r-3*d-7)*cos((app_pi/6)*temp); 
	px1=sx+r+r1*sin((app_pi/6)*temp); 
	py1=sy+r-r1*cos((app_pi/6)*temp); 
	gui_draw_bline1(px0,py0,px1,py1,2,BLACK);
	//�������
	r1=d/2+3;
	temp=(float)oldsec/60;
	temp+=oldmin;
	//�����һ�ε�����
	px0=sx+r+(r-2*d-7)*sin((app_pi/30)*temp); 
	py0=sy+r-(r-2*d-7)*cos((app_pi/30)*temp); 
	px1=sx+r+r1*sin((app_pi/30)*temp); 
	py1=sy+r-r1*cos((app_pi/30)*temp); 
	gui_draw_bline1(px0,py0,px1,py1,1,BLACK); 
	//������� 
	r1=d/2+3;
	//�����һ�ε�����
	px0=sx+r+(r-d-7)*sin((app_pi/30)*oldsec); 
	py0=sy+r-(r-d-7)*cos((app_pi/30)*oldsec); 
	px1=sx+r+r1*sin((app_pi/30)*oldsec); 
	py1=sy+r-r1*cos((app_pi/30)*oldsec); 
	gui_draw_bline1(px0,py0,px1,py1,0,BLACK); 
///////////////////////////////////////////////
	//��ʾСʱ 
	r1=d/2+4; 
	//��ʾ�µ�ʱ��
	temp=(float)min/60;
	temp+=hour;
	px0=sx+r+(r-3*d-7)*sin((app_pi/6)*temp); 
	py0=sy+r-(r-3*d-7)*cos((app_pi/6)*temp); 
	px1=sx+r+r1*sin((app_pi/6)*temp); 
	py1=sy+r-r1*cos((app_pi/6)*temp); 
	gui_draw_bline1(px0,py0,px1,py1,2,YELLOW); 
	//��ʾ���� 
	r1=d/2+3; 
	temp=(float)sec/60;
	temp+=min;
	//��ʾ�µķ���
	px0=sx+r+(r-2*d-7)*sin((app_pi/30)*temp); 
	py0=sy+r-(r-2*d-7)*cos((app_pi/30)*temp); 
	px1=sx+r+r1*sin((app_pi/30)*temp); 
	py1=sy+r-r1*cos((app_pi/30)*temp); 
	gui_draw_bline1(px0,py0,px1,py1,1,GREEN); 	
	//��ʾ����  
	r1=d/2+3;
	//��ʾ�µ�����
	px0=sx+r+(r-d-7)*sin((app_pi/30)*sec); 
	py0=sy+r-(r-d-7)*cos((app_pi/30)*sec); 
	px1=sx+r+r1*sin((app_pi/30)*sec); 
	py1=sy+r-r1*cos((app_pi/30)*sec); 
	gui_draw_bline1(px0,py0,px1,py1,0,RED); 
	oldhour=hour;	//����ʱ
	oldmin=min;		//�����
	oldsec=sec;		//������
}	    
//ʱ����ʾģʽ    
u8 calendar_play(void)
{
	u8 second=0;
	short temperate=0;	//�¶�ֵ		   
	u8 t=0;
	u8 tempdate=0;
	u8 rval=0;			//����ֵ	
	u8 res;
	u16 xoff=0;
	u16 yoff=0;	//����yƫ����
	u16 r=0;	//���̰뾶
	u8 d=0;		//ָ�볤��
	  
	u8 TEMP_SEN_TYPE=0;	//Ĭ��ʹ��DS18B20
	FIL* f_calendar=0;	 
	
  	f_calendar=(FIL *)gui_memin_malloc(sizeof(FIL));//����FIL�ֽڵ��ڴ����� 
	if(f_calendar==NULL)rval=1;		//����ʧ��
	else
	{
		res=f_open(f_calendar,(const TCHAR*)APP_ASCII_S6030,FA_READ);//���ļ� 
		if(res==FR_OK)
		{
			asc2_s6030=(u8*)gui_memex_malloc(f_calendar->fsize);	//Ϊ�����忪�ٻ����ַ
			if(asc2_s6030==0)rval=1;
			else 
			{
				res=f_read(f_calendar,asc2_s6030,f_calendar->fsize,(UINT*)&br);	//һ�ζ�ȡ�����ļ�
 			}
			f_close(f_calendar);
		} 
		if(res)rval=res;
	} 	    
	if(rval==0)//�޴���
	{	  
 		LCD_Clear(BLACK);//�����    	  
		second=calendar.sec;//�õ��˿̵�����
		POINT_COLOR=GBLUE;
		Show_Str(48,60,lcddev.width,lcddev.height,(u8*)calendar_loading_str[gui_phy.language][0],16,0x01); //��ʾ������Ϣ	    
		if(DS18B20_Init())
		{
			Show_Str(48,76,lcddev.width,lcddev.height,(u8*)calendar_loading_str[gui_phy.language][1],16,0x01);  
			delay_ms(500);
			Show_Str(48,92,lcddev.width,lcddev.height,(u8*)calendar_loading_str[gui_phy.language][2],16,0x01);  
			TEMP_SEN_TYPE=1; 
		}   	    
		delay_ms(1100);//�ȴ�1.1s 
		BACK_COLOR= BLACK;
		LCD_Clear(BLACK);//����� 
		if(lcddev.width==240)
		{
			r=80;
			d=7; 
		}else if(lcddev.width==320)
		{
			r=120;
			d=9; 
		}else if(lcddev.width==480)
		{
			r=160;
			d=12; 
		}
		yoff=(lcddev.height-r*2-140)/2;
		TIME_TOPY=yoff+r*2+10;
		OTHER_TOPY=TIME_TOPY+60+10;
		xoff=(lcddev.width-240)/2;   
		calendar_circle_clock_drawpanel(lcddev.width/2,yoff+r,r*2,d);//��ʾָ��ʱ�ӱ��� 
		calendar_date_refresh();  //��������
		tempdate=calendar.w_date;//�����ݴ��� 		
		gui_phy.back_color=BLACK;
		gui_show_ptchar(xoff+70-4,TIME_TOPY,lcddev.width,lcddev.height,0,GBLUE,60,':',0);	//":"
		gui_show_ptchar(xoff+150-4,TIME_TOPY,lcddev.width,lcddev.height,0,GBLUE,60,':',0);	//":" 
	}
  	while(rval==0)
	{	
		RTC_Get();					//����ʱ�� 
		if(system_task_return)break;//��Ҫ����	  
 		if(second!=calendar.sec)	//���Ӹı���
		{ 	
  			second=calendar.sec;  
			calendar_circle_clock_showtime(lcddev.width/2,yoff+r,r*2,d,calendar.hour,calendar.min,calendar.sec);//ָ��ʱ����ʾʱ��		
			gui_phy.back_color=BLACK;
			gui_show_num(xoff+10,TIME_TOPY,2,GBLUE,60,calendar.hour,0X80);	//��ʾʱ
			gui_show_num(xoff+90,TIME_TOPY,2,GBLUE,60,calendar.min,0X80);	//��ʾ��
			gui_show_num(xoff+170,TIME_TOPY,2,GBLUE,60,calendar.sec,0X80);	//��ʾ�� 					   
			if(t%2==0)//�ȴ�2����
			{		 
  				if(TEMP_SEN_TYPE)temperate=Get_Temp();//�õ��ڲ��¶ȴ��������¶�ֵ,0.1��
				else temperate=DS18B20_Get_Temp();//�õ�18b20�¶�
				if(temperate<0)//�¶�Ϊ������ʱ�򣬺�ɫ��ʾ
				{
					POINT_COLOR=RED;
					temperate=-temperate;	//��Ϊ���¶�
				}else POINT_COLOR=BRRED;	//����Ϊ�غ�ɫ������ʾ		
				gui_show_num(xoff+90,OTHER_TOPY,2,GBLUE,60,temperate/10,0X80);	//XX					   
				gui_show_ptchar(xoff+150,OTHER_TOPY,lcddev.width,lcddev.height,0,GBLUE,60,'.',0);	//"." 
				gui_show_ptchar(xoff+180-15,OTHER_TOPY,lcddev.width,lcddev.height,0,GBLUE,60,temperate%10+'0',0);//��ʾС��
				gui_show_ptchar(xoff+210-10,OTHER_TOPY,lcddev.width,lcddev.height,0,GBLUE,60,95+' ',0);//��ʾ��
				if(t>0)t=0;			 
			}  	
			if(calendar.w_date!=tempdate)
			{
				calendar_date_refresh();	//�����仯��,��������.  
				tempdate=calendar.w_date;	//�޸�tempdate����ֹ�ظ�����
			}
			t++;   
 		} 
		delay_ms(20);
 	};
 	while(tp_dev.sta&TP_PRES_DOWN)tp_dev.scan(0);//�ȴ�TP�ɿ�.
 	gui_memex_free(asc2_s6030);	//ɾ��������ڴ�
	asc2_s6030=0;				//����
	gui_memin_free(f_calendar);	//ɾ��������ڴ�
	POINT_COLOR=BLUE;
	BACK_COLOR=WHITE ;	
	return rval;
}




















