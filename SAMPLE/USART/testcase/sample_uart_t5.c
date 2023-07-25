#include "sys.h"
#include "delay.h"
#include "sample_usart.h"
#include "stm32f10x.h"



/*******************************************************************************
 * @brief   sample_uart_test_t5
 * Function 红外发送模式
 * @author  taohu
 * @date    2022-1-6
 * @param   
		USARTx:USART基地址
 * @return void
*******************************************************************************/    
void sample_uart_t5(USART_TypeDef* USARTx)
{	   
	u16 count = 0;
	//初始化红外参数
	USART_SetPrescaler(USARTx, 0x02);
	USART_IrDAConfig(USARTx, USART_IrDAMode_LowPower);
	USART_IrDACmd(USARTx, ENABLE);

	sample_uart_set_param(eUSART_BAUD,eUSART_BAUD_9600); //以115200初始化打印串口
    sample_uart_init(USARTx);	//以默认参数初始化打印串口

	printf("test is started\r\nfile:%s,line:%d\r\n",__FILE__,__LINE__);
	
	while(1)
	{
		//测试流程，测试完成释放WHILE循环
		delay_ms(5000);
		USART_SendData(USARTx, 0x55);
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)!=SET);//等待发送结束
		
		//f(count++ > 10)
			//break;
	}

	//测试流程，释放资源
	printf("file:%s,line:%d\r\ntest is finished\r\n",__FILE__,__LINE__);
}
