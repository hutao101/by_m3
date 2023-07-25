#ifndef __MALLOC_H
#define __MALLOC_H
#include "stm32f10x.h"
 
#ifndef NULL
#define NULL 0
#endif

//���������ڴ��
#define SRAMIN	 0		//�ڲ��ڴ��
#define SRAMEX   1		//�ⲿ�ڴ�� 

#define SRAMBANK 	1	//����֧�ֵ�SRAM����.	


//mem1�ڴ�����趨.mem1��ȫ�����ڲ�SRAM����.
#define MEM1_BLOCK_SIZE			32  	  						//�ڴ���СΪ32�ֽ�
#define MEM1_MAX_SIZE			8*1024  						//�������ڴ� 40K
#define MEM1_ALLOC_TABLE_SIZE	MEM1_MAX_SIZE/MEM1_BLOCK_SIZE 	//�ڴ���С

//mem2�ڴ�����趨.mem2���ڴ�ش����ⲿSRAM����
//#define MEM2_BLOCK_SIZE			32  	  					//�ڴ���СΪ32�ֽ�
//#define MEM2_MAX_SIZE			960 *1024  						//�������ڴ�960K
//#define MEM2_ALLOC_TABLE_SIZE	MEM2_MAX_SIZE/MEM2_BLOCK_SIZE 	//�ڴ���С 
		 
 
//�ڴ���������
struct _m_mallco_dev
{
	void (*init)(void);					//��ʼ��
	u8 (*perused)(void);		  	    	//�ڴ�ʹ����
	u8 	*membase[SRAMBANK];				//�ڴ�� ����SRAMBANK��������ڴ�
	u16 *memmap[SRAMBANK]; 				//�ڴ����״̬��
	u8  memrdy[SRAMBANK]; 				//�ڴ�����Ƿ����
};
extern struct _m_mallco_dev mallco_dev;	 //��mallco.c���涨��

void mem_set(void *s,u8 c,u32 count);	//�����ڴ�
void mem_cpy(void *des,void *src,u32 n);//�����ڴ�     
void mem_init(void);				//�ڴ�����ʼ������(��/�ڲ�����)
u32 mem_malloc(u32 size);	//�ڴ����(�ڲ�����)
u8 mem_free(u32 offset);		//�ڴ��ͷ�(�ڲ�����)
u8 mem_perused(void);				//����ڴ�ʹ����(��/�ڲ�����) 
////////////////////////////////////////////////////////////////////////////////
//�û����ú���
void by_free(void *ptr);  			//�ڴ��ͷ�(�ⲿ����)
void *by_malloc(u32 size);			//�ڴ����(�ⲿ����)
void *by_realloc(void *ptr,u32 size);//���·����ڴ�(�ⲿ����)
#endif













