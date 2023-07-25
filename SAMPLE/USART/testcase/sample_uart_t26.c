#include "sys.h"
#include "delay.h"
#include "sample_usart.h"
#include "stm32f10x.h"



/*******************************************************************************
 * @brief   sample_uart_test_t1
 * Function ��CTS����ģʽ��������������
 * @author  taohu
 * @date    2022-1-6
 * @param   
		USARTx:USART����ַ
 * @return void
*******************************************************************************/    
void sample_uart_t26(USART_TypeDef* USARTx)
{
	printf("test is started\r\nfile:%s,line:%d\r\n",__FILE__,__LINE__);
	USARTx->CR1 = 0x0000;//FPGA_BIT��Ĭ������CR1Ϊ204C������֮ǰ�������

#if 0
	//TXE�жϴ�
	sample_uart_init(USARTx);	//��Ĭ�ϲ�����ʼ����ӡ����

	USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);
#endif

#if 0
	//TC�жϴ�
	sample_uart_init(USARTx);	//��Ĭ�ϲ�����ʼ����ӡ����

	USART_ITConfig(USARTx, USART_IT_TC, ENABLE);
#endif

#if 0
	//RXNE�жϴ�
	sample_uart_init(USARTx);	//��Ĭ�ϲ�����ʼ����ӡ����
	
	USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);
#endif

#if 0
	//RXNE�жϴ�,����ORE
	sample_uart_init(USARTx);	//��Ĭ�ϲ�����ʼ����ӡ����
	
	USART_ITConfig(USARTx, USART_IT_ORE, ENABLE);
#endif

#if 0
	//PE�жϴ�
	sample_uart_set_param(eUSART_PRT,USART_Parity_Odd);
	sample_uart_init(USARTx);	//��Ĭ�ϲ�����ʼ����ӡ����
	
	USART_ITConfig(USARTx, USART_IT_PE, ENABLE);
#endif

#if 0
		//PE�жϴ�
		sample_uart_init(USARTx);	//��Ĭ�ϲ�����ʼ����ӡ����
		
		USART_ITConfig(USARTx, USART_IT_LBD, ENABLE);
		USART_LINCmd(USARTx, ENABLE);
#endif

#if 0
			//CTS�жϴ�
			sample_uart_set_param(eUSART_FLOW,USART_HardwareFlowControl_CTS);
			sample_uart_init(USARTx);	//��Ĭ�ϲ�����ʼ����ӡ����
			
			USART_ITConfig(USARTx, USART_IT_CTS, ENABLE);
#endif

#if 1
				//IDLE�жϴ�
				sample_uart_init(USARTx);	//��Ĭ�ϲ�����ʼ����ӡ����
				
				USART_ITConfig(USARTx, USART_IT_IDLE, ENABLE);
#endif

	printf("PD0:0x%x\r\n",read_ahb32(0xE000E200));

	while(1)
	{
		//printf("SR:0x%x",USARTx->SR);
	}	

	//sample_uart_deinit(USARTx);	//����deinit
	//printf("file:%s,line:%d\r\ntest is finished\r\n",__FILE__,__LINE__);
}
