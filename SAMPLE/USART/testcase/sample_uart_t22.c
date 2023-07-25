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
void sample_usart_t22(USART_TypeDef* USARTx)
{	   

	USARTx->CR1 = 0x0000;//FPGA_BIT中默认配置CR1为204C，启动之前清除配置
	USARTx->CR2 = 0x800; //LBCL=0
	sample_uart_set_param(eUSART_BAUD,eUSART_BAUD_115200);
    sample_uart_init(USARTx);	//以默认参数初始化打印串口
	
	printf("CR2:0x%x\r\n",USARTx->CR2);
	USART_SendData(USARTx, 0x55);
	USART_SendData(USARTx, 0x55);
	
	delay_ms(80);
	USARTx->CR2 = 0x900;//LBCL=1
	sample_uart_init(USARTx);	//以默认参数初始化打印串口
	USART_SendData(USARTx, 0x55);
    USART_SendData(USARTx, 0x55);
	printf("test is started\r\nfile:%s,line:%d\r\n",__FILE__,__LINE__);
	
	//printf("CR1:%x\r\n",USARTx->CR1);
	//printf("SR1:%x\r\n",USARTx->SR);
	//printf("PD0:0x%x\r\n",reg(0xE000E200));
	
	//测试完成，释放资源;FPGA版本无RCC，不释放
	//sample_uart_deinit(USARTx);	//外设deinit
	//printf("file:%s,line:%d\r\ntest is finished\r\n",__FILE__,__LINE__);
}

