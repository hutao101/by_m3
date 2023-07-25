#include "sys.h"
#include "delay.h"
#include "sample_usart.h"
#include "stm32f10x.h"



/*******************************************************************************
 * @brief   sample_uart_test_t2
 * Function 硬件流控模式：接收未完成时把RTS拉高
 * @author  taohu
 * @date    2022-1-6
 * @param   
		USARTx:USART基地址
 * @return void
*******************************************************************************/    
void sample_uart_t2(USART_TypeDef* USARTx)
{	   
	u16 temp,count = 0;
	printf("test is started\r\nfile:%s,line:%d\r\n",__FILE__,__LINE__);
	USARTx->CR1 = 0x0000;//FPGA_BIT中默认配置CR1为204C，启动之前清除配置
	read_ahb32(0xE000E280) = 0xFFFFFFFF;//清除PENDING位
	
	//sample_uart_set_param(eUSART_BAUD,1000000); //以115200初始化打印串口
	sample_uart_set_param(eUSART_FLOW,USART_HardwareFlowControl_RTS_CTS); //以115200初始化打印串口
    sample_uart_init(USARTx);	//以默认参数初始化打印串口

	while(1)
	{
		if(count > 10)
			break;
	
		temp = USARTx->SR;
		printf("SR:0x%x\r\n",temp);
		
		//delay_ms(10000);
		USART_SendData(USARTx, count++);
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)!=SET);//等待发送结束
		printf("DR:0x%x\r\n",USART_ReceiveData(USARTx));
	}
	

	//测试流程，释放资源
	//sample_uart_deinit(USARTx);	//测试完成，释放资源，
	printf("file:%s,line:%d\r\ntest is finished\r\n",__FILE__,__LINE__);

}
