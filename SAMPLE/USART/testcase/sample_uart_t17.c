#include "sys.h"
#include "delay.h"
#include "sample_usart.h"
#include "stm32f10x.h"


#define reg(mreg)   *(volatile unsigned int *)(mreg)
/*******************************************************************************
 * @brief   sample_uart_test_t17
 * Function 溢出错误验证
 * @author  taohu
 * @date    2022-1-6
 * @param   
		USARTx:USART基地址
 * @return void
*******************************************************************************/    
void sample_uart_t17(USART_TypeDef* USARTx)
{	   
    u16 t,len;  
	
	USARTx->CR1 = 0x0000;//FPGA_BIT中默认配置CR1为204C，启动之前清除配置
	sample_uart_set_param(eUSART_BAUD,eUSART_BAUD_115200); //以115200初始化打印串口
    sample_uart_init(USARTx);	//初始化打印串口
    USART_ITConfig(USARTx, USART_IT_RXNE, DISABLE); //关闭接收中断
	
    //printf("test is started\r\nfile:%s,line:%d\r\n",__FILE__,__LINE__);

	//串口助手发送4个数据，验证ORE时移位寄存器数据会不会更新
	//循环读SR，发生ORE时打印SR值；读SR再读DR
	while(USART_GetFlagStatus(USARTx,USART_FLAG_ORE) != SET);//等待发送结束
	delay_us(100);//延时100微秒，确保发送端4个数据都发送完成

	//连续读两次DR，验证读出的数据
	USART_RX_BUF[USART_RX_STA&0X3FFF] = USART_ReceiveData(USARTx); 
	USART_RX_STA++;
	USART_SendData(USARTx, USARTx->SR);
	USART_RX_BUF[USART_RX_STA&0X3FFF] = USART_ReceiveData(USARTx);
	USART_RX_STA++;
	len=USART_RX_STA&0x3fff;
	for(t=0;t<len;t++)
	{
		USART_SendData(USARTx, USART_RX_BUF[t]);//向串口1发送数据
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)!=SET);//等待发送结束
	}
	USART_RX_STA=0;

	//sample_uart_deinit(USARTx);	//复位USART1，
	//printf("file:%s,line:%d\r\ntest is finished\r\n",__FILE__,__LINE__);
}

