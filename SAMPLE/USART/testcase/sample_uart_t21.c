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
void sample_usart_t21(USART_TypeDef* USARTx)
{	   
	u32 count = 0,t,len;
	
	printf("test is started\r\nfile:%s,line:%d\r\n",__FILE__,__LINE__);
	USARTx->CR1 = 0x0000;//FPGA_BIT中默认配置CR1为204C，启动之前清除配置
	read_ahb32(0xE000E280) = 0xFFFFFFFF;//清除PENDING位
	USARTx->CR2 = 0x900; //使能同步模式，CLKEN/LBCL=1
	
	sample_uart_set_param(eUSART_BAUD,eUSART_BAUD_9600);
    sample_uart_init(USARTx);	//以默认参数初始化打印串口
	
	USART_SendData(USARTx, 0x55);
	while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)!=SET);//等待发送结束
	USART_SendData(USARTx, 0x0d);
	while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)!=SET);//等待发送结束
	USART_SendData(USARTx, 0x0a);
	while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)!=SET);//等待发送结束
	
	while(1)
	{
		if(USART_RX_STA&0x8000)
		{	
			printf("MODE:%d;RDR:0x%x\r\n",count,USART_RX_BUF[0]);	
			//修改同步模式,重新初始化
			if(++count > 3)
				break;
			
			USARTx->CR2 &= 0xFF9F;//清除CPOL/CPHA位
			USARTx->CR2 |= count << 9; //设置CPOL/CPHA
			
			sample_uart_init(USARTx);	//以默认参数初始化打印串口
			
			//发送收到的数据
			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
			for(t=0;t<len;t++)
			{
				USART_RX_STA=0;
				delay_us(50);
				USART_SendData(USARTx, ++USART_RX_BUF[t]);//向串口1发送数据
				while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)!=SET);//等待发送结束
				delay_us(50);
				
				USART_SendData(USARTx, 0x0d);
				while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)!=SET);//等待发送结束
				delay_us(50);
				
				USART_SendData(USARTx, 0x0a);
				while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)!=SET);//等待发送结束

			}
	
		}
	}
	//printf("CR1:%x\r\n",USARTx->CR1);
	//printf("SR1:%x\r\n",USARTx->SR);
	//printf("PD0:0x%x\r\n",reg(0xE000E200));
	
	//测试完成，释放资源;FPGA版本无RCC，不释放
	//sample_uart_deinit(USARTx);	//外设deinit
	printf("file:%s,line:%d\r\ntest is finished\r\n",__FILE__,__LINE__);
}

