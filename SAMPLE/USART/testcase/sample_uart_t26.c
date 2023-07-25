#include "sys.h"
#include "delay.h"
#include "sample_usart.h"
#include "stm32f10x.h"



/*******************************************************************************
 * @brief   sample_uart_test_t1
 * Function 打开CTS流控模式，连续发送数据
 * @author  taohu
 * @date    2022-1-6
 * @param   
		USARTx:USART基地址
 * @return void
*******************************************************************************/    
void sample_uart_t26(USART_TypeDef* USARTx)
{
	printf("test is started\r\nfile:%s,line:%d\r\n",__FILE__,__LINE__);
	USARTx->CR1 = 0x0000;//FPGA_BIT中默认配置CR1为204C，启动之前清除配置

#if 0
	//TXE中断打开
	sample_uart_init(USARTx);	//以默认参数初始化打印串口

	USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);
#endif

#if 0
	//TC中断打开
	sample_uart_init(USARTx);	//以默认参数初始化打印串口

	USART_ITConfig(USARTx, USART_IT_TC, ENABLE);
#endif

#if 0
	//RXNE中断打开
	sample_uart_init(USARTx);	//以默认参数初始化打印串口
	
	USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);
#endif

#if 0
	//RXNE中断打开,测试ORE
	sample_uart_init(USARTx);	//以默认参数初始化打印串口
	
	USART_ITConfig(USARTx, USART_IT_ORE, ENABLE);
#endif

#if 0
	//PE中断打开
	sample_uart_set_param(eUSART_PRT,USART_Parity_Odd);
	sample_uart_init(USARTx);	//以默认参数初始化打印串口
	
	USART_ITConfig(USARTx, USART_IT_PE, ENABLE);
#endif

#if 0
		//PE中断打开
		sample_uart_init(USARTx);	//以默认参数初始化打印串口
		
		USART_ITConfig(USARTx, USART_IT_LBD, ENABLE);
		USART_LINCmd(USARTx, ENABLE);
#endif

#if 0
			//CTS中断打开
			sample_uart_set_param(eUSART_FLOW,USART_HardwareFlowControl_CTS);
			sample_uart_init(USARTx);	//以默认参数初始化打印串口
			
			USART_ITConfig(USARTx, USART_IT_CTS, ENABLE);
#endif

#if 1
				//IDLE中断打开
				sample_uart_init(USARTx);	//以默认参数初始化打印串口
				
				USART_ITConfig(USARTx, USART_IT_IDLE, ENABLE);
#endif

	printf("PD0:0x%x\r\n",read_ahb32(0xE000E200));

	while(1)
	{
		//printf("SR:0x%x",USARTx->SR);
	}	

	//sample_uart_deinit(USARTx);	//外设deinit
	//printf("file:%s,line:%d\r\ntest is finished\r\n",__FILE__,__LINE__);
}
