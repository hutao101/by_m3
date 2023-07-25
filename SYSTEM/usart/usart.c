#include "sys.h"
#include "usart.h"	   

//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{

#ifdef by_debug
    while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;
    return ch;
#endif

}
#endif 


#ifndef USART_TEST  

u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节

//接收状态
//bit15，	接收到0x0a,/n标志
//bit14，	接收到0x0d，/r标志
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	  

void uart_init(u32 bound)
{
	USART_InitTypeDef USART_InitStructure;
	/*
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟

	//USART1_TX   GPIOA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9

	//USART1_RX	  GPIOA.10初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	*/

	//USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

	USART_Init(USART1, &USART_InitStructure); //初始化串口1
	//USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启串口接受中断
	USART_Cmd(USART1, ENABLE);                    //使能串口1 
  
	printf("Uart Init：%d\r\n",bound);

}

#endif

/*******************************************************************************
 * @brief   uart_int_handle
 * Function UART中断处理函数
 * @author  taohu
 * @date    2022-1-6
 * @param   
		USARTx:USART基地址
 * @return void
*******************************************************************************/
void uart_rev_int_handle(USART_TypeDef* USARTx)
{
	u8 Res;
	//printf("SR:0x%x",USARTx->SR);
	if(USART_GetITStatus(USARTx, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		if(USART_GetITStatus(USARTx, USART_IT_PE) != RESET)
		{
			Res =USART_ReceiveData(USARTx);	//读取接收到的数据
			if(Res >= 0x0F)
				USART_SendData(USARTx,Res);
			//printf("%x",Res);	
			if((Res&0x7f) <= 0x0F)
				USART_RX_STA|=0x8000;
		}
		else
		{
			Res =USART_ReceiveData(USARTx);	//读取接收到的数据
			printf("0x%x\r\n",Res);	
		}
		if((USART_RX_STA&0x8000)==0)//接收未完成
		{
			if(USART_RX_STA&0x4000)//接收到了0x0d,回车键/r
			{
					if(Res!=0x0a)	//接收到了0x0a,换行键/n
						USART_RX_STA=0;//接收错误,重新开始
					else 
						USART_RX_STA|=0x8000;	//接收完成了 
			}
			else //还没收到0X0D
			{	
				if(Res==0x0d)
					USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))
						USART_RX_STA=0;//接收数据错误,重新开始接收	  
				}		 
			}
		}   		 
    } 
}


void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	uart_rev_int_handle(USART1);
} 

void USART2_IRQHandler(void)                	//串口2中断服务程序
{
	uart_rev_int_handle(USART2);
} 

void USART3_IRQHandler(void)                	//串口3中断服务程序
{
	uart_rev_int_handle(USART3);
} 

void UART4_IRQHandler(void)                	//串口3中断服务程序
{
	uart_rev_int_handle(UART4);
} 

void UART5_IRQHandler(void)                	//串口3中断服务程序
{
	uart_rev_int_handle(UART5);
} 

