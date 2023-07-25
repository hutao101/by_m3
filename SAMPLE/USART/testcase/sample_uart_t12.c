#include "sys.h"
#include "delay.h"
#include "sample_usart.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_uart_test_t1
 * Function LIN模式：主机发送和接收
 * @author  taohu
 * @date    2022-3-17
 * @param   
		USARTx:USART基地址
 * @return void
*******************************************************************************/    
void sample_uart_t12(USART_TypeDef* USARTx)
{	   
	u32 i;
	
	printf("test is started\r\nfile:%s,line:%d\r\n",__FILE__,__LINE__);
	
	USARTx->CR1 = 0x0000;//FPGA_BIT中默认配置CR1为204C，启动之前清除配置
	read_ahb32(0xE000E280) = 0xFFFFFFFF;//清除PENDING位
	
	sample_uart_init(USARTx);
	
	USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);//开启串口发送中断
	USART_ITConfig(USARTx, USART_IT_LBD, ENABLE);//开启串口BREAK帧检测中断
	
	USART_LINCmd(USARTx, ENABLE);
	
	while(1)
	{
		if(USART_GetITStatus(USARTx, USART_IT_LBD) != RESET)
		{
			printf("SR:%x",USARTx->SR);
			if(USART_GetITStatus(USARTx, USART_IT_RXNE) != RESET)
			{
				if(USART_ReceiveData(USARTx) == 0x32)
				{
					//发送响应帧,不做校验
					for(i = 0;i < 5;i++)
					{
						USART_SendData(USARTx, i); 
						while(USART_GetITStatus(USARTx, USART_IT_TXE) != SET);
					}		
				}
			}
		}
	}
	//printf("file:%s,line:%d\r\ntest is finished\r\n",__FILE__,__LINE__);
}

