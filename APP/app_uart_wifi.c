#include "app_uart_wifi.h"
#include "usart2.h"
/*************	���ر�������	**************/
s8	B_TX1_Flag=0;	//����æ��־
s8 LYMS[]="AT+CWMODE=2";//AT+CWMODE=2 ���ó�·��ģʽ
s8 SZLY[]="AT+CWSAP=\"MAGIC_PRINCE\",\"0123456789\",11,0";//AT+CWSAP="ESP8266","0123456789",11,0 ����·��
s8 SZDLJ[]="AT+CIPMUX=1";//AT+CIPMUX=1 ���óɶ�����
s8 KQFU[21]="AT+CIPSERVER=1,5000";//AT+CIPSERVER=1,5000 ����TCP����˿�
s8 FSSJ[15]="AT+CIPSEND=0,15";//AT+CIPSEND= ��������

s8 RST[]="AT+RST";//����ģ��
s8 SET_STA_AP[]="AT+CWMODE=3";//����STA��APģʽ
s8 CWLAP[]="AT+CWLAP";//�г�·�����б�
s8 CONET[]="AT+CWJAP=\"greater41000000\",\"goodgood410\"";//����·����
s8 QUEREN[]="AT+CWJAP?";//��ѯ�Ƿ����ӳɹ�
s8 SET_DUOMOKUAI[]="AT+CIPMUX=1";//������ģ������
s8 LINK_TCP[]="AT+CIPSTART=4,\"TCP\",\"192.168.1.106\",9999";//��ģ������Զ��TCP������ x1.x2.x3.x4Ϊ��ַ9999Ϊ�˿�
s8 SEND_DAT[]="AT+CIPSEND=4,15";//ģ�鷢������ 4ΪID�� ���淢��Ҫ���͵�����
s8 START_TCP[]="AT+CIPSERVER=1,8888";//��������TCP�����ַ 1Ϊ����0Ϊ�ر� 8888Ϊ�����˿�
s8 INQUIRY_IP[]="AT+CIFSR=?";//��ѯIP

s8 MYIP[]="AT+CIFSR";//��ȡ����IP
								 
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
	u8 i,j=0;     //ѭ������
	u8 NUM;
	u8 MODE;
	u8 rval=0;	 
	u16 sx=(lcddev.width-200)/2;
	u16 sy=(lcddev.height-240)/2;
	
 	_window_obj* twin=0;	//����
 	twin=window_creat(sx,sy,200,240,0,1<<6|1<<0,16);//��������
	if(twin)
	{	
		twin->caption=caption;					 
	 	twin->windowbkc=APP_WIN_BACK_COLOR;	 								//������ɫ
		window_draw(twin);													//������
		BACK_COLOR=APP_WIN_BACK_COLOR;
 	}else rval=1;
	if(rval==0)
	{
	    BACK_COLOR=APP_WIN_BACK_COLOR;	//����ɫΪ������ɫ
		POINT_COLOR=RED;				//��ɫ����	
		
	  Show_Str(sx+10,sy+32+1,240,320,"ѡ��ģʽ",16,1);	
	  for(i=0;i<2;i++)
		{
			LCD_Fill(sx+10,sy+32+20+40*i,sx+180,sy+32+50+40*i,BLUE);
		}
		i=0;
	  Show_Str(sx+15,sy+32+30+40*i++,sx+200,16,"·��ģʽ",16,1);
	  Show_Str(sx+15,sy+32+30+40*i++,sx+200,16,"STA��APģʽ",16,1);
	  while(1)
	  {
	  tp_dev.scan(0);
		if(tp_dev.sta&TP_PRES_DOWN)		//������������
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
	  Show_Str(sx+10,sy+32+1,sx+200,sy+32+1+200,"·��ģʽ",16,0);
		u2_printf(LYMS);delay_ms(5);//���ó�·��ģʽ
		u2_printf(SZLY);delay_ms(5);//����·�ɲ���
		u2_printf(SZDLJ);delay_ms(5);//���óɶ�����
		u2_printf(KQFU);delay_ms(5);//����TCP����˿�	
	}		
	else 	
	{
		Show_Str(sx+10,sy+32+1,sx+200,sy+32+1+200,"STA��APģʽ",16,0);
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
			break;//��Ҫ����
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
			 Show_Str(sx,sy+32+1+20+12*j,sx+200,sy+32+1+200,"��������",12,0);
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











