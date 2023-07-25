#include "sys.h"
#include "delay.h"
#include "sample_usart.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_uart_test_t1
 * Function ȫ˫���첽ͨ�ţ�����������1.2K-4.5M������UART�͵��Զ�ͨ�Ź���
 * @author  taohu
 * @date    2022-1-6
 * @param   
		USARTx:USART����ַ
 * @return void
*******************************************************************************/    
void sample_uart_t20(USART_TypeDef* USARTx)
{	   
    u16 t;  
    u16 len; 

	USARTx->CR1 = 0x0000;//FPGA_BIT��Ĭ������CR1Ϊ204C������֮ǰ�������
	sample_uart_set_param(eUSART_BAUD,eUSART_BAUD_115200);
	sample_uart_set_param(eUSART_PRT,USART_Parity_Even);
    sample_uart_init(USARTx);	//��Ĭ�ϲ�����ʼ����ӡ����
	printf("test is started\r\nfile:%s,line:%d\r\n",__FILE__,__LINE__);
	
	printf("CR1:%x\r\n",USARTx->CR1);
	printf("SR1:%x\r\n",USARTx->SR);
	printf("PD0:0x%x\r\n",read_ahb32(0xE000E200));
	//�ȴ�FE��־����λ
    while(USART_GetFlagStatus(USARTx,USART_FLAG_PE) != SET);//�ȴ����ͽ���
	delay_ms(10000);
	//����������SR������
	USART_RX_BUF[USART_RX_STA&0X3FFF] = USARTx->SR; 
	USART_RX_STA++;
    USART_ReceiveData(USARTx);
	delay_us(50);
	
	USART_RX_BUF[USART_RX_STA&0X3FFF] = USARTx->SR; 
	USART_RX_STA++;
	len=USART_RX_STA&0x3fff;
	for(t=0;t<len;t++)
	{
		USART_SendData(USARTx,USART_RX_BUF[t]);
	}
	USART_RX_STA=0;
    
	//������ɣ��ͷ���Դ;FPGA�汾��RCC�����ͷ�
	//sample_uart_deinit(USARTx);	//����deinit
	printf("file:%s,line:%d\r\ntest is finished\r\n",__FILE__,__LINE__);
}

