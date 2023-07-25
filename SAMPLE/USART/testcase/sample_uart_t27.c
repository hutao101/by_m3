#include "sys.h"
#include "delay.h"
#include "sample_usart.h"
#include "stm32f10x.h"
#include "snps_dmac.h"   // DMA Controller public header

/*******************************************************************************
 * @brief   sample_uart_test_t7
 * Function 智能卡模式：卡端通信
 * @author  taohu
 * @date    2022-1-6
 * @param   
		USARTx:USART基地址
 * @return void
*******************************************************************************/    
void sample_uart_t27(USART_TypeDef* USARTx)
{
	u8 channel = 0x07;
    u32  i;
	
	printf("test is started\r\nfile:%s,line:%d\r\n",__FILE__,__LINE__);
	 
	USARTx->CR1 = 0x0000;//FPGA_BIT中默认配置CR1为204C，启动之前清除配置
	read_ahb32(0xE000E200) = 0;

	//clear all the status register of select channel
	dw_dmac_clearIrq(channel, IRQ_TYPE_ALL);
	dmac_mem2per_channel_cfg(channel); //DMA配置
	 
	printf("DMAC_CTL_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_L(channel)));
	printf("DMAC_CTL_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_H(channel)));

	printf("DMAC_CFG_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(channel)));
	printf("DMAC_CFG_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_H(channel)));

	//enable the channel to start the transfer
	dw_dmac_enableChannel(channel);
	printf("DAMC_ENABLE\r\n");

	USART_DMACmd(USARTx, 1<<7, ENABLE);//DMA传输使能
	
	sample_uart_set_param(eUSART_BAUD,9600);
    sample_uart_init(USARTx);	//以默认参数初始化打印串口

	printf("USART_CR1:0x%x\r\n",USARTx->CR1);
	printf("USART_CR2:0x%x\r\n",USARTx->CR2);
	printf("USART_CR3:0x%x\r\n",USARTx->CR3);
	
	printf("SR:0x%x\r\n",USARTx->SR);

	while(1)
	{
		delay_ms(5000);
		printf("USART_SR:0x%x\r\n",USARTx->SR);
		printf("DMAC_CFG_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(channel)));
	}	

	//sample_uart_deinit(USARTx);	//外设deinit
	//printf("file:%s,line:%d\r\ntest is finished\r\n",__FILE__,__LINE__);
}

