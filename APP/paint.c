#include "paint.h"	    
#include "calendar.h"	    
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//APP-画板 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/7/20
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//*******************************************************************************
//修改信息
//无
////////////////////////////////////////////////////////////////////////////////// 	   

//窗体内嵌字颜色
#define PAINT_INWIN_FONT_COLOR		0X736C		//0XAD53		

//画笔设置
const u8* paint_penset_tbl[GUI_LANGUAGE_NUM]=
{"画笔设置","畫筆設置","PEN SET"};
//模式选择
const u8* paint_mode_tbl[GUI_LANGUAGE_NUM][2]=
{
{"新建画板","打开位图",},
{"新建畫板","打開位圖",},		 
{"Create new file","Open exist file",},
};

//画笔设置选择
const u8* pen_set_tbl[GUI_LANGUAGE_NUM][2]=
{
{"画笔颜色设置","画笔尺寸设置",},
{"畫筆顏色設置","畫筆尺寸設置",},		 
{"Pen color Set","Pen size set",},
};	   

/////////////////////////////////////////////////////////////////////////////////
//通过时间获取文件名
//组合成:形如"0:PAINT/PAINT20120321210633.bmp"的文件名
void paint_new_pathname(u8 *pname)
{	 
	RTC_Get();//更新时间
	sprintf((char*)pname,"0:PAINT/PAINT%04d%02d%02d%02d%02d%02d.bmp",calendar.w_year,calendar.w_month,calendar.w_date,calendar.hour,calendar.min,calendar.sec);
}

//彩条颜色定义
const u16 colorbar_tbl[20]={0XF800,0XFFE0,0X07E0,0X07FF,0X001F,0XF81F,0XFFF0,0X07F0,0X87FF,0X841F,
                            0XF810,0XFC08,0X8200,0X081F,0X0210,0X041F,0X8010,0X8408,0X0000,0XFFFF};
//显示彩条(宽度固定:100=20*5,边框占1个像素,总共尺寸是:(100+2)*(height+2))
//sx,sy:彩条的起始坐标
//height:高度值 
//20个颜色值
void paint_show_colorbar(u16 sx,u16 sy,u8 height)
{ 
	u16 i;
	if(sx==0||sy==0)return;
	gui_draw_rectangle(sx-1,sy-1,100+2,height+2,0X8452);//画预览边框
	for(i=0;i<20;i++)
	{
		gui_fill_rectangle(sx+5*i,sy,5,height,colorbar_tbl[i]);	//画预览图
	}
}		

//尺寸固定:150*96
//x,y:左上角坐标
//color:画笔颜色指针
//caption:窗口名字				  
//返回值:0,成功设置;
//    其他,不进行设置
u8 paint_pen_color_set(u16 x,u16 y,u16* color,u8*caption)
{
 	u8 rval=0,res;
  	_window_obj* twin=0;			//窗体
 	_btn_obj * rbtn=0;				//取消按钮		  
 	_btn_obj * okbtn=0;				//确定按钮
 	u16 tempcolor=*color;
	u8 cmask=1;						//颜色是否有变动
	
   	twin=window_creat(x,y,150,96,0,1|1<<5,16);		//创建窗口 　 
  	okbtn=btn_creat(x+15,y+66,50,24,0,0x02);		//创建按钮
 	rbtn=btn_creat(x+15+50+20,y+66,50,24,0,0x02);	//创建按钮
	if(twin==NULL||rbtn==NULL||okbtn==NULL||rval)rval=1;
	else
	{
		//窗口的名字和背景色
		twin->caption=caption;
		twin->captionheight=24;		//修改caption高度为24
		twin->windowbkc=APP_WIN_BACK_COLOR;	
		//返回按钮的颜色    
		rbtn->bkctbl[0]=0X8452;		//边框颜色
		rbtn->bkctbl[1]=0XAD97;		//第一行的颜色				
		rbtn->bkctbl[2]=0XAD97;		//上半部分颜色
		rbtn->bkctbl[3]=0X8452;		//下半部分颜色
		okbtn->bkctbl[0]=0X8452;	//边框颜色
		okbtn->bkctbl[1]=0XAD97;	//第一行的颜色				
		okbtn->bkctbl[2]=0XAD97;	//上半部分颜色
		okbtn->bkctbl[3]=0X8452;	//下半部分颜色

		rbtn->caption=(u8*)GUI_CANCEL_CAPTION_TBL[gui_phy.language];	//标题为取消
 		okbtn->caption=(u8*)GUI_OK_CAPTION_TBL[gui_phy.language];		//标题为确定
  		window_draw(twin);				//画出窗体
		btn_draw(rbtn);					//画按钮
		btn_draw(okbtn);				//画按钮
		paint_show_colorbar(x+45,y+32,25);//画彩色条
		while(rval==0)
		{
			tp_dev.scan(0);    
			in_obj.get_key(&tp_dev,IN_TYPE_TOUCH);	//得到按键键值   
			delay_ms(1000/OS_TICKS_PER_SEC);		//延时一个时钟节拍
			if(system_task_return)break;
			res=btn_check(rbtn,&in_obj);			//取消按钮检测
			if(res&&((rbtn->sta&0X80)==0))			//有有效操作
			{
				rval=1;
				break;//退出
			}
			res=btn_check(okbtn,&in_obj); 			//确认按钮检测
			if(res&&((okbtn->sta&0X80)==0))			//有有效操作
			{
				rval=0XFF;		   
				break;//确认了
			}
			if(cmask) //检测数据是否需要更新
			{ 
				cmask=0; 
				gui_fill_rectangle(x+5,y+30,30,30,tempcolor);	//画预览图
 			}	   
			if(app_tp_is_in_area(&tp_dev,x+45,y+32,99,24)&&(tp_dev.sta&TP_PRES_DOWN))
			{
				tempcolor=gui_phy.read_point(tp_dev.x[0],tp_dev.y[0]);	
				cmask=1;
			}	
 		}
	}
	window_delete(twin);							//删除窗口
	btn_delete(rbtn);								//删除按钮	
 	btn_delete(okbtn);								//删除按钮	
	if(rval==0XFF)
	{
		*color=tempcolor;
		return 0;
 	}
	system_task_return=0;
	return rval;
}
//x,y:左上角坐标
//color:画笔颜色指针
//*mode:画笔大小
//caption:窗口名字				  
//返回值:0,成功设置;
//    其他,不进行设置
u8 paint_pen_size_set(u16 x,u16 y,u16 color,u8 *mode,u8*caption)
{
 	u8 rval=0,res;
   	_window_obj* twin=0;			//窗体
 	_btn_obj * rbtn=0;				//取消按钮		  
 	_btn_obj * okbtn=0;				//确定按钮		  
	_progressbar_obj * sizeprgb;	//尺寸设置进度条
 	u8 smask=1;						//尺寸是否有变动
	u8 tempsize=*mode;

  	twin=window_creat(x,y,150,96,0,1|1<<5,16);			//创建窗口 　
 	sizeprgb=progressbar_creat(x+36,y+35,100,20,0X20);	//创建进度条
	if(sizeprgb==NULL)rval=1;
  	okbtn=btn_creat(x+15,y+66,50,24,0,0x02);		//创建按钮
 	rbtn=btn_creat(x+15+50+20,y+66,50,24,0,0x02);	//创建按钮
	if(twin==NULL||rbtn==NULL||okbtn==NULL||rval)rval=1;
	else
	{
		//窗口的名字和背景色
		twin->caption=caption;
		twin->captionheight=24;		//修改caption高度为24
		twin->windowbkc=APP_WIN_BACK_COLOR;	
		//返回按钮的颜色    
		rbtn->bkctbl[0]=0X8452;		//边框颜色
		rbtn->bkctbl[1]=0XAD97;		//第一行的颜色				
		rbtn->bkctbl[2]=0XAD97;		//上半部分颜色
		rbtn->bkctbl[3]=0X8452;		//下半部分颜色
		okbtn->bkctbl[0]=0X8452;	//边框颜色
		okbtn->bkctbl[1]=0XAD97;	//第一行的颜色				
		okbtn->bkctbl[2]=0XAD97;	//上半部分颜色
		okbtn->bkctbl[3]=0X8452;	//下半部分颜色

		sizeprgb->totallen=10;		//最大半径为10 
	   	sizeprgb->curpos=tempsize;	//当前尺寸

		rbtn->caption=(u8*)GUI_CANCEL_CAPTION_TBL[gui_phy.language];	//标题为取消
 		okbtn->caption=(u8*)GUI_OK_CAPTION_TBL[gui_phy.language];		//标题为确定
  		window_draw(twin);				//画出窗体
		btn_draw(rbtn);					//画按钮
		btn_draw(okbtn);				//画按钮 
		while(rval==0)
		{
			tp_dev.scan(0);    
			in_obj.get_key(&tp_dev,IN_TYPE_TOUCH);	//得到按键键值   
			delay_ms(1000/OS_TICKS_PER_SEC);		//延时一个时钟节拍
			if(system_task_return)break;
			res=btn_check(rbtn,&in_obj);			//取消按钮检测
			if(res&&((rbtn->sta&0X80)==0))			//有有效操作
			{
				rval=1;
				break;//退出
			}
			res=btn_check(okbtn,&in_obj); 			//确认按钮检测
			if(res&&((okbtn->sta&0X80)==0))			//有有效操作
			{
				rval=0XFF;		   
				break;//确认了
			}
			if(smask) //检测数据是否需要更新
			{
				if(smask==1)//进度条需要更新.
				{
 	 				progressbar_draw_progressbar(sizeprgb);	//更新进度条
 				}
				smask=0;
  				gui_phy.back_color=APP_WIN_BACK_COLOR;		//设置背景色 
				gui_fill_rectangle(x+6,y+32,24,24,gui_phy.back_color);//填充预览区域	 
				paint_draw_point(x+18,y+32+12,color,tempsize);	//画预览图
 			}	  
			res=progressbar_check(sizeprgb,&in_obj);
 			if(res&&(tempsize!=sizeprgb->curpos))	//进度条改动了
			{
				tempsize=sizeprgb->curpos;	 		//保存最新的结果  
				smask=2;//进度条不更新,但是数据要更新
			} 	
 		}
	}
	window_delete(twin);							//删除窗口
	btn_delete(rbtn);								//删除按钮	
	btn_delete(okbtn);								//删除按钮	
	progressbar_delete(sizeprgb);//删除进度条  
	if(rval==0XFF)
	{
		*mode=tempsize;
		return 0;
 	}
	system_task_return=0;
	return rval;
} 
//x,y:坐标
//color:颜色
//mode:
//[7:4]:保留
//[3:0]:尺寸.(圆形,即半径)
void paint_draw_point(u16 x,u16 y,u16 color,u8 mode)
{
	u8 size=mode&0X0F;//得到尺寸大小	    
	if(size==0)gui_phy.draw_point(x,y,color);
	else gui_fill_circle(x,y,size,color);		   
}
	  
//画图功能
//所有画图文件,均保存在SD卡PAINT文件夹内.
u8 paint_play(void)
{
 	DIR paintdir;					//paintdir专用	  
	FILINFO paintinfo;		 	   
  	_btn_obj* rbtn=0;				//返回按钮控件
   	_filelistbox_obj * flistbox=0;
	_filelistbox_list * filelistx=0;//文件表
 	u16 lastpos[5][2];				//最后一次的数据 
	
 	u16 pencolor=RED;
   	u8 *pname=0;
	u8 *fn;	 
	u8 res;
	u8 rval=0;
   	u8 key=0;
	u8 mode=0X02;					//画图模式				 
									//[7:4]:保留
									//[3:0]:点半径
	u8 editmask=0;					//改动标志位
  	u16 *pset_bkctbl=0;				//设置时背景色指针 
  	br=0;
	//先选择模式                                                                                                
	res=app_items_sel((lcddev.width-180)/2,(lcddev.height-152)/2,180,72+40*2,(u8**)paint_mode_tbl[gui_phy.language],2,(u8*)&br,0X90,(u8*)APP_MODESEL_CAPTION_TBL[gui_phy.language]);//2个选择
 	if(res==0)//确认键按下了,同时按键正常创建
	{	   
		LCD_Clear(0XFFFF);				//清屏
 		if(br==0)						//新建图片文件
		{
 			if(gui_phy.memdevflag&(1<<0))f_mkdir("0:PAINT");		//强制创建文件夹,给画图用
			pname=gui_memin_malloc(40);	//申请40个字节内存,类似"0:PAINT/PAINT20120321210633.bmp"
			pname[0]='\0';//添加结束符
			paint_new_pathname(pname);
			if(pname==NULL)rval=1;
 		}else	//选择一个BMP文件打开
		{
			app_gui_tcbar(0,lcddev.height-gui_phy.tbheight,lcddev.width,gui_phy.tbheight,0x01);	//上分界线
			rbtn=btn_creat(lcddev.width-2*gui_phy.tbfsize-8-1,lcddev.height-gui_phy.tbheight,2*gui_phy.tbfsize+8,gui_phy.tbheight-1,0,0x03);//创建文字按钮
			if(rbtn==NULL)rval=1;	//没有足够内存够分配
			else
			{																				
				rbtn->caption=(u8*)GUI_BACK_CAPTION_TBL[gui_phy.language];//返回 
				rbtn->font=16;			//字体		 
				rbtn->bcfdcolor=WHITE;	//按下时的颜色
				rbtn->bcfucolor=WHITE;	//松开时的颜色
				btn_draw(rbtn);			//重画按钮
			}
			app_filebrower((u8*)paint_mode_tbl[gui_phy.language][1],0X07);//显示信息
			btn_draw(rbtn);//画按钮
			flistbox=filelistbox_creat(0,gui_phy.tbheight,lcddev.width,lcddev.height-gui_phy.tbheight*2,1,gui_phy.listfsize);//创建一个filelistbox
			if(flistbox==NULL)rval=1;							//申请内存失败.
			else  
			{
				flistbox->fliter=FLBOX_FLT_PICTURE;	//查找图片文件
				flistbox->fliterex=1<<0;			//仅仅查找位图文件
				filelistbox_add_disk(flistbox);		//添加磁盘路径
				filelistbox_draw_listbox(flistbox);
			} 	 
 			//为长文件名申请缓存区
		 	paintinfo.lfsize = 120;
			paintinfo.lfname = gui_memin_malloc(paintinfo.lfsize);
			if(paintinfo.lfname==NULL)rval=1;//申请内存失败 
		   	else gui_memset((u8*)paintinfo.lfname,0,paintinfo.lfsize);
			for(res=0;res<5;res++)lastpos[res][0]=0XFFFF; //坐标设定
		   	while(rval==0)
			{
				tp_dev.scan(0);    
				in_obj.get_key(&tp_dev,IN_TYPE_TOUCH);	//得到按键键值   
				delay_ms(1000/OS_TICKS_PER_SEC);		//延时一个时钟节拍
    			if(system_task_return)				   	//需要返回
				{
					rval=1;
					break;			
				}
				filelistbox_check(flistbox,&in_obj);	//扫描文件
				res=btn_check(rbtn,&in_obj);
				if(res)
				{
					if(((rbtn->sta&0X80)==0))//按钮状态改变了
					{
						if(flistbox->dbclick!=0X81)
						{
		 					filelistx=filelist_search(flistbox->list,flistbox->selindex);//得到此时选中的list的信息
							if(filelistx->type==FICO_DISK)
							{
								rval=1;
								break;	//已经不能再往上了
						 	}
							else filelistbox_back(flistbox);	//退回上一层目录	 
						} 
		 			}	 
				}
				if(flistbox->dbclick==0X81)//双击文件了
				{	    
 		     		rval=f_opendir(&paintdir,(const TCHAR*)flistbox->path); //打开选中的目录
					if(rval)break;	 			   
					dir_sdi(&paintdir,flistbox->findextbl[flistbox->selindex-flistbox->foldercnt]);
			 		rval=f_readdir(&paintdir,&paintinfo);//读取文件信息
					if(rval)break;//打开成功    
					fn=(u8*)(*paintinfo.lfname?paintinfo.lfname:paintinfo.fname);
					pname=gui_memin_malloc(strlen((const char*)fn)+strlen((const char*)flistbox->path)+2);//申请内存
					if(pname==NULL)rval=1;//申请失败
					{
						pname=gui_path_name(pname,flistbox->path,fn);	//文件名加入路径  
						LCD_Clear(WHITE);
						ai_load_picfile(pname,0,0,lcddev.width,lcddev.height,1);//解码这个图片
    					break;
					} 
				}
			}
			filelistbox_delete(flistbox);		//清除filelistbox节省内存
			gui_memin_free(paintinfo.lfname);	//清除长文件名缓存内存 
		}
		pset_bkctbl=gui_memex_malloc(180*80*2);//申请内存
		if(pset_bkctbl==NULL)rval=1;
   	 	while(rval==0) 
		{		 
			tp_dev.scan(0); 		 
			in_obj.get_key(&tp_dev,IN_TYPE_TOUCH);	//得到按键键值 	  
    		if(system_task_return)break;			//需要返回	 
 			key=KEY_Scan(0);					//扫描按键
			if(key>=2)							//KEY1/WK_UP 按下,颜色设置
			{
				app_read_bkcolor((lcddev.width-150)/2,(lcddev.height-96)/2,150,96,pset_bkctbl);//读取背景色
				if(key==WKUP_PRES)paint_pen_color_set((lcddev.width-150)/2,(lcddev.height-96)/2,&pencolor,(u8*)pen_set_tbl[gui_phy.language][0]);	//KEY1按下
 				else paint_pen_size_set((lcddev.width-150)/2,(lcddev.height-96)/2,pencolor,&mode,(u8*)pen_set_tbl[gui_phy.language][1]);	//WK_UP按下
				app_recover_bkcolor((lcddev.width-150)/2,(lcddev.height-96)/2,150,96,pset_bkctbl);//恢复背景色
			} 
			//电阻屏 
			if(tp_dev.sta&TP_PRES_DOWN)			
			{	
				editmask=1;
				paint_draw_point(tp_dev.x[0],tp_dev.y[0],pencolor,mode);	//电阻屏	
			}else delay_ms(1000/OS_TICKS_PER_SEC);	//没有按键按下的时候 延时一个时钟节拍	 
			//电容屏
			if(tp_dev.touchtype&0X80)		//电容屏
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
							gui_draw_bline(lastpos[res][0],lastpos[res][1],tp_dev.x[res],tp_dev.y[res],mode&0X0F,pencolor);//画线
							lastpos[res][0]=tp_dev.x[res];
							lastpos[res][1]=tp_dev.y[res];  
						}
					}else lastpos[res][0]=0XFFFF;	                                                                                                                 
				} 
				if((tp_dev.sta&0X1F)==0)delay_ms(1000/OS_TICKS_PER_SEC);//没有按键按下的时候 延时一个时钟节拍	 
			} 
		}
 	}	 
	if(editmask)//有编辑过,需要保存
	{	   
		app_read_bkcolor((lcddev.width-180)/2,(lcddev.height-80)/2,180,80,pset_bkctbl);		//读取背景色
 		res=window_msg_box((lcddev.width-180)/2,(lcddev.height-80)/2,180,80,"",(u8*)APP_SAVE_CAPTION_TBL[gui_phy.language],12,0,0X03,0);
		app_recover_bkcolor((lcddev.width-180)/2,(lcddev.height-80)/2,180,80,pset_bkctbl);	//恢复背景色
 		if(res==1)//需要保存
		{
			if(bmp_encode(pname,0,0,lcddev.width,lcddev.height,1))//编码BMP文件
			{
				window_msg_box((lcddev.width-200)/2,(lcddev.height-100)/2,200,100,(u8*)APP_CREAT_ERR_MSG_TBL[gui_phy.language],(u8*)APP_REMIND_CAPTION_TBL[gui_phy.language],12,0,0,0);//提示SD卡是否存在
				delay_ms(2000);//等待2秒钟
		 	}
		}
	} 
	btn_delete(rbtn);					//删除按钮
   	gui_memex_free(pset_bkctbl);
  	gui_memin_free(pname);
	return rval;

}








