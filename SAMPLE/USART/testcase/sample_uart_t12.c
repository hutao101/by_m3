#include "sys.h"
#include "delay.h"
#include "sample_usart.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_uart_test_t1
 * Function LINģʽ���������ͺͽ���
 * @author  taohu
 * @date    2022-3-17
 * @param   
		USARTx:USART����ַ
 * @return void
*******************************************************************************/    
void sample_uart_t12(USART_TypeDef* USARTx)
{	   
	u32 i;
	
	printf("test is started\r\nfile:%s,line:%d\r\n",__FILE__,__LINE__);
	
	USARTx->CR1 = 0x0000;//FPGA_BIT��Ĭ������CR1Ϊ204C������֮ǰ�������
	read_ahb32(0xE000E280) = 0xFFFFFFFF;//���PENDINGλ
	
	sample_uart_init(USARTx);
	
	USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);//�������ڷ����ж�
	USART_ITConfig(USARTx, USART_IT_LBD, ENABLE);//��������BREAK֡����ж�
	
	USART_LINCmd(USARTx, ENABLE);
	
	while(1)
	{
		if(USART_GetITStatus(USARTx, USART_IT_LBD) != RESET)
		{
			printf("SR:%x",USARTx->SR);
			if(USART_GetITStatus(USARTx, USART_IT_RXNE) != RESET)
			{
				if(USART_ReceiveData(USARTx) == 0x32)
				{
					//������Ӧ֡,����У��
					for(i = 0;i < 5;i++)
					{
						USART_SendData(USARTx, i); 
						while(USART_GetITStatus(USARTx, USART_IT_TXE) != SET);
					}		
				}
			}
		}
	}
	//printf("file:%s,line:%d\r\ntest is finished\r\n",__FILE__,__LINE__);
}

