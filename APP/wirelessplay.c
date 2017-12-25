#include "wirelessplay.h"
#include "24l01.h"	    
#include "spi.h"	    
#include "paint.h"	    
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//APP-����ͨ�Ų��� ����	   
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

//�ػ���ť������
u8*const wireless_cbtn_caption_tbl[GUI_LANGUAGE_NUM]=
{
"�ػ�","�خ�","CLEAR",	 
}; 
//��������
u8*const wireless_remind_msg_tbl[GUI_LANGUAGE_NUM]=
{
"δ��⵽NRF24L01,����!","δ�z�y��NRF24L01,Ո�z��!","No NRF24L01,Please Check...",	 
};
//ģʽѡ��
u8*const wireless_mode_tbl[GUI_LANGUAGE_NUM][2]=
{
{"����ģʽ","����ģʽ",},
{"�l��ģʽ","����ģʽ",},		 
{"TX Mode","RX Mode",},
};	   
/////////////////////////////////////////////////////////////////////////////////
 
//���ߴ���
u8 wireless_play(void)
{
   	_btn_obj* cbtn=0;		//�����ť�ؼ�
  	u8 res;
	u8 *caption;			//����
	u8 mode=0;				//0,����ģʽ;1,����ģʽ
	u8 tmp_buf[6];			//buf[0~3]:����ֵ;buf[4]:0,������ͼ;1,����;2,�˳�. 
	u16 x=0,y=0;
	u16 lastx=0XFFFF,lasty=0XFFFF;
 	u8 rval=0;

  	br=0;
	//��ѡ��ģʽ
	res=app_items_sel((lcddev.width-180)/2,(lcddev.height-152)/2,180,72+40*2,(u8**)wireless_mode_tbl[gui_phy.language],2,(u8*)&mode,0X90,(u8*)APP_MODESEL_CAPTION_TBL[gui_phy.language]);//2��ѡ��
 	if(res==0)//ȷ�ϼ�������,ͬʱ������������
	{
    	caption=(u8*)APP_MFUNS_CAPTION_TBL[7][gui_phy.language];//���ߴ���	 
		gui_fill_rectangle(0,0,lcddev.width,lcddev.height,LGRAY);//��䱳��ɫ
 		NRF24L01_Init();    		//��ʼ��NRF24L01 
		res=0;
		while(NRF24L01_Check())		//��ⲻ��24L01,�������5��
		{
			if(res==0)window_msg_box((lcddev.width-200)/2,(lcddev.height-70)/2-15,200,70,(u8*)wireless_remind_msg_tbl[gui_phy.language],(u8*)APP_REMIND_CAPTION_TBL[gui_phy.language],12,0,0,0);
			delay_ms(500);
			res++;
			if(res>5)return 0;//��ʱδ��⵽,�˳�
		}				  
   		gui_fill_rectangle(0,gui_phy.tbheight,lcddev.width,lcddev.height-gui_phy.tbheight,LGRAY);	//��䱳��ɫ
 		if(mode==0)NRF24L01_TX_Mode();			//����ģʽ
		else NRF24L01_RX_Mode();    
		caption=(u8*)wireless_mode_tbl[gui_phy.language][mode];	//����
 		app_filebrower(caption,0X07);	 						//��ʾ���� 
 		cbtn=btn_creat(0,lcddev.height-gui_phy.tbheight,2*gui_phy.tbheight+8,gui_phy.tbheight-1,0,0x03);//�������ְ�ť
		if(cbtn==NULL)rval=1;	//û���㹻�ڴ湻����
		else
		{																				
		 	cbtn->caption=(u8*)wireless_cbtn_caption_tbl[gui_phy.language];//�ػ�
			cbtn->font=gui_phy.tbfsize;//�����µ������С	 	 
			cbtn->bcfdcolor=WHITE;	//����ʱ����ɫ
			cbtn->bcfucolor=WHITE;	//�ɿ�ʱ����ɫ
			if(mode==0)btn_draw(cbtn);//����ģʽ����Ҫ�ػ���ť
 		}
		tmp_buf[4]=tp_dev.touchtype&0X80;//�õ�����������
                    		//tmp_buf[4]:
							//b3~0:0,�����κδ���
							//     1,����
							//     2,����
							//     3,�˳�
							//b4~6:����
							//b7:0,������;1,������    
		while(rval==0)
		{	 
			tp_dev.scan(0); 		 
			in_obj.get_key(&tp_dev,IN_TYPE_TOUCH);	//�õ�������ֵ 
			tmp_buf[4]&=0X80;//���ԭ��������
 			if(system_task_return)	//TPAD����
			{  
		 		tmp_buf[4]|=0X03;	//����3,�˳�
				if(mode==0)			//����ģʽ,��Ҫ�����˳�ָ��
				{
					NRF24L01_TxPacket(tmp_buf);	//����ģʽ,�����˳�ָ��	
 				}else break;//����ģʽ,ֱ���˳�				 
 			}	    
			if(mode==0)//����ģʽ
			{	    
				res=btn_check(cbtn,&in_obj);//����ػ���ť
				if(res)//�ػ���ť����Ч����
				{
					if(((cbtn->sta&0X80)==0))//��ť״̬�ı���
					{ 
						tmp_buf[4]|=0X02;			//����2,����
			 			NRF24L01_TxPacket(tmp_buf);	//�������ָ��
		 			}	 
				}
	 			if(tp_dev.sta&TP_PRES_DOWN)			//������������
				{	
				 	if(tp_dev.y[0]<(lcddev.height-gui_phy.tbheight)&&tp_dev.y[0]>(gui_phy.tbheight+1))	//�ڻ�ͼ������
					{		
						x=tp_dev.x[0];
						y=tp_dev.y[0];
						tmp_buf[0]=tp_dev.x[0]>>8;
						tmp_buf[1]=tp_dev.x[0]&0xFF;
						tmp_buf[2]=tp_dev.y[0]>>8;	 
						tmp_buf[3]=tp_dev.y[0]&0xFF;  
						tmp_buf[4]|=0X01;			//����Ϊ1,������ͼ													        			   
						NRF24L01_TxPacket(tmp_buf);	//��������
					}
				}
			}else	//����ģʽ
			{
				if(NRF24L01_RxPacket(tmp_buf)==0)//һ�����յ���Ϣ,����ʾ����.
				{
					x=tmp_buf[0];
					x=(x<<8)+tmp_buf[1];
	 				y=tmp_buf[2];
					y=(y<<8)+tmp_buf[3];   
 				}	  
 			}
			if(tmp_buf[4]&0X7F)	//��Ҫ����
			{
				switch(tmp_buf[4]&0X7F)
				{
					case 1://��������
						if(tmp_buf[4]&0X80)//������
						{ 
							if(lastx==0XFFFF)//��һ��
							{
								lastx=x;
								lasty=y;
							}
							gui_draw_bline(lastx,lasty,x,y,2,RED);	//����
							lastx=x;
							lasty=y; 	
						}else paint_draw_point(x,y,RED,2);			//��ͼ,�뾶Ϊ2 
						break;
					case 2://���
						gui_fill_rectangle(0,gui_phy.tbheight,lcddev.width,lcddev.height-2*gui_phy.tbheight,LGRAY);//�����Ļ		
						break;
					case 3://�˳�						  
						rval=1;//��־�˳�
						break; 
				} 
			}else lastx=0XFFFF;
			delay_ms(1000/OS_TICKS_PER_SEC);//���� ��ʱһ��ʱ�ӽ��� 
		}   
	}  
  	SPI2_SetSpeed(SPI_SPEED_4);//�ָ�SPI1�ٶ�Ϊ18Mhz   
	btn_delete(cbtn);	//ɾ����ť
	return 0;
}

void rx_test(void)
{
	u8 res=0;
 	u8 tmp_buf[6];			//buf[0~3]:����ֵ;buf[4]:0,������ͼ;1,����;2,�˳�. 
 	u16 t=0;
 	NRF24L01_Init();    		//��ʼ��NRF24L01 
	while(NRF24L01_Check())		//��ⲻ��24L01,�������5��
	{
 		res++;
		if(res>5)return ;//��ʱδ��⵽,�˳�
	}	
	NRF24L01_RX_Mode();  			  
	while(1)
	{
 	    		    				 
		if(NRF24L01_RxPacket(tmp_buf)==0)//һ�����յ���Ϣ,����ʾ����.
		{
			tmp_buf[5]=0;//�����ַ��������� 
			LCD_ShowString(0,190,240,320,16,tmp_buf); 
		}else delay_us(100);	   
		t++;
		if(t==10000)//��Լ1s�Ӹı�һ��״̬
		{
			t=0;
			LED0=!LED0;
		} 				    
	};	 
}


























