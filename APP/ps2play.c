#include "ps2play.h"
//#include "joypad.h"
#include "pstwo.h"


u8 PS2_Play(u8* caption)	 
{
	u8 rval=0; 
	u8 key;
	u16 sx=(lcddev.width-120)/2;
	u16 sy=(lcddev.height-160)/2;
	_window_obj* twin=0;	//窗体
//	s16 speed; 
//	s16 swerve;           //转弯量	  
		
 	twin=window_creat(sx,sy,120,160,0,1<<6|1<<0,16);//创建窗口
	if(twin)
	{	
		twin->caption=caption;					 
	 	twin->windowbkc=APP_WIN_BACK_COLOR;	 								//窗体主色
		window_draw(twin);													//画窗体
	 	app_draw_smooth_line(sx+5,sy+32+1+22,110,1,0Xb1ffc4,0X1600b1);		//画彩线
	 	app_draw_smooth_line(sx+5,sy+32+1+22+22,110,1,0Xb1ffc4,0X1600b1);	//画彩线
		app_draw_smooth_line(sx+5,sy+32+1+22+44,110,1,0Xb1ffc4,0X1600b1);	//画彩线
		app_draw_smooth_line(sx+5,sy+32+1+22+66,110,1,0Xb1ffc4,0X1600b1);	//画彩线
		app_draw_smooth_line(sx+5,sy+32+1+22+88,110,1,0Xb1ffc4,0X1600b1);	//画彩线
		BACK_COLOR=APP_WIN_BACK_COLOR;
 		//显示条目信息
		gui_show_ptstr(sx+8,sy+32+1+3,sx+8+56,sy+32+1+3+16,0,BLACK,16,"key:",1);
		gui_show_ptstr(sx+8,sy+32+1+3+22,sx+8+56,sy+32+1+3+22+16,0,BLACK,16,"LX:",1);
		gui_show_ptstr(sx+8,sy+32+1+3+44,sx+8+56,sy+32+1+3+44+16,0,BLACK,16,"LY:",1);
		gui_show_ptstr(sx+8,sy+32+1+3+66,sx+8+56,sy+32+1+3+66+16,0,BLACK,16,"RX:",1);
		gui_show_ptstr(sx+8,sy+32+1+3+88,sx+8+56,sy+32+1+3+88+16,0,BLACK,16,"RY:",1);

 	}else rval=1;
	if(rval==0)
	{
		BACK_COLOR=APP_WIN_BACK_COLOR;	//背景色为窗体主色
		POINT_COLOR=RED;				//红色的字
		PS2_Init();			 //驱动端口初始化
		PS2_SetInit();		 //配配置初始化,配置“红绿灯模式”，并选择是否可以修改
	                     //开启震动模式
		while(1)
		{
			if(system_task_return)break;//需要返回
/*			
			if( !PS2_RedLight()) //判断手柄是否为红灯模式，是，指示灯LED点亮
			{
				PS2_DataKey();	 //手柄按键捕获处理
				speed = PS2_AnologData(PSS_LY)-127;	   
				swerve = (PS2_AnologData(PSS_RX)-128)*2.3*((float)abs(speed)/128); //	speed取绝对值，	算数运算，得到转弯量。
				speed = -(PS2_AnologData(PSS_LY)-127)*7;	   //正：前进；  负：后退
			}
*/
				key=PS2_DataKey();
				if(key!=0)                   //有按键按下
				{
					if(key == 11)
					{
						PS2_Vibration(0xFF,0x00);  //发出震动后必须有延时  delay_ms(1000);
						delay_ms(500);
					}
					else if(key == 12)
					{
						PS2_Vibration(0x00,0xFF);  //发出震动后必须有延时  delay_ms(1000);
						delay_ms(500);
					}
					else
						PS2_Vibration(0x00,0x00); 
				}
				
				LCD_ShowNum(sx+7*8,sy+32+1+3,key,3,16);//显示模拟量
				LCD_ShowNum(sx+7*8,sy+32+1+3+22,PS2_AnologData(PSS_LX),3,16);//显示模拟量
				LCD_ShowNum(sx+7*8,sy+32+1+3+44,PS2_AnologData(PSS_LY),3,16);		//显示模拟量
			  LCD_ShowNum(sx+7*8,sy+32+1+3+66,PS2_AnologData(PSS_RX),3,16);		//显示模拟量
				LCD_ShowNum(sx+7*8,sy+32+1+3+88,PS2_AnologData(PSS_RY),3,16);		//显示模拟量
				
				LED1=~LED1;

			delay_ms(50);
		}	
  }
	window_delete(twin);
	return rval;
}

