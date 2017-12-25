#include "paint.h"	    
#include "calendar.h"	    
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

//������Ƕ����ɫ
#define PAINT_INWIN_FONT_COLOR		0X736C		//0XAD53		

//��������
const u8* paint_penset_tbl[GUI_LANGUAGE_NUM]=
{"��������","���P�O��","PEN SET"};
//ģʽѡ��
const u8* paint_mode_tbl[GUI_LANGUAGE_NUM][2]=
{
{"�½�����","��λͼ",},
{"�½�����","���_λ�D",},		 
{"Create new file","Open exist file",},
};

//��������ѡ��
const u8* pen_set_tbl[GUI_LANGUAGE_NUM][2]=
{
{"������ɫ����","���ʳߴ�����",},
{"���P�ɫ�O��","���P�ߴ��O��",},		 
{"Pen color Set","Pen size set",},
};	   

/////////////////////////////////////////////////////////////////////////////////
//ͨ��ʱ���ȡ�ļ���
//��ϳ�:����"0:PAINT/PAINT20120321210633.bmp"���ļ���
void paint_new_pathname(u8 *pname)
{	 
	RTC_Get();//����ʱ��
	sprintf((char*)pname,"0:PAINT/PAINT%04d%02d%02d%02d%02d%02d.bmp",calendar.w_year,calendar.w_month,calendar.w_date,calendar.hour,calendar.min,calendar.sec);
}

//������ɫ����
const u16 colorbar_tbl[20]={0XF800,0XFFE0,0X07E0,0X07FF,0X001F,0XF81F,0XFFF0,0X07F0,0X87FF,0X841F,
                            0XF810,0XFC08,0X8200,0X081F,0X0210,0X041F,0X8010,0X8408,0X0000,0XFFFF};
//��ʾ����(��ȹ̶�:100=20*5,�߿�ռ1������,�ܹ��ߴ���:(100+2)*(height+2))
//sx,sy:��������ʼ����
//height:�߶�ֵ 
//20����ɫֵ
void paint_show_colorbar(u16 sx,u16 sy,u8 height)
{ 
	u16 i;
	if(sx==0||sy==0)return;
	gui_draw_rectangle(sx-1,sy-1,100+2,height+2,0X8452);//��Ԥ���߿�
	for(i=0;i<20;i++)
	{
		gui_fill_rectangle(sx+5*i,sy,5,height,colorbar_tbl[i]);	//��Ԥ��ͼ
	}
}		

//�ߴ�̶�:150*96
//x,y:���Ͻ�����
//color:������ɫָ��
//caption:��������				  
//����ֵ:0,�ɹ�����;
//    ����,����������
u8 paint_pen_color_set(u16 x,u16 y,u16* color,u8*caption)
{
 	u8 rval=0,res;
  	_window_obj* twin=0;			//����
 	_btn_obj * rbtn=0;				//ȡ����ť		  
 	_btn_obj * okbtn=0;				//ȷ����ť
 	u16 tempcolor=*color;
	u8 cmask=1;						//��ɫ�Ƿ��б䶯
	
   	twin=window_creat(x,y,150,96,0,1|1<<5,16);		//�������� �� 
  	okbtn=btn_creat(x+15,y+66,50,24,0,0x02);		//������ť
 	rbtn=btn_creat(x+15+50+20,y+66,50,24,0,0x02);	//������ť
	if(twin==NULL||rbtn==NULL||okbtn==NULL||rval)rval=1;
	else
	{
		//���ڵ����ֺͱ���ɫ
		twin->caption=caption;
		twin->captionheight=24;		//�޸�caption�߶�Ϊ24
		twin->windowbkc=APP_WIN_BACK_COLOR;	
		//���ذ�ť����ɫ    
		rbtn->bkctbl[0]=0X8452;		//�߿���ɫ
		rbtn->bkctbl[1]=0XAD97;		//��һ�е���ɫ				
		rbtn->bkctbl[2]=0XAD97;		//�ϰ벿����ɫ
		rbtn->bkctbl[3]=0X8452;		//�°벿����ɫ
		okbtn->bkctbl[0]=0X8452;	//�߿���ɫ
		okbtn->bkctbl[1]=0XAD97;	//��һ�е���ɫ				
		okbtn->bkctbl[2]=0XAD97;	//�ϰ벿����ɫ
		okbtn->bkctbl[3]=0X8452;	//�°벿����ɫ

		rbtn->caption=(u8*)GUI_CANCEL_CAPTION_TBL[gui_phy.language];	//����Ϊȡ��
 		okbtn->caption=(u8*)GUI_OK_CAPTION_TBL[gui_phy.language];		//����Ϊȷ��
  		window_draw(twin);				//��������
		btn_draw(rbtn);					//����ť
		btn_draw(okbtn);				//����ť
		paint_show_colorbar(x+45,y+32,25);//����ɫ��
		while(rval==0)
		{
			tp_dev.scan(0);    
			in_obj.get_key(&tp_dev,IN_TYPE_TOUCH);	//�õ�������ֵ   
			delay_ms(1000/OS_TICKS_PER_SEC);		//��ʱһ��ʱ�ӽ���
			if(system_task_return)break;
			res=btn_check(rbtn,&in_obj);			//ȡ����ť���
			if(res&&((rbtn->sta&0X80)==0))			//����Ч����
			{
				rval=1;
				break;//�˳�
			}
			res=btn_check(okbtn,&in_obj); 			//ȷ�ϰ�ť���
			if(res&&((okbtn->sta&0X80)==0))			//����Ч����
			{
				rval=0XFF;		   
				break;//ȷ����
			}
			if(cmask) //��������Ƿ���Ҫ����
			{ 
				cmask=0; 
				gui_fill_rectangle(x+5,y+30,30,30,tempcolor);	//��Ԥ��ͼ
 			}	   
			if(app_tp_is_in_area(&tp_dev,x+45,y+32,99,24)&&(tp_dev.sta&TP_PRES_DOWN))
			{
				tempcolor=gui_phy.read_point(tp_dev.x[0],tp_dev.y[0]);	
				cmask=1;
			}	
 		}
	}
	window_delete(twin);							//ɾ������
	btn_delete(rbtn);								//ɾ����ť	
 	btn_delete(okbtn);								//ɾ����ť	
	if(rval==0XFF)
	{
		*color=tempcolor;
		return 0;
 	}
	system_task_return=0;
	return rval;
}
//x,y:���Ͻ�����
//color:������ɫָ��
//*mode:���ʴ�С
//caption:��������				  
//����ֵ:0,�ɹ�����;
//    ����,����������
u8 paint_pen_size_set(u16 x,u16 y,u16 color,u8 *mode,u8*caption)
{
 	u8 rval=0,res;
   	_window_obj* twin=0;			//����
 	_btn_obj * rbtn=0;				//ȡ����ť		  
 	_btn_obj * okbtn=0;				//ȷ����ť		  
	_progressbar_obj * sizeprgb;	//�ߴ����ý�����
 	u8 smask=1;						//�ߴ��Ƿ��б䶯
	u8 tempsize=*mode;

  	twin=window_creat(x,y,150,96,0,1|1<<5,16);			//�������� ��
 	sizeprgb=progressbar_creat(x+36,y+35,100,20,0X20);	//����������
	if(sizeprgb==NULL)rval=1;
  	okbtn=btn_creat(x+15,y+66,50,24,0,0x02);		//������ť
 	rbtn=btn_creat(x+15+50+20,y+66,50,24,0,0x02);	//������ť
	if(twin==NULL||rbtn==NULL||okbtn==NULL||rval)rval=1;
	else
	{
		//���ڵ����ֺͱ���ɫ
		twin->caption=caption;
		twin->captionheight=24;		//�޸�caption�߶�Ϊ24
		twin->windowbkc=APP_WIN_BACK_COLOR;	
		//���ذ�ť����ɫ    
		rbtn->bkctbl[0]=0X8452;		//�߿���ɫ
		rbtn->bkctbl[1]=0XAD97;		//��һ�е���ɫ				
		rbtn->bkctbl[2]=0XAD97;		//�ϰ벿����ɫ
		rbtn->bkctbl[3]=0X8452;		//�°벿����ɫ
		okbtn->bkctbl[0]=0X8452;	//�߿���ɫ
		okbtn->bkctbl[1]=0XAD97;	//��һ�е���ɫ				
		okbtn->bkctbl[2]=0XAD97;	//�ϰ벿����ɫ
		okbtn->bkctbl[3]=0X8452;	//�°벿����ɫ

		sizeprgb->totallen=10;		//���뾶Ϊ10 
	   	sizeprgb->curpos=tempsize;	//��ǰ�ߴ�

		rbtn->caption=(u8*)GUI_CANCEL_CAPTION_TBL[gui_phy.language];	//����Ϊȡ��
 		okbtn->caption=(u8*)GUI_OK_CAPTION_TBL[gui_phy.language];		//����Ϊȷ��
  		window_draw(twin);				//��������
		btn_draw(rbtn);					//����ť
		btn_draw(okbtn);				//����ť 
		while(rval==0)
		{
			tp_dev.scan(0);    
			in_obj.get_key(&tp_dev,IN_TYPE_TOUCH);	//�õ�������ֵ   
			delay_ms(1000/OS_TICKS_PER_SEC);		//��ʱһ��ʱ�ӽ���
			if(system_task_return)break;
			res=btn_check(rbtn,&in_obj);			//ȡ����ť���
			if(res&&((rbtn->sta&0X80)==0))			//����Ч����
			{
				rval=1;
				break;//�˳�
			}
			res=btn_check(okbtn,&in_obj); 			//ȷ�ϰ�ť���
			if(res&&((okbtn->sta&0X80)==0))			//����Ч����
			{
				rval=0XFF;		   
				break;//ȷ����
			}
			if(smask) //��������Ƿ���Ҫ����
			{
				if(smask==1)//��������Ҫ����.
				{
 	 				progressbar_draw_progressbar(sizeprgb);	//���½�����
 				}
				smask=0;
  				gui_phy.back_color=APP_WIN_BACK_COLOR;		//���ñ���ɫ 
				gui_fill_rectangle(x+6,y+32,24,24,gui_phy.back_color);//���Ԥ������	 
				paint_draw_point(x+18,y+32+12,color,tempsize);	//��Ԥ��ͼ
 			}	  
			res=progressbar_check(sizeprgb,&in_obj);
 			if(res&&(tempsize!=sizeprgb->curpos))	//�������Ķ���
			{
				tempsize=sizeprgb->curpos;	 		//�������µĽ��  
				smask=2;//������������,��������Ҫ����
			} 	
 		}
	}
	window_delete(twin);							//ɾ������
	btn_delete(rbtn);								//ɾ����ť	
	btn_delete(okbtn);								//ɾ����ť	
	progressbar_delete(sizeprgb);//ɾ��������  
	if(rval==0XFF)
	{
		*mode=tempsize;
		return 0;
 	}
	system_task_return=0;
	return rval;
} 
//x,y:����
//color:��ɫ
//mode:
//[7:4]:����
//[3:0]:�ߴ�.(Բ��,���뾶)
void paint_draw_point(u16 x,u16 y,u16 color,u8 mode)
{
	u8 size=mode&0X0F;//�õ��ߴ��С	    
	if(size==0)gui_phy.draw_point(x,y,color);
	else gui_fill_circle(x,y,size,color);		   
}
	  
//��ͼ����
//���л�ͼ�ļ�,��������SD��PAINT�ļ�����.
u8 paint_play(void)
{
 	DIR paintdir;					//paintdirר��	  
	FILINFO paintinfo;		 	   
  	_btn_obj* rbtn=0;				//���ذ�ť�ؼ�
   	_filelistbox_obj * flistbox=0;
	_filelistbox_list * filelistx=0;//�ļ���
 	u16 lastpos[5][2];				//���һ�ε����� 
	
 	u16 pencolor=RED;
   	u8 *pname=0;
	u8 *fn;	 
	u8 res;
	u8 rval=0;
   	u8 key=0;
	u8 mode=0X02;					//��ͼģʽ				 
									//[7:4]:����
									//[3:0]:��뾶
	u8 editmask=0;					//�Ķ���־λ
  	u16 *pset_bkctbl=0;				//����ʱ����ɫָ�� 
  	br=0;
	//��ѡ��ģʽ                                                                                                
	res=app_items_sel((lcddev.width-180)/2,(lcddev.height-152)/2,180,72+40*2,(u8**)paint_mode_tbl[gui_phy.language],2,(u8*)&br,0X90,(u8*)APP_MODESEL_CAPTION_TBL[gui_phy.language]);//2��ѡ��
 	if(res==0)//ȷ�ϼ�������,ͬʱ������������
	{	   
		LCD_Clear(0XFFFF);				//����
 		if(br==0)						//�½�ͼƬ�ļ�
		{
 			if(gui_phy.memdevflag&(1<<0))f_mkdir("0:PAINT");		//ǿ�ƴ����ļ���,����ͼ��
			pname=gui_memin_malloc(40);	//����40���ֽ��ڴ�,����"0:PAINT/PAINT20120321210633.bmp"
			pname[0]='\0';//��ӽ�����
			paint_new_pathname(pname);
			if(pname==NULL)rval=1;
 		}else	//ѡ��һ��BMP�ļ���
		{
			app_gui_tcbar(0,lcddev.height-gui_phy.tbheight,lcddev.width,gui_phy.tbheight,0x01);	//�Ϸֽ���
			rbtn=btn_creat(lcddev.width-2*gui_phy.tbfsize-8-1,lcddev.height-gui_phy.tbheight,2*gui_phy.tbfsize+8,gui_phy.tbheight-1,0,0x03);//�������ְ�ť
			if(rbtn==NULL)rval=1;	//û���㹻�ڴ湻����
			else
			{																				
				rbtn->caption=(u8*)GUI_BACK_CAPTION_TBL[gui_phy.language];//���� 
				rbtn->font=16;			//����		 
				rbtn->bcfdcolor=WHITE;	//����ʱ����ɫ
				rbtn->bcfucolor=WHITE;	//�ɿ�ʱ����ɫ
				btn_draw(rbtn);			//�ػ���ť
			}
			app_filebrower((u8*)paint_mode_tbl[gui_phy.language][1],0X07);//��ʾ��Ϣ
			btn_draw(rbtn);//����ť
			flistbox=filelistbox_creat(0,gui_phy.tbheight,lcddev.width,lcddev.height-gui_phy.tbheight*2,1,gui_phy.listfsize);//����һ��filelistbox
			if(flistbox==NULL)rval=1;							//�����ڴ�ʧ��.
			else  
			{
				flistbox->fliter=FLBOX_FLT_PICTURE;	//����ͼƬ�ļ�
				flistbox->fliterex=1<<0;			//��������λͼ�ļ�
				filelistbox_add_disk(flistbox);		//��Ӵ���·��
				filelistbox_draw_listbox(flistbox);
			} 	 
 			//Ϊ���ļ������뻺����
		 	paintinfo.lfsize = 120;
			paintinfo.lfname = gui_memin_malloc(paintinfo.lfsize);
			if(paintinfo.lfname==NULL)rval=1;//�����ڴ�ʧ�� 
		   	else gui_memset((u8*)paintinfo.lfname,0,paintinfo.lfsize);
			for(res=0;res<5;res++)lastpos[res][0]=0XFFFF; //�����趨
		   	while(rval==0)
			{
				tp_dev.scan(0);    
				in_obj.get_key(&tp_dev,IN_TYPE_TOUCH);	//�õ�������ֵ   
				delay_ms(1000/OS_TICKS_PER_SEC);		//��ʱһ��ʱ�ӽ���
    			if(system_task_return)				   	//��Ҫ����
				{
					rval=1;
					break;			
				}
				filelistbox_check(flistbox,&in_obj);	//ɨ���ļ�
				res=btn_check(rbtn,&in_obj);
				if(res)
				{
					if(((rbtn->sta&0X80)==0))//��ť״̬�ı���
					{
						if(flistbox->dbclick!=0X81)
						{
		 					filelistx=filelist_search(flistbox->list,flistbox->selindex);//�õ���ʱѡ�е�list����Ϣ
							if(filelistx->type==FICO_DISK)
							{
								rval=1;
								break;	//�Ѿ�������������
						 	}
							else filelistbox_back(flistbox);	//�˻���һ��Ŀ¼	 
						} 
		 			}	 
				}
				if(flistbox->dbclick==0X81)//˫���ļ���
				{	    
 		     		rval=f_opendir(&paintdir,(const TCHAR*)flistbox->path); //��ѡ�е�Ŀ¼
					if(rval)break;	 			   
					dir_sdi(&paintdir,flistbox->findextbl[flistbox->selindex-flistbox->foldercnt]);
			 		rval=f_readdir(&paintdir,&paintinfo);//��ȡ�ļ���Ϣ
					if(rval)break;//�򿪳ɹ�    
					fn=(u8*)(*paintinfo.lfname?paintinfo.lfname:paintinfo.fname);
					pname=gui_memin_malloc(strlen((const char*)fn)+strlen((const char*)flistbox->path)+2);//�����ڴ�
					if(pname==NULL)rval=1;//����ʧ��
					{
						pname=gui_path_name(pname,flistbox->path,fn);	//�ļ�������·��  
						LCD_Clear(WHITE);
						ai_load_picfile(pname,0,0,lcddev.width,lcddev.height,1);//�������ͼƬ
    					break;
					} 
				}
			}
			filelistbox_delete(flistbox);		//���filelistbox��ʡ�ڴ�
			gui_memin_free(paintinfo.lfname);	//������ļ��������ڴ� 
		}
		pset_bkctbl=gui_memex_malloc(180*80*2);//�����ڴ�
		if(pset_bkctbl==NULL)rval=1;
   	 	while(rval==0) 
		{		 
			tp_dev.scan(0); 		 
			in_obj.get_key(&tp_dev,IN_TYPE_TOUCH);	//�õ�������ֵ 	  
    		if(system_task_return)break;			//��Ҫ����	 
 			key=KEY_Scan(0);					//ɨ�谴��
			if(key>=2)							//KEY1/WK_UP ����,��ɫ����
			{
				app_read_bkcolor((lcddev.width-150)/2,(lcddev.height-96)/2,150,96,pset_bkctbl);//��ȡ����ɫ
				if(key==WKUP_PRES)paint_pen_color_set((lcddev.width-150)/2,(lcddev.height-96)/2,&pencolor,(u8*)pen_set_tbl[gui_phy.language][0]);	//KEY1����
 				else paint_pen_size_set((lcddev.width-150)/2,(lcddev.height-96)/2,pencolor,&mode,(u8*)pen_set_tbl[gui_phy.language][1]);	//WK_UP����
				app_recover_bkcolor((lcddev.width-150)/2,(lcddev.height-96)/2,150,96,pset_bkctbl);//�ָ�����ɫ
			} 
			//������ 
			if(tp_dev.sta&TP_PRES_DOWN)			
			{	
				editmask=1;
				paint_draw_point(tp_dev.x[0],tp_dev.y[0],pencolor,mode);	//������	
			}else delay_ms(1000/OS_TICKS_PER_SEC);	//û�а������µ�ʱ�� ��ʱһ��ʱ�ӽ���	 
			//������
			if(tp_dev.touchtype&0X80)		//������
			{
				for(res=0;res<CT_MAX_TOUCH; res++)
				{
					if((tp_dev.sta)&(1<<res))
					{
						if(tp_dev.x[res]<lcddev.width&&tp_dev.y[res]<lcddev.height)
						{
							if(lastpos[res][0]==0XFFFF)
							{
								lastpos[res][0] = tp_dev.x[res];
								lastpos[res][1] = tp_dev.y[res];
							}
							gui_draw_bline(lastpos[res][0],lastpos[res][1],tp_dev.x[res],tp_dev.y[res],mode&0X0F,pencolor);//����
							lastpos[res][0]=tp_dev.x[res];
							lastpos[res][1]=tp_dev.y[res];  
						}
					}else lastpos[res][0]=0XFFFF;	                                                                                                                 
				} 
				if((tp_dev.sta&0X1F)==0)delay_ms(1000/OS_TICKS_PER_SEC);//û�а������µ�ʱ�� ��ʱһ��ʱ�ӽ���	 
			} 
		}
 	}	 
	if(editmask)//�б༭��,��Ҫ����
	{	   
		app_read_bkcolor((lcddev.width-180)/2,(lcddev.height-80)/2,180,80,pset_bkctbl);		//��ȡ����ɫ
 		res=window_msg_box((lcddev.width-180)/2,(lcddev.height-80)/2,180,80,"",(u8*)APP_SAVE_CAPTION_TBL[gui_phy.language],12,0,0X03,0);
		app_recover_bkcolor((lcddev.width-180)/2,(lcddev.height-80)/2,180,80,pset_bkctbl);	//�ָ�����ɫ
 		if(res==1)//��Ҫ����
		{
			if(bmp_encode(pname,0,0,lcddev.width,lcddev.height,1))//����BMP�ļ�
			{
				window_msg_box((lcddev.width-200)/2,(lcddev.height-100)/2,200,100,(u8*)APP_CREAT_ERR_MSG_TBL[gui_phy.language],(u8*)APP_REMIND_CAPTION_TBL[gui_phy.language],12,0,0,0);//��ʾSD���Ƿ����
				delay_ms(2000);//�ȴ�2����
		 	}
		}
	} 
	btn_delete(rbtn);					//ɾ����ť
   	gui_memex_free(pset_bkctbl);
  	gui_memin_free(pname);
	return rval;

}








