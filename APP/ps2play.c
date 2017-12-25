#include "ps2play.h"
//#include "joypad.h"
#include "pstwo.h"


u8 PS2_Play(u8* caption)	 
{
	u8 rval=0; 
	u8 key;
	u16 sx=(lcddev.width-120)/2;
	u16 sy=(lcddev.height-160)/2;
	_window_obj* twin=0;	//����
//	s16 speed; 
//	s16 swerve;           //ת����	  
		
 	twin=window_creat(sx,sy,120,160,0,1<<6|1<<0,16);//��������
	if(twin)
	{	
		twin->caption=caption;					 
	 	twin->windowbkc=APP_WIN_BACK_COLOR;	 								//������ɫ
		window_draw(twin);													//������
	 	app_draw_smooth_line(sx+5,sy+32+1+22,110,1,0Xb1ffc4,0X1600b1);		//������
	 	app_draw_smooth_line(sx+5,sy+32+1+22+22,110,1,0Xb1ffc4,0X1600b1);	//������
		app_draw_smooth_line(sx+5,sy+32+1+22+44,110,1,0Xb1ffc4,0X1600b1);	//������
		app_draw_smooth_line(sx+5,sy+32+1+22+66,110,1,0Xb1ffc4,0X1600b1);	//������
		app_draw_smooth_line(sx+5,sy+32+1+22+88,110,1,0Xb1ffc4,0X1600b1);	//������
		BACK_COLOR=APP_WIN_BACK_COLOR;
 		//��ʾ��Ŀ��Ϣ
		gui_show_ptstr(sx+8,sy+32+1+3,sx+8+56,sy+32+1+3+16,0,BLACK,16,"key:",1);
		gui_show_ptstr(sx+8,sy+32+1+3+22,sx+8+56,sy+32+1+3+22+16,0,BLACK,16,"LX:",1);
		gui_show_ptstr(sx+8,sy+32+1+3+44,sx+8+56,sy+32+1+3+44+16,0,BLACK,16,"LY:",1);
		gui_show_ptstr(sx+8,sy+32+1+3+66,sx+8+56,sy+32+1+3+66+16,0,BLACK,16,"RX:",1);
		gui_show_ptstr(sx+8,sy+32+1+3+88,sx+8+56,sy+32+1+3+88+16,0,BLACK,16,"RY:",1);

 	}else rval=1;
	if(rval==0)
	{
		BACK_COLOR=APP_WIN_BACK_COLOR;	//����ɫΪ������ɫ
		POINT_COLOR=RED;				//��ɫ����
		PS2_Init();			 //�����˿ڳ�ʼ��
		PS2_SetInit();		 //�����ó�ʼ��,���á����̵�ģʽ������ѡ���Ƿ�����޸�
	                     //������ģʽ
		while(1)
		{
			if(system_task_return)break;//��Ҫ����
/*			
			if( !PS2_RedLight()) //�ж��ֱ��Ƿ�Ϊ���ģʽ���ǣ�ָʾ��LED����
			{
				PS2_DataKey();	 //�ֱ�����������
				speed = PS2_AnologData(PSS_LY)-127;	   
				swerve = (PS2_AnologData(PSS_RX)-128)*2.3*((float)abs(speed)/128); //	speedȡ����ֵ��	�������㣬�õ�ת������
				speed = -(PS2_AnologData(PSS_LY)-127)*7;	   //����ǰ����  ��������
			}
*/
				key=PS2_DataKey();
				if(key!=0)                   //�а�������
				{
					if(key == 11)
					{
						PS2_Vibration(0xFF,0x00);  //�����𶯺��������ʱ  delay_ms(1000);
						delay_ms(500);
					}
					else if(key == 12)
					{
						PS2_Vibration(0x00,0xFF);  //�����𶯺��������ʱ  delay_ms(1000);
						delay_ms(500);
					}
					else
						PS2_Vibration(0x00,0x00); 
				}
				
				LCD_ShowNum(sx+7*8,sy+32+1+3,key,3,16);//��ʾģ����
				LCD_ShowNum(sx+7*8,sy+32+1+3+22,PS2_AnologData(PSS_LX),3,16);//��ʾģ����
				LCD_ShowNum(sx+7*8,sy+32+1+3+44,PS2_AnologData(PSS_LY),3,16);		//��ʾģ����
			  LCD_ShowNum(sx+7*8,sy+32+1+3+66,PS2_AnologData(PSS_RX),3,16);		//��ʾģ����
				LCD_ShowNum(sx+7*8,sy+32+1+3+88,PS2_AnologData(PSS_RY),3,16);		//��ʾģ����
				
				LED1=~LED1;

			delay_ms(50);
		}	
  }
	window_delete(twin);
	return rval;
}

