#include "sys.h"
#include "delay.h"
#include "sample_usart.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_uart_test_t1
 * Function 全双工异步通信：遍历波特率1.2K-4.5M；测试UART和电脑端通信功能
 * @author  taohu
 * @date    2022-1-6
 * @param   
		USARTx:USART基地址
 * @return void
*******************************************************************************/    
void sample_uart_t13(USART_TypeDef* USARTx)
{	   
    u16 t,count = 0;  
    u16 len; 

	USARTx->CR1 = 0x0000;//FPGA_BIT中默认配置CR1为204C，启动之前清除配置
	sample_uart_set_param(eUSART_BAUD,Baudrate[count++]);
    sample_uart_init(USARTx);	//以默认参数初始化打印串口
	printf("test is started\r\nfile:%s,line:%d\r\n",__FILE__,__LINE__);
	
	printf("CR1:%x\r\n",USARTx->CR1);
	printf("SR1:%x\r\n",USARTx->SR);
	printf("PD0:0x%x\r\n",read_ahb32(0xE000E200));
	//测试流程，测试完成后应结束while循环
    while(1)
    {		
		//printf("SR1:%x\r\n",USARTx->SR);
    	//重新初始化USARTx,遍历波特率
		//sample_uart_set_param(eUSART_BAUD,Baudrate[count++]);	
		//sample_uart_init(USARTx);	//测试串口初始化
		
	   //如果遍历完成，退出测试;电脑端不支持2.25M/4.5M波特率；这里不测这两个波特率
			//if(count >= (USART_BUAD_COUNT-2))
				//break;
		//printf("SR1:%x\r\n",USARTx->SR);
		//printf("PD0:0x%x\r\n",reg(0xE000E200));
		if(USART_RX_STA&0x8000)
		{	
			printf("SR1:%x\r\n",USARTx->SR);
			//发送收到的数据
			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
			for(t=0;t<len;t++)
			{
				USART_SendData(USARTx, USART_RX_BUF[t]);//向串口1发送数据
				while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)!=SET);//等待发送结束
			}
			USART_RX_STA=0;
			
			//如果遍历完成，退出测试;电脑端不支持2.25M/4.5M波特率；这里不测这两个波特率
			if(count >= (USART_BUAD_COUNT-2))
				break;
			
			delay_ms(20000);
			//重新初始化USARTx,遍历波特率
			sample_uart_set_param(eUSART_BAUD,Baudrate[count++]);
			//sample_uart_deinit(USARTx);	//复位测试USART，
			sample_uart_init(USARTx);	//测试串口初始化
		} 
    }
	//测试完成，释放资源;FPGA版本无RCC，不释放
	//sample_uart_deinit(USARTx);	//外设deinit
	printf("file:%s,line:%d\r\ntest is finished\r\n",__FILE__,__LINE__);
}

