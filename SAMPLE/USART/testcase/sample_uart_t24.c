#include "sys.h"
#include "delay.h"
#include "sample_usart.h"
#include "stm32f10x.h"


/*******************************************************************************
 * @brief   sample_uart_test_t1
 * Function ��Ĭģʽ����ַƥ����
 * @author  taohu
 * @date    2022-1-6
 * @param   
		USARTx:USART����ַ
 * @return void
*******************************************************************************/    
void sample_uart_t24(USART_TypeDef* USARTx)
{	   
	u16 t,len,temp,count = 0;
	printf("test is started\r\nfile:%s,line:%d\r\n",__FILE__,__LINE__);
	USARTx->CR1 = 0x0000;//FPGA_BIT��Ĭ������CR1Ϊ204C������֮ǰ�������
	USARTx->CR2 = 0x0000;//FPGA_BIT��Ĭ������CR1Ϊ204C������֮ǰ�������
	read_ahb32(0xE000E280) = 0xFFFFFFFF;//���PENDINGλ

    sample_uart_init(USARTx);	//��Ĭ�ϲ�����ʼ����ӡ����
	
	USART_SetAddress(USARTx, 0x05);
	USART_WakeUpConfig(USARTx, 0x800); //WAKE--11λ��1-��ַƥ����
	USART_ReceiverWakeUpCmd(USARTx, ENABLE);


	while(1)
	{
		//temp = USARTx->CR1;
		//printf("CR1:0x%x\r\n",temp);
			
		if(USART_RX_STA&0x8000)
		{	
			printf("CR1:%x\r\n",USARTx->CR1);
			//�����յ�������
			len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
			for(t=0;t<len;t++)
			{
				USART_SendData(USARTx, USART_RX_BUF[t]);//�򴮿�1��������
				while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
			}
			USART_RX_STA=0;
			
			//���������ɣ��˳�����;���Զ˲�֧��2.25M/4.5M�����ʣ����ﲻ��������������
			if(count++ >= 10)
				break;
    	}
	}
	
	//�������̣��ͷ���Դ
	//sample_uart_deinit(USARTx);	//������ɣ��ͷ���Դ��
	printf("file:%s,line:%d\r\ntest is finished\r\n",__FILE__,__LINE__);
}
