#include "usbplay.h"
#include "mass_mal.h"
#include "usb_lib.h"
#include "hw_config.h"
#include "usb_pwr.h"
#include "memory.h"	    
#include "usb_bot.h"  
#include "mmc_sd.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//APP-USB连接 代码	   
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

extern u8 Max_Lun;	//支持的磁盘个数,0表示1个,1表示2个.
extern u8*const nes_remindmsg_tbl[5][GUI_LANGUAGE_NUM];//在nesplay.c里面定义
//USB连接  				 
u8 usb_play(void)
{			  
	u8 rval=0;			//返回值	  
 	u8 offline_cnt=0;
 	u8 USB_STA=0;
	u8 Divece_STA=0;
	u8 i=0,err_back=0;
 	u16 werr=0,rerr=0;

   	POINT_COLOR=BLUE;  
  	BACK_COLOR=LGRAY;  
	LCD_Clear(BACK_COLOR);		//清屏
	app_filebrower((u8*)APP_MFUNS_CAPTION_TBL[2][gui_phy.language],0X05);//显示标题  
	
    Data_Buffer=(u32*)gui_memin_malloc(BULK_MAX_PACKET_SIZE*2*4);	//为USB通讯申请内存
	Bulk_Data_Buff=gui_memin_malloc(BULK_MAX_PACKET_SIZE);			//为USB通讯申请内存
	if(Data_Buffer!=NULL&&Bulk_Data_Buff!=NULL)
	{
		//USB配置
		USB_Port_Set(0); 	//USB先断开
		delay_ms(700);  
		USB_Port_Set(1);	//USB再次连接 
		
		if(gui_phy.memdevflag&(1<<0))		//SD卡存在
		{  		
			Max_Lun=1;						//有2个磁盘 
			Mass_Memory_Size[1]=(long long)SD_GetSectorCount()*512;//得到SD卡容量，当SD卡容量超过4G的时候,需要用到两个u32来表示
			Mass_Block_Size[1] =512;							//因为我们在Init里面设置了SD卡的操作字节为512个,所以这里一定是512个字节.
			Mass_Block_Count[1]=Mass_Memory_Size[1]/Mass_Block_Size[1];
 		}else Max_Lun=0;					//SD卡错误,则仅只有一个磁盘. 	 
		Mass_Memory_Size[0]=4916*1024;		//前4.8M字节用作FLASH U盘.
		Mass_Block_Size[0] =512;			//设置SPI FLASH的操作扇区大小为512
		Mass_Block_Count[0]=Mass_Memory_Size[0]/Mass_Block_Size[0];  
		USB_Interrupts_Config();    
		Set_USBClock();   
		USB_Init();	 
		LCD_ShowString(60+(lcddev.width-240)/2,130+(lcddev.height-320)/2,lcddev.width,lcddev.height,16,"USB DisConnected ");//首先提示USB无连接
	}else rval=1;//内存申请失败
   	while(rval==0)
	{
 		delay_ms(1000/OS_TICKS_PER_SEC);		//延时一个时钟节拍
  		if(system_task_return)//TPAD返回
		{	  
			if(USB_STATUS_REG&0x03)//USB正在读写
			{
				POINT_COLOR=RED;  					    
				LCD_ShowString(60+(lcddev.width-240)/2,110+(lcddev.height-320)/2,lcddev.width,lcddev.height,16,"USB BUSY!!!");//提示SD卡已经准备了
				POINT_COLOR=BLUE;  
				err_back=0x80;//标记最高位
				system_task_return=0;//取消   
			}else break;//USB空闲,则退出USB	 		
		} 
		if(USB_STA!=USB_STATUS_REG)//状态改变了 
		{	 						   
			LCD_ShowString(60+(lcddev.width-240)/2,150+(lcddev.height-320)/2,lcddev.width,lcddev.height,16,"              ");//清除  
			if(USB_STATUS_REG&0x01)//正在写		  
			{
				LCD_ShowString(60+(lcddev.width-240)/2,150+(lcddev.height-320)/2,lcddev.width,lcddev.height,16,"USB Writing...");//提示USB正在写入数据
				USB_STATUS_REG&=0xFD;//清除读标记
			}
			if(USB_STATUS_REG&0x02)//正在读
			{
				LCD_ShowString(60+(lcddev.width-240)/2,150+(lcddev.height-320)/2,lcddev.width,lcddev.height,16,"USB Reading...");//提示USB正在读出数据  
				USB_STATUS_REG&=0xFE;//清除写标记	 
			}	 										  
			if(USB_STATUS_REG&0x04)
			{
				POINT_COLOR=RED; 
				werr++; 
				LCD_ShowString(60+(lcddev.width-240)/2,170+(lcddev.height-320)/2,lcddev.width,lcddev.height,16,"USB Write Err:");//提示写入错误
				LCD_ShowNum(172,170,werr,3,16);//显示读数错误的次数
				POINT_COLOR=BLUE;  
			}													        
			if(USB_STATUS_REG&0x08)
			{
				POINT_COLOR=RED;  
				rerr++;
				LCD_ShowString(60+(lcddev.width-240)/2,190+(lcddev.height-320)/2,lcddev.width,lcddev.height,16,"USB Read  Err:");//提示读出错误
				LCD_ShowNum(172+(lcddev.width-240)/2,190+(lcddev.height-320)/2,rerr,3,16);//显示读数错误的次数
				POINT_COLOR=BLUE;  
			}														   
			USB_STA=USB_STATUS_REG;//记录最后的状态
		}
		if(Divece_STA!=bDeviceState) 
		{
			if(bDeviceState==CONFIGURED)LCD_ShowString(60+(lcddev.width-240)/2,130+(lcddev.height-320)/2,lcddev.width,lcddev.height,16,"USB Connected    ");//提示USB连接已经建立
			else LCD_ShowString(60+(lcddev.width-240)/2,130+(lcddev.height-320)/2,lcddev.width,320,16,"USB DisConnected ");//提示USB被拔出了
			Divece_STA=bDeviceState;
		}
		i++;
		if(i==200)//200ms时间到了
		{
			i=0;
			if(USB_STATUS_REG&0x10)
			{
				offline_cnt=0;//USB连接了,则清除offline计数器
				bDeviceState=CONFIGURED;
			}else//没有得到轮询 
			{
				offline_cnt++;  
				if(offline_cnt>10)bDeviceState=UNCONNECTED;//2s内没收到在线标记,代表USB被拔出了
			}
			if(err_back&0x80)//发生了错误操作
			{
				err_back++;
				if((err_back&0x07)==0x07)
				{
					err_back=0;//清除 	 
					LCD_ShowString(60+(lcddev.width-240)/2,110+(lcddev.height-320)/2,lcddev.width,lcddev.height,16,"           ");//清除显示
				}
			}
			USB_STATUS_REG=0;
		}
	}	
	USB_Port_Set(0); 				//USB先断开
 	gui_memin_free(Data_Buffer);	//释放申请到的内存
	gui_memin_free(Bulk_Data_Buff);	//释放申请到的内存
	return rval; 
}







