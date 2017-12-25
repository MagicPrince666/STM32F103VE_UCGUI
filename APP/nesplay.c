#include "nesplay.h"
#include "nes_main.h"	 
#include "nes_rom.h"	 
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//APP-NES模拟器 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2014/2/16
//版本：V1.1
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//*******************************************************************************
//V1.1 20140216
//新增对各种分辨率LCD的支持.
////////////////////////////////////////////////////////////////////////////////// 	   


u8 *rom_file;
const u8* nes_caption_tbl[GUI_LANGUAGE_NUM]={"NES模拟器","NES模擬器","NES Emulator",};


//加载游戏界面
void nes_load_ui(void)
{	
	app_filebrower((u8*)nes_caption_tbl[gui_phy.language],0X05);//显示标题 
  	gui_fill_rectangle(0,20,240,300,BLACK);				//填充底色 		    	  
}
//频率设置
//PLL,倍频数
void nes_clock_set(u8 PLL)
{
	u8 tPLL=PLL;
	u8 temp=0;	 
	RCC->CFGR&=0XFFFFFFFC;	//修改时钟频率为内部8M	   
	RCC->CR&=~0x01000000;  	//PLLOFF  
 	RCC->CFGR&=~(0XF<<18);	//清空原来的设置
 	PLL-=2;//抵消2个单位
	RCC->CFGR|=PLL<<18;   	//设置PLL值 2~16
	RCC->CFGR|=1<<16;	  	//PLLSRC ON 
	FLASH->ACR|=0x12;	  	//FLASH 2个延时周期
 	RCC->CR|=0x01000000;  	//PLLON
	while(!(RCC->CR>>25));	//等待PLL锁定
	RCC->CFGR|=0x02;		//PLL作为系统时钟	 
	while(temp!=0x02)    	//等待PLL作为系统时钟设置成功
	{   
		temp=RCC->CFGR>>2;
		temp&=0x03;
	}  
 	//顺便设置延时和串口										  
	delay_init(tPLL*8);		//延时初始化
	uart_init(tPLL*8,115200); //串口1初始化   
} 
 
//////////////////////////////////	    		  
   
//加载游戏,开始玩
void load_nes(u8* path)
{
	u8 res;
	u32 len;
	u16 i;
	FIL* f_nes;		 
	u8 *p;
	u16 tpad_temp=system_task_return;
	nes_load_ui();
	f_nes=(FIL *)gui_memin_malloc(sizeof(FIL));	//开辟FIL字节的内存区域 
	if(f_nes==NULL)return;//申请失败
 	res=f_open(f_nes,(const TCHAR*)path,FA_READ);//打开文件夹
   	if(res==0)
	{
		rom_file=gui_memex_malloc(f_nes->fsize);
		if(rom_file)
		{	   
			p=rom_file;
			for(len=0;len<f_nes->fsize;len+=512)
			{
				f_read(f_nes,fatbuf,512,&br);//读取文件
				for(i=0;i<br;i++)//复制到SRAM里面
				{
					*p=fatbuf[i];
					p++;
				}
				if(br!=512)break;  	
			} 
		}else res=1;
		f_close(f_nes);
	}
	gui_memin_free(f_nes);	 
	if(res==0)
	{
 		nes_clock_set(16);			//设置为128M
 		delay_init(128);    
		system_task_return=(system_task_return*16/9)+1;	 
		nes_main();
		system_task_return=tpad_temp;
 		nes_clock_set(9);			//设置为72M  
 		delay_init(72);		 
	} 	
	gui_memex_free(rom_file);	 
} 

//NES游戏
u8 nes_play(void)
{
  	DIR nesdir;		//nesdir专用	  
	FILINFO nesinfo;	 	   
	u8 res;
	u8 rval=0;			//返回值	  
	u8 *pname=0;
	u8 *fn;								   
 
 	_btn_obj* rbtn;		//返回按钮控件
   	_filelistbox_obj * flistbox;
	_filelistbox_list * filelistx; 	//文件

	JOYPAD_Init();
	app_filebrower((u8*)APP_MFUNS_CAPTION_TBL[6][gui_phy.language],0X07);	//选择目标文件,并得到目标数量
   	flistbox=filelistbox_creat(0,APP_FB_TOPBAR_HEIGHT,lcddev.width,lcddev.height-APP_FB_TOPBAR_HEIGHT-APP_FB_BTMBAR_HEIGHT,1,12);	//创建一个filelistbox
	if(flistbox==NULL)rval=1;							//申请内存失败.
	else  
	{
		flistbox->fliter=FLBOX_FLT_NES;	//查找文本文件
 		filelistbox_addlist(flistbox,"SD Card",0);		//磁盘0
//		filelistbox_addlist(flistbox,(u8*)APP_DISK_NAME_TBL[1][gui_phy.language],0);		//磁盘1
		filelistbox_draw_listbox(flistbox);
	} 	 

	//为长文件名申请缓存区
 	nesinfo.lfsize = _MAX_LFN * 2 + 1;
	nesinfo.lfname = gui_memin_malloc(nesinfo.lfsize);
	if(nesinfo.lfname==NULL)rval=1;//申请内存失败 
   	else gui_memset((u8*)nesinfo.lfname,0,nesinfo.lfsize); 
	rbtn=btn_creat(lcddev.width-41,lcddev.height-APP_FB_BTMBAR_HEIGHT,40,19,0,0x03);//创建文字按钮
 	if(rbtn==NULL)rval=1;	//没有足够内存够分配
	else
	{
	 	rbtn->caption=(u8*)GUI_BACK_CAPTION_TBL[gui_phy.language];//返回 
	 	rbtn->font=16;			//字体		 
		rbtn->bcfdcolor=WHITE;	//按下时的颜色
		rbtn->bcfucolor=WHITE;	//松开时的颜色
		btn_draw(rbtn);//画按钮
	}   
   	while(rval==0)
	{
		tp_dev.scan(0);    
		in_obj.get_key(&tp_dev,IN_TYPE_TOUCH);	//得到按键键值   
		delay_ms(5);
		if(system_task_return)break;	  		//TPAD返回
 		filelistbox_check(flistbox,&in_obj);	//扫描文件
		res=btn_check(rbtn,&in_obj);
		if(res)
		{
			if(((rbtn->sta&0X80)==0))//按钮状态改变了
			{
				if(flistbox->dbclick!=0X81)
				{
 					filelistx=filelist_search(flistbox->list,flistbox->selindex);//得到此时选中的list的信息
					if(filelistx->type==FICO_DISK)//已经不能再往上了
					{				 
						break;
					}else filelistbox_back(flistbox);//退回上一层目录	 
				} 
 			}	 
		}
		if(flistbox->dbclick==0X81)//双击文件了
		{
     		rval=f_opendir(&nesdir,(const TCHAR*)flistbox->path); //打开选中的目录
			if(rval)break;	 			   
			dir_sdi(&nesdir,flistbox->findextbl[flistbox->selindex-flistbox->foldercnt]);
	 		rval=f_readdir(&nesdir,&nesinfo);//读取文件信息
			if(rval)break;//打开成功    
			fn=(u8*)(*nesinfo.lfname?nesinfo.lfname:nesinfo.fname);
			pname=gui_memin_malloc(strlen((const char*)fn)+strlen((const char*)flistbox->path)+2);//申请内存
			if(pname==NULL)rval=1;//申请失败
			{
				pname=gui_path_name(pname,flistbox->path,fn);	//文件名加入路径  
 				LCD_Clear(0X0);//黑屏 
 				FSMC_Bank1E->BWTR[1]|=2<<0; 			//地址建立时间（ADDSET）为3个HCLK  	 	 
 				FSMC_Bank1E->BWTR[1]|=3<<8; 		 	//数据保持时间（ADDSET）为4个HCLK  	 	 
 				//SRAM设置
//				FSMC_Bank1->BTCR[0]|=6<<8; 				//数据保持时间（DATAST）为7个HCLK 7/128M=55ns	 	 

 				load_nes(pname);	//开始nes游戏
				LCD_Clear(BLACK);	//清屏
				FSMC_Bank1E->BWTR[1]&=0XFFFF00F0;		//清除之前的设置
				FSMC_Bank1E->BWTR[1]|=0<<0; 			//地址建立时间（ADDSET）为1个HCLK 	 	 
 				FSMC_Bank1E->BWTR[1]|=2<<8;  			//数据保持时间（ADDSET）为3个HCLK 1/36M=27ns	 	 
 				//SRAM设置复原
//				FSMC_Bank1->BTCR[0]&=0XFFFF00FF;		//清除之前的设置
//				FSMC_Bank1->BTCR[0]|=3<<8; 				//数据保持时间（DATAST）为4个HCLK 4/72M=55ns
 			}	       
			flistbox->dbclick=0;	//设置非文件浏览状态
			gui_memin_free(pname);	//释放内存		  
		    pname=NULL;
			app_filebrower("FC模拟器",0X07);	//选择目标文件,并得到目标数量
			btn_draw(rbtn);//画按钮
			delay_ms(100);
			filelistbox_rebuild_filelist(flistbox);//重建flistbox
 			system_task_return=0;//刚刚退出正在玩的游戏,还不能退出这个循环
		}
	}	
	filelistbox_delete(flistbox);	//删除filelist
	btn_delete(rbtn);				//删除按钮	  	 
	gui_memin_free(pname);			//释放内存		  
 	gui_memin_free(nesinfo.lfname);		 			   
	return rval;  								  
}






















