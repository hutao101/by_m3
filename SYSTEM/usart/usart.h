#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 

#ifndef USART_TEST   //���������USART����
#define by_debug


#define USART_REC_LEN  			200  	//�����������ֽ��� 200

extern u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
extern u16 USART_RX_STA;       //����״̬���	  
#endif

void uart_init(u32 bound);
#endif


