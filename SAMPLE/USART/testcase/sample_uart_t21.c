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
void sample_usart_t21(USART_TypeDef* USARTx)
{	   
	u32 count = 0,t,len;
	
	printf("test is started\r\nfile:%s,line:%d\r\n",__FILE__,__LINE__);
	USARTx->CR1 = 0x0000;//FPGA_BIT��Ĭ������CR1Ϊ204C������֮ǰ�������
	read_ahb32(0xE000E280) = 0xFFFFFFFF;//���PENDINGλ
	USARTx->CR2 = 0x900; //ʹ��ͬ��ģʽ��CLKEN/LBCL=1
	
	sample_uart_set_param(eUSART_BAUD,eUSART_BAUD_9600);
    sample_uart_init(USARTx);	//��Ĭ�ϲ�����ʼ����ӡ����
	
	USART_SendData(USARTx, 0x55);
	while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
	USART_SendData(USARTx, 0x0d);
	while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
	USART_SendData(USARTx, 0x0a);
	while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
	
	while(1)
	{
		if(USART_RX_STA&0x8000)
		{	
			printf("MODE:%d;RDR:0x%x\r\n",count,USART_RX_BUF[0]);	
			//�޸�ͬ��ģʽ,���³�ʼ��
			if(++count > 3)
				break;
			
			USARTx->CR2 &= 0xFF9F;//���CPOL/CPHAλ
			USARTx->CR2 |= count << 9; //����CPOL/CPHA
			
			sample_uart_init(USARTx);	//��Ĭ�ϲ�����ʼ����ӡ����
			
			//�����յ�������
			len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
			for(t=0;t<len;t++)
			{
				USART_RX_STA=0;
				delay_us(50);
				USART_SendData(USARTx, ++USART_RX_BUF[t]);//�򴮿�1��������
				while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
				delay_us(50);
				
				USART_SendData(USARTx, 0x0d);
				while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
				delay_us(50);
				
				USART_SendData(USARTx, 0x0a);
				while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)!=SET);//�ȴ����ͽ���

			}
	
		}
	}
	//printf("CR1:%x\r\n",USARTx->CR1);
	//printf("SR1:%x\r\n",USARTx->SR);
	//printf("PD0:0x%x\r\n",reg(0xE000E200));
	
	//������ɣ��ͷ���Դ;FPGA�汾��RCC�����ͷ�
	//sample_uart_deinit(USARTx);	//����deinit
	printf("file:%s,line:%d\r\ntest is finished\r\n",__FILE__,__LINE__);
}

