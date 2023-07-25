#include "sys.h"
#include "delay.h"
#include "sample_usart.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_uart_test_t11
 * Function LIN Master:发送和接收
 * @author  taohu
 * @date    2022-1-6
 * @param   
		USARTx:USART基地址
 * @return void
*******************************************************************************/    
void sample_uart_t11(USART_TypeDef* USARTx)
{	   
	u32 i;
	
	printf("test is started\r\nfile:%s,line:%d\r\n",__FILE__,__LINE__);
	
	USARTx->CR1 = 0x0000;//FPGA_BIT中默认配置CR1为204C，启动之前清除配置
	read_ahb32(0xE000E280) = 0xFFFFFFFF;//清除PENDING位
	
	sample_uart_init(USARTx);
	USART_LINCmd(USARTx, ENABLE);
	
	//关掉NVIC使能
	sample_uart_nvic_cfg((u32)USARTx,DISABLE);
	
	USART_ITConfig(USARTx, USART_IT_LBD, ENABLE);//开启串口接受中断
	USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);//开启串口接受中断

	
	USART_SendBreak(USARTx); //发生断开帧
	printf("SR:%x\r\n",USARTx->SR);
	
	//发送报头
	USART_SendData(USARTx, 0x55);
	while(USART_GetITStatus(USARTx, USART_IT_TXE) != SET);
	
	USART_SendData(USARTx, 0x01);
	while(USART_GetITStatus(USARTx, USART_IT_TXE) != SET);
	
	//发送响应帧,不做校验
	for(i = 0;i < 5;i++)
	{
		USART_SendData(USARTx, i);
		while(USART_GetITStatus(USARTx, USART_IT_TXE) != SET);
	}
	
	//发送报头,从机发送
	USART_SendBreak(USARTx); //发生断开帧
	printf("SR:%x\r\n",USARTx->SR);
	
	USART_SendData(USARTx, 0x55);
	while(USART_GetITStatus(USARTx, USART_IT_TXE) != SET);
	USART_SendData(USARTx, 0x32);
	while(USART_GetITStatus(USARTx, USART_IT_TXE) != SET);

	while(USART_GetITStatus(USARTx, USART_IT_LBD) != SET);
	printf("RDR:%x\r\n",USART_ReceiveData(USARTx));
	printf("RDR:%x\r\n",USART_ReceiveData(USARTx));
	printf("RDR:%x\r\n",USART_ReceiveData(USARTx));
	printf("RDR:%x\r\n",USART_ReceiveData(USARTx));
	printf("RDR:%x\r\n",USART_ReceiveData(USARTx));
	printf("file:%s,line:%d\r\ntest is finished\r\n",__FILE__,__LINE__);
}

