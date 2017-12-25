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
//ALIENTEK MiniSTM32������ʵ��38
//�ۺ�ʵ��
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾  
 
/////////////////////////UCOSII��������///////////////////////////////////
//START ����
//�����������ȼ�
#define START_TASK_PRIO      			10 //��ʼ��������ȼ�����Ϊ���
//���������ջ��С
#define START_STK_SIZE  				64
//�����ջ��8�ֽڶ���	
__align(8) static OS_STK START_TASK_STK[START_STK_SIZE];
//������
void start_task(void *pdata);	
 			   
//��������
//�����������ȼ�
#define USART_TASK_PRIO       			7 
//���������ջ��С
#define USART_STK_SIZE  		    	64
//�����ջ��8�ֽڶ���	
__align(8) static OS_STK USART_TASK_STK[USART_STK_SIZE];
//������
void usart_task(void *pdata);
							 
//������
//�����������ȼ�
#define MAIN_TASK_PRIO       			6 
//���������ջ��С
#define MAIN_STK_SIZE  					256
//�����ջ��8�ֽڶ���	
__align(8) static OS_STK MAIN_TASK_STK[MAIN_STK_SIZE];
//������
void main_task(void *pdata);

//���ڼ�������
//�����������ȼ�
#define WATCH_TASK_PRIO       			3 
//���������ջ��С
#define WATCH_STK_SIZE  		   		128 
//�����ջ��8�ֽڶ���	
__align(8) static OS_STK WATCH_TASK_STK[WATCH_STK_SIZE];
//������
void watch_task(void *pdata);

//////////////////////////////////////////////////////////////////////////////	 

//��ʾ������Ϣ
//x,y:����
//fsize:�����С
//x,y:����.err:������Ϣ
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
//��������SPI FLASH(��������Դ��ɾ��),�Կ��ٸ���ϵͳ.
//x,y:����
//fsize:�����С
//x,y:����.err:������Ϣ
//����ֵ:0,û�в���;1,������
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
			gui_fill_rectangle(x,y+fsize,lcddev.width,fsize,BLACK);//�����ʾ
			t=0;
			LED0=!LED0;
		}
		key=KEY_Scan(0);
		if(key==KEY0_PRES)//������,�û�ȡ����
		{ 
			gui_fill_rectangle(x,y,lcddev.width,fsize*2,BLACK);//�����ʾ
			POINT_COLOR=WHITE;
			LED0=1;
			return 0;
		}
		if(key==KEY1_PRES)//Ҫ����,Ҫ��������
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
//�ֿ����ȷ����ʾ.
//x,y:����
//fsize:�����С 
//����ֵ:0,����Ҫ����;1,ȷ��Ҫ����
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
			gui_fill_rectangle(x,y+fsize,lcddev.width,fsize,BLACK);//�����ʾ
			t=0;
			LED0=!LED0;
		}
		key=KEY_Scan(0);
		if(key==KEY1_PRES)break;//������ 
		if(key==KEY0_PRES){res=1;break;}//Ҫ���� 
		delay_ms(10);
	}
	LED0=1;
	gui_fill_rectangle(x,y,lcddev.width,fsize*2,BLACK);//�����ʾ
	POINT_COLOR=WHITE;
	return res;
}

extern u8 Max_Lun;	//֧�ֵĴ��̸���,0��ʾ1��,1��ʾ2��.
//USB����SPI FLASHȷ��.
//x,y:����
//fsize:�����С 
//����ֵ:0,����Ҫ����;1,ȷ�ϸ���
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
			gui_fill_rectangle(x,y+fsize,lcddev.width,fsize,BLACK);//�����ʾ
			t=0;
			LED0=!LED0;
		}
		delay_ms(10);
		key=KEY_Scan(0);
		if(key==KEY0_PRES)break;//������ 
		if(key==KEY1_PRES){res=1;break;}//ȷ������
	}
	gui_fill_rectangle(x,y,lcddev.width,fsize*2,BLACK);//�����ʾ
	if(res)
	{
		//�Ȳ���SPI FLASH
		LCD_ShowString(x,y,lcddev.width,lcddev.height,fsize,"Erasing SPI FLASH...");
		SPI_Flash_Erase_Chip();
		//�ٸ�ʽ��SPI FLASH
		LCD_ShowString(x,y,lcddev.width,lcddev.height,fsize,"Formating SPI FLASH...");
		res=f_mkfs("1:",1,4096);//��ʽ��FLASH,1,�̷�;1,����Ҫ������,8������Ϊ1����
		if(res==0)
		{
			f_setlabel((const TCHAR *)"1:ALIENTEK");				//����Flash���̵�����Ϊ��ALIENTEK
		}
		LCD_ShowString(x,y,lcddev.width,lcddev.height,fsize,"Please push USB cable to USB_SLAVE");
		Data_Buffer=(u32*)gui_memin_malloc(BULK_MAX_PACKET_SIZE*2*4);	//ΪUSBͨѶ�����ڴ�
		Bulk_Data_Buff=gui_memin_malloc(BULK_MAX_PACKET_SIZE);			//ΪUSBͨѶ�����ڴ�
		//USB����
		USB_Port_Set(0); 	//USB�ȶϿ�
		delay_ms(700);  
		USB_Port_Set(1);	//USB�ٴ�����   
		Max_Lun=0;			//��һ������. 	  
	 	//SPI FLASH���� 
 		Mass_Memory_Size[0]=9832*512;	//ǰ4.8M�ֽ�����FLASH U��.
	  Mass_Block_Size[0] =512;		//��Ϊ������Init����������SD���Ĳ����ֽ�Ϊ512��,��������һ����512���ֽ�.
	  Mass_Block_Count[0]=Mass_Memory_Size[0]/Mass_Block_Size[0];		
		USB_Interrupts_Config();    
		Set_USBClock();   
		USB_Init();
		t=0;
		while(1)
		{
			t++;
			delay_ms(5);
			if(t==40)//200msʱ�䵽�� 
			{
				t=0;
				if(USB_STATUS_REG&0x10)
				{
					offline_cnt=0;//USB������,�����offline������
					bDeviceState=CONFIGURED;
				}else//û�еõ���ѯ 
				{
					offline_cnt++;  
					if(offline_cnt>10)bDeviceState=UNCONNECTED;//2s��û�յ����߱��,����USB���γ���
				}
				USB_STATUS_REG=0;
				LED0=!LED0;
			}			
			if(USB_STA!=USB_STATUS_REG)//״̬�ı��� 
			{	 						   
				gui_fill_rectangle(x,y+fsize*4,lcddev.width,fsize,BLACK);//�����ʾ 
				if(USB_STATUS_REG&0x01)//����д		  
				{
					LCD_ShowString(x,y+fsize*4,lcddev.width,lcddev.height,fsize,"USB Writing..."); 
					USB_STATUS_REG&=0xFD;//��������
				}
				USB_STA=USB_STATUS_REG;//��¼����״̬
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
					LCD_ShowString(x,y+fsize,lcddev.width,lcddev.height,fsize,"USB DisConnected ");//��ʾUSB���γ���
					gui_fill_rectangle(x,y+fsize*2,lcddev.width,fsize*3,BLACK);//�����ʾ 
				}
				Divece_STA=bDeviceState;
			} 
			if(bDeviceState==CONFIGURED)//����״̬��,�ſ����˳�
			{
				key=KEY_Scan(0);
				if(key==KEY0_PRES){res=1;break;}
			}
		}
		USB_Port_Set(0); 				//USB�ȶϿ�
		gui_memin_free(Data_Buffer);	//�ͷ����뵽���ڴ�
		gui_memin_free(Bulk_Data_Buff);	//�ͷ����뵽���ڴ�
	} 
	LED0=1;
	gui_fill_rectangle(x,y,lcddev.width,fsize*2,BLACK);//�����ʾ
	POINT_COLOR=WHITE;
	return res;
}

//ϵͳ��ʼ��
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
	
 	Stm32_Clock_Init(9);	//ϵͳʱ������
  delay_init(72);			//��ʱ��ʼ��
 	uart_init(72,115200); 	//����1��ʼ��
	usart2_init(36,115200);  //����3��ʼ��
	usart3_init(36,115200);  //����3��ʼ��
	LCD_Init();				//LCD��ʼ�� 
 	LED_Init();	    		//LED��ʼ��
 	KEY_Init();				//������ʼ�� 
	gui_init();		
	Adc_Init();			  	//ADC��ʼ��,�ڲ��¶ȴ����� 
	SPI_Flash_Init();		//W25Qxx��ʼ��												  
 	usmart_dev.init(72);    
  mem_init();				//�ڴ�س�ʼ��
	version=mymalloc(31);	//����31���ֽ��ڴ�
	exfuns_init();
REINIT://���³�ʼ��
	LCD_Clear(BLACK);		//����
	POINT_COLOR=WHITE;
	BACK_COLOR=BLACK;
	j=0;   
/////////////////////////////////////////////////////////////////////////
//��ʾ��Ȩ��Ϣ
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
	sprintf((char*)verbuf,"LCD ID:%04X",lcddev.id);		//LCD ID��ӡ��verbuf����
	LCD_ShowString(5,ypos+fsize*j++,lcddev.width,lcddev.height,fsize, verbuf);	//��ʾLCD ID 
//////////////////////////////////////////////////////////////////////////
//��ʼӲ������ʼ��
	LED0=0;LED1=0;				//ͬʱ��������LED
	LCD_ShowString(5,ypos+fsize*j++,lcddev.width,lcddev.height,fsize, "CPU:STM32F103VET6 72Mhz");
	LCD_ShowString(5,ypos+fsize*j++,lcddev.width,lcddev.height,fsize, "FLASH:512KB  SRAM:64KB");	
	LED0=1;LED1=1;				//ͬʱ�ر�����LED  
	if(SPI_Flash_ReadID()!=W25Q64)//��ⲻ��W25Q64
	{	 
		system_error_show(5,ypos+fsize*j++,"Ex Flash Error!!",fsize); 
	}else temp=8*1024;			//8M�ֽڴ�С
	LCD_ShowString(5,ypos+fsize*j,lcddev.width,lcddev.height,fsize, "Ex Flash:     KB");			   
	LCD_ShowxNum(5+9*(fsize/2),ypos+fsize*j,temp,5,fsize,0);//��ʾflash��С  
	LCD_ShowString(5+okoffset,ypos+fsize*j++,lcddev.width,lcddev.height,fsize, "OK");   
	//����Ƿ���Ҫ����SPI FLASH?
	res=KEY_Scan(1);//
	if(res==WKUP_PRES)
	{
		res=system_files_erase(5,ypos+fsize*j,fsize);
		if(res)goto REINIT; 
	}
	//���SPI FLASH���ļ�ϵͳ
	LCD_ShowString(5,ypos+fsize*j,lcddev.width,lcddev.height,fsize, "FATFS Check...");//FATFS���			   
  	f_mount(fs[0],"0:",1); 		//����SD��  
  	f_mount(fs[1],"1:",1); 		//���ع���FLASH. 
 	LCD_ShowString(5+okoffset,ypos+fsize*j++,lcddev.width,lcddev.height,fsize, "OK");			   
	//SD�����
	LCD_ShowString(5,ypos+fsize*j,lcddev.width,lcddev.height,fsize, "SD Card:     MB");//SD�����
	temp=0;	
 	do
	{
		temp++;
 		res=exf_getfree("0:",&dtsize,&dfsize);//�õ�SD��ʣ��������������
		delay_ms(200);		   
	}while(res&&temp<5);//�������5��
 	if(res==0)//�õ���������
	{
		gui_phy.memdevflag|=1<<0;	//����SD����λ.
		temp=dtsize>>10;//��λת��ΪMB
		stastr="OK";
 	}else 
	{
 		temp=0;//������,��λΪ0
		stastr="ERROR";
	}
 	LCD_ShowxNum(5+8*(fsize/2),ypos+fsize*j,temp,5,fsize,0);					//��ʾSD��������С
	LCD_ShowString(5+okoffset,ypos+fsize*j++,lcddev.width,lcddev.height,fsize,stastr);	//SD��״̬			   
	//W25Q64���,����������ļ�ϵͳ,���ȴ���.
	temp=0;	
 	do
	{
		temp++;
 		res=exf_getfree("1:",&dtsize,&dfsize);//�õ�FLASHʣ��������������
		delay_ms(200);		   
	}while(res&&temp<20);//�������20��		  
	if(res==0X0D)//�ļ�ϵͳ������
	{
		LCD_ShowString(5,ypos+fsize*j,lcddev.width,lcddev.height,fsize, "Flash Disk Formatting...");	//��ʽ��FLASH
		res=f_mkfs("1:",1,4096);//��ʽ��FLASH,1,�̷�;1,����Ҫ������,8������Ϊ1����
		if(res==0)
		{
			f_setlabel((const TCHAR *)"1:ALIENTEK");				//����Flash���̵�����Ϊ��ALIENTEK
			LCD_ShowString(5+okoffset,ypos+fsize*j++,lcddev.width,lcddev.height,fsize, "OK");//��־��ʽ���ɹ�
 			res=exf_getfree("1:",&dtsize,&dfsize);//���»�ȡ����
		}
	}   
	if(res==0)//�õ�FLASH��ʣ��������������
	{
		gui_phy.memdevflag|=1<<1;	//����SPI FLASH��λ.
		LCD_ShowString(5,ypos+fsize*j,lcddev.width,lcddev.height,fsize, "Flash Disk:     KB");//FATFS���			   
		temp=dtsize; 	   
 	}else system_error_show(5,ypos+fsize*(j+1),"Flash Fat Error!",fsize);	//flash �ļ�ϵͳ���� 
 	LCD_ShowxNum(5+11*(fsize/2),ypos+fsize*j,temp,5,fsize,0);						//��ʾFLASH������С
	LCD_ShowString(5+okoffset,ypos+fsize*j++,lcddev.width,lcddev.height,fsize,"OK");//FLASH��״̬	
    //RTC���
  	LCD_ShowString(5,ypos+fsize*j,lcddev.width,lcddev.height,fsize, "RTC Check...");			   
 	if(RTC_Init())system_error_show(5,ypos+fsize*(j+1),"RTC Error!",fsize);//RTC���
	else 
	{ 
		RTC_Get();//��ȡʱ��
		LCD_ShowString(5+okoffset,ypos+fsize*j++,lcddev.width,lcddev.height,fsize, "OK");			   
	}
	//24C02���
 //  	LCD_ShowString(5,ypos+fsize*j,lcddev.width,lcddev.height,fsize, "24C02 Check...");			   
 //	if(AT24CXX_Check())system_error_show(5,ypos+fsize*(j+1),"24C02 Error!",fsize);//24C02���
//	else LCD_ShowString(5+okoffset,ypos+fsize*j++,lcddev.width,lcddev.height,fsize, "OK");  
  	//�ֿ���									    
   	LCD_ShowString(5,ypos+fsize*j,lcddev.width,lcddev.height,fsize, "Font Check...");
	res=KEY_Scan(1);//��ⰴ��	
	if(res==KEY1_PRES)//���£�ȷ��
	{
		res=system_font_update_confirm(5,ypos+fsize*(j+1),fsize);
	}else res=0;
  	while(font_init()||res==1)	//�������,������岻����/����KEY0ȷ�ϸ���,������ֿ�
	{
		res=0;//������Ч
 		if(update_font(5,ypos+fsize*j,fsize,"0:")!=0)//��SD������
		{ 
 			if(update_font(5,ypos+fsize*j,fsize,"1:")!=0)//��SPI FLASH����
			{ 
				res=system_usb_update_confirm(5,ypos+fsize*(j+1),fsize);	//�Ƿ��Ǵ�USB�����ļ���SPI FLASH?
				if(res)goto REINIT;
				system_error_show(5,ypos+fsize*(j+1),"Font Error!",fsize);	//�������
			} 
		}			
		LCD_Fill(5,ypos+fsize*j,lcddev.width,ypos+fsize*(j+1),BLACK);//����ɫ
    	LCD_ShowString(5,ypos+fsize*j,lcddev.width,lcddev.height,fsize, "Font Check...");			   
 	} 
	LCD_ShowString(5+okoffset,ypos+fsize*j++,lcddev.width,lcddev.height,fsize, "OK");//�ֿ���OK
	//ϵͳ�ļ����
   	LCD_ShowString(5,ypos+fsize*j,lcddev.width,lcddev.height,fsize, "SYSTEM Files Check...");			   
 	while(app_system_file_check("1"))//ϵͳ�ļ����
	{
		LCD_Fill(5,ypos+fsize*j,lcddev.width,ypos+fsize*(j+1),BLACK);		//����ɫ
    	LCD_ShowString(5,ypos+fsize*j,(fsize/2)*8,fsize,fsize, "Updating"); 	//��ʾupdating	
		app_boot_cpdmsg_set(5,ypos+fsize*j,fsize);		//���õ�����
		if(app_system_file_check("0"))					//���SD��ϵͳ�ļ�������
		{ 
			res=system_usb_update_confirm(5,ypos+fsize*(j+1),fsize);//SD�����������Ƿ��Ǵ�USB�����ļ���SPI FLASH?
			if(res)goto REINIT;
			system_error_show(5,ypos+fsize*(j+1),"SYSTEM File Error!",fsize); 	//����ʧ�ܣ�����  
		}else if(app_system_update(app_boot_cpdmsg))	//SD������ 
		{ 
			system_error_show(5,ypos+fsize*(j+1),"SYSTEM File Error!",fsize);	//����ʧ�ܣ�����  
		} 
		LCD_Fill(5,ypos+fsize*j,lcddev.width,ypos+fsize*(j+1),BLACK);//����ɫ
    	LCD_ShowString(5,ypos+fsize*j,lcddev.width,lcddev.height,fsize, "SYSTEM Files Check..."); 
		if(app_system_file_check("1"))//������һ�Σ��ټ�⣬������в�ȫ��˵��SD���ļ��Ͳ�ȫ��
		{
			system_error_show(5,ypos+fsize*(j+1),"SYSTEM File Lost!",fsize);
		}else break;	
	}
	LCD_ShowString(5+okoffset,ypos+fsize*j++,lcddev.width,lcddev.height,fsize, "OK");	
 	//��������� 
	LCD_ShowString(5,ypos+fsize*j,lcddev.width,lcddev.height,fsize, "Touch Check...");			   
	res=KEY_Scan(1);//��ⰴ��			   
	if(TP_Init()||res==1)//�и���/������KEY0,ִ��У׼	 	
	{
		if(res==1)TP_Adjust();
		res=0;//������Ч
		goto REINIT;				//���¿�ʼ��ʼ��
	}
	LCD_ShowString(5+okoffset,ypos+fsize*j++,lcddev.width,lcddev.height,fsize, "OK");//���������OK
   	//ϵͳ��������			   
 	LCD_ShowString(5,ypos+fsize*j,lcddev.width,lcddev.height,fsize, "SYSTEM Parameter Load...");			   
 	if(app_system_parameter_init())system_error_show(5,ypos+fsize*(j+1),"Parameter Load Error!",fsize);//��������
	else LCD_ShowString(5+okoffset,ypos+fsize*j++,lcddev.width,lcddev.height,fsize, "OK");			   
  	LCD_ShowString(5,ypos+fsize*j,lcddev.width,lcddev.height,fsize, "SYSTEM Starting...");  
	delay_ms(1500);
	myfree(version);	  
} 

			 
int main(void)
{		 
  system_init(); 
	OSInit();   
 	OSTaskCreate(start_task,(void *)0,(OS_STK *)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO );//������ʼ����
	OSStart();	  
}
//��ʼ����
void start_task(void *pdata)
{
    OS_CPU_SR cpu_sr=0;
	pdata = pdata; 	   
	OSStatInit();		//��ʼ��ͳ������.�������ʱ1��������	
 	app_srand(OSTime);
	gui_init();			//gui��ʼ��		  
	piclib_init();		//piclib��ʼ��
	OS_ENTER_CRITICAL();//�����ٽ���(�޷����жϴ��)    
 	OSTaskCreate(main_task,(void *)0,(OS_STK*)&MAIN_TASK_STK[MAIN_STK_SIZE-1],MAIN_TASK_PRIO);						   
 	OSTaskCreate(usart_task,(void *)0,(OS_STK*)&USART_TASK_STK[USART_STK_SIZE-1],USART_TASK_PRIO);						   
 	OSTaskCreate(watch_task,(void *)0,(OS_STK*)&WATCH_TASK_STK[WATCH_STK_SIZE-1],WATCH_TASK_PRIO); 					   
	OSTaskSuspend(START_TASK_PRIO);	//������ʼ����.
	OS_EXIT_CRITICAL();	//�˳��ٽ���(���Ա��жϴ��)
}
u8 wirelessmode=0;
//������
void main_task(void *pdata)
{ 
	u8 selx=0; 
	mui_init(1); 	//Main UI ��ʼ��
	while(1)
	{
		selx=mui_touch_chk();
		system_task_return=0;//���˳���־ 
		switch(selx)
		{
			case 0:	//����ͼ��
				ebook_play(); 
				break;
			case 1:	//ͼƬ���		
				picviewer_play(); 
				break;
			case 2:	//USB����
				usb_play(); 
				break;
			case 3:	//APP����
				app_play(); 
				break;
			case 4:	//����
				calendar_play(); 
				break;
			case 5:	//ϵͳ����
				sysset_play(); 
				break;
			case 6:	//����
				paint_play(); 
				break;
			case 7:	//����ͨ��ʵ��
				nes_play();
				//wirelessmode=1;
				//wireless_play();
				//wirelessmode=0;
				break;
			case 8:	//���±�����
				notepad_play(); 
				break; 
		} 
		if(selx<9)mui_load_icos();
		delay_ms(1000/OS_TICKS_PER_SEC);//��ʱһ��ʱ�ӽ���
	}
}
//ִ�����ҪʱЧ�ԵĴ���
void usart_task(void *pdata)
{	   
    OS_CPU_SR cpu_sr=0; 
	u16 alarmtimse=0;
	pdata=pdata;
	while(1)
	{			  
		delay_ms(1000);	  
		if(alarm.ringsta&1<<7)//ִ������ɨ�躯��
		{
			calendar_alarm_ring(alarm.ringsta&0x3);//����(DS1��˸)
			alarmtimse++;
			if(alarmtimse>300)//����300����,5��������
			{
				alarm.ringsta&=~(1<<7);//�ر�����	
			}
	 	}else if(alarmtimse)
		{		 
			alarmtimse=0;
			LED1=1;//�ر�DS1
		}
		OS_ENTER_CRITICAL();//�����ٽ���(�޷����жϴ��) 
 		printf("sram used:%d\r\n",mem_perused());	//��ӡ�ڴ�ռ����
		OS_EXIT_CRITICAL();	//�˳��ٽ���(���Ա��жϴ��)
	}
}
//ǿ��Ҫ�����񷵻�
volatile u8 system_task_return=0;   
//��������
void watch_task(void *pdata)
{
    OS_CPU_SR cpu_sr=0; 
 	u8 t=0;	    
	u8 key;
 	pdata=pdata;
	while(1)
	{	 
		if(alarm.ringsta&1<<7)//������ִ��
		{
			calendar_alarm_msg((lcddev.width-44)/2,(lcddev.height-20)/2);//���Ӵ���
		}
		if(gifdecoding)//gif���ڽ�����
		{
			key=pic_tp_scan();
			if(key==1||key==3)gifdecoding=0;//ֹͣGIF����
		}
		if(t==200)			//2������һ��
		{
			LED0=0;
			t=0;
		}
		if(t==8)LED0=1;		//��80ms���� 
		t++;
		key=KEY0_Scan();	//����ɨ��KEY0����
		if(key==1)			//KEY0������
		{ 
			system_task_return=1;
 			if(gifdecoding)gifdecoding=0;	//���ٲ���gif 
		}	  
		if((t%60)==0&&wirelessmode==0)//600ms���Ҽ��1�Σ�����ͨ��ʱ�����ܼ��SD������
		{ 
			//SD����λ���
			OS_ENTER_CRITICAL();//�����ٽ���(�޷����жϴ��) 
			key=SD_GetStatus();	//��ѯSD��״̬
			OS_EXIT_CRITICAL();	//�˳��ٽ���(���Ա��жϴ��) 
			if(key==0)
			{
				gui_phy.memdevflag&=~(1<<0);//���SD������λ 
				OS_ENTER_CRITICAL();//�����ٽ���(�޷����жϴ��) 
				SD_Initialize();	//���¼��SD�� 
				OS_EXIT_CRITICAL();	//�˳��ٽ���(���Ա��жϴ��) 
			}else if((gui_phy.memdevflag&(1<<0))==0)//SD����λ?
			{
				f_mount(fs[0],"0:",1);		//���¹���sd��
				gui_phy.memdevflag|=1<<0;	//���SD����λ��		
			}  
		} 
 		delay_ms(10);
	}
}
//Ӳ��������
void HardFault_Handler(void)
{
	u32 i;
	u8 t=0;
	u32 temp;
	temp=SCB->CFSR;					//fault״̬�Ĵ���(@0XE000ED28)����:MMSR,BFSR,UFSR
 	printf("CFSR:%8X\r\n",temp);	//��ʾ����ֵ
	temp=SCB->HFSR;					//Ӳ��fault״̬�Ĵ���
 	printf("HFSR:%8X\r\n",temp);	//��ʾ����ֵ
 	temp=SCB->DFSR;					//����fault״̬�Ĵ���
 	printf("DFSR:%8X\r\n",temp);	//��ʾ����ֵ
   	temp=SCB->AFSR;					//����fault״̬�Ĵ���
 	printf("AFSR:%8X\r\n",temp);	//��ʾ����ֵ
 	LED1=!LED1;
 	while(t<5)
	{
		t++;
		LED0=!LED0;
		for(i=0;i<0X1FFFFF;i++);
 	}

}





