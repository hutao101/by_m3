#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 

#ifndef USART_TEST   //如果定义了USART测试
#define by_debug


#define USART_REC_LEN  			200  	//定义最大接收字节数 200

extern u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
extern u16 USART_RX_STA;       //接收状态标记	  
#endif

void uart_init(u32 bound);
#endif


