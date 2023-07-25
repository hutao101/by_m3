#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
 
 
/************************************************
LIN SLAVE模式
************************************************/

void delay_ms(u16 nms)
{
	u32 i,j;
	for(i=0;i<nms;i++)
	{
		for(j=0;j<1000;j++);
	}	
}

#define KEY0  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)//读取按键0

//按键初始化函数

void KEY_Init(void) //IO初始化
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOE,ENABLE);//使能PORTA,PORTE时钟

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;//KEY0-KEY2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIOE2,3,4

	//初始化 WK_UP-->GPIOA.0	  下拉输入
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0设置成输入，默认下拉	  
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.0

}
//u8 array[5] ={0x55,0x32,1,2,3,4,5};
void uart_init(u32 bound)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;


	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟

	//USART1_TX   GPIOA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9

	//USART1_RX	  GPIOA.10初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  

	//USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

	USART_Init(USART1, &USART_InitStructure); //初始化串口1
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
	USART_ITConfig(USART1, USART_IT_TXE, ENABLE);//开启串口发送中断
	USART_ITConfig(USART1, USART_IT_LBD, ENABLE);//开启串口接受中断
	USART_Cmd(USART1, ENABLE);                    //使能串口1 
	
	
}

void lin_master(void)
{
	u32 i;
	
	//USART_SendBreak(USART1); //发生断开帧
	
	if(USART_GetITStatus(USART1, USART_IT_LBD) != RESET)
	{
		USART_SendData(USART1, 0x66); 
		while(USART_GetITStatus(USART1, USART_IT_TXE) != SET);
	}

	//发送报头
	USART_SendData(USART1, 0x00);
	while(USART_GetITStatus(USART1, USART_IT_TXE) != SET);
	
	USART_SendData(USART1, 0x01);
	while(USART_GetITStatus(USART1, USART_IT_TXE) != SET);
	
	//发送响应帧,不做校验
	for(i = 0;i < 5;i++)
	{
		USART_SendData(USART1, i);
		while(USART_GetITStatus(USART1, USART_IT_TXE) != SET);
	}
	
	
	//USART_SendBreak(USART1); //发生断开帧
	
	//发送报头,从机发送
	USART_SendData(USART1, 0x55);
	while(USART_GetITStatus(USART1, USART_IT_TXE) != SET);
	USART_SendData(USART1, 0x32);
	while(USART_GetITStatus(USART1, USART_IT_TXE) != SET);
	
	if(USART_GetITStatus(USART1, USART_IT_LBD) != RESET)
	{
		USART_SendData(USART1, 0x66); 
		while(USART_GetITStatus(USART1, USART_IT_TXE) != SET);
	}

}


void lin_slave(void)
{
	u32 i;
	
	while(1)
	{
		if(USART_GetITStatus(USART1, USART_IT_LBD) != RESET)
		{
			if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
			{
				if(USART_ReceiveData(USART1) == 0x32)
				{
					//发送响应帧,不做校验
					for(i = 0;i < 5;i++)
					{
						USART_SendData(USART1, i); 
						while(USART_GetITStatus(USART1, USART_IT_TXE) != SET);
					}		
				}
			}
		}
	}
}


 int main(void)
{	
	KEY_Init();
	uart_init(9600);	 //串口初始化为115200
	USART_LINCmd(USART1, ENABLE);
	
	while(1)
	{
		if(KEY0 == 0)
		{
			delay_ms(100);
			if(KEY0 == 0)
			{
				lin_master();
				lin_slave();
			}
		}	
		//lin_slave();
	}
		
}

