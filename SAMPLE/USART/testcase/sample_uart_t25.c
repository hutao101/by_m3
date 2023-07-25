#include "sys.h"
#include "delay.h"
#include "sample_usart.h"
#include "stm32f10x.h"



/*******************************************************************************
 * @brief   sample_uart_test_t1
 * Function 单线半双工模式通信测试
 * @author  taohu
 * @date    2022-1-6
 * @param   
		USARTx:USART基地址
 * @return void
*******************************************************************************/    
void sample_uart_t25(USART_TypeDef* USARTx)
{	   
	u16 t,len,count;
	
	printf("test is started\r\nfile:%s,line:%d\r\n",__FILE__,__LINE__);
	USARTx->CR1 = 0x0000;//FPGA_BIT中默认配置CR1为204C，启动之前清除配置
	read_ahb32(0xE000E280) = 0xFFFFFFFF;//清除PENDING位
	
	USART_HalfDuplexCmd(USARTx, ENABLE);
    sample_uart_init(USARTx);	//以默认参数初始化打印串口

	 while(1)
    {	
		
//		delay_ms(200);
//		printf("receive data:0x%x\r\n",USART_ReceiveData(USARTx));
		
		USART_SendData(USARTx, count++);//向串口1发送数据
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)!=SET);//等待发送结束
		delay_ms(2000);
		printf("abc");
		
//		if(USART_RX_STA&0x8000)
//		{	
//			//printf("SR1:%x\r\n",USARTx->SR);
//			//发送收到的数据
//			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
//			for(t=0;t<len;t++)
//			{
//				USART_SendData(USARTx, ~USART_RX_BUF[t]);//向串口1发送数据
//				while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)!=SET);//等待发送结束
//			}
//			USART_RX_STA=0;
//			
//			//如果遍历完成，退出测试;电脑端不支持2.25M/4.5M波特率；这里不测这两个波特率
//			if(count++ >= 5)
//				break;
//		} 
	}

	//测试流程，释放资源
	//sample_uart_deinit(USARTx);	//测试完成，释放资源，
	printf("file:%s,line:%d\r\ntest is finished\r\n",__FILE__,__LINE__);
}
