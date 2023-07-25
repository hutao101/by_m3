#include "sys.h"
#include "delay.h"
#include "sample_usart.h"
#include "stm32f10x.h"
#include "snps_dmac.h"   // DMA Controller public header

extern unsigned char buffer[20];
/*******************************************************************************
 * @brief   sample_uart_test_t8
 * Function ȫ˫���첽ͨ�ţ�����������1.2K-4.5M������UART�͵��Զ�ͨ�Ź���
 * @author  taohu
 * @date    2022-1-6
 * @param   
		USARTx:USART����ַ
 * @return void
*******************************************************************************/    
	void sample_uart_t28(USART_TypeDef* USARTx)
	{
		u8 channel = 0x07;
		//u32 *src_addr = (u32 *)0x20004000;
		u32  i;
		
		printf("test is started\r\nfile:%s,line:%d\r\n",__FILE__,__LINE__);
		
		for(i=0;i< 20; i ++)
		 {
			//*src_addr++ = 0;//0x20004000 + i*4;
		 }
		 
		USARTx->CR1 = 0x0000;//FPGA_BIT中默认配置CR1为204C，启动之前清除配置
		
		USART_DMACmd(USARTx, 1<<6, ENABLE);//DMA RX传输使能
		
		sample_uart_set_param(eUSART_BAUD,9600);
		sample_uart_init(USARTx);	//以默认参数初始化打印串口

		USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);
	
		//clear all the status register of select channel
		dw_dmac_clearIrq(channel, IRQ_TYPE_ALL);
		dmac_per2mem_channel_cfg(channel); //DMA配置
		 
		printf("DMAC_CTL_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_L(channel)));
		printf("DMAC_CTL_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_H(channel)));

		printf("DMAC_CFG_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(channel)));
		printf("DMAC_CFG_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_H(channel)));
	
		printf("SR:0x%x\r\n",USARTx->SR);
		//enable the channel to start the transfer
		dw_dmac_enableChannel(channel);
		
		printf("PD0:0x%x\r\n",read_ahb32(0xE000E200));
	
		printf("SR:0x%x\r\n",USARTx->SR);
	
		while(1)
		{
			delay_ms(5000);
			printf("SR:0x%x\r\n",USARTx->SR);
			//printf("DR:0x%x\r\n",USARTx->DR);
			for(i = 0;i <20;i++ )
			{
				if(buffer[i] != 0x55)
					printf("i:0x%x;0x%x\r\n",i,buffer[i]);
			}
			//USART_SendData(USARTx,0x55);
		}	
	
		//sample_uart_deinit(USARTx);	//外设deinit
		//printf("file:%s,line:%d\r\ntest is finished\r\n",__FILE__,__LINE__);
	}

