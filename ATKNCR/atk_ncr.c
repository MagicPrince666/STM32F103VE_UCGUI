#include "atk_ncr.h"
#include "malloc.h"					   										   
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//
//��������ĸʶ�������ALIENTEK�ṩ,�����ṩ2��LIB,�����ʹ��
//ATKNCR_M_Vx.x.lib��ATKNCR_N_Vx.x.lib��Ψһ�������Ƿ�ʹ�ö�̬�ڴ����.
//����:M,������Ҫ�õ�malloc�İ汾,����ʵ��alientek_ncr_malloc��alientek_ncr_free��������
//     N,������ͨ�汾,����Ҫʵ��alientek_ncr_malloc��alientek_ncr_free��������
//     Vx.x,����ǰʶ�����İ汾.		 	  
//����:֧������/Сд��ĸ/��д��ĸ/�������ʶ��ģʽ.		  
//��ʶ�����ʹ�������൱��.					   
//��һ��:����alientek_ncr_init����,��ʼ��ʶ�����
//�ڶ���:��ȡ����ĵ�������(������2�������ϵĲ�ͬ������������)
//������:����alientek_ncr����,�õ�ʶ����.
//���Ĳ�:�������Ҫ��ʶ��,�����alientek_ncr_stop����,��ֹʶ��.�������Ҫ����,���ظ�2,3������.		   
//�汾:V2.0
//����ԭ��@ALIENTEK
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved													    								  
//��Ȩ���У�����ؾ���
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//�ڴ����ú���
void alientek_ncr_memset(char *p,char c,unsigned long len) 
{
	mymemset((u8*)p,(u8)c,(u32)len);
}	 					  
//�ڴ����뺯��
void *alientek_ncr_malloc(unsigned int size) 
{
	return mymalloc(size);
}
//�ڴ���պ���
void alientek_ncr_free(void *ptr) 
{
	myfree(ptr);
}		  
		  



















