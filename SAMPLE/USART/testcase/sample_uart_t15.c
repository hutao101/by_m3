#include "sys.h"
#include "delay.h"
#include "sample_usart.h"
#include "stm32f10x.h"



/*******************************************************************************
 * @brief   sample_uart_test_t1
 * Function ȫ˫���첽ͨ�ű���ֹͣλ��1/2λ
 * @author  taohu
 * @date    2022-1-6
 * @param   
		USARTx:USART����ַ
 * @return void
*******************************************************************************/    
void sample_uart_t15(USART_TypeDef* USARTx)
{	   
   u16 temp = 0 ;  
	

	sample_uart_set_param(eUSART_BAUD,eUSART_BAUD_115200); //��115200��ʼ����ӡ����
    sample_uart_init(USARTx);	//��ʼ����ӡ����
    USART_ITConfig(USARTx, USART_IT_RXNE, DISABLE); //�رս����ж�
	
    //printf("test is started\r\nfile:%s,line:%d\r\n",__FILE__,__LINE__);

	//��������0X55
	USART_SendData(USARTx, 0x55);//�򴮿�1��������
	while(USART_GetFlagStatus(USARTx,USART_FLAG_RXNE) != SET);//�ȴ����ͽ���
	temp = USART_ReceiveData(USARTx);
	delay_us(50);
	
	if(temp == 0x55)
	{
		sample_uart_set_param(eUSART_STPB,USART_StopBits_2); 
		sample_uart_init(USARTx);	//��ʼ����ӡ����
		USART_ITConfig(USARTx, USART_IT_RXNE, DISABLE); //�رս����ж�	
		
		USART_SendData(USARTx, 0xAA);//�򴮿ڷ�������
		while(USART_GetFlagStatus(USARTx,USART_FLAG_RXNE) != SET);//�ȴ����ͽ���
		temp = USART_ReceiveData(USARTx);

		USART_SendData(USARTx, temp);//�򴮿ڷ�������
		while(USART_GetFlagStatus(USARTx,USART_FLAG_RXNE) != SET);//�ȴ����ͽ���
	}
	//sample_uart_deinit(USARTx);	//��λUSART1��
	//printf("file:%s,line:%d\r\ntest is finished\r\n",__FILE__,__LINE__);
}

