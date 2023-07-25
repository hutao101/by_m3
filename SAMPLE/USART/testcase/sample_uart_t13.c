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
void sample_uart_t13(USART_TypeDef* USARTx)
{	   
    u16 t,count = 0;  
    u16 len; 

	USARTx->CR1 = 0x0000;//FPGA_BIT��Ĭ������CR1Ϊ204C������֮ǰ�������
	sample_uart_set_param(eUSART_BAUD,Baudrate[count++]);
    sample_uart_init(USARTx);	//��Ĭ�ϲ�����ʼ����ӡ����
	printf("test is started\r\nfile:%s,line:%d\r\n",__FILE__,__LINE__);
	
	printf("CR1:%x\r\n",USARTx->CR1);
	printf("SR1:%x\r\n",USARTx->SR);
	printf("PD0:0x%x\r\n",read_ahb32(0xE000E200));
	//�������̣�������ɺ�Ӧ����whileѭ��
    while(1)
    {		
		//printf("SR1:%x\r\n",USARTx->SR);
    	//���³�ʼ��USARTx,����������
		//sample_uart_set_param(eUSART_BAUD,Baudrate[count++]);	
		//sample_uart_init(USARTx);	//���Դ��ڳ�ʼ��
		
	   //���������ɣ��˳�����;���Զ˲�֧��2.25M/4.5M�����ʣ����ﲻ��������������
			//if(count >= (USART_BUAD_COUNT-2))
				//break;
		//printf("SR1:%x\r\n",USARTx->SR);
		//printf("PD0:0x%x\r\n",reg(0xE000E200));
		if(USART_RX_STA&0x8000)
		{	
			printf("SR1:%x\r\n",USARTx->SR);
			//�����յ�������
			len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
			for(t=0;t<len;t++)
			{
				USART_SendData(USARTx, USART_RX_BUF[t]);//�򴮿�1��������
				while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
			}
			USART_RX_STA=0;
			
			//���������ɣ��˳�����;���Զ˲�֧��2.25M/4.5M�����ʣ����ﲻ��������������
			if(count >= (USART_BUAD_COUNT-2))
				break;
			
			delay_ms(20000);
			//���³�ʼ��USARTx,����������
			sample_uart_set_param(eUSART_BAUD,Baudrate[count++]);
			//sample_uart_deinit(USARTx);	//��λ����USART��
			sample_uart_init(USARTx);	//���Դ��ڳ�ʼ��
		} 
    }
	//������ɣ��ͷ���Դ;FPGA�汾��RCC�����ͷ�
	//sample_uart_deinit(USARTx);	//����deinit
	printf("file:%s,line:%d\r\ntest is finished\r\n",__FILE__,__LINE__);
}

