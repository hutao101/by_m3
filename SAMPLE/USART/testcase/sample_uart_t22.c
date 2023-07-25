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
void sample_usart_t22(USART_TypeDef* USARTx)
{	   

	USARTx->CR1 = 0x0000;//FPGA_BIT��Ĭ������CR1Ϊ204C������֮ǰ�������
	USARTx->CR2 = 0x800; //LBCL=0
	sample_uart_set_param(eUSART_BAUD,eUSART_BAUD_115200);
    sample_uart_init(USARTx);	//��Ĭ�ϲ�����ʼ����ӡ����
	
	printf("CR2:0x%x\r\n",USARTx->CR2);
	USART_SendData(USARTx, 0x55);
	USART_SendData(USARTx, 0x55);
	
	delay_ms(80);
	USARTx->CR2 = 0x900;//LBCL=1
	sample_uart_init(USARTx);	//��Ĭ�ϲ�����ʼ����ӡ����
	USART_SendData(USARTx, 0x55);
    USART_SendData(USARTx, 0x55);
	printf("test is started\r\nfile:%s,line:%d\r\n",__FILE__,__LINE__);
	
	//printf("CR1:%x\r\n",USARTx->CR1);
	//printf("SR1:%x\r\n",USARTx->SR);
	//printf("PD0:0x%x\r\n",reg(0xE000E200));
	
	//������ɣ��ͷ���Դ;FPGA�汾��RCC�����ͷ�
	//sample_uart_deinit(USARTx);	//����deinit
	//printf("file:%s,line:%d\r\ntest is finished\r\n",__FILE__,__LINE__);
}

