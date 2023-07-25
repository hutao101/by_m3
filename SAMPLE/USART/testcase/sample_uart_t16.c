#include "sys.h"
#include "delay.h"
#include "sample_usart.h"
#include "stm32f10x.h"



/*******************************************************************************
 * @brief   sample_uart_test_t1
 * Function ȫ˫���첽ͨ�ű���У��λ����/��/ż
 * @author  taohu
 * @date    2022-1-6
 * @param   
		USARTx:USART����ַ
 * @return void
*******************************************************************************/    
void sample_uart_t16(USART_TypeDef* USARTx)
{	   
    u16 t,count = 0;  
    u16 len; 
	

	sample_uart_set_param(eUSART_BAUD,eUSART_BAUD_115200); //��115200��ʼ����ӡ����
    sample_uart_init(USARTx);	//��Ĭ�ϲ�����ʼ����ӡ����

	printf("test is started\r\nfile:%s,line:%d\r\n",__FILE__,__LINE__);
	//sample_uart_set_param(eUSART_BAUD,eUSART_BAUD_9600); //�ָ�Ĭ�ϲ��Բ���
    //sample_uart_init(USARTx);	//���Դ��ڳ�ʼ��
    USART_ITConfig(USARTx, USART_IT_PE, ENABLE);//�������ڽ����ж�

	//�������̣�������ɺ�Ӧ����whileѭ��
	while(1)
	{
	    if(USART_RX_STA&0x8000)
		{	
			//printf("SR1:%x\r\n",USARTx->SR);
			//�����յ�������
			len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
			for(t=0;t<len;t++)
			{
				USART_SendData(USARTx, USART_RX_BUF[t]);//�򴮿�1��������
				while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
			}
			USART_RX_STA=0;
			
			//���������ɣ��˳�����;���Զ˲�֧��2.25M/4.5M�����ʣ����ﲻ��������������
			if(++count >= USART_PRT_COUNT)
				break;
			
			delay_ms(20000);
			//���³�ʼ��USARTx,����������
			sample_uart_set_param(eUSART_PRT,Prt[++count]);
			//sample_uart_deinit(USARTx);	//��λ����USART��
			sample_uart_init(USARTx);	//���Դ��ڳ�ʼ��
		} 
	}

	//sample_uart_deinit(USARTx);	//������ɣ��ͷ���Դ��
	//printf("file:%s,line:%d\r\ntest is finished\r\n",__FILE__,__LINE__);
}
