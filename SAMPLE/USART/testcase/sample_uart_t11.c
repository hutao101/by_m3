#include "sys.h"
#include "delay.h"
#include "sample_usart.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_uart_test_t11
 * Function LIN Master:���ͺͽ���
 * @author  taohu
 * @date    2022-1-6
 * @param   
		USARTx:USART����ַ
 * @return void
*******************************************************************************/    
void sample_uart_t11(USART_TypeDef* USARTx)
{	   
	u32 i;
	
	printf("test is started\r\nfile:%s,line:%d\r\n",__FILE__,__LINE__);
	
	USARTx->CR1 = 0x0000;//FPGA_BIT��Ĭ������CR1Ϊ204C������֮ǰ�������
	read_ahb32(0xE000E280) = 0xFFFFFFFF;//���PENDINGλ
	
	sample_uart_init(USARTx);
	USART_LINCmd(USARTx, ENABLE);
	
	//�ص�NVICʹ��
	sample_uart_nvic_cfg((u32)USARTx,DISABLE);
	
	USART_ITConfig(USARTx, USART_IT_LBD, ENABLE);//�������ڽ����ж�
	USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);//�������ڽ����ж�

	
	USART_SendBreak(USARTx); //�����Ͽ�֡
	printf("SR:%x\r\n",USARTx->SR);
	
	//���ͱ�ͷ
	USART_SendData(USARTx, 0x55);
	while(USART_GetITStatus(USARTx, USART_IT_TXE) != SET);
	
	USART_SendData(USARTx, 0x01);
	while(USART_GetITStatus(USARTx, USART_IT_TXE) != SET);
	
	//������Ӧ֡,����У��
	for(i = 0;i < 5;i++)
	{
		USART_SendData(USARTx, i);
		while(USART_GetITStatus(USARTx, USART_IT_TXE) != SET);
	}
	
	//���ͱ�ͷ,�ӻ�����
	USART_SendBreak(USARTx); //�����Ͽ�֡
	printf("SR:%x\r\n",USARTx->SR);
	
	USART_SendData(USARTx, 0x55);
	while(USART_GetITStatus(USARTx, USART_IT_TXE) != SET);
	USART_SendData(USARTx, 0x32);
	while(USART_GetITStatus(USARTx, USART_IT_TXE) != SET);

	while(USART_GetITStatus(USARTx, USART_IT_LBD) != SET);
	printf("RDR:%x\r\n",USART_ReceiveData(USARTx));
	printf("RDR:%x\r\n",USART_ReceiveData(USARTx));
	printf("RDR:%x\r\n",USART_ReceiveData(USARTx));
	printf("RDR:%x\r\n",USART_ReceiveData(USARTx));
	printf("RDR:%x\r\n",USART_ReceiveData(USARTx));
	printf("file:%s,line:%d\r\ntest is finished\r\n",__FILE__,__LINE__);
}

