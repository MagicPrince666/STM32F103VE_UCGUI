#ifndef _PPU_H_
#define _PPU_H_

//assumes WORD = 16bit, BYTE = 8bit!! 
typedef unsigned short WORD;
typedef unsigned char  BYTE;
							 
//sprite �洢��		    
typedef struct
{
	u8 spr_ram[256];	//����洢�����洢��ͨ���� 
	u8 spr_addrcnt;		//sprite ��ַ������ 
}Spr_MemType;

typedef struct
{
 	u8	y;		//��ֱY������(��ֱλ��-1)��Vertical Position-1 (FFh,00h..EEh=Scanline 0..239, EFh..FEh=Not displayed)��
	u8	t_num;	//title �ţ�(R1.5 )0��(8*8: [7:0]title����) , 1:(8*16: [7:1]title����(0~127) ,[0] pattern 0 ��1)
	u8	attr;	//��ʾ���� [7]��ֱ��ת��[6]ˮƽ��ׯ��[5]��ʾ�ڱ�����(��),[4:2]δʹ�ã�[1:0]sprite palette(0~3��ɫ������)
	u8	x;		//ˮƽx������
}SpriteType;

//PPU �洢�� map		 
typedef struct
{    
	u16 PPU_addrcnt;		//PPU��ַ�������߰�λ����һ��д�룬 PPU��ַ�������Ͱ�λ����һ��д��*/
	u8  PPU_readtemp; 		//��ȡ�������� 
							//PPU �ڴ�ӳ�� 64KBѰַ 16KB($0000 ~ &3FFF)�����ڴ�,�����Ϊ����	 
	u8 *patterntable0;		//$0000 ~ $0FFF ͼ����0			 
	u8 *patterntable1; 		//$1000 ~ $1FFF ͼ����1				 
	u8 *name_table[4]; 		//$2000 ~ $23BF ������0��32x30�飩	 
							//$23C0 ~ $23FF ���Ա�0�� 			 
							//$2400 ~ $27BF ������1��32x30�飩	 
							//$27C0 ~ $27FF ���Ա�1 		 
							//$2800 ~ $2BBF ������2��32x30�飩	 
							//$2BC0 ~ $2BFF ���Ա�2���� 		 
							//$2C00 ~ $2FBF ������3��32x30�飩�� 
							//$2FC0 ~ $2FFF ���Ա�3 		 
	u8 image_palette[16];	//$3F00 ~ $3F0F ������ɫ��#1����ɫ����,ʹ����ɫֵ��RG565�� 
	u8 sprite_palette[16];	//$3F00 ~ $3F0F �����ɫ��#1����ɫ����,ʹ����ɫֵ��RG565�� 
}PPU_MemType;


//PPU �Ĵ���	 
typedef struct{
	u8 NES_R0;	 //$2000
	u8 NES_R1;	 //$2001
	u8 NES_R2;	 //$2002
//	u8 NES_R3;	 //$2003
//	u8 NES_R4;	 //$2004
	u8 NES_R5;	 //$2005
//	u8 NES_R6;	 //$2006
//	u8 NES_R7;	 //$2007
}PPU_RegType;

//PPU  ��־		  

//sprite ���Ա�־ 
#define SPR_VFLIP		0x80
#define SPR_HFLIP		0x40
#define SPR_BG_PRIO		0x20	//(0=Sprite In front of BG, 1=Sprite Behind BG)

//R0
#define R0_VB_NMI_EN	0x80
#define R0_SPR_SIZE		0x20	//(0=8x8, 1=8x16)
#define BG_PATTERN_ADDR	0x10
#define SPR_PATTERN_ADDR 0x08
#define PPU_ADDRINCR	0x04  	//bit2 ��0: +1�� ��1: +32��
#define R0_NAME_TABLE	0x03

//R1 
#define R1_SPR_VISIBLE	0x10
#define R1_BG_VISIBLE	0x08
#define R1_SPR_LEFT8	0x04
#define R1_BG_LEFT8		0x02
#define R1_DISPMODE		0x01

//R2 
#define R2_VBlank_Flag	0x80
#define R2_SPR0_HIT		0x40
#define R2_LOST_SPR		0x20


//����ʾ���й�  
#define NES_DISP_WIDTH					256	  //ÿһ��ɨ�����ؿ��
#define CLOCKS_PER_SCANLINE				113   //ÿһ��ɨ���ߣ�CPUʱ��113.66

//ɨ���кţ���ɨ��262��(0~261) 
#define SCAN_LINE_START_NUM				0
#define SCAN_LINE_DISPALY_START_NUM		21
#define	SCAN_LINE_DISPALY_END_NUM		261
#define	SCAN_LINE_END_NUM				262

//������ʾ name table �� 
#define NAME_TABLE_H_MASK	1
#define NAME_TABLE_V_MASK	2

///�ⲿ���� scanline��ʾ����	    
extern int PPU_scanline;
extern u8 SpriteHitFlag;

//�ⲿ���� �洢����������	  
extern Spr_MemType Spr_Mem;
extern PPU_RegType PPU_Reg;
extern PPU_MemType PPU_Mem;	    
extern SpriteType  * sprite;

//��������      
void  PPU_Init(u8* vromaddr, u8  ScreenMirrorType);

//PPU �洢����Ĵ��������� 		 
void PPU_MemWrite(u8 value);
u8 	 PPU_MemRead(void);	 
void PPU_RegWrite(u16 addr, u8 byte);
u8 	 PPU_RegRead(u16 addr);	 
u8 	 PPU_NameTablesRead(void);
void PPU_NameTablesWrite(u8 value);    
void NES_GetSpr0HitFlag(int y_axes);
void NES_RenderSprite88(SpriteType *sprptr, int dy_axes);
void NES_RenderSprite16(SpriteType *sprptr, int dy_axes);
void NES_RenderLine(int y_axes);
void NES_LCD_DisplayLine(int y_axes, u16 *Disaplyline_buffer);
void NES_LCD_BG_DisplayLine(u16 color);		  
#endif 







