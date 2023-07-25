#include "sys.h"
#include "delay.h"
#include "sample_usart.h"
#include "stm32f10x.h"



/*******************************************************************************
 * @brief   sample_uart_t4
 * Function ����ģʽ������ģʽ
 * @author  taohu
 * @date    2022-3-21
 * @param   
		USARTx:USART����ַ
 * @return void
*******************************************************************************/    
void sample_uart_t6(USART_TypeDef* USARTx)
{	   
    u16 count = 0;  
	
	//��ʼ���������
	USART_SetPrescaler(USARTx, 0x02);
	USART_IrDAConfig(USARTx, USART_IrDAMode_LowPower);
	USART_IrDACmd(USARTx, ENABLE);
	
	sample_uart_set_param(eUSART_BAUD,9600); //��115200��ʼ����ӡ����
    sample_uart_init(USARTx);	//��Ĭ�ϲ�����ʼ����ӡ����

	printf("test is started\r\nfile:%s,line:%d\r\n",__FILE__,__LINE__);
	
	while(1)
	{
		if(USART_GetFlagStatus(USARTx,USART_FLAG_RXNE)==SET)//�ȴ����ͽ���
		{
			//printf("receive data:0x%x\r\n",USARTx->SR);
			printf("receive data:0x%x\r\n",USART_ReceiveData(USARTx));
			
			if(count++ >10)
				break;
		}
	}
	
	printf("file:%s,line:%d\r\ntest is finished\r\n",__FILE__,__LINE__);
}
