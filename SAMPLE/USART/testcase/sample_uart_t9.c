#include "sys.h"
#include "delay.h"
#include "sample_usart.h"
#include "stm32f10x.h"


#define reg(mreg)   *(volatile unsigned int *)(mreg)
/*******************************************************************************
 * @brief   sample_uart_test_t1
 * Function ���ܿ�ģʽ������NACK
 * @author  taohu
 * @date    2022-1-6
 * @param   
		USARTx:USART����ַ
 * @return void
*******************************************************************************/    
void sample_uart_t9(USART_TypeDef* USARTx)
{	   
   USART_ClockInitTypeDef USART_ClockInitStructure;  

	printf("test is started\r\nfile:%s,line:%d\r\n",__FILE__,__LINE__);
	USARTx->CR1 = 0x0000;//FPGA_BIT��Ĭ������CR1Ϊ204C������֮ǰ�������
	
	/* USART Clock set to 3.6 MHz (PCLK1 (36 MHZ) / 10) */
	USART_SetPrescaler(USARTx, 0x05);

	/* USART Guard Time set to 16 Bit */
	USART_SetGuardTime(USARTx,0X02);

	USART_ClockInitStructure.USART_Clock = USART_Clock_Enable;
	USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
	USART_ClockInitStructure.USART_CPHA = USART_CPHA_1Edge;
	USART_ClockInitStructure.USART_LastBit = USART_LastBit_Enable;
	USART_ClockInit(USARTx, &USART_ClockInitStructure);

	sample_uart_set_param(eUSART_BAUD,9600);
	sample_uart_set_param(eUSART_WORDLEN,USART_WordLength_9b);
	sample_uart_set_param(eUSART_STPB,USART_StopBits_1_5);
	sample_uart_set_param(eUSART_PRT,USART_Parity_Even);
    sample_uart_init(USARTx);	//��Ĭ�ϲ�����ʼ����ӡ����
    
	USART_ITConfig(USARTx, USART_IT_PE, ENABLE);

	/* Enable the NACK Transmission */
	USART_SmartCardNACKCmd(USARTx, ENABLE);

	/* Enable the Smartcard Interface */
	USART_SmartCardCmd(USARTx, ENABLE);
  
	printf("CR1:%x\r\n",USARTx->CR1);
	printf("SR1:%x\r\n",USARTx->SR);
	printf("PD0:0x%x\r\n",read_ahb32(0xE000E200));

	USART_SendData(USARTx, 0x55);//�򴮿�1��������
	
	while(1)
	{
		printf("SR:0x%x\r\n",USARTx->SR);
		delay_ms(1000);
		//printf("CR1:0x%x,CR2:0x%x,CR3:0x%x\r\n",USARTx->CR1,USARTx->CR2,USARTx->CR3);
		//while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
	}	

	//sample_uart_deinit(USARTx);	//����deinit
	//printf("file:%s,line:%d\r\ntest is finished\r\n",__FILE__,__LINE__);
}

