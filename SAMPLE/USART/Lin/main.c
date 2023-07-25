#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
 
 
/************************************************
LIN SLAVEģʽ
************************************************/

void delay_ms(u16 nms)
{
	u32 i,j;
	for(i=0;i<nms;i++)
	{
		for(j=0;j<1000;j++);
	}	
}

#define KEY0  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)//��ȡ����0

//������ʼ������

void KEY_Init(void) //IO��ʼ��
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOE,ENABLE);//ʹ��PORTA,PORTEʱ��

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;//KEY0-KEY2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIOE2,3,4

	//��ʼ�� WK_UP-->GPIOA.0	  ��������
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0���ó����룬Ĭ������	  
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.0

}
//u8 array[5] ={0x55,0x32,1,2,3,4,5};
void uart_init(u32 bound)
{
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;


	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��

	//USART1_TX   GPIOA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9

	//USART1_RX	  GPIOA.10��ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10  

	//USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

	USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	USART_ITConfig(USART1, USART_IT_TXE, ENABLE);//�������ڷ����ж�
	USART_ITConfig(USART1, USART_IT_LBD, ENABLE);//�������ڽ����ж�
	USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1 
	
	
}

void lin_master(void)
{
	u32 i;
	
	//USART_SendBreak(USART1); //�����Ͽ�֡
	
	if(USART_GetITStatus(USART1, USART_IT_LBD) != RESET)
	{
		USART_SendData(USART1, 0x66); 
		while(USART_GetITStatus(USART1, USART_IT_TXE) != SET);
	}

	//���ͱ�ͷ
	USART_SendData(USART1, 0x00);
	while(USART_GetITStatus(USART1, USART_IT_TXE) != SET);
	
	USART_SendData(USART1, 0x01);
	while(USART_GetITStatus(USART1, USART_IT_TXE) != SET);
	
	//������Ӧ֡,����У��
	for(i = 0;i < 5;i++)
	{
		USART_SendData(USART1, i);
		while(USART_GetITStatus(USART1, USART_IT_TXE) != SET);
	}
	
	
	//USART_SendBreak(USART1); //�����Ͽ�֡
	
	//���ͱ�ͷ,�ӻ�����
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
					//������Ӧ֡,����У��
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
	uart_init(9600);	 //���ڳ�ʼ��Ϊ115200
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

