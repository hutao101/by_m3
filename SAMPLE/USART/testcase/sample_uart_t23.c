#include "sys.h"
#include "delay.h"
#include "sample_usart.h"
#include "stm32f10x.h"


/*******************************************************************************
 * @brief   sample_uart_test_t23
 * Function 打开CTS流控模式，连续发送数据
 * @author  taohu
 * @date    2022-1-6
 * @param   
		USARTx:USART基地址
 * @return void
*******************************************************************************/    
void sample_uart_t23(USART_TypeDef* USARTx)
{	   
	u16 t,len,temp,count = 0;
	printf("test is started\r\nfile:%s,line:%d\r\n",__FILE__,__LINE__);
	USARTx->CR1 = 0x0000;//FPGA_BIT中默认配置CR1为204C，启动之前清除配置
	USARTx->CR2 = 0x0000;//FPGA_BIT中默认配置CR1为204C，启动之前清除配置
	read_ahb32(0xE000E280) = 0xFFFFFFFF;//清除PENDING位

    sample_uart_init(USARTx);	//以默认参数初始化打印串口

	while(1)
	{

		while(USART_GetFlagStatus(USARTx,USART_FLAG_RXNE)!=SET);//等待发送结束
		USART_ReceiverWakeUpCmd(USARTx, ENABLE);
		printf("DR:0x%x\r\n",USARTx->DR);
		printf("CR1:0x%x\r\n",USARTx->CR1);		
	
	}
	
	//测试流程，释放资源
	//sample_uart_deinit(USARTx);	//测试完成，释放资源，
	printf("file:%s,line:%d\r\ntest is finished\r\n",__FILE__,__LINE__);
}
