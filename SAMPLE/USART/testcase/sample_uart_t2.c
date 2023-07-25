#include "sys.h"
#include "delay.h"
#include "sample_usart.h"
#include "stm32f10x.h"



/*******************************************************************************
 * @brief   sample_uart_test_t2
 * Function Ӳ������ģʽ������δ���ʱ��RTS����
 * @author  taohu
 * @date    2022-1-6
 * @param   
		USARTx:USART����ַ
 * @return void
*******************************************************************************/    
void sample_uart_t2(USART_TypeDef* USARTx)
{	   
	u16 temp,count = 0;
	printf("test is started\r\nfile:%s,line:%d\r\n",__FILE__,__LINE__);
	USARTx->CR1 = 0x0000;//FPGA_BIT��Ĭ������CR1Ϊ204C������֮ǰ�������
	read_ahb32(0xE000E280) = 0xFFFFFFFF;//���PENDINGλ
	
	//sample_uart_set_param(eUSART_BAUD,1000000); //��115200��ʼ����ӡ����
	sample_uart_set_param(eUSART_FLOW,USART_HardwareFlowControl_RTS_CTS); //��115200��ʼ����ӡ����
    sample_uart_init(USARTx);	//��Ĭ�ϲ�����ʼ����ӡ����

	while(1)
	{
		if(count > 10)
			break;
	
		temp = USARTx->SR;
		printf("SR:0x%x\r\n",temp);
		
		//delay_ms(10000);
		USART_SendData(USARTx, count++);
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
		printf("DR:0x%x\r\n",USART_ReceiveData(USARTx));
	}
	

	//�������̣��ͷ���Դ
	//sample_uart_deinit(USARTx);	//������ɣ��ͷ���Դ��
	printf("file:%s,line:%d\r\ntest is finished\r\n",__FILE__,__LINE__);

}
