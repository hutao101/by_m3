#include "sys.h"
#include "delay.h"
#include "sample_usart.h"
#include "stm32f10x.h"



/*******************************************************************************
 * @brief   sample_uart_test_t1
 * Function 全双工异步通信遍历停止位：1/2位
 * @author  taohu
 * @date    2022-1-6
 * @param   
		USARTx:USART基地址
 * @return void
*******************************************************************************/    
void sample_uart_t15(USART_TypeDef* USARTx)
{	   
   u16 temp = 0 ;  
	

	sample_uart_set_param(eUSART_BAUD,eUSART_BAUD_115200); //以115200初始化打印串口
    sample_uart_init(USARTx);	//初始化打印串口
    USART_ITConfig(USARTx, USART_IT_RXNE, DISABLE); //关闭接收中断
	
    //printf("test is started\r\nfile:%s,line:%d\r\n",__FILE__,__LINE__);

	//发送数据0X55
	USART_SendData(USARTx, 0x55);//向串口1发送数据
	while(USART_GetFlagStatus(USARTx,USART_FLAG_RXNE) != SET);//等待发送结束
	temp = USART_ReceiveData(USARTx);
	delay_us(50);
	
	if(temp == 0x55)
	{
		sample_uart_set_param(eUSART_STPB,USART_StopBits_2); 
		sample_uart_init(USARTx);	//初始化打印串口
		USART_ITConfig(USARTx, USART_IT_RXNE, DISABLE); //关闭接收中断	
		
		USART_SendData(USARTx, 0xAA);//向串口发送数据
		while(USART_GetFlagStatus(USARTx,USART_FLAG_RXNE) != SET);//等待发送结束
		temp = USART_ReceiveData(USARTx);

		USART_SendData(USARTx, temp);//向串口发送数据
		while(USART_GetFlagStatus(USARTx,USART_FLAG_RXNE) != SET);//等待发送结束
	}
	//sample_uart_deinit(USARTx);	//复位USART1，
	//printf("file:%s,line:%d\r\ntest is finished\r\n",__FILE__,__LINE__);
}

