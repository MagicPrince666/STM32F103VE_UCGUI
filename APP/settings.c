#include "settings.h"
#include "rtc.h" 	   
#include "calendar.h" 	
#include "mainui.h" 	  
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
 
_system_setings systemset;	  
#define SYS_MENU_SIZE 			12		//��Ŀ����	
	 
//ϵͳ������Ŀ¼��
u8*const sysset_mmenu_tbl[GUI_LANGUAGE_NUM][SYS_MENU_SIZE]=//ϵͳһ��Ŀ¼�ĸ���
{
	{
	"1.ʱ������",
	"2.��������",
	"3.����ʱ������",
	"4.���ӿ�������",
	"5.������ʾ����",
	"6.��������",
	"7.�����������",
 	"8.��ĻУ׼",
	"9.ϵͳ�ļ�����",
	"10.ϵͳ��Ϣ",
	"11.ϵͳ״̬", 
	"12.����"
	},
	{			   								
	"1.�r�g�O��",
	"2.�����O��",
	"3.�[犕r�g�O��",
	"4.�[��_�P�O��",
	"5.�[���ʾ�O��",
	"6.�Z���O��",
	"7.���a����O��",  
 	"8.��ĻУ��",
	"9.ϵ�y�ļ�����", 
	"10.ϵ�y��Ϣ",
	"11.ϵ�y��B", 
	"12.�P�"
	}, 	 
	{
	"1.TIME SET",
	"2.DATE SET",
	"3.ALARM TIME SET",
	"4.ALARM ON/OFF SET",
	"5.ALARM RING SET",
	"6.LANGUAGE SET",
	"7.DIGITAL PHOTO FRAME SET",  
 	"8.TOUCH SCREEN ADJUST", 
	"9.SYSTEM FILE UPDATE", 
	"10.SYSTEM INFORMATION",
	"11.SYSTEM STATUS", 
	"12.ABOUT"
	},
};	    
//���������б�
u8*const sysset_language_tbl[GUI_LANGUAGE_NUM]=
{
	"��������","���w����","English",
};  	    
//���������б�
u8*const sysset_ring_tbl[GUI_LANGUAGE_NUM][4]=
{
{"DS1��˸1","DS1��˸2","DS1��˸3","DS1��˸4",},
{"DS1�W�q1","DS1�W�q2","DS1�W�q3","DS1�W�q4",},
{"DS1 FLASH 1","DS1 FLASH 2","DS1 FLASH 3","DS1 FLASH 4",},	 
};
//����/ͼƬ����ģʽ�����б�
u8*const sysset_avplaymode_tbl[GUI_LANGUAGE_NUM][3]=
{
{"ȫ��ѭ��","�������","����ѭ��"},
{"ȫ��ѭ�h","�S�C����","����ѭ�h"},
{"LOOP PLAY","SHUFFLE PLAY","SINGLE PLAY"},
};  
//ϵͳ���������ʾ��Ϣ 
u8*const sysset_system_remaindmsg_tbl[3][GUI_LANGUAGE_NUM]=
{
{"��ȷ�ϸ���ϵͳ�ļ�?","���_�J����ϵ�y�ļ�?","Are you sure to update?"},
{"��ȷ�ϻָ�Ĭ������?","���_�J�֏�Ĭ�J�O��?","Are you sure to restore?"},
{"�ָ�Ĭ��������...","�֏�Ĭ�J�O����...","Default set restoring...",},
};
//ϵͳ���¸�����ʾ��Ϣ
u8*const sysset_system_update_cpymsg_tbl[2][GUI_LANGUAGE_NUM]=
{
{"���ڸ���:","�����}�u:"," Copying:",},
{"��ǰ�ļ���:","��ǰ�ļ��A:","Cur Folder:",},
}; 
//ϵͳ������ʾ
u8*const sysset_system_update_msg_tbl[GUI_LANGUAGE_NUM]=
{"ϵͳ���ڸ���...","ϵ�y���ڸ���...","SYSTEM Updating..."};
//ϵͳ���½����ʾ
u8*const sysset_system_update_err_tbl[3][GUI_LANGUAGE_NUM]=
{
{"ϵͳ�ļ����³ɹ�!","ϵ�y�ļ����³ɹ�!","SYSTEM file lost!",},
{"ϵͳ�ļ���ʧ!","ϵ�y�ļ��Gʧ!","SYSTEM file lost!",},
{"�û���ֹ����!","�Ñ��Kֹ����!","User stop update!",},
};
//ϵͳ��Ϣ��ע��
u8*const sysset_system_info_caption_tbl[9][GUI_LANGUAGE_NUM]=
{
{"������:","̎����:","CPU:"},
{"�ڴ�:","�ȴ�:","RAM:"},
{"SD��:","SD��:","SD Card:"},
{"FLASH��:","FLASH�P:","FLASH Disk:"}, 
{"����ϵͳ:","����ϵ�y:","OS:"},
{"ͼ�ν���:","�D�ν���:","GUI:"},
{"Ӳ��ƽ̨:","Ӳ��ƽ�_:","Hardware:"},
{"��Ȩ��Ϣ:","�����Ϣ:","Copyright:"},
{"����֧��:","���g֧��:","Tech Support:"},
};
//ϵͳ��ʾ��Ϣ��
u8*const sysset_system_info_msg_tbl[9]=
{
"STM32F103RCT6 @ 72Mhz",
"48KB",
"MB",
"KB", 
"uCOS II ",
"ALIENTEK GUI ",
"ALIENTEK MiniSTM32 V3",
"�����������",
"www.openedv.com",
}; 
//ϵͳ״��ʾ��Ϣ
u8*const sysset_sysstatus_tbl[3][GUI_LANGUAGE_NUM]=
{
{"CPUʹ����:","CPUʹ����:","CPU USAGE:",},
{"�ڲ��ڴ�ʹ����:","�ڲ��ڴ�ʹ����:","IN MEMORY USAGE:",}, 
{"�¶�:","�ض�:","TEMP:"},
};

//ϵͳ������ʾ��Ϣ
u8*const sysset_system_about_caption_tbl[2][GUI_LANGUAGE_NUM]=
{
{"ALEINTEK ����","ALIENTEK ����","ALENTEK Mini",},
{"��Ʒ���к�","�aƷ����̖","Serial Number",},
}; 
////////////////////////////////////////////////////////////////////////////////////////////

#define SYSSET_BTN1_WIDTH			50			//һ�ఴ�����(�Ӽ���ť)
#define SYSSET_BTN1_HEIGHT			40			//һ�ఴ���߶�(�Ӽ���ť)
#define SYSSET_BTN2_WIDTH			60			//���ఴ�����(ȷ��/ȡ����ť)
#define SYSSET_BTN2_HEIGHT			30			//���ఴ���߶�(ȷ��/ȡ����ť)
#define SYSSET_BTN3_WIDTH			100			//���ఴ�����(������ť�Ŀ��)
#define SYSSET_EDIT_WIDTH			50			//���ֱ༭���Ŀ��
#define SYSSET_EDIT_HEIGHT			25			//���ֱ༭���ĸ߶�
 
#define SYSSET_INWIN_FONT_COLOR		0X736C		//0XAD53		//������Ƕ����ɫ
#define SYSSET_EDIT_BACK_COLOR		0XE79E		//��ʾ����������ɫ
 
/////////////////////////////////////////////////////////////////////////////////////////////

//ʱ��/��������
//x,y:��������(���ڳߴ��Ѿ��̶��˵�)	 
//hour,min:ʱ��
//caption:��������				  
//����ֵ:0,ok;����,ȡ�����ߴ���.
u8 sysset_time_set(u16 x,u16 y,u8 *hour,u8 *min,u8*caption) 
{
	u8 rval=0,res;
	u8 i;
	_window_obj* twin=0;	//����
 	_btn_obj * tbtn[6];		//�ܹ�������ť:0,ʱ�ӼӰ�ť;1,ʱ�Ӽ���ť;2,���ӼӰ�ť;3,���Ӽ���ť;4,ȷ�ϰ�ť;5,ȡ����ť.		  
 	twin=window_creat(x,y,150,199,0,1|1<<5,16);//��������
 	tbtn[0]=btn_creat(x+20,y+42,SYSSET_BTN1_WIDTH,SYSSET_BTN1_HEIGHT,0,0x02);			//������ť
	tbtn[1]=btn_creat(x+20,y+42+67,SYSSET_BTN1_WIDTH,SYSSET_BTN1_HEIGHT,0,0x02);		//������ť
	tbtn[2]=btn_creat(x+20+60,y+42,SYSSET_BTN1_WIDTH,SYSSET_BTN1_HEIGHT,0,0x02);		//������ť
	tbtn[3]=btn_creat(x+20+60,y+42+67,SYSSET_BTN1_WIDTH,SYSSET_BTN1_HEIGHT,0,0x02);		//������ť
	tbtn[4]=btn_creat(x+10,y+42+117,SYSSET_BTN2_WIDTH,SYSSET_BTN2_HEIGHT,0,0x02);		//������ť
	tbtn[5]=btn_creat(x+10+70,y+42+117,SYSSET_BTN2_WIDTH,SYSSET_BTN2_HEIGHT,0,0x02);	//������ť
	for(i=0;i<6;i++)
	{
		if(tbtn[i]==NULL)
		{
			rval=1;
			break;
		}
		if(i<4)//�Ӽ�����
		{
			tbtn[i]->bcfucolor=BLACK;//�ɿ�ʱΪ��ɫ
			tbtn[i]->bcfdcolor=WHITE;//����ʱΪ��ɫ			
			tbtn[i]->bkctbl[0]=0X453A;//�߿���ɫ
			tbtn[i]->bkctbl[1]=0X5DDC;//��һ�е���ɫ				
			tbtn[i]->bkctbl[2]=0X5DDC;//�ϰ벿����ɫ
			tbtn[i]->bkctbl[3]=0X453A;//�°벿����ɫ
		}else//ȷ�Ϻ�ȡ������
		{
			tbtn[i]->bkctbl[0]=0X8452;//�߿���ɫ
			tbtn[i]->bkctbl[1]=0XAD97;//��һ�е���ɫ				
			tbtn[i]->bkctbl[2]=0XAD97;//�ϰ벿����ɫ
			tbtn[i]->bkctbl[3]=0X8452;//�°벿����ɫ
		}
		if(i==0||i==2)tbtn[i]->caption="��";
		if(i==1||i==3)tbtn[i]->caption="��";
		if(i==4)tbtn[i]->caption=(u8*)GUI_OK_CAPTION_TBL[gui_phy.language];
		if(i==5)tbtn[i]->caption=(u8*)GUI_CANCEL_CAPTION_TBL[gui_phy.language];				
	}
    if(twin==NULL)rval=1;
	else
	{
		twin->caption=caption;
		twin->windowbkc=APP_WIN_BACK_COLOR;	     
	}
	if(rval==0)//�޴���
	{
		window_draw(twin);					//��������
		for(i=0;i<6;i++)btn_draw(tbtn[i]);	//����ť
		gui_fill_rectangle(x+20+1,y+42+41,SYSSET_EDIT_WIDTH-2,SYSSET_EDIT_HEIGHT,SYSSET_EDIT_BACK_COLOR);  		//���ʱ�ӱ���
		gui_fill_rectangle(x+20+60+1,y+42+41,SYSSET_EDIT_WIDTH-2,SYSSET_EDIT_HEIGHT,SYSSET_EDIT_BACK_COLOR);	//�����ӱ���
 		app_show_nummid(x+20,y+42+41,SYSSET_EDIT_WIDTH,SYSSET_EDIT_HEIGHT,*hour,2,16,BLACK,SYSSET_EDIT_BACK_COLOR);
		app_show_nummid(x+20+60,y+42+41,SYSSET_EDIT_WIDTH,SYSSET_EDIT_HEIGHT,*min,2,16,BLACK,SYSSET_EDIT_BACK_COLOR);  
		while(rval==0)
		{
			tp_dev.scan(0);    
			in_obj.get_key(&tp_dev,IN_TYPE_TOUCH);	//�õ�������ֵ   
			delay_ms(1000/OS_TICKS_PER_SEC);		//��ʱһ��ʱ�ӽ���
 			if(system_task_return)
			{
				rval=1;			//ȡ������
				break;			//TPAD����	
			}
			for(i=0;i<6;i++)
			{
				res=btn_check(tbtn[i],&in_obj);//ȷ�ϰ�ť���
				if(res)
				{
					if((tbtn[i]->sta&0X80)==0)//����Ч����
					{
						switch(i)
						{
							case 0://ʱ�����Ӱ�ť������
								(*hour)++;
								if(*hour>23)*hour=0;
								break;
							case 1://ʱ�Ӽ��ٰ�ť������	  
								if(*hour)(*hour)--;
								else *hour=23;
								break;
							case 2://�������Ӱ�ť������
								(*min)++;
								if(*min>59)(*min)=0;
								break;
							case 3://���Ӽ��ٰ�ť������	  
								if(*min)(*min)--;
								else *min=59;
								break;
							case 4://ȷ�ϰ�ť����  
								rval=0XFF;
								break;	  
							case 5://ȡ����ť����	  
								rval=1;
								break;
 						}
					}
					app_show_nummid(x+20,y+42+41,SYSSET_EDIT_WIDTH,SYSSET_EDIT_HEIGHT,*hour,2,16,BLACK,SYSSET_EDIT_BACK_COLOR);
					app_show_nummid(x+20+60,y+42+41,SYSSET_EDIT_WIDTH,SYSSET_EDIT_HEIGHT,*min,2,16,BLACK,SYSSET_EDIT_BACK_COLOR);  

				}
			}
			
		}
 	}
	window_delete(twin);				//ɾ������
	for(i=0;i<6;i++)btn_delete(tbtn[i]);//ɾ����ť					   
	system_task_return=0;				//����˳���־
	if(rval==0XFF)return 0;
	return rval;
}
//�ж��Ƿ������꺯��
//�·�   1  2  3  4  5  6  7  8  9  10 11 12
//����   31 29 31 30 31 30 31 31 30 31 30 31
//������ 31 28 31 30 31 30 31 31 30 31 30 31
//year:���
//����ֵ:������ǲ�������.1,��.0,����
u8 sysset_is_leap_year(u16 year)
{			  
	if(year%4==0) //�����ܱ�4����
	{ 
		if(year%100==0) 
		{ 
			if(year%400==0)return 1;//�����00��β,��Ҫ�ܱ�400���� 	   
			else return 0;   
		}else return 1;   
	}else return 0;	
}	
//��������
//x,y:��������(���ڳߴ��Ѿ��̶��˵�)
//year,month,date:������
//caption:��������
//����ֵ:0,ok;����,ȡ�����ߴ���.
u8 sysset_date_set(u16 x,u16 y,u16 *year,u8 *month,u8 *date,u8*caption) 
{
	u8 rval=0,res;
	u8 i;
	u8 maxdate=31;			//2�·���������
	_window_obj* twin=0;	//����
 	_btn_obj * tbtn[8];		//�ܹ��˸���ť:0,��ݼӰ�ť;1,��ݼ���ť;2,�·ݼӰ�ť;3�·ݼ���ť;4,���ڼӰ�ť;5,���ڼ���ť;6,ȷ�ϰ�ť;7,ȡ����ť		  
 	twin=window_creat(x,y,190,199,0,1|1<<5,16);//��������
 	tbtn[0]=btn_creat(x+10,y+42,SYSSET_BTN1_WIDTH,SYSSET_BTN1_HEIGHT,0,0x02);			//������ť
	tbtn[1]=btn_creat(x+10,y+42+67,SYSSET_BTN1_WIDTH,SYSSET_BTN1_HEIGHT,0,0x02);		//������ť
	tbtn[2]=btn_creat(x+10+60,y+42,SYSSET_BTN1_WIDTH,SYSSET_BTN1_HEIGHT,0,0x02);		//������ť
	tbtn[3]=btn_creat(x+10+60,y+42+67,SYSSET_BTN1_WIDTH,SYSSET_BTN1_HEIGHT,0,0x02);		//������ť
	tbtn[4]=btn_creat(x+10+120,y+42,SYSSET_BTN1_WIDTH,SYSSET_BTN1_HEIGHT,0,0x02);		//������ť
	tbtn[5]=btn_creat(x+10+120,y+42+67,SYSSET_BTN1_WIDTH,SYSSET_BTN1_HEIGHT,0,0x02);	//������ť

	tbtn[6]=btn_creat(x+20,y+42+117,SYSSET_BTN2_WIDTH,SYSSET_BTN2_HEIGHT,0,0x02);		//������ť
	tbtn[7]=btn_creat(x+20+90,y+42+117,SYSSET_BTN2_WIDTH,SYSSET_BTN2_HEIGHT,0,0x02);	//������ť
 	for(i=0;i<8;i++)
	{
		if(tbtn[i]==NULL)
		{
			rval=1;
			break;
		}
		if(i<6)//�Ӽ�����
		{
			tbtn[i]->bcfucolor=BLACK;//�ɿ�ʱΪ��ɫ
			tbtn[i]->bcfdcolor=WHITE;//����ʱΪ��ɫ			
			tbtn[i]->bkctbl[0]=0X453A;//�߿���ɫ
			tbtn[i]->bkctbl[1]=0X5DDC;//��һ�е���ɫ				
			tbtn[i]->bkctbl[2]=0X5DDC;//�ϰ벿����ɫ
			tbtn[i]->bkctbl[3]=0X453A;//�°벿����ɫ
		}else//ȷ�Ϻ�ȡ������
		{
			tbtn[i]->bkctbl[0]=0X8452;//�߿���ɫ
			tbtn[i]->bkctbl[1]=0XAD97;//��һ�е���ɫ				
			tbtn[i]->bkctbl[2]=0XAD97;//�ϰ벿����ɫ
			tbtn[i]->bkctbl[3]=0X8452;//�°벿����ɫ
		}
		if(i==0||i==2||i==4)tbtn[i]->caption="��";
		if(i==1||i==3||i==5)tbtn[i]->caption="��";
		if(i==6)tbtn[i]->caption=(u8*)GUI_OK_CAPTION_TBL[gui_phy.language];;
		if(i==7)tbtn[i]->caption=(u8*)GUI_CANCEL_CAPTION_TBL[gui_phy.language];;				
	}
    if(twin==NULL)rval=1;
	else
	{
		twin->caption=caption;
		twin->windowbkc=APP_WIN_BACK_COLOR;	     
	}
	if(rval==0)//�޴���
	{
		window_draw(twin);					//��������
		for(i=0;i<8;i++)btn_draw(tbtn[i]);	//����ť
		gui_fill_rectangle(x+10+1,y+42+41,SYSSET_EDIT_WIDTH-2,SYSSET_EDIT_HEIGHT,SYSSET_EDIT_BACK_COLOR);  		//�����ݱ���
		gui_fill_rectangle(x+10+60+1,y+42+41,SYSSET_EDIT_WIDTH-2,SYSSET_EDIT_HEIGHT,SYSSET_EDIT_BACK_COLOR);	//����·ݱ���
 		gui_fill_rectangle(x+10+120+1,y+42+41,SYSSET_EDIT_WIDTH-2,SYSSET_EDIT_HEIGHT,SYSSET_EDIT_BACK_COLOR);	//������ڱ���

 		app_show_nummid(x+10,y+42+41,SYSSET_EDIT_WIDTH,SYSSET_EDIT_HEIGHT,*year,4,16,BLACK,SYSSET_EDIT_BACK_COLOR);
		app_show_nummid(x+10+60,y+42+41,SYSSET_EDIT_WIDTH,SYSSET_EDIT_HEIGHT,*month,2,16,BLACK,SYSSET_EDIT_BACK_COLOR);  
		app_show_nummid(x+10+120,y+42+41,SYSSET_EDIT_WIDTH,SYSSET_EDIT_HEIGHT,*date,2,16,BLACK,SYSSET_EDIT_BACK_COLOR);  
		while(rval==0)
		{
			tp_dev.scan(0);    
			in_obj.get_key(&tp_dev,IN_TYPE_TOUCH);	//�õ�������ֵ   
			delay_ms(1000/OS_TICKS_PER_SEC);		//��ʱһ��ʱ�ӽ���
 			if(system_task_return)
			{
				rval=1;			//ȡ������
				break;			//TPAD����	
			}
			for(i=0;i<8;i++)
			{
				res=btn_check(tbtn[i],&in_obj);//ȷ�ϰ�ť���
				if(res)
				{
					if((tbtn[i]->sta&0X80)==0)//����Ч����
					{
						if(*month==2)
						{
							if(sysset_is_leap_year(*year))maxdate=29;//�������2�·�
	 						else maxdate=28;	  
						}else maxdate=31;  
						switch(i)
						{
							case 0://������Ӱ�ť������
								(*year)++;
								if(*year>2100)*year=2000;
								break;
							case 1://��ݼ��ٰ�ť������	  
								if(*year>2000)(*year)--;
								else *year=2100;
								break;
							case 2://�·����Ӱ�ť������
								(*month)++;
								if(*month>12)(*month)=1;
								break;
							case 3://�·ݼ��ٰ�ť������	  
								if(*month>1)(*month)--;
								else *month=12;
								break;
							case 4://�������Ӱ�ť������
								(*date)++;
								if(*date>maxdate)(*date)=1;
								break;
							case 5://���ڼ��ٰ�ť������	  
								if(*date>1)(*date)--;
								else *date=maxdate;
								break;
							case 6://ȷ�ϰ�ť����  
								rval=0XFF;
								break;	  
							case 7://ȡ����ť����  
								rval=1;
								break;
 						}
					}   
			 		app_show_nummid(x+10,y+42+41,SYSSET_EDIT_WIDTH,SYSSET_EDIT_HEIGHT,*year,4,16,BLACK,SYSSET_EDIT_BACK_COLOR);
					app_show_nummid(x+10+60,y+42+41,SYSSET_EDIT_WIDTH,SYSSET_EDIT_HEIGHT,*month,2,16,BLACK,SYSSET_EDIT_BACK_COLOR);  
					app_show_nummid(x+10+120,y+42+41,SYSSET_EDIT_WIDTH,SYSSET_EDIT_HEIGHT,*date,2,16,BLACK,SYSSET_EDIT_BACK_COLOR);  
				}
			}
			
		}
 	}
	window_delete(twin);				//ɾ������
	for(i=0;i<8;i++)btn_delete(tbtn[i]);//ɾ����ť					   
	system_task_return=0;				//����˳���־
	if(rval==0XFF)return 0;
	return rval;
}	  
//�����ⲿ��������
u16 cpymsg_x;
u16 cpymsg_y;
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
u8 sysset_system_update_cpymsg(u8*pname,u8 pct,u8 mode)
{		
	u16 filled; 
	if(system_task_return)return 1;//TPAD����	
	if(mode&0X01)//��ʾ�ļ���
	{  
 		gui_phy.back_color=APP_WIN_BACK_COLOR;
		gui_fill_rectangle(cpymsg_x+10+9*6,cpymsg_y+32+15,126,12,APP_WIN_BACK_COLOR);		//����ɫ  0XE73C 
 		gui_show_string(pname,cpymsg_x+10+9*6,cpymsg_y+32+15,126,12,12,BLACK);				//��ʾ�µ��ļ���	
   		gui_show_string((u8*)sysset_system_update_cpymsg_tbl[1][gui_phy.language],cpymsg_x+10,cpymsg_y+32+15+21,66,12,12,BLACK);	
 	}
	if(mode&0X04)//��ʾ�ļ�����
	{
 		gui_phy.back_color=APP_WIN_BACK_COLOR;
		gui_fill_rectangle(cpymsg_x+10+11*6,cpymsg_y+32+15+21,114,12,APP_WIN_BACK_COLOR);	//����ɫ  0XE73C 
 		gui_show_string(pname,cpymsg_x+10+11*6,cpymsg_y+32+15+21,114,12,12,BLACK);			//��ʾ�µ��ļ�����	
	}
	if(mode&0X02)//���°ٷֱ�
	{
		if(pct>100)pct=100;
	    filled=pct;
		filled=(filled*9)/5;
		gui_fill_rectangle(cpymsg_x+10,cpymsg_y+32+15+21*2,filled,16,0X071F);				//���ռ���˵İٷֱȲ���     
		gui_fill_rectangle(cpymsg_x+10+filled,cpymsg_y+32+15+21*2,180-filled,16,WHITE);		//���δռ�õİٷֱȲ���  
		gui_show_num(cpymsg_x+10+72,cpymsg_y+32+15+21*2+2,3,BLACK,12,pct,1);				//��ʾ���½���
		gui_show_string("%",cpymsg_x+10+72+18,cpymsg_y+32+15+21*2+2,6,12,12,BLACK);			//��ʾ�ٷֺ�
  	}
	delay_ms(1000/OS_TICKS_PER_SEC);//��ʱһ��ʱ�ӽ���
	return 0;	
}
//����ϵͳ�ļ���
//caption:����
void sysset_system_update(u8*caption,u16 sx,u16 sy)
{
  	_window_obj* twin=0;			//����
	u8 res;
	u8 *p;  
	res=app_system_file_check("0");
  	if(res)
	{
		p=(u8*)sysset_system_update_err_tbl[1][gui_phy.language];	//ϵͳ�ļ���ʧ
		window_msg_box(sx,sy+20,200,80,p,APP_REMIND_CAPTION_TBL[gui_phy.language],12,0,0,0);
		delay_ms(1500);
		system_task_return=0;//ȡ��TPAD
		return;
	}
	twin=window_creat(sx,sy,200,120,0,1,16);//��������
  	if(twin!=NULL)
	{
		//���ڵ����ֺͱ���ɫ
		twin->caption=caption;
		twin->windowbkc=APP_WIN_BACK_COLOR;	
 		window_draw(twin);						//��������
		gui_phy.back_color=APP_WIN_BACK_COLOR;
  		gui_show_string((u8*)sysset_system_update_cpymsg_tbl[0][gui_phy.language],sx+10,sy+32+15,54,12,12,BLACK);	//���ڸ���:
  		gui_show_string((u8*)sysset_system_update_cpymsg_tbl[1][gui_phy.language],sx+10,sy+32+15+21,66,12,12,BLACK);//��ǰ�ļ���:
 		gui_draw_rectangle(sx+10-1,sy+32+15+21*2-1,182,18,0X4A49);		//���߿�
		gui_fill_rectangle(sx+10,sy+32+15+21*2,180,16,WHITE);			//����ɫ  0XE73C 
		gui_show_num(sx+10+72,sy+32+15+21*2+2,3,BLACK,12,0,1);			//��ʾ���½���
		gui_show_string("%",sx+10+72+18,sy+32+15+21*2+2,6,12,12,BLACK);	//��ʾ�ٷֺ�
		cpymsg_x=sx;
		cpymsg_y=sy;
		res=app_system_update(sysset_system_update_cpymsg);				//��SD������
	   	p=(u8*)sysset_system_update_err_tbl[0][gui_phy.language];		//���³ɹ���ʾ
 		if(res==0XFF)p=(u8*)sysset_system_update_err_tbl[2][gui_phy.language];	//ǿ���˳�
		else if(res)p=(u8*)sysset_system_update_err_tbl[1][gui_phy.language];	//ϵͳ�ļ���ʧ
  		window_msg_box(sx,sy+20,200,80,p,APP_REMIND_CAPTION_TBL[gui_phy.language],12,0,0,0);
		delay_ms(1500); 
	}
	window_delete(twin);
	system_task_return=0;//ȡ��TPAD
}  
//��ʾϵͳ��Ϣ
//x,y:��ʼ����
//���:200,�߶�:272.
//caption:����
void sysset_system_info(u16 x,u16 y,u8*caption)
{
  	_window_obj* twin=0;			//����
 	_btn_obj * rbtn=0;				//���ذ�ť		  
	u8 rval=0;
	u8 *msgbuf;
	u8 numstrbuf[6];
 	u8 i;
	u32 dtsize,dfsize; 
	msgbuf=mymalloc(31);			//����31���ֽ�
  	twin=window_creat(x,y,200,272,0,1|1<<5,16);//��������
 	rbtn=btn_creat(x+50,y+32+10+20*9+10,SYSSET_BTN3_WIDTH,SYSSET_BTN2_HEIGHT,0,0x02);//������ť
  	if(twin!=NULL&&rbtn!=NULL)
	{
		//���ڵ����ֺͱ���ɫ
		twin->caption=caption;
		twin->windowbkc=APP_WIN_BACK_COLOR;	
 		//���ذ�ť����ɫ    
		rbtn->bkctbl[0]=0X8452;//�߿���ɫ
		rbtn->bkctbl[1]=0XAD97;//��һ�е���ɫ				
		rbtn->bkctbl[2]=0XAD97;//�ϰ벿����ɫ
		rbtn->bkctbl[3]=0X8452;//�°벿����ɫ
 		rbtn->caption=(u8*)GUI_BACK_CAPTION_TBL[gui_phy.language];//����Ϊ����

 		window_draw(twin);	//��������	
		btn_draw(rbtn);	  	//������
 		gui_draw_rectangle(x+10-4,y+32+14-4,188,20*9,0X4A49);		//���߿�
 		gui_phy.back_color=APP_WIN_BACK_COLOR;
 		rval=strlen((const char*)sysset_system_info_caption_tbl[0][gui_phy.language]);
		for(i=0;i<9;i++)
		{
	 		strcpy((char*)msgbuf,(const char*)sysset_system_info_caption_tbl[i][gui_phy.language]);
	 		if(i==2||i==3)//SD��/FLASH��  ����
			{
				dtsize=0;
				dfsize=0;
				if(i==2)
				{
					if(i==2)exf_getfree("0:",&dtsize,&dfsize);	//�õ�SD��ʣ��������������
					else exf_getfree("2:",&dtsize,&dfsize);		//�õ�SD��ʣ�������������� 
					dtsize>>=10;
					dfsize>>=10;
				}else exf_getfree("1:",&dtsize,&dfsize);//�õ�FLASH��ʣ��������������  
  				gui_num2str(numstrbuf,dfsize);
				strcat((char*)msgbuf,(const char*)numstrbuf);//���ӿ�������
				if(i==2)strcat((char*)msgbuf,(const char*)"MB/");
				else strcat((char*)msgbuf,(const char*)"KB/"); 
				gui_num2str(numstrbuf,dtsize);
				strcat((char*)msgbuf,(const char*)numstrbuf);//����������	  
			}
			strcat((char*)msgbuf,(const char*)sysset_system_info_msg_tbl[i]);
			if(i==4||i==5||i==6)
			{									   
				if(i==4)app_get_version(numstrbuf,OS_VERSION,3);	  	//OS�汾
				else if(i==5)app_get_version(numstrbuf,GUI_VERSION,3);	//GUI�汾
				else app_get_version(numstrbuf,HARDWARE_VERSION,2);		//Ӳ���汾
				strcat((char*)msgbuf,(const char*)numstrbuf);		 
 			}     
	   		gui_show_string(msgbuf,x+10,y+32+14+20*i,180,12,12,BLACK);//��ʾ��Ϣ
		} 
 		while(1)
		{
			tp_dev.scan(0);    
			in_obj.get_key(&tp_dev,IN_TYPE_TOUCH);	//�õ�������ֵ   
			delay_ms(1000/OS_TICKS_PER_SEC);		//��ʱһ��ʱ�ӽ���
	 		if(system_task_return)break;			//TPAD����	
			rval=btn_check(rbtn,&in_obj);			//���ذ�ť���
			if(rval&&(rbtn->sta&0X80)==0)break;	 	//����Ч����
	 
		}
 	}
	myfree(msgbuf);
	window_delete(twin);
	btn_delete(rbtn);
	system_task_return=0;//ȡ��TPAD
}	  

//��ʾϵͳ״̬
//x,y:��ʾ������
//���:200,�߶�:190.
//caption:����
void sysset_system_status(u16 x,u16 y,u8* caption)
{
	u8 rval=0,res;
	u8 i=0;
	u16 temperate=0;
 	_window_obj* twin=0;			//����
 	_btn_obj * rbtn=0;				//���ذ�ť		  
	_progressbar_obj * cpuprgb=0;	//CPUʹ���ʽ�����
	_progressbar_obj * meminprgb=0;	//�ڲ��ڴ�ʹ�ý����� 

 	twin=window_creat(x,y,200,190,0,1|1<<5,16);					//��������
	cpuprgb=progressbar_creat(x+10,y+32+22,180,18,0X61);		//����������
	meminprgb=progressbar_creat(x+10,y+32+22+45,180,18,0X61);	//����������
	rbtn=btn_creat(x+50,y+32+22+45+38,SYSSET_BTN3_WIDTH,SYSSET_BTN2_HEIGHT,0,0x02);//������ť
 	if(twin==NULL||cpuprgb==NULL||meminprgb==NULL||rbtn==NULL)rval=1;
	else
	{
		//���ڵ����ֺͱ���ɫ
		twin->caption=caption;
		twin->windowbkc=APP_WIN_BACK_COLOR;	
		//���ذ�ť����ɫ    
		rbtn->bkctbl[0]=0X8452;	//�߿���ɫ
		rbtn->bkctbl[1]=0XAD97;	//��һ�е���ɫ				
		rbtn->bkctbl[2]=0XAD97;	//�ϰ벿����ɫ
		rbtn->bkctbl[3]=0X8452;	//�°벿����ɫ

		rbtn->caption=(u8*)GUI_BACK_CAPTION_TBL[gui_phy.language];//����Ϊ����

		cpuprgb->totallen=100; 			//�����100
		meminprgb->totallen=100;    	//�����100
														 
 		window_draw(twin);						//��������
		btn_draw(rbtn);							//����ť

		gui_show_string((u8*)sysset_sysstatus_tbl[0][gui_phy.language],x+10,y+32+5,190,12,12,SYSSET_INWIN_FONT_COLOR);		//��ʾCPUʹ����
 		gui_show_string((u8*)sysset_sysstatus_tbl[1][gui_phy.language],x+10,y+32+5+45,190,12,12,SYSSET_INWIN_FONT_COLOR);	//��ʾ�ڲ��ڴ�ʹ����
 		gui_show_string((u8*)sysset_sysstatus_tbl[2][gui_phy.language],x+200-10-36-36,y+32+5,190,12,12,SYSSET_INWIN_FONT_COLOR);//��ʾ�¶�
 		gui_show_string("��",x+200-10-12,y+32+5,190,12,12,SYSSET_INWIN_FONT_COLOR);	//��ʾCPU��ǰ�¶�
 	}
 	while(rval==0)
	{
		tp_dev.scan(0);    
		in_obj.get_key(&tp_dev,IN_TYPE_TOUCH);	//�õ�������ֵ   
		delay_ms(1000/OS_TICKS_PER_SEC);		//��ʱһ��ʱ�ӽ���
 		if(system_task_return)break;			//��Ҫ����	
		res=btn_check(rbtn,&in_obj);			//���ذ�ť���
		if(res)
		{
			if((rbtn->sta&0X80)==0)//����Ч����
			{
				break;//�˳�
			}
		}	
		if(i==0)//ʱ�䵽��,�����ڴ�,CPU�ȵ�״̬
		{
			cpuprgb->curpos=OSCPUUsage;
			meminprgb->curpos=mem_perused();//�ڲ��ڴ�ʹ����

			progressbar_draw_progressbar(cpuprgb);	//��ʾ������
			progressbar_draw_progressbar(meminprgb);//��ʾ������
			//��ʾ�¶�ֵ
			temperate=Get_Temp();//�õ��ڲ��¶�
			app_show_float(x+200-10-12,y+32+5,temperate,1,5,12,SYSSET_INWIN_FONT_COLOR,twin->windowbkc);//��ʾ�¶�
  			i=100;
		}
		i--;
	}
	window_delete(twin);			//ɾ������
	btn_delete(rbtn);				//ɾ����ť	
	progressbar_delete(cpuprgb);	//ɾ��������
	progressbar_delete(meminprgb);	//ɾ��������
	system_task_return=0;		 	//����˳���־
}
//����ϵͳ
//caption:����
void sysset_system_about(u16 x,u16 y,u8*caption)
{
  	_window_obj* twin=0;			//����
 	_btn_obj * rbtn=0;				//���ذ�ť		  
	u8 rval=0;
 	u32 sn0,sn1,sn2;
	u8 *tbuf;		  
	u8 verbuf[6];

 	tbuf=mymalloc(31);					//����31���ֽ�
  	twin=window_creat(x,y,200,262,0,1|1<<5,16);	//��������
 	rbtn=btn_creat(x+50,y+32+180+10,SYSSET_BTN3_WIDTH,SYSSET_BTN2_HEIGHT,0,0x02);//������ť
  	if(twin!=NULL&&rbtn!=NULL)
	{
		//���ڵ����ֺͱ���ɫ
		twin->caption=caption;
		twin->windowbkc=APP_WIN_BACK_COLOR;	
 		//���ذ�ť����ɫ    
		rbtn->bkctbl[0]=0X8452;	//�߿���ɫ
		rbtn->bkctbl[1]=0XAD97;	//��һ�е���ɫ				
		rbtn->bkctbl[2]=0XAD97;	//�ϰ벿����ɫ
		rbtn->bkctbl[3]=0X8452;	//�°벿����ɫ
 		rbtn->caption=(u8*)GUI_BACK_CAPTION_TBL[gui_phy.language];//����Ϊ����

 		window_draw(twin);		//��������	
		btn_draw(rbtn);	  		//������
 		gui_draw_rectangle(x+10-4,y+32+10+8,188,102,0X4A49);		//���߿�
 		gui_draw_rectangle(x+10-4,y+32+120+18,188,42,0X4A49);		//���߿�
 
  		gui_phy.back_color=APP_WIN_BACK_COLOR;
		gui_show_ptstr(x+10,y+32+10,x+10+180,y+32+10+16,0,BLACK,16,(u8*)sysset_system_about_caption_tbl[0][gui_phy.language],0);//ALIENTEK Mini
 	 	strcpy((char*)tbuf,"HARDWARE:");
		app_get_version(verbuf,HARDWARE_VERSION,2);
		strcat((char*)tbuf,(const char*)verbuf);
		strcat((char*)tbuf,", SOFTWARE:");						   
		app_get_version(verbuf,SOFTWARE_VERSION,3);
		strcat((char*)tbuf,(const char*)verbuf);
  		gui_show_string(tbuf,x+10,y+32+34,180,12,12,BLACK);//��ʾ��Ϣ
 		gui_show_string("Copyright (C) 2015~2025",x+10,y+32+34+20*1,180,12,12,BLACK);//��ʾ��Ϣ
 		gui_show_string("����������ӿƼ����޹�˾",x+10,y+32+34+20*2,180,12,12,BLACK);//��ʾ��Ϣ
 		gui_show_string("www.openedv.com",x+10,y+32+34+20*3,180,12,12,BLACK);//��ʾ��Ϣ
 
  		gui_show_ptstr(x+10,y+32+120+10,x+10+180,y+32+120+10+16,0,BLACK,16,(u8*)sysset_system_about_caption_tbl[1][gui_phy.language],0);//��Ʒ���к�
		app_getstm32_sn(&sn0,&sn1,&sn2);//�õ����к�
		sprintf((char*)tbuf,"SN:%X%X%X",sn0,sn1,sn2);
		gui_show_string(tbuf,x+10,y+32+120+10+24,180,12,12,BLACK);//��ʾ��Ϣ
  		while(1)
		{
			tp_dev.scan(0);    
			in_obj.get_key(&tp_dev,IN_TYPE_TOUCH);	//�õ�������ֵ   
			delay_ms(1000/OS_TICKS_PER_SEC);		//��ʱһ��ʱ�ӽ���
	 		if(system_task_return)break;			//TPAD����	
			rval=btn_check(rbtn,&in_obj);			//���ذ�ť���
			if(rval&&(rbtn->sta&0X80)==0)break;	 	//����Ч����
	 
		}
 	}
	myfree(tbuf);
	window_delete(twin);
	btn_delete(rbtn);
	system_task_return=0;//ȡ��TPAD
} 

//������Ŀ����
//mcaption:��Ŀ¼�µ�Ŀ¼��(һ��Ҫ��'.'�ַ�������)
u8 * set_search_caption(const u8 *mcaption)
{
	while(*mcaption!='.')mcaption++;
	return (u8*)(++mcaption);
}
//ϵͳ�������ݱ�����:SYSTEM_PARA_SAVE_BASE
//��ȡϵͳ������Ϣ
//sysset:ϵͳ��Ϣ 
void sysset_read_para(_system_setings * sysset)
{
	STMFLASH_Read(SYSTEM_PARA_SAVE_BASE,(u16*)sysset,sizeof(_system_setings));
}
//д��ϵͳ������Ϣ
//sysset:ϵͳ��Ϣ 
void sysset_save_para(_system_setings * sysset)
{
  	OS_CPU_SR cpu_sr=0;
	OS_ENTER_CRITICAL();//�����ٽ���(�޷����жϴ��) 
	STMFLASH_Write(SYSTEM_PARA_SAVE_BASE,(u16*)sysset,sizeof(_system_setings));
	OS_EXIT_CRITICAL();	//�˳��ٽ���(���Ա��жϴ��)
}

//ϵͳ���� 
u8 sysset_play(void)
{	
	u8 tlanguage;			//����ʱ���Ե�����					  	   
	u8 res;		 
	u8 selitem=0;
	u8 topitem=0;  	 
	u8 **items;	     					   	   
 	u8 *caption;			//�����б� 
	u8 *scaption;			//�ӱ���
	u16 temp1;
	u8 temp2,temp3;			//ʱ��/�����ݴ�
	u8 savemask=0;		    //0,����Ҫ�����κ�����;
							//[0]:systemset���ݸĶ����
							//[1]:vsset���ݸĶ����
							//[2]:alarm���ݸĶ����	 
  
    caption=(u8*)APP_MFUNS_CAPTION_TBL[5][gui_phy.language];	 
	items=(u8**)sysset_mmenu_tbl[gui_phy.language];
	tlanguage=gui_phy.language;							//����ս��������Ե�����
	while(1)
	{   						   
   		res=app_listbox_select(&selitem,&topitem,caption,items,SYS_MENU_SIZE);
 		if(system_task_return)break;					//TPAD����
		if(res&0X80)
		{	  
			scaption=set_search_caption(items[selitem]); 	 						   
			switch(selitem)//�ж�ѡ�����Ŀ
			{
				case 0://ʱ������ 			    
					RTC_Get();//���µ����µ�ʱ�� 
					temp1=calendar.hour;
					temp2=calendar.min;
					res=sysset_time_set((lcddev.width-150)/2,(lcddev.height-200)/2,(u8*)&temp1,(u8*)&temp2,scaption);
					if(res==0)//�ɹ�
					{
						RTC_Set(calendar.w_year,calendar.w_month,calendar.w_date,temp1,temp2,0);//����ʱ��
					} 
  					break;	  
				case 1://��������  	    
					RTC_Get();//���µ����µ�ʱ�� 
					temp1=calendar.w_year;
					temp2=calendar.w_month;	 
					temp3=calendar.w_date;
					res=sysset_date_set((lcddev.width-190)/2,(lcddev.height-200)/2,&temp1,(u8*)&temp2,(u8*)&temp3,scaption);
					if(res==0)//�ɹ�
					{
						RTC_Set(temp1,temp2,temp3,calendar.hour,calendar.min,calendar.sec);//����ʱ��
					} 																	  
					break;
				case 2://����ʱ������
					temp1=alarm.hour;
					temp2=alarm.min;
					res=sysset_time_set((lcddev.width-150)/2,(lcddev.height-200)/2,(u8*)&temp1,(u8*)&temp2,scaption);
					if(res==0)//�ɹ�,�����ȷ�ϰ�ť
					{
						alarm.hour=temp1;
						alarm.min=temp2;
						calendar_alarm_init(&alarm,&calendar);//���³�ʼ������
						savemask|=1<<2;//����������ݸĶ���
					} 
  					break;
				case 3://���ӿ�������
					temp1=alarm.weekmask;
			 		res=app_items_sel((lcddev.width-200)/2,(lcddev.height-302)/2,200,302,(u8**)calendar_week_table[gui_phy.language],7,(u8*)&temp1,0XB0,scaption);//��ѡ
 					if(res==0)//���óɹ�
					{
						alarm.weekmask=temp1;
						calendar_alarm_init(&alarm,&calendar);//���³�ʼ������	
						savemask|=1<<2;//����������ݸĶ���
					}   
					break;
				case 4://����������������
					temp1=alarm.ringsta&0X03;
			 		res=app_items_sel((lcddev.width-180)/2,(lcddev.height-232)/2,180,72+40*4,(u8**)sysset_ring_tbl[gui_phy.language],4,(u8*)&temp1,0X90,scaption);//��ѡ
					if(res==0)//���óɹ�
					{
						alarm.ringsta&=0XFC;//ȡ�������λԭ��������
						alarm.ringsta|=temp1;//������������						    
						savemask|=1<<2;//����������ݸĶ���
					}   
					break;
				case 5://��������
					temp1=gui_phy.language;//�õ�֮ǰ������
			 		res=app_items_sel((lcddev.width-180)/2,(lcddev.height-192)/2,180,72+40*3,(u8**)sysset_language_tbl,3,(u8*)&temp1,0X90,scaption);//��ѡ
					if(res==0)//���óɹ�
					{												   
						gui_phy.language=temp1;		//������������
						systemset.syslanguage=temp1;//��������Ĭ������
					    caption=(u8*)APP_MFUNS_CAPTION_TBL[5][gui_phy.language];	 
						items=(u8**)sysset_mmenu_tbl[gui_phy.language];			 
						savemask|=1<<0;//���ϵͳ�������ݸĶ���
					} 
					break; 
				case 6://ͼƬ���ģʽ����				 
					temp1=systemset.picmode;//�õ�֮ǰ������
			 		res=app_items_sel((lcddev.width-180)/2,(lcddev.height-152)/2,180,72+40*2,(u8**)sysset_avplaymode_tbl[gui_phy.language],2,(u8*)&temp1,0X90,scaption);//2��ģʽ
					if(res==0)//���óɹ�
					{												   
						systemset.picmode=temp1;//����ͼƬ���ģʽ			   		 
						savemask|=1<<0;//���ϵͳ�������ݸĶ���
					} 
					break; 	 
				case 7://��ĻУ׼  
					if((tp_dev.touchtype&0X80)==0)TP_Adjust(); //����������ҪУ׼
					system_task_return=0;	//����˳���־
					break; 
				case 8://ϵͳ����
					res=window_msg_box((lcddev.width-200)/2,(lcddev.height-80)/2,200,80,"",(u8*)sysset_system_remaindmsg_tbl[0][gui_phy.language],12,0,0X03,0);
					if(res==1)//��Ҫ����
					{
				    	sysset_system_update((u8*)sysset_system_update_msg_tbl[gui_phy.language],(lcddev.width-200)/2,(lcddev.height-120)/2-10);
					}
					system_task_return=0;//ȡ��TPAD 
					break;
				case 9://ϵͳ��Ϣ	
					sysset_system_info((lcddev.width-200)/2,(lcddev.height-292)/2,scaption);
					break;
				case 10://ϵͳ״̬ ��ʾCPUʹ����/�ڴ�״̬��
					sysset_system_status((lcddev.width-200)/2,(lcddev.height-190)/2,scaption);
					break;
				case 11://����
					sysset_system_about((lcddev.width-200)/2,(lcddev.height-262)/2,scaption);
					break;	  				 				
			}
  		}else break;//�д�������	 			   
	}
	if(savemask&1<<0)//ϵͳ�������ݸĶ���
	{
		printf("sysset save!\r\n");
		sysset_save_para(&systemset);	//����ϵͳ������Ϣ
		mui_init(0); 					//Main UI ��ʼ��		
	} 
	if(savemask&1<<2)//�������ݸĶ���
	{
		printf("alarm save!\r\n");
		calendar_save_para(&alarm);		//��������������Ϣ 
	}   
	if(tlanguage!=gui_phy.language)		//���Է����˸ı�
	{
		LCD_Clear(BLACK); 
	}
	return res;			   							  
}






















