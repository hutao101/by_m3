#include "sys.h"
#include "delay.h"
#include "sample_usart.h"
#include "stm32f10x.h"



/*******************************************************************************
 * @brief   sample_uart_test_t1
 * Function ���ⷢ��ģʽ
 * @author  taohu
 * @date    2022-1-6
 * @param   
		USARTx:USART����ַ
 * @return void
*******************************************************************************/    
void sample_uart_t3(USART_TypeDef* USARTx)
{	   
	u16 count = 0;
	//��ʼ���������
	USART_SetPrescaler(USARTx, 0x01);
	USART_IrDAConfig(USARTx, USART_IrDAMode_Normal);
	USART_IrDACmd(USARTx, ENABLE);

	sample_uart_set_param(eUSART_BAUD,eUSART_BAUD_9600); //��115200��ʼ����ӡ����
    sample_uart_init(USARTx);	//��Ĭ�ϲ�����ʼ����ӡ����

	printf("test is started\r\nfile:%s,line:%d\r\n",__FILE__,__LINE__);
	
	while(1)
	{
		//�������̣���������ͷ�WHILEѭ��
		delay_ms(5000);
		USART_SendData(USARTx, 0x55);
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
		
		if(count++ > 10)
			break;
	}

	//�������̣��ͷ���Դ
	printf("file:%s,line:%d\r\ntest is finished\r\n",__FILE__,__LINE__);
}
