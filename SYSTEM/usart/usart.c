#include "sys.h"
#include "usart.h"	   

//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{

#ifdef by_debug
    while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
    USART1->DR = (u8) ch;
    return ch;
#endif

}
#endif 


#ifndef USART_TEST  

u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�

//����״̬
//bit15��	���յ�0x0a,/n��־
//bit14��	���յ�0x0d��/r��־
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���	  

void uart_init(u32 bound)
{
	USART_InitTypeDef USART_InitStructure;
	/*
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

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

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	*/

	//USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

	USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	//USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1 
  
	printf("Uart Init��%d\r\n",bound);

}

#endif

/*******************************************************************************
 * @brief   uart_int_handle
 * Function UART�жϴ�����
 * @author  taohu
 * @date    2022-1-6
 * @param   
		USARTx:USART����ַ
 * @return void
*******************************************************************************/
void uart_rev_int_handle(USART_TypeDef* USARTx)
{
	u8 Res;
	//printf("SR:0x%x",USARTx->SR);
	if(USART_GetITStatus(USARTx, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		if(USART_GetITStatus(USARTx, USART_IT_PE) != RESET)
		{
			Res =USART_ReceiveData(USARTx);	//��ȡ���յ�������
			if(Res >= 0x0F)
				USART_SendData(USARTx,Res);
			//printf("%x",Res);	
			if((Res&0x7f) <= 0x0F)
				USART_RX_STA|=0x8000;
		}
		else
		{
			Res =USART_ReceiveData(USARTx);	//��ȡ���յ�������
			printf("0x%x\r\n",Res);	
		}
		if((USART_RX_STA&0x8000)==0)//����δ���
		{
			if(USART_RX_STA&0x4000)//���յ���0x0d,�س���/r
			{
					if(Res!=0x0a)	//���յ���0x0a,���м�/n
						USART_RX_STA=0;//���մ���,���¿�ʼ
					else 
						USART_RX_STA|=0x8000;	//��������� 
			}
			else //��û�յ�0X0D
			{	
				if(Res==0x0d)
					USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))
						USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		}   		 
    } 
}


void USART1_IRQHandler(void)                	//����1�жϷ������
{
	uart_rev_int_handle(USART1);
} 

void USART2_IRQHandler(void)                	//����2�жϷ������
{
	uart_rev_int_handle(USART2);
} 

void USART3_IRQHandler(void)                	//����3�жϷ������
{
	uart_rev_int_handle(USART3);
} 

void UART4_IRQHandler(void)                	//����3�жϷ������
{
	uart_rev_int_handle(UART4);
} 

void UART5_IRQHandler(void)                	//����3�жϷ������
{
	uart_rev_int_handle(UART5);
} 

