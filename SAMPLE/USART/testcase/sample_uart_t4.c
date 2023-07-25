#include "sys.h"
#include "delay.h"
#include "sample_usart.h"
#include "stm32f10x.h"



/*******************************************************************************
 * @brief   sample_uart_t4
 * Function 红外模式：接收模式
 * @author  taohu
 * @date    2022-3-21
 * @param   
		USARTx:USART基地址
 * @return void
*******************************************************************************/    
void sample_uart_t4(USART_TypeDef* USARTx)
{	   
    u16 count = 0;  
	
	//初始化红外参数
	USART_SetPrescaler(USARTx, 0x01);
	USART_IrDAConfig(USARTx, USART_IrDAMode_Normal);
	USART_IrDACmd(USARTx, ENABLE);
	
	sample_uart_set_param(eUSART_BAUD,9600); //以115200初始化打印串口
    sample_uart_init(USARTx);	//以默认参数初始化打印串口

	printf("test is started\r\nfile:%s,line:%d\r\n",__FILE__,__LINE__);
	
	while(1)
	{
		if(USART_GetFlagStatus(USARTx,USART_FLAG_RXNE)==SET)//等待发送结束
		{
			//printf("receive data:0x%x\r\n",USARTx->SR);
			printf("receive data:0x%x\r\n",USART_ReceiveData(USARTx));
			
			if(count++ >10)
				break;
		}
	}
	
	printf("file:%s,line:%d\r\ntest is finished\r\n",__FILE__,__LINE__);
}
