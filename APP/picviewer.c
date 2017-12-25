 #include "picviewer.h"
#include "piclib.h"
#include "settings.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//APP-�������ʵ�� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/2/16
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//*******************************************************************************
//�޸���Ϣ
//��
////////////////////////////////////////////////////////////////////////////////// 	   
 
//ͼƬ���
u8 picviewer_play(void)
{				 	 
	DIR picdir;			//picdirר��	  
	FILINFO picinfo;   		   
	u8 rval=0;			//����ֵ	  
	u8 *pname=0;
	u8 *fn=0;
	u8 picsta=0;		//ebook״̬
						//0,�����ļ����״̬    
						//1,˳�򲥷�ͼƬ
						//2,��ͣ״̬    
	
	u16 curindex=0;	//��ǰ�����ͼƬ�ļ���������
	u8  endecode=0;	//ʹ�ܽ���
	u8 key; 
	u16 dtime=0;	//��ʱʱ��
	u8 keyup=1;		//�ɿ����
	u8 pictype=0;	//ͼƬ���� 

   	_filelistbox_obj * flistbox; 
	_filelistbox_list * filelistx; 	//�ļ�
 	_btn_obj* rbtn;		//���ذ�ť�ؼ�

	app_filebrower((u8*)APP_MFUNS_CAPTION_TBL[1][gui_phy.language],0X07);//ѡ��Ŀ���ļ�,���õ�Ŀ������
   	flistbox=filelistbox_creat(0,gui_phy.tbheight,lcddev.width,lcddev.height-gui_phy.tbheight*2,1,gui_phy.listfsize);//����һ��filelistbox
 	if(flistbox==NULL)rval=1;							//�����ڴ�ʧ��.
	else  
	{
		flistbox->fliter=FLBOX_FLT_PICTURE;				//ͼƬ�ļ�
		filelistbox_add_disk(flistbox);					//��Ӵ���·��
 		filelistbox_draw_listbox(flistbox);
	} 	  
	//Ϊ���ļ������뻺����
 	picinfo.lfsize = _MAX_LFN * 2 + 1;
	picinfo.lfname = gui_memin_malloc(picinfo.lfsize);
	if(picinfo.lfname==NULL)rval=1;//�����ڴ�ʧ�� 
   	else gui_memset((u8*)picinfo.lfname,0,picinfo.lfsize);
 	rbtn=btn_creat(lcddev.width-2*gui_phy.tbfsize-8-1,lcddev.height-gui_phy.tbheight,2*gui_phy.tbfsize+8,gui_phy.tbheight-1,0,0x03);//�������ְ�ť
 	if(rbtn==NULL)rval=1;	//û���㹻�ڴ湻����
	else
	{																				
	 	rbtn->caption=(u8*)GUI_BACK_CAPTION_TBL[gui_phy.language];//���� 
	 	rbtn->font=gui_phy.tbfsize;//�����µ������С	 	 
		rbtn->bcfdcolor=WHITE;	//����ʱ����ɫ
		rbtn->bcfucolor=WHITE;	//�ɿ�ʱ����ɫ
		btn_draw(rbtn);//����ť
	}
	LED1=1;//�ر�LED1
	while(rval==0)//��ѭ��
	{
		tp_dev.scan(0);    
		in_obj.get_key(&tp_dev,IN_TYPE_TOUCH);	//�õ�������ֵ   
		delay_ms(5);
		if(system_task_return)break;			//TPAD����   	
		if(picsta==0)
		{
			filelistbox_check(flistbox,&in_obj);	//ɨ���ļ�
	 		if(flistbox->dbclick==0X81)				//˫���ļ���
			{	
				curindex=flistbox->selindex-flistbox->foldercnt;//�õ���ǰͼƬ������
				picsta=1;	//ͼƬ����״̬		 
				endecode=1;	//��һ��ͼƬ�Զ����� 
				LCD_Clear(0x0);//���� 	 
			}
		}
		key=btn_check(rbtn,&in_obj);
  		if(key&&((rbtn->sta&0X80)==0))
		{		  
			if(flistbox->dbclick!=0X81)//���ļ������ʱ���˷��ذ�ť,�򷵻���һ��Ŀ¼
			{
				filelistx=filelist_search(flistbox->list,flistbox->selindex);//�õ���ʱѡ�е�list����Ϣ
				if(filelistx->type==FICO_DISK)//�Ѿ�������������,���˳�ͼƬ���
				{				 
					break;
				}else filelistbox_back(flistbox);//�˻���һ��Ŀ¼	 
			} 
		} 
		if(endecode)
		{
     		rval=f_opendir(&picdir,(const TCHAR*)flistbox->path); //��ѡ�е�Ŀ¼
			if(rval)break;	 			   
			ff_enter(picdir.fs);//����fatfs,��ֹ�����.
			dir_sdi(&picdir,flistbox->findextbl[curindex]);
			ff_leave(picdir.fs);//�˳�fatfs,��������os��
	 		rval=f_readdir(&picdir,&picinfo);//��ȡ�ļ���Ϣ
			if(rval)break;//�򿪳ɹ�    
			fn=(u8*)(*picinfo.lfname?picinfo.lfname:picinfo.fname);
			pname=gui_memin_malloc(strlen((const char*)fn)+strlen((const char*)flistbox->path)+2);//�����ڴ�
			if(pname==NULL)break;	//����ʧ��
			pname=gui_path_name(pname,flistbox->path,fn);	//�ļ�������·�� 
			pictype=f_typetell(pname); 
			if(pictype==T_GIF)gui_show_string(fn,5,5,lcddev.width-5,gui_phy.tbfsize,gui_phy.tbfsize,RED);	//��ʾGIF����
			ai_load_picfile(pname,0,0,lcddev.width,lcddev.height,1);			//�������ͼƬ
			if(pictype!=T_GIF)gui_show_string(fn,5,5,lcddev.width-5,gui_phy.tbfsize,gui_phy.tbfsize,RED);	//��ʾͼƬ����						  
 			gui_memin_free(pname);			//�ͷ��ڴ� 
			pname=NULL;
			endecode=0;		    
		}
		keyup=0;
		dtime=0;
		while(picsta)//����ɨ��ѭ��
		{     
			key=pic_tp_scan();   
			dtime++;
			if(dtime>400&&(picsta==1))key=3;//˳�򲥷�״̬��ʱ�����,�Զ�������һ��ͼƬ    
			if(key)//�а�������
			{ 
				dtime=0;
				if(keyup)
				{   
					keyup=0;
					if(key==1)//��һ��ͼƬ
					{
						if(curindex)curindex--;
						else curindex=flistbox->filecnt-1;
						endecode=1;
						LCD_Clear(0x0);//���� 	 
						break;
					}else if(key==2)//����ͣ�ͷ���֮ͣ���л�
					{ 
						if(picsta==1)
						{
							picsta=2;
							LED1=0;	 	//��ʾ��ͣ
						}else 
						{
							picsta=1;
							LED1=1;	   	//��ͣ����
						} 
					}else if(key==3)
					{
						if(systemset.picmode==0)//˳�򲥷�
						{
							if(curindex<(flistbox->filecnt-1))curindex++;
							else curindex=0;
						}else	//�������
						{   
							curindex=app_get_rand(flistbox->filecnt);//����õ���һ��ͼƬ�ı��   	 
						}
						endecode=1;
						LCD_Clear(0x0);//���� 	 
						break;	 
					}  
				}
			}else keyup=1;//��ǰ����ɿ�
			delay_ms(10);  
			if(system_task_return)picsta=0;//TPAD����
			if(picsta==0)//�ص��ļ����״̬֮ǰ�Ĵ���
			{
				LED1=1;	   				//�ر�LED1
				flistbox->dbclick=0;	//���÷��ļ����״̬
				app_filebrower((u8*)APP_MFUNS_CAPTION_TBL[1][gui_phy.language],0X07);//ѡ��Ŀ���ļ�,���õ�Ŀ������
 				btn_draw(rbtn);			//����ť
				filelistbox_rebuild_filelist(flistbox);//�ؽ�flistbox
 				system_task_return=0;	//�������˳�ͼƬ���
				break;
			}
		}
	}	
	LED1=1;//�ر�LED1
	filelistbox_delete(flistbox);	//ɾ��filelist 	  
	btn_delete(rbtn);				//ɾ����ť	 	  
 	gui_memin_free(pname);			//�ͷ��ڴ�		  
 	gui_memin_free(picinfo.lfname);
 	return rval; 
}
//ͼƬ������������
u8 pic_tp_scan(void)
{
	u8 res=0;
	tp_dev.scan(0);    
	if(tp_dev.sta&TP_PRES_DOWN)//�а�������
	{ 
		if(tp_dev.y[0]<lcddev.height/3)res=1;			//��������
		else if(tp_dev.y[0]<lcddev.height*2/3)res=2;	//�����м�
		else if(tp_dev.y[0]<lcddev.height)res=3;		//�������� 
	} 
	while(res)//�ȴ������ɿ�
	{
		tp_dev.scan(0);  
		if((tp_dev.sta&TP_PRES_DOWN)==0)break;
		delay_ms(5);
	} 
	return res;
}






































