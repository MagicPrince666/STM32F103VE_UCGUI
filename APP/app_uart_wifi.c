#include "app_uart_wifi.h"
#include "usart2.h"
/*************	本地变量声明	**************/
s8	B_TX1_Flag=0;	//发送忙标志
s8 LYMS[]="AT+CWMODE=2";//AT+CWMODE=2 设置成路由模式
s8 SZLY[]="AT+CWSAP=\"MAGIC_PRINCE\",\"0123456789\",11,0";//AT+CWSAP="ESP8266","0123456789",11,0 设置路由
s8 SZDLJ[]="AT+CIPMUX=1";//AT+CIPMUX=1 设置成多连接
s8 KQFU[21]="AT+CIPSERVER=1,5000";//AT+CIPSERVER=1,5000 开启TCP服务端口
s8 FSSJ[15]="AT+CIPSEND=0,15";//AT+CIPSEND= 发送数据

s8 RST[]="AT+RST";//重启模块
s8 SET_STA_AP[]="AT+CWMODE=3";//设置STA和AP模式
s8 CWLAP[]="AT+CWLAP";//列出路由器列表
s8 CONET[]="AT+CWJAP=\"greater41000000\",\"goodgood410\"";//连接路由器
s8 QUEREN[]="AT+CWJAP?";//查询是否连接成功
s8 SET_DUOMOKUAI[]="AT+CIPMUX=1";//启动多模块连接
s8 LINK_TCP[]="AT+CIPSTART=4,\"TCP\",\"192.168.1.106\",9999";//让模块连接远程TCP服务器 x1.x2.x3.x4为地址9999为端口
s8 SEND_DAT[]="AT+CIPSEND=4,15";//模块发送数据 4为ID号 后面发送要传送的数据
s8 START_TCP[]="AT+CIPSERVER=1,8888";//开启本地TCP服务地址 1为开启0为关闭 8888为监听端口
s8 INQUIRY_IP[]="AT+CIFSR=?";//查询IP

s8 MYIP[]="AT+CIFSR";//获取本地IP
								 
void Send(u8 *dat)
{
  while(*dat!='\0')
	{
		USART2->DR=*dat++;
		while((USART2->SR&0X40)==0);
	}
}
u8 ESP8266_Play(u8* caption)
{
	u8 i,j=0;     //循环控制
	u8 NUM;
	u8 MODE;
	u8 rval=0;	 
	u16 sx=(lcddev.width-200)/2;
	u16 sy=(lcddev.height-240)/2;
	
 	_window_obj* twin=0;	//窗体
 	twin=window_creat(sx,sy,200,240,0,1<<6|1<<0,16);//创建窗口
	if(twin)
	{	
		twin->caption=caption;					 
	 	twin->windowbkc=APP_WIN_BACK_COLOR;	 								//窗体主色
		window_draw(twin);													//画窗体
		BACK_COLOR=APP_WIN_BACK_COLOR;
 	}else rval=1;
	if(rval==0)
	{
	    BACK_COLOR=APP_WIN_BACK_COLOR;	//背景色为窗体主色
		POINT_COLOR=RED;				//红色的字	
		
	  Show_Str(sx+10,sy+32+1,240,320,"选择模式",16,1);	
	  for(i=0;i<2;i++)
		{
			LCD_Fill(sx+10,sy+32+20+40*i,sx+180,sy+32+50+40*i,BLUE);
		}
		i=0;
	  Show_Str(sx+15,sy+32+30+40*i++,sx+200,16,"路由模式",16,1);
	  Show_Str(sx+15,sy+32+30+40*i++,sx+200,16,"STA和AP模式",16,1);
	  while(1)
	  {
	  tp_dev.scan(0);
		if(tp_dev.sta&TP_PRES_DOWN)		//触摸屏被按下
		{
			if(tp_dev.y[0]>sy+32+1&&tp_dev.y[0]<sy+32+1+40)
				{
					MODE=0;break;
				}
			if(tp_dev.y[0]>sy+32+1+40&&tp_dev.y[0]<sy+32+1+80)
				{
					MODE=1;break;
				}	
		}
	 if(KEY0==0)
	 {
		 delay_ms(5);
		 while(KEY0==0);
		 MODE=0;
		 break;
	 }
	 if(KEY1==0)
	 {
		 delay_ms(5);
		 while(KEY1==0);
	   MODE=1;
		 break;
	 }
	}
	LCD_Fill(sx,sy+32+1,sx+200-1,sy+32+200,BACK_COLOR);
	if(MODE==0)
	{
	  Show_Str(sx+10,sy+32+1,sx+200,sy+32+1+200,"路由模式",16,0);
		u2_printf(LYMS);delay_ms(5);//设置成路由模式
		u2_printf(SZLY);delay_ms(5);//设置路由参数
		u2_printf(SZDLJ);delay_ms(5);//设置成多连接
		u2_printf(KQFU);delay_ms(5);//开启TCP服务端口	
	}		
	else 	
	{
		Show_Str(sx+10,sy+32+1,sx+200,sy+32+1+200,"STA和AP模式",16,0);
		u2_printf(SET_STA_AP);delay_ms(5);
		u2_printf(CWLAP);delay_ms(5);
		u2_printf(QUEREN);delay_ms(5);
		u2_printf(LINK_TCP);delay_ms(5);
		u2_printf(SEND_DAT);delay_ms(5);
		u2_printf(START_TCP);delay_ms(5);
		u2_printf(INQUIRY_IP);delay_ms(5);
		u2_printf(SET_DUOMOKUAI);delay_ms(5);
	}		
	delay_ms(100);
	while (1)
	{
		if(system_task_return)
		{
			break;//需要返回
		}
		if(WK_UP==1)
		{
		  delay_ms(10);
			while(WK_UP==1);
		  NUM++;
			if(NUM>8)NUM=0;	
      B_TX1_Flag=1;			
		}
		if(MODE==0&&KEY1==0)
		{
			delay_ms(5);
			while(KEY1==0);
			 Show_Str(sx,sy+32+1+20+12*j,sx+200,sy+32+1+200,"发送数据",12,0);
			j++;
		  Send(FSSJ);u2_printf("I can hear you!");delay_ms(5);
		}
    if(USART2_RX_STA&0x8000)
		 {		 			 
		   Show_Str(sx,sy+32+1+20+12*j,sx+200,sy+32+1+200,USART2_RX_BUF,12,0);
			 j++;
			 if(j>23)
			 {
				 LCD_Fill(sx,sy+32+1+40,sx+200-1,sy+32+200,WHITE);
				 j=0;
			 }
			 USART2_RX_STA=0;
			 for(i=0;i<64;i++)
			 USART2_RX_BUF[i]=0;
		 }
	}
  }
	window_delete(twin);
  return rval;
  }











