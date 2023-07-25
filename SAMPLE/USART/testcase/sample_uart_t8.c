#include "sys.h"
#include "delay.h"
#include "sample_usart.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_uart_test_t8
 * Function 全双工异步通信：遍历波特率1.2K-4.5M；测试UART和电脑端通信功能
 * @author  taohu
 * @date    2022-1-6
 * @param   
		USARTx:USART基地址
 * @return void
*******************************************************************************/    
void sample_uart_t8(USART_TypeDef* USARTx)
{	
	USART_ClockInitTypeDef USART_ClockInitStructure;  
	u8 count = 0;
	printf("test is started\r\nfile:%s,line:%d\r\n",__FILE__,__LINE__);
	USARTx->CR1 = 0x0000;//FPGA_BIT中默认配置CR1为204C，启动之前清除配置
	
	/* USART Clock set to 3.6 MHz (PCLK1 (36 MHZ) / 10) */
	USART_SetPrescaler(USARTx, 0x05);

	/* USART Guard Time set to 16 Bit */
	USART_SetGuardTime(USARTx,0x02);

	USART_ClockInitStructure.USART_Clock = USART_Clock_Enable;
	USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
	USART_ClockInitStructure.USART_CPHA = USART_CPHA_1Edge;
	USART_ClockInitStructure.USART_LastBit = USART_LastBit_Enable;
	USART_ClockInit(USARTx, &USART_ClockInitStructure);

	sample_uart_set_param(eUSART_BAUD,9600);
	sample_uart_set_param(eUSART_WORDLEN,USART_WordLength_9b);
	sample_uart_set_param(eUSART_STPB,USART_StopBits_1_5);
	sample_uart_set_param(eUSART_PRT,USART_Parity_Even);
    sample_uart_init(USARTx);	//以默认参数初始化打印串口
    
	USART_ITConfig(USARTx, USART_IT_PE, ENABLE);

	/* Enable the NACK Transmission */
	USART_SmartCardNACKCmd(USARTx, ENABLE);

	/* Enable the Smartcard Interface */
	USART_SmartCardCmd(USARTx, ENABLE);
  
	printf("CR1:%x\r\n",USARTx->CR1);
	printf("SR1:%x\r\n",USARTx->SR);
	printf("PD0:0x%x\r\n",read_ahb32(0xE000E200));

	while(1)
	{
		USART_SetGuardTime(USARTx,count += 4);
		USART_SendData(USARTx, 0x55);//向串口1发送数据
		printf("SR:0x%x\r\n",USARTx->SR);
		printf("CR1:0x%x,CR2:0x%x,CR3:0x%x\r\n",USARTx->CR1,USARTx->CR2,USARTx->CR3);
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)!=SET);//等待发送结束
	}	

	//sample_uart_deinit(USARTx);	//外设deinit
	//printf("file:%s,line:%d\r\ntest is finished\r\n",__FILE__,__LINE__);
}

