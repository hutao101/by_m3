#include "sys.h"
#include "delay.h"
#include "sample_usart.h"
#include "stm32f10x.h"



/*******************************************************************************
 * @brief   sample_uart_test_t1
 * Function 全双工异步通信遍历校验位：无/奇/偶
 * @author  taohu
 * @date    2022-1-6
 * @param   
		USARTx:USART基地址
 * @return void
*******************************************************************************/    
void sample_uart_t16(USART_TypeDef* USARTx)
{	   
    u16 t,count = 0;  
    u16 len; 
	

	sample_uart_set_param(eUSART_BAUD,eUSART_BAUD_115200); //以115200初始化打印串口
    sample_uart_init(USARTx);	//以默认参数初始化打印串口

	printf("test is started\r\nfile:%s,line:%d\r\n",__FILE__,__LINE__);
	//sample_uart_set_param(eUSART_BAUD,eUSART_BAUD_9600); //恢复默认测试参数
    //sample_uart_init(USARTx);	//测试串口初始化
    USART_ITConfig(USARTx, USART_IT_PE, ENABLE);//开启串口接收中断

	//测试流程，测试完成后应结束while循环
	while(1)
	{
	    if(USART_RX_STA&0x8000)
		{	
			//printf("SR1:%x\r\n",USARTx->SR);
			//发送收到的数据
			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
			for(t=0;t<len;t++)
			{
				USART_SendData(USARTx, USART_RX_BUF[t]);//向串口1发送数据
				while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)!=SET);//等待发送结束
			}
			USART_RX_STA=0;
			
			//如果遍历完成，退出测试;电脑端不支持2.25M/4.5M波特率；这里不测这两个波特率
			if(++count >= USART_PRT_COUNT)
				break;
			
			delay_ms(20000);
			//重新初始化USARTx,遍历波特率
			sample_uart_set_param(eUSART_PRT,Prt[++count]);
			//sample_uart_deinit(USARTx);	//复位测试USART，
			sample_uart_init(USARTx);	//测试串口初始化
		} 
	}

	//sample_uart_deinit(USARTx);	//测试完成，释放资源，
	//printf("file:%s,line:%d\r\ntest is finished\r\n",__FILE__,__LINE__);
}
