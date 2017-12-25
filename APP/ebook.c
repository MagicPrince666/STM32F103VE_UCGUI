#include "ebook.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//APP-����ͼ��ʵ�� ����	   
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
 
//��������
u8*const ebook_remind_msg_tbl[GUI_LANGUAGE_NUM]=
{
"�ڴ治��!","�ȴ治��!","Out of memory!",	 
};
//��setings.c���涨��
extern u8*const sysset_remindmsg_tbl[2][GUI_LANGUAGE_NUM];
//�����鹦��
u8 ebook_play(void)
{
	_memo_obj * tmemo=0;	    
	FIL* f_txt;		 
	DIR ebookdir;		//ebookdirר��	  
	FILINFO ebookinfo;	 	   
	u8 res;
	u8 rval=0;			//����ֵ	  
	u8 *pname=0;
	u8 *fn;
	u8 errtype=0;
	u8 ebooksta=0;		//ebook״̬
						//0,�����ļ����״̬    
						//1,�Ѿ���ʼ�ļ��Ķ�
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
 	_btn_obj* rbtn;		//���ذ�ť�ؼ�
   	_filelistbox_obj * flistbox;
	_filelistbox_list * filelistx; 	//�ļ� 
	app_filebrower((u8*)APP_MFUNS_CAPTION_TBL[0][gui_phy.language],0X07);//ѡ��Ŀ���ļ�,���õ�Ŀ������
   	flistbox=filelistbox_creat(0,gui_phy.tbheight,lcddev.width,lcddev.height-gui_phy.tbheight*2,1,gui_phy.listfsize);//����һ��filelistbox
 	if(flistbox==NULL)rval=1;							//�����ڴ�ʧ��.
	else  
	{
		flistbox->fliter=FLBOX_FLT_TEXT|FLBOX_FLT_LRC;	//�����ı��ļ�
		filelistbox_add_disk(flistbox);					//��Ӵ���·��
 		filelistbox_draw_listbox(flistbox);
	} 	  
	//Ϊ���ļ������뻺����
 	ebookinfo.lfsize = _MAX_LFN * 2 + 1;
	ebookinfo.lfname = gui_memin_malloc(ebookinfo.lfsize);
	if(ebookinfo.lfname==NULL)rval=1;//�����ڴ�ʧ�� 
   	else gui_memset((u8*)ebookinfo.lfname,0,ebookinfo.lfsize);
	f_txt=(FIL *)gui_memin_malloc(sizeof(FIL));	//����FIL�ֽڵ��ڴ����� 
	if(f_txt==NULL)rval=1;//����ʧ�� 
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
	if(rval)errtype=1;//�ڴ治�� 
   	while(rval==0)
	{
		tp_dev.scan(0);    
		in_obj.get_key(&tp_dev,IN_TYPE_TOUCH);	//�õ�������ֵ   
		delay_ms(5);
		if(system_task_return)	 				//TPAD����
		{
			if(ebooksta)						//�ļ����״̬,���˳��ļ����
			{			  
				memo_delete(tmemo);		//�ͷ��ڴ�  
				tmemo=NULL;
				gui_memin_free(pname);	//�ͷ��ڴ�		  
			    pname=NULL;
			    ebooksta=0;				//�ص��ļ����״̬
				app_filebrower((u8*)APP_MFUNS_CAPTION_TBL[0][gui_phy.language],0X07);	//ѡ��Ŀ���ļ�,���õ�Ŀ������
				btn_draw(rbtn);			//����ť
				filelistbox_rebuild_filelist(flistbox);//�ؽ�flistbox
				system_task_return=0;
			}else break;	  		
		}
		if(ebooksta==0)							//��δ��ʼ�ļ����
		{
			filelistbox_check(flistbox,&in_obj);//ɨ���ļ�
			res=btn_check(rbtn,&in_obj);		//ɨ�践�ذ���
			if(res)
			{
				if(((rbtn->sta&0X80)==0))//��ť״̬�ı���
				{
 					filelistx=filelist_search(flistbox->list,flistbox->selindex);//�õ���ʱѡ�е�list����Ϣ
					if(filelistx->type==FICO_DISK)//�Ѿ�������������
					{				 
						break;
					}else filelistbox_back(flistbox);//�˻���һ��Ŀ¼	 
	 			}	 
			}
			if(flistbox->dbclick==0X81)//˫���ļ���
			{
	     		rval=f_opendir(&ebookdir,(const TCHAR*)flistbox->path); //��ѡ�е�Ŀ¼
				if(rval)break;
				ff_enter(ebookdir.fs);//����fatfs,��ֹ�����.
				dir_sdi(&ebookdir,flistbox->findextbl[flistbox->selindex-flistbox->foldercnt]);
		 		ff_leave(ebookdir.fs);//�˳�fatfs,��������os��
				rval=f_readdir(&ebookdir,&ebookinfo);//��ȡ�ļ���Ϣ
				if(rval)break;//�򿪳ɹ�    
				fn=(u8*)(*ebookinfo.lfname?ebookinfo.lfname:ebookinfo.fname);
				pname=gui_memin_malloc(strlen((const char*)fn)+strlen((const char*)flistbox->path)+2);//�����ڴ�
				if(pname==NULL)rval=1;//����ʧ��
				else
				{
					app_gui_tcbar(0,0,lcddev.width,gui_phy.tbheight,0x02);//�·ֽ���
					gui_show_string(fn,2,(gui_phy.tbheight-gui_phy.tbfsize)/2,lcddev.width-2,gui_phy.tbfsize,gui_phy.tbfsize,WHITE);			//��ʾ�ļ���
					pname=gui_path_name(pname,flistbox->path,fn);	//�ļ�������·��  
					rval=f_open(f_txt,(const TCHAR*)pname,FA_READ);	//ֻ����ʽ���ļ�
					if(rval)break;//�򿪳ɹ�   
 					tmemo=memo_creat(0,gui_phy.tbheight,lcddev.width,lcddev.height-gui_phy.tbheight,0,1,gui_phy.tbfsize,f_txt->fsize+1);//��������txt��ô����ڴ��һ��.	
					if(tmemo==NULL)
					{
						errtype=1;//�ڴ治��
						break;
					}
	 				rval=f_read(f_txt,tmemo->text,f_txt->fsize,(UINT*)&br);	//����txt���������  
					if(rval)break;//�򿪳ɹ�      
					memo_draw_memo(tmemo,0);
				}
				flistbox->dbclick=0;//����Ѵ���
				ebooksta=1;
			}
		}
		if(ebooksta==1)//�����ı��Ķ�״̬.
		{
			memo_check(tmemo,&in_obj); 
		}
	}
	if(errtype)//��Ҫ������ʾ
	{
		window_msg_box((lcddev.width-160)/2,(lcddev.height-70)/2-15,160,70,(u8*)ebook_remind_msg_tbl[gui_phy.language],(u8*)APP_REMIND_CAPTION_TBL[gui_phy.language],12,0,0,0);		
 		delay_ms(500);
	}
	filelistbox_delete(flistbox);	//ɾ��filelist
	btn_delete(rbtn);				//ɾ����ť	 	  
	memo_delete(tmemo);				//�ͷ��ڴ�  	 
	gui_memin_free(pname);			//�ͷ��ڴ�		  
 	gui_memin_free(ebookinfo.lfname);
	gui_memin_free(f_txt);		 
	return rval;   
}















