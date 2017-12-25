#include "includes.h"		
#include "common.h"	   	   
#include "mainui.h"	  	   
#include "ebook.h"	  	   
#include "picviewer.h"  
#include "appplay.h"
#include "usbplay.h"
#include "calendar.h"
#include "settings.h"
#include "paint.h"
#include "wirelessplay.h"
#include "notepad.h"	
#include "mass_mal.h"
#include "usb_lib.h"
#include "hw_config.h"
#include "usb_pwr.h"
#include "memory.h"	    
#include "usb_bot.h"  
#include "nesplay.h"
//ALIENTEK MiniSTM32开发板实验38
//综合实验
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司  
 
/////////////////////////UCOSII任务设置///////////////////////////////////
//START 任务
//设置任务优先级
#define START_TASK_PRIO      			10 //开始任务的优先级设置为最低
//设置任务堆栈大小
#define START_STK_SIZE  				64
//任务堆栈，8字节对齐	
__align(8) static OS_STK START_TASK_STK[START_STK_SIZE];
//任务函数
void start_task(void *pdata);	
 			   
//串口任务
//设置任务优先级
#define USART_TASK_PRIO       			7 
//设置任务堆栈大小
#define USART_STK_SIZE  		    	64
//任务堆栈，8字节对齐	
__align(8) static OS_STK USART_TASK_STK[USART_STK_SIZE];
//任务函数
void usart_task(void *pdata);
							 
//主任务
//设置任务优先级
#define MAIN_TASK_PRIO       			6 
//设置任务堆栈大小
#define MAIN_STK_SIZE  					256
//任务堆栈，8字节对齐	
__align(8) static OS_STK MAIN_TASK_STK[MAIN_STK_SIZE];
//任务函数
void main_task(void *pdata);

//串口监视任务
//设置任务优先级
#define WATCH_TASK_PRIO       			3 
//设置任务堆栈大小
#define WATCH_STK_SIZE  		   		128 
//任务堆栈，8字节对齐	
__align(8) static OS_STK WATCH_TASK_STK[WATCH_STK_SIZE];
//任务函数
void watch_task(void *pdata);

//////////////////////////////////////////////////////////////////////////////	 

//显示错误信息
//x,y:坐标
//fsize:字体大小
//x,y:坐标.err:错误信息
void system_error_show(u16 x,u16 y,u8*err,u8 fsize)
{
	POINT_COLOR=RED;
 	while(1)
	{
		LCD_ShowString(x,y,lcddev.width,lcddev.height,fsize,err);
		delay_ms(400);
		LCD_Fill(x,y,lcddev.width,y+fsize,BLACK);
		delay_ms(100);
		LED0=!LED0;
	} 
}	
//擦除整个SPI FLASH(即所有资源都删除),以快速更新系统.
//x,y:坐标
//fsize:字体大小
//x,y:坐标.err:错误信息
//返回值:0,没有擦除;1,擦除了
u8 system_files_erase(u16 x,u16 y,u8 fsize)
{
	u8 key;
	u8 t=0;
	POINT_COLOR=RED;
	LCD_ShowString(x,y,lcddev.width,lcddev.height,fsize,"Erase all system files?");
	while(1)
	{
		t++;
		if(t==20)LCD_ShowString(x,y+fsize,lcddev.width,lcddev.height,fsize,"KEY0:NO / KEY1:YES");
		if(t==40)
		{
			gui_fill_rectangle(x,y+fsize,lcddev.width,fsize,BLACK);//清除显示
			t=0;
			LED0=!LED0;
		}
		key=KEY_Scan(0);
		if(key==KEY0_PRES)//不擦除,用户取消了
		{ 
			gui_fill_rectangle(x,y,lcddev.width,fsize*2,BLACK);//清除显示
			POINT_COLOR=WHITE;
			LED0=1;
			return 0;
		}
		if(key==KEY1_PRES)//要擦除,要重新来过
		{
			LED0=1;
			LCD_ShowString(x,y+fsize,lcddev.width,lcddev.height,fsize,"Erasing SPI FLASH...");
			SPI_Flash_Erase_Chip();
			LCD_ShowString(x,y+fsize,lcddev.width,lcddev.height,fsize,"Erasing SPI FLASH OK");
			delay_ms(600);
			return 1;
		}
		delay_ms(10);
	}
}
//字库更新确认提示.
//x,y:坐标
//fsize:字体大小 
//返回值:0,不需要更新;1,确认要更新
u8 system_font_update_confirm(u16 x,u16 y,u8 fsize)
{
	u8 key;
	u8 t=0;
	u8 res=0;
	POINT_COLOR=RED;
	LCD_ShowString(x,y,lcddev.width,lcddev.height,fsize,"Update font?");
	while(1)
	{
		t++;
		if(t==20)LCD_ShowString(x,y+fsize,lcddev.width,lcddev.height,fsize,"KEY0:YES / KEY1:NO");
		if(t==40)
		{
			gui_fill_rectangle(x,y+fsize,lcddev.width,fsize,BLACK);//清除显示
			t=0;
			LED0=!LED0;
		}
		key=KEY_Scan(0);
		if(key==KEY1_PRES)break;//不更新 
		if(key==KEY0_PRES){res=1;break;}//要更新 
		delay_ms(10);
	}
	LED0=1;
	gui_fill_rectangle(x,y,lcddev.width,fsize*2,BLACK);//清除显示
	POINT_COLOR=WHITE;
	return res;
}

extern u8 Max_Lun;	//支持的磁盘个数,0表示1个,1表示2个.
//USB更新SPI FLASH确认.
//x,y:坐标
//fsize:字体大小 
//返回值:0,不需要更新;1,确认更新
u8 system_usb_update_confirm(u16 x,u16 y,u8 fsize)
{
	u8 key;
	u8 t=0;
	u8 res=0;
 	u8 offline_cnt=0;
	u8 Divece_STA=0;
 	u8 USB_STA=0;
	POINT_COLOR=RED;
	LCD_ShowString(x,y,lcddev.width,lcddev.height,fsize,"Use USB Update SPI FLASH Files?");
	while(1)
	{
		t++;
		if(t==20)LCD_ShowString(x,y+fsize,lcddev.width,lcddev.height,fsize,"KEY0:NO / KEY1:YES");
		if(t==40)
		{
			gui_fill_rectangle(x,y+fsize,lcddev.width,fsize,BLACK);//清除显示
			t=0;
			LED0=!LED0;
		}
		delay_ms(10);
		key=KEY_Scan(0);
		if(key==KEY0_PRES)break;//不更新 
		if(key==KEY1_PRES){res=1;break;}//确定更新
	}
	gui_fill_rectangle(x,y,lcddev.width,fsize*2,BLACK);//清除显示
	if(res)
	{
		//先擦除SPI FLASH
		LCD_ShowString(x,y,lcddev.width,lcddev.height,fsize,"Erasing SPI FLASH...");
		SPI_Flash_Erase_Chip();
		//再格式化SPI FLASH
		LCD_ShowString(x,y,lcddev.width,lcddev.height,fsize,"Formating SPI FLASH...");
		res=f_mkfs("1:",1,4096);//格式化FLASH,1,盘符;1,不需要引导区,8个扇区为1个簇
		if(res==0)
		{
			f_setlabel((const TCHAR *)"1:ALIENTEK");				//设置Flash磁盘的名字为：ALIENTEK
		}
		LCD_ShowString(x,y,lcddev.width,lcddev.height,fsize,"Please push USB cable to USB_SLAVE");
		Data_Buffer=(u32*)gui_memin_malloc(BULK_MAX_PACKET_SIZE*2*4);	//为USB通讯申请内存
		Bulk_Data_Buff=gui_memin_malloc(BULK_MAX_PACKET_SIZE);			//为USB通讯申请内存
		//USB配置
		USB_Port_Set(0); 	//USB先断开
		delay_ms(700);  
		USB_Port_Set(1);	//USB再次连接   
		Max_Lun=0;			//仅一个磁盘. 	  
	 	//SPI FLASH部分 
 		Mass_Memory_Size[0]=9832*512;	//前4.8M字节用作FLASH U盘.
	  Mass_Block_Size[0] =512;		//因为我们在Init里面设置了SD卡的操作字节为512个,所以这里一定是512个字节.
	  Mass_Block_Count[0]=Mass_Memory_Size[0]/Mass_Block_Size[0];		
		USB_Interrupts_Config();    
		Set_USBClock();   
		USB_Init();
		t=0;
		while(1)
		{
			t++;
			delay_ms(5);
			if(t==40)//200ms时间到了 
			{
				t=0;
				if(USB_STATUS_REG&0x10)
				{
					offline_cnt=0;//USB连接了,则清除offline计数器
					bDeviceState=CONFIGURED;
				}else//没有得到轮询 
				{
					offline_cnt++;  
					if(offline_cnt>10)bDeviceState=UNCONNECTED;//2s内没收到在线标记,代表USB被拔出了
				}
				USB_STATUS_REG=0;
				LED0=!LED0;
			}			
			if(USB_STA!=USB_STATUS_REG)//状态改变了 
			{	 						   
				gui_fill_rectangle(x,y+fsize*4,lcddev.width,fsize,BLACK);//清除显示 
				if(USB_STATUS_REG&0x01)//正在写		  
				{
					LCD_ShowString(x,y+fsize*4,lcddev.width,lcddev.height,fsize,"USB Writing..."); 
					USB_STATUS_REG&=0xFD;//清除读标记
				}
				USB_STA=USB_STATUS_REG;//记录最后的状态
			}
			if(Divece_STA!=bDeviceState)   
			{                                                                                        
				if(bDeviceState==CONFIGURED)
				{
					LCD_ShowString(x,y+fsize,lcddev.width,lcddev.height,fsize,"USB Connected   ");
					LCD_ShowString(x,y+fsize*2,lcddev.width,lcddev.height,fsize,"Copy SYSTEM folder to ALIENTEK Disk");
					LCD_ShowString(x,y+fsize*3,lcddev.width,lcddev.height,fsize,"When File Copy finished Press KEY0!");
				}else
				{
					LCD_ShowString(x,y+fsize,lcddev.width,lcddev.height,fsize,"USB DisConnected ");//提示USB被拔出了
					gui_fill_rectangle(x,y+fsize*2,lcddev.width,fsize*3,BLACK);//清除显示 
				}
				Divece_STA=bDeviceState;
			} 
			if(bDeviceState==CONFIGURED)//连接状态下,才可以退出
			{
				key=KEY_Scan(0);
				if(key==KEY0_PRES){res=1;break;}
			}
		}
		USB_Port_Set(0); 				//USB先断开
		gui_memin_free(Data_Buffer);	//释放申请到的内存
		gui_memin_free(Bulk_Data_Buff);	//释放申请到的内存
	} 
	LED0=1;
	gui_fill_rectangle(x,y,lcddev.width,fsize*2,BLACK);//清除显示
	POINT_COLOR=WHITE;
	return res;
}

//系统初始化
void system_init(void)
{
 	u16 okoffset=162; 
 	u16 ypos=0;
	u16 j=0;
	u16 temp=0;
	u8 res;
	u32 dtsize,dfsize;
	u8 *stastr=0;
	u8 *version=0; 
	u8 verbuf[12];
	u8 fsize;
	u8 icowidth;
	
 	Stm32_Clock_Init(9);	//系统时钟设置
  delay_init(72);			//延时初始化
 	uart_init(72,115200); 	//串口1初始化
	usart2_init(36,115200);  //串口3初始化
	usart3_init(36,115200);  //串口3初始化
	LCD_Init();				//LCD初始化 
 	LED_Init();	    		//LED初始化
 	KEY_Init();				//按键初始化 
	gui_init();		
	Adc_Init();			  	//ADC初始化,内部温度传感器 
	SPI_Flash_Init();		//W25Qxx初始化												  
 	usmart_dev.init(72);    
  mem_init();				//内存池初始化
	version=mymalloc(31);	//申请31个字节内存
	exfuns_init();
REINIT://重新初始化
	LCD_Clear(BLACK);		//黑屏
	POINT_COLOR=WHITE;
	BACK_COLOR=BLACK;
	j=0;   
/////////////////////////////////////////////////////////////////////////
//显示版权信息
	ypos=2;
	ypos=2;
	if(lcddev.width==240)
	{
		fsize=12;
		icowidth=18;
		okoffset=190;
		app_show_mono_icos(5,ypos,icowidth,24,(u8*)APP_ALIENTEK_ICO1824,YELLOW,BLACK);
	}else if(lcddev.width==320)
	{
		fsize=16;
		icowidth=24;
		okoffset=250;
		app_show_mono_icos(5,ypos,icowidth,32,(u8*)APP_ALIENTEK_ICO2432,YELLOW,BLACK);		
	}else if(lcddev.width==480)
	{
		fsize=24;
		icowidth=36;
		okoffset=370;
		app_show_mono_icos(5,ypos,icowidth,48,(u8*)APP_ALIENTEK_ICO3648,YELLOW,BLACK);		
	}
	LCD_ShowString(icowidth+5*2,ypos+fsize*j++,lcddev.width,lcddev.height,fsize, "ALIENTEK MiniSTM32F1 V3");
	LCD_ShowString(icowidth+5*2,ypos+fsize*j++,lcddev.width,lcddev.height,fsize,"Copyright (C) 2015-2025");    
	app_get_version(verbuf,HARDWARE_VERSION,2);
	strcpy((char*)version,"HARDWARE:");
	strcat((char*)version,(const char*)verbuf);
	strcat((char*)version,", SOFTWARE:");
	app_get_version(verbuf,SOFTWARE_VERSION,3);
	strcat((char*)version,(const char*)verbuf);
	LCD_ShowString(5,ypos+fsize*j++,lcddev.width,lcddev.height,fsize,version);
	sprintf((char*)verbuf,"LCD ID:%04X",lcddev.id);		//LCD ID打印到verbuf里面
	LCD_ShowString(5,ypos+fsize*j++,lcddev.width,lcddev.height,fsize, verbuf);	//显示LCD ID 
//////////////////////////////////////////////////////////////////////////
//开始硬件检测初始化
	LED0=0;LED1=0;				//同时点亮两个LED
	LCD_ShowString(5,ypos+fsize*j++,lcddev.width,lcddev.height,fsize, "CPU:STM32F103VET6 72Mhz");
	LCD_ShowString(5,ypos+fsize*j++,lcddev.width,lcddev.height,fsize, "FLASH:512KB  SRAM:64KB");	
	LED0=1;LED1=1;				//同时关闭两个LED  
	if(SPI_Flash_ReadID()!=W25Q64)//检测不到W25Q64
	{	 
		system_error_show(5,ypos+fsize*j++,"Ex Flash Error!!",fsize); 
	}else temp=8*1024;			//8M字节大小
	LCD_ShowString(5,ypos+fsize*j,lcddev.width,lcddev.height,fsize, "Ex Flash:     KB");			   
	LCD_ShowxNum(5+9*(fsize/2),ypos+fsize*j,temp,5,fsize,0);//显示flash大小  
	LCD_ShowString(5+okoffset,ypos+fsize*j++,lcddev.width,lcddev.height,fsize, "OK");   
	//检测是否需要擦除SPI FLASH?
	res=KEY_Scan(1);//
	if(res==WKUP_PRES)
	{
		res=system_files_erase(5,ypos+fsize*j,fsize);
		if(res)goto REINIT; 
	}
	//检查SPI FLASH的文件系统
	LCD_ShowString(5,ypos+fsize*j,lcddev.width,lcddev.height,fsize, "FATFS Check...");//FATFS检测			   
  	f_mount(fs[0],"0:",1); 		//挂载SD卡  
  	f_mount(fs[1],"1:",1); 		//挂载挂载FLASH. 
 	LCD_ShowString(5+okoffset,ypos+fsize*j++,lcddev.width,lcddev.height,fsize, "OK");			   
	//SD卡检测
	LCD_ShowString(5,ypos+fsize*j,lcddev.width,lcddev.height,fsize, "SD Card:     MB");//SD卡检测
	temp=0;	
 	do
	{
		temp++;
 		res=exf_getfree("0:",&dtsize,&dfsize);//得到SD卡剩余容量和总容量
		delay_ms(200);		   
	}while(res&&temp<5);//连续检测5次
 	if(res==0)//得到容量正常
	{
		gui_phy.memdevflag|=1<<0;	//设置SD卡在位.
		temp=dtsize>>10;//单位转换为MB
		stastr="OK";
 	}else 
	{
 		temp=0;//出错了,单位为0
		stastr="ERROR";
	}
 	LCD_ShowxNum(5+8*(fsize/2),ypos+fsize*j,temp,5,fsize,0);					//显示SD卡容量大小
	LCD_ShowString(5+okoffset,ypos+fsize*j++,lcddev.width,lcddev.height,fsize,stastr);	//SD卡状态			   
	//W25Q64检测,如果不存在文件系统,则先创建.
	temp=0;	
 	do
	{
		temp++;
 		res=exf_getfree("1:",&dtsize,&dfsize);//得到FLASH剩余容量和总容量
		delay_ms(200);		   
	}while(res&&temp<20);//连续检测20次		  
	if(res==0X0D)//文件系统不存在
	{
		LCD_ShowString(5,ypos+fsize*j,lcddev.width,lcddev.height,fsize, "Flash Disk Formatting...");	//格式化FLASH
		res=f_mkfs("1:",1,4096);//格式化FLASH,1,盘符;1,不需要引导区,8个扇区为1个簇
		if(res==0)
		{
			f_setlabel((const TCHAR *)"1:ALIENTEK");				//设置Flash磁盘的名字为：ALIENTEK
			LCD_ShowString(5+okoffset,ypos+fsize*j++,lcddev.width,lcddev.height,fsize, "OK");//标志格式化成功
 			res=exf_getfree("1:",&dtsize,&dfsize);//重新获取容量
		}
	}   
	if(res==0)//得到FLASH卡剩余容量和总容量
	{
		gui_phy.memdevflag|=1<<1;	//设置SPI FLASH在位.
		LCD_ShowString(5,ypos+fsize*j,lcddev.width,lcddev.height,fsize, "Flash Disk:     KB");//FATFS检测			   
		temp=dtsize; 	   
 	}else system_error_show(5,ypos+fsize*(j+1),"Flash Fat Error!",fsize);	//flash 文件系统错误 
 	LCD_ShowxNum(5+11*(fsize/2),ypos+fsize*j,temp,5,fsize,0);						//显示FLASH容量大小
	LCD_ShowString(5+okoffset,ypos+fsize*j++,lcddev.width,lcddev.height,fsize,"OK");//FLASH卡状态	
    //RTC检测
  	LCD_ShowString(5,ypos+fsize*j,lcddev.width,lcddev.height,fsize, "RTC Check...");			   
 	if(RTC_Init())system_error_show(5,ypos+fsize*(j+1),"RTC Error!",fsize);//RTC检测
	else 
	{ 
		RTC_Get();//获取时间
		LCD_ShowString(5+okoffset,ypos+fsize*j++,lcddev.width,lcddev.height,fsize, "OK");			   
	}
	//24C02检测
 //  	LCD_ShowString(5,ypos+fsize*j,lcddev.width,lcddev.height,fsize, "24C02 Check...");			   
 //	if(AT24CXX_Check())system_error_show(5,ypos+fsize*(j+1),"24C02 Error!",fsize);//24C02检测
//	else LCD_ShowString(5+okoffset,ypos+fsize*j++,lcddev.width,lcddev.height,fsize, "OK");  
  	//字库检测									    
   	LCD_ShowString(5,ypos+fsize*j,lcddev.width,lcddev.height,fsize, "Font Check...");
	res=KEY_Scan(1);//检测按键	
	if(res==KEY1_PRES)//更新？确认
	{
		res=system_font_update_confirm(5,ypos+fsize*(j+1),fsize);
	}else res=0;
  	while(font_init()||res==1)	//检测字体,如果字体不存在/按下KEY0确认更新,则更新字库
	{
		res=0;//按键无效
 		if(update_font(5,ypos+fsize*j,fsize,"0:")!=0)//从SD卡更新
		{ 
 			if(update_font(5,ypos+fsize*j,fsize,"1:")!=0)//从SPI FLASH更新
			{ 
				res=system_usb_update_confirm(5,ypos+fsize*(j+1),fsize);	//是否考虑从USB拷贝文件到SPI FLASH?
				if(res)goto REINIT;
				system_error_show(5,ypos+fsize*(j+1),"Font Error!",fsize);	//字体错误
			} 
		}			
		LCD_Fill(5,ypos+fsize*j,lcddev.width,ypos+fsize*(j+1),BLACK);//填充底色
    	LCD_ShowString(5,ypos+fsize*j,lcddev.width,lcddev.height,fsize, "Font Check...");			   
 	} 
	LCD_ShowString(5+okoffset,ypos+fsize*j++,lcddev.width,lcddev.height,fsize, "OK");//字库检测OK
	//系统文件检测
   	LCD_ShowString(5,ypos+fsize*j,lcddev.width,lcddev.height,fsize, "SYSTEM Files Check...");			   
 	while(app_system_file_check("1"))//系统文件检测
	{
		LCD_Fill(5,ypos+fsize*j,lcddev.width,ypos+fsize*(j+1),BLACK);		//填充底色
    	LCD_ShowString(5,ypos+fsize*j,(fsize/2)*8,fsize,fsize, "Updating"); 	//显示updating	
		app_boot_cpdmsg_set(5,ypos+fsize*j,fsize);		//设置到坐标
		if(app_system_file_check("0"))					//检查SD卡系统文件完整性
		{ 
			res=system_usb_update_confirm(5,ypos+fsize*(j+1),fsize);//SD卡不完整，是否考虑从USB拷贝文件到SPI FLASH?
			if(res)goto REINIT;
			system_error_show(5,ypos+fsize*(j+1),"SYSTEM File Error!",fsize); 	//更新失败，报错  
		}else if(app_system_update(app_boot_cpdmsg))	//SD卡更新 
		{ 
			system_error_show(5,ypos+fsize*(j+1),"SYSTEM File Error!",fsize);	//更新失败，报错  
		} 
		LCD_Fill(5,ypos+fsize*j,lcddev.width,ypos+fsize*(j+1),BLACK);//填充底色
    	LCD_ShowString(5,ypos+fsize*j,lcddev.width,lcddev.height,fsize, "SYSTEM Files Check..."); 
		if(app_system_file_check("1"))//更新了一次，再检测，如果还有不全，说明SD卡文件就不全！
		{
			system_error_show(5,ypos+fsize*(j+1),"SYSTEM File Lost!",fsize);
		}else break;	
	}
	LCD_ShowString(5+okoffset,ypos+fsize*j++,lcddev.width,lcddev.height,fsize, "OK");	
 	//触摸屏检测 
	LCD_ShowString(5,ypos+fsize*j,lcddev.width,lcddev.height,fsize, "Touch Check...");			   
	res=KEY_Scan(1);//检测按键			   
	if(TP_Init()||res==1)//有更新/按下了KEY0,执行校准	 	
	{
		if(res==1)TP_Adjust();
		res=0;//按键无效
		goto REINIT;				//重新开始初始化
	}
	LCD_ShowString(5+okoffset,ypos+fsize*j++,lcddev.width,lcddev.height,fsize, "OK");//触摸屏检测OK
   	//系统参数加载			   
 	LCD_ShowString(5,ypos+fsize*j,lcddev.width,lcddev.height,fsize, "SYSTEM Parameter Load...");			   
 	if(app_system_parameter_init())system_error_show(5,ypos+fsize*(j+1),"Parameter Load Error!",fsize);//参数加载
	else LCD_ShowString(5+okoffset,ypos+fsize*j++,lcddev.width,lcddev.height,fsize, "OK");			   
  	LCD_ShowString(5,ypos+fsize*j,lcddev.width,lcddev.height,fsize, "SYSTEM Starting...");  
	delay_ms(1500);
	myfree(version);	  
} 

			 
int main(void)
{		 
  system_init(); 
	OSInit();   
 	OSTaskCreate(start_task,(void *)0,(OS_STK *)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO );//创建起始任务
	OSStart();	  
}
//开始任务
void start_task(void *pdata)
{
    OS_CPU_SR cpu_sr=0;
	pdata = pdata; 	   
	OSStatInit();		//初始化统计任务.这里会延时1秒钟左右	
 	app_srand(OSTime);
	gui_init();			//gui初始化		  
	piclib_init();		//piclib初始化
	OS_ENTER_CRITICAL();//进入临界区(无法被中断打断)    
 	OSTaskCreate(main_task,(void *)0,(OS_STK*)&MAIN_TASK_STK[MAIN_STK_SIZE-1],MAIN_TASK_PRIO);						   
 	OSTaskCreate(usart_task,(void *)0,(OS_STK*)&USART_TASK_STK[USART_STK_SIZE-1],USART_TASK_PRIO);						   
 	OSTaskCreate(watch_task,(void *)0,(OS_STK*)&WATCH_TASK_STK[WATCH_STK_SIZE-1],WATCH_TASK_PRIO); 					   
	OSTaskSuspend(START_TASK_PRIO);	//挂起起始任务.
	OS_EXIT_CRITICAL();	//退出临界区(可以被中断打断)
}
u8 wirelessmode=0;
//主任务
void main_task(void *pdata)
{ 
	u8 selx=0; 
	mui_init(1); 	//Main UI 初始化
	while(1)
	{
		selx=mui_touch_chk();
		system_task_return=0;//清退出标志 
		switch(selx)
		{
			case 0:	//电子图书
				ebook_play(); 
				break;
			case 1:	//图片浏览		
				picviewer_play(); 
				break;
			case 2:	//USB连接
				usb_play(); 
				break;
			case 3:	//APP界面
				app_play(); 
				break;
			case 4:	//日历
				calendar_play(); 
				break;
			case 5:	//系统设置
				sysset_play(); 
				break;
			case 6:	//画板
				paint_play(); 
				break;
			case 7:	//无线通信实验
				nes_play();
				//wirelessmode=1;
				//wireless_play();
				//wirelessmode=0;
				break;
			case 8:	//记事本功能
				notepad_play(); 
				break; 
		} 
		if(selx<9)mui_load_icos();
		delay_ms(1000/OS_TICKS_PER_SEC);//延时一个时钟节拍
	}
}
//执行最不需要时效性的代码
void usart_task(void *pdata)
{	   
    OS_CPU_SR cpu_sr=0; 
	u16 alarmtimse=0;
	pdata=pdata;
	while(1)
	{			  
		delay_ms(1000);	  
		if(alarm.ringsta&1<<7)//执行闹钟扫描函数
		{
			calendar_alarm_ring(alarm.ringsta&0x3);//闹铃(DS1闪烁)
			alarmtimse++;
			if(alarmtimse>300)//超过300次了,5分钟以上
			{
				alarm.ringsta&=~(1<<7);//关闭闹铃	
			}
	 	}else if(alarmtimse)
		{		 
			alarmtimse=0;
			LED1=1;//关闭DS1
		}
		OS_ENTER_CRITICAL();//进入临界区(无法被中断打断) 
 		printf("sram used:%d\r\n",mem_perused());	//打印内存占用率
		OS_EXIT_CRITICAL();	//退出临界区(可以被中断打断)
	}
}
//强制要求任务返回
volatile u8 system_task_return=0;   
//监视任务
void watch_task(void *pdata)
{
    OS_CPU_SR cpu_sr=0; 
 	u8 t=0;	    
	u8 key;
 	pdata=pdata;
	while(1)
	{	 
		if(alarm.ringsta&1<<7)//闹钟在执行
		{
			calendar_alarm_msg((lcddev.width-44)/2,(lcddev.height-20)/2);//闹钟处理
		}
		if(gifdecoding)//gif正在解码中
		{
			key=pic_tp_scan();
			if(key==1||key==3)gifdecoding=0;//停止GIF解码
		}
		if(t==200)			//2秒钟亮一次
		{
			LED0=0;
			t=0;
		}
		if(t==8)LED0=1;		//亮80ms左右 
		t++;
		key=KEY0_Scan();	//单独扫描KEY0按键
		if(key==1)			//KEY0按下了
		{ 
			system_task_return=1;
 			if(gifdecoding)gifdecoding=0;	//不再播放gif 
		}	  
		if((t%60)==0&&wirelessmode==0)//600ms左右检测1次，无线通信时，不能检测SD卡！！
		{ 
			//SD卡在位检测
			OS_ENTER_CRITICAL();//进入临界区(无法被中断打断) 
			key=SD_GetStatus();	//查询SD卡状态
			OS_EXIT_CRITICAL();	//退出临界区(可以被中断打断) 
			if(key==0)
			{
				gui_phy.memdevflag&=~(1<<0);//标记SD卡不在位 
				OS_ENTER_CRITICAL();//进入临界区(无法被中断打断) 
				SD_Initialize();	//重新检测SD卡 
				OS_EXIT_CRITICAL();	//退出临界区(可以被中断打断) 
			}else if((gui_phy.memdevflag&(1<<0))==0)//SD不在位?
			{
				f_mount(fs[0],"0:",1);		//重新挂载sd卡
				gui_phy.memdevflag|=1<<0;	//标记SD卡在位了		
			}  
		} 
 		delay_ms(10);
	}
}
//硬件错误处理
void HardFault_Handler(void)
{
	u32 i;
	u8 t=0;
	u32 temp;
	temp=SCB->CFSR;					//fault状态寄存器(@0XE000ED28)包括:MMSR,BFSR,UFSR
 	printf("CFSR:%8X\r\n",temp);	//显示错误值
	temp=SCB->HFSR;					//硬件fault状态寄存器
 	printf("HFSR:%8X\r\n",temp);	//显示错误值
 	temp=SCB->DFSR;					//调试fault状态寄存器
 	printf("DFSR:%8X\r\n",temp);	//显示错误值
   	temp=SCB->AFSR;					//辅助fault状态寄存器
 	printf("AFSR:%8X\r\n",temp);	//显示错误值
 	LED1=!LED1;
 	while(t<5)
	{
		t++;
		LED0=!LED0;
		for(i=0;i<0X1FFFFF;i++);
 	}

}





