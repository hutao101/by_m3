#include "sys.h"
#include "delay.h"
#include "sample_usart.h"
#include "stm32f10x.h"


#define reg(mreg)   *(volatile unsigned int *)(mreg)
/*******************************************************************************
 * @brief   sample_uart_test_t17
 * Function ���������֤
 * @author  taohu
 * @date    2022-1-6
 * @param   
		USARTx:USART����ַ
 * @return void
*******************************************************************************/    
void sample_uart_t17(USART_TypeDef* USARTx)
{	   
    u16 t,len;  
	
	USARTx->CR1 = 0x0000;//FPGA_BIT��Ĭ������CR1Ϊ204C������֮ǰ�������
	sample_uart_set_param(eUSART_BAUD,eUSART_BAUD_115200); //��115200��ʼ����ӡ����
    sample_uart_init(USARTx);	//��ʼ����ӡ����
    USART_ITConfig(USARTx, USART_IT_RXNE, DISABLE); //�رս����ж�
	
    //printf("test is started\r\nfile:%s,line:%d\r\n",__FILE__,__LINE__);

	//�������ַ���4�����ݣ���֤OREʱ��λ�Ĵ������ݻ᲻�����
	//ѭ����SR������OREʱ��ӡSRֵ����SR�ٶ�DR
	while(USART_GetFlagStatus(USARTx,USART_FLAG_ORE) != SET);//�ȴ����ͽ���
	delay_us(100);//��ʱ100΢�룬ȷ�����Ͷ�4�����ݶ��������

	//����������DR����֤����������
	USART_RX_BUF[USART_RX_STA&0X3FFF] = USART_ReceiveData(USARTx); 
	USART_RX_STA++;
	USART_SendData(USARTx, USARTx->SR);
	USART_RX_BUF[USART_RX_STA&0X3FFF] = USART_ReceiveData(USARTx);
	USART_RX_STA++;
	len=USART_RX_STA&0x3fff;
	for(t=0;t<len;t++)
	{
		USART_SendData(USARTx, USART_RX_BUF[t]);//�򴮿�1��������
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
	}
	USART_RX_STA=0;

	//sample_uart_deinit(USARTx);	//��λUSART1��
	//printf("file:%s,line:%d\r\ntest is finished\r\n",__FILE__,__LINE__);
}

