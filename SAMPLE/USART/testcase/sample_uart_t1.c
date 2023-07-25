#include "sys.h"
#include "delay.h"
#include "sample_usart.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_uart_test_t1
 * Function ��CTS����ģʽ��������������
 * @author  taohu
 * @date    2022-03-15
 * @param   
		USARTx:USART����ַ
 * @return void
*******************************************************************************/    
void sample_uart_t1(USART_TypeDef* USARTx)
{	   
	u16 temp,count = 0;
	
	//���Կ�ʼ��ӡ
	printf("test is started\r\nfile:%s,line:%d\r\n",__FILE__,__LINE__);
	USARTx->CR1 = 0x0000;//FPGA_BIT��Ĭ������CR1Ϊ204C������֮ǰ�������
	read_ahb32(0xE000E280) = 0xFFFFFFFF;//���PENDINGλ

	//���ò�������ʼ��
	sample_uart_set_param(eUSART_FLOW,USART_HardwareFlowControl_CTS); //��115200��ʼ����ӡ����
    sample_uart_init(USARTx);	//��Ĭ�ϲ�����ʼ����ӡ����

	//�������̣���������ͷ�WHILEѭ��
	while(1)
	{
		temp = USARTx->SR;
		printf("SR:0x%x\r\n",temp);
		
		if((temp & 0x200) != 0)
		{
			USARTx->SR &= 0xFDFF;
			printf("CTS clear-SR:0x%x\r\n",USARTx->SR);
		}
		delay_ms(10000);
		USART_SendData(USARTx, count++);
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
        
        if(count > 256)
        {
            break;
        }
	}
	

	//�������̣��ͷ���Դ��������ɴ�ӡ
	//sample_uart_deinit(USARTx);	//������ɣ��ͷ���Դ��
	printf("file:%s,line:%d\r\ntest is finished\r\n",__FILE__,__LINE__);
}
