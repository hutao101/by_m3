#include "sys.h"
#include "delay.h"
#include "sample_usart.h"
#include "stm32f10x.h"


/*******************************************************************************
 * @brief   sample_uart_test_t23
 * Function ��CTS����ģʽ��������������
 * @author  taohu
 * @date    2022-1-6
 * @param   
		USARTx:USART����ַ
 * @return void
*******************************************************************************/    
void sample_uart_t23(USART_TypeDef* USARTx)
{	   
	u16 t,len,temp,count = 0;
	printf("test is started\r\nfile:%s,line:%d\r\n",__FILE__,__LINE__);
	USARTx->CR1 = 0x0000;//FPGA_BIT��Ĭ������CR1Ϊ204C������֮ǰ�������
	USARTx->CR2 = 0x0000;//FPGA_BIT��Ĭ������CR1Ϊ204C������֮ǰ�������
	read_ahb32(0xE000E280) = 0xFFFFFFFF;//���PENDINGλ

    sample_uart_init(USARTx);	//��Ĭ�ϲ�����ʼ����ӡ����

	while(1)
	{

		while(USART_GetFlagStatus(USARTx,USART_FLAG_RXNE)!=SET);//�ȴ����ͽ���
		USART_ReceiverWakeUpCmd(USARTx, ENABLE);
		printf("DR:0x%x\r\n",USARTx->DR);
		printf("CR1:0x%x\r\n",USARTx->CR1);		
	
	}
	
	//�������̣��ͷ���Դ
	//sample_uart_deinit(USARTx);	//������ɣ��ͷ���Դ��
	printf("file:%s,line:%d\r\ntest is finished\r\n",__FILE__,__LINE__);
}
