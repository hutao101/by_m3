#include "sys.h"
#include "delay.h"
#include "sample_usart.h"
#include "stm32f10x.h"
#include "snps_dmac.h"   // DMA Controller public header

extern unsigned char buffer[20];
/*******************************************************************************
 * @brief   sample_uart_test_t8
 * Function È«Ë«¹¤Òì²½Í¨ĞÅ£º±éÀú²¨ÌØÂÊ1.2K-4.5M£»²âÊÔUARTºÍµçÄÔ¶ËÍ¨ĞÅ¹¦ÄÜ
 * @author  taohu
 * @date    2022-1-6
 * @param   
		USARTx:USART»ùµØÖ·
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
		 
		USARTx->CR1 = 0x0000;//FPGA_BITä¸­é»˜è®¤é…ç½®CR1ä¸º204Cï¼Œå¯åŠ¨ä¹‹å‰æ¸…é™¤é…ç½®
		
		USART_DMACmd(USARTx, 1<<6, ENABLE);//DMA RXä¼ è¾“ä½¿èƒ½
		
		sample_uart_set_param(eUSART_BAUD,9600);
		sample_uart_init(USARTx);	//ä»¥é»˜è®¤å‚æ•°åˆå§‹åŒ–æ‰“å°ä¸²å£

		USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);
	
		//clear all the status register of select channel
		dw_dmac_clearIrq(channel, IRQ_TYPE_ALL);
		dmac_per2mem_channel_cfg(channel); //DMAé…ç½®
		 
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
	
		//sample_uart_deinit(USARTx);	//å¤–è®¾deinit
		//printf("file:%s,line:%d\r\ntest is finished\r\n",__FILE__,__LINE__);
	}

