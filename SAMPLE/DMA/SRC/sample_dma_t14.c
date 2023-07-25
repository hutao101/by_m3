#include "sample_dma.h"
//m2p 8bit 中断测试
u8 ch_t14 = 0;
int dmac_8bit_mem2iic_cfg_t14(int channel)
{
    u32 reg_value; 
	//configure the source address 
	write_ahb32(AHB_DMAC_BASE +DMAC_SAR_L(channel),0x20004A00);
	//configure the destination address
	write_ahb32(AHB_DMAC_BASE +DMAC_DAR_L(channel), I2C1_BASE+I2C_DATA_CMD);
	//configure the CTL_L
	reg_value =   (1 << 20)	   										// memory to peripheral ,control by ahb-dmac
				 | (0 << 17)                                	 	// gather enable
				 | (0 << 18)										//scatter disable
				 | (1 << 11)| (1 << 14)		         //source burst length is 4	and dest also is 4 
				 | (2 << 7) | (0 << 9)		 						//increment the source and no change destination address
				 | (0 << 4) | (0 << 1)								//source and destination transfer width is 8bits
				 | 1;				   								//interrupt enable 
	write_ahb32(AHB_DMAC_BASE + DMAC_CTL_L(channel), reg_value);
	//configure the CTL_L	 
	reg_value =  16;	   										 
	write_ahb32(AHB_DMAC_BASE + DMAC_CTL_H(channel), reg_value);
	//configure the CFG_L
	reg_value =    (0 << 19)| (0 << 18)								//handshake polarrity, src default 0, dest base on setting	   										// memory to memory ,control by ahb-dmac
				 | (0 << 11)| (0 << 10)								//handshake select hardware
				 | (0 << 8 ) 										//DMA transfer from the source is not sspended
				 | (channel << 5);				    				//channel prority is set to the channel index,means the channel 7 is the hishgest channel
	write_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(channel), reg_value);
	//configure the CFG_H
	reg_value =    (0 << 7)| (I2C1_TX_DMA_BREQ << 11)								//USART3 TX-- 5
				 | (1 << 2) 										//prot non-cache, non-buff,prvilge data
				 | (0 << 1 ) 										//Space/data available for single transfer 
				 | 0  ;				    							//source data pre-fetching is enable
	write_ahb32(AHB_DMAC_BASE + DMAC_CFG_H(channel), reg_value);


	return 0;
}

/****************************************************************************
 * @brief   sample_dma_t14
 * Function test the SRAM to IIC dma transfer of SRAM and IIC are 8bit for per ch,to check 
 * whether Interrupt is ok ,this test not include err Interrupt
 * @author  he
 * @date    2022-04-20
 * @param   
		channel:DMA CH 0-7
 * @return void
******************************************************************************/

void sample_dma_t14(int channel)
{
	u32 i = 0;
	u32 j =0;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = 1;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 		//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure); //根据指定的参数初始化NVIC寄存器
	
	//注意：需要先使能IIC，再往I2C_DATA_CMD寄存器写数据
	sample_i2c_t1_toDMA_t3();
	
	for(j=0;j<6;j++)
	{
		for(i=0;i<200;i++)  //1200
		{
			write_ahb32(0x20004A00+j*200+i,i);
		}
	}
	dw_dmac_clearIrq(channel, IRQ_TYPE_ALL);
	dw_dmac_unmaskIrq(channel,IRQ_TYPE_ALL);
	
	dmac_8bit_mem2iic_cfg_t14(channel);
	dw_dmac_enableChannel(channel);
	
	delay_ms(100);
	//IIC1_DMA 支持
	write_ahb32(I2C1_BASE + I2C_DMA_CR, 0x3); //0x1 rdmae  0x2 Tmdae
//write_ahb32(I2C1_BASE+ I2C_DMA_RDLR, I2C_FIFO_DEPTH/2-1);
//write_ahb32(I2C1_BASE + I2C_DMA_TDLR, I2C_FIFO_DEPTH/2);

	for(i=0;i<16;i++)
	{
		delay_ms(10);
		printf("\r\n\r\nI2C2_BASE+I2C_DATA_CMD：0x%x\r\n",read_ahb32(I2C2_BASE+I2C_DATA_CMD));
		printf("StatusBlock_L：0x%x\r\n",         read_ahb32(AHB_DMAC_BASE+DMAC_STATUS_BLOCK_L));
		printf("RwaBlock_L：0x%x\r\n",            read_ahb32(AHB_DMAC_BASE+DMAC_RAW_BLOCK_L));
		printf("StatusSrcTran_L：0x%x\r\n",       read_ahb32(AHB_DMAC_BASE+DMAC_STATUS_SRCTRAN_L));
		printf("RawSrcTran_L：0x%x\r\n",          read_ahb32(AHB_DMAC_BASE+DMAC_RAW_SRCTRAN_L));
		printf("StatusDesTran_L：0x%x\r\n",       read_ahb32(AHB_DMAC_BASE+DMAC_STATUS_DSTTRAN_L));
		printf("RawDesTran_L：0x%x\r\n",          read_ahb32(AHB_DMAC_BASE+DMAC_RAW_DSTTRAN_L));
		printf("DMAC_CFG_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(channel)));
	}
}


/****************************************************************************
 * @brief   sample_dma_t3_8CH
 * Function test the IIC to SRAM dma transfer of SRAM and IIC are 8bit for 8CH
 * @author  he
 * @date    2022-3-31
 * @param  void
 * @return void
******************************************************************************/
void sample_dma_t14_8ch(void)
{
	u8 i = 0;
	//注意：需要先使能IIC，再往I2C_DATA_CMD寄存器写数据
	sample_i2c_t1_toDMA_t3();
	
	write_ahb32(0x20004A00,0xAABBCCDD);
	write_ahb32(0x20004A04,0x01020304);
	for(ch_t14=0;ch_t14<8;ch_t14++)
	{
		dw_dmac_clearIrq(ch_t14, IRQ_TYPE_ALL);
		dmac_8bit_mem2iic_cfg_t14(ch_t14);
		
		printf("DMAC_CTL_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_L(ch_t14)));
		printf("DMAC_CTL_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_H(ch_t14)));

		printf("DMAC_CFG_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(ch_t14)));
		printf("DMAC_CFG_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_H(ch_t14)));
		

		dw_dmac_enableChannel(ch_t14);
		
		//IIC1_DMA 支持
		write_ahb32(I2C1_BASE + I2C_DMA_CR, 0x3); //0x1 rdmae  0x2 Tmdae
		//write_ahb32(I2C1_BASE+ I2C_DMA_RDLR, I2C_FIFO_DEPTH/2-1);
		//write_ahb32(I2C1_BASE + I2C_DMA_TDLR, I2C_FIFO_DEPTH/2);

		for(i=0;i<7;i++)
		{
			delay_ms(10000);
			printf("\r\nI2C2_BASE+I2C_DATA_CMD：0x%x",read_ahb32(I2C2_BASE+I2C_DATA_CMD));
		}
		printf("\r\n\r\n");
		dw_dmac_disableChannel(ch_t14);
		
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////
//公共函数名，用不到的时候需要注释
/**********************************************************************/
//void DMA_IRQHandler(void)
//{
//    u32  irq_active;
//    // read the interrupt status register
//    irq_active = read_ahb32(AHB_DMAC_BASE +DMAC_STATUS_INT_L);
//		
//		printf("STATUS_INT_L: 0x%x\r\n",irq_active);

//    // ERR INTERRUPT
//    if((irq_active & IRQ_TYPE_ERROR) == IRQ_TYPE_ERROR) {

//        // clear the interrupt
//        dw_dmac_clearIrq(ch_t14, IRQ_TYPE_ERROR);
//    }
//    // TFR INTERRUPT
//	// not enable in this driver
//   	else if((irq_active & IRQ_TYPE_TFR) == IRQ_TYPE_TFR) {
//        
// 	    // clear the interrupt
//        dw_dmac_clearIrq(ch_t14, IRQ_TYPE_TFR);
//    }   
//    // BLOCK INTERRUPT
//    else if((irq_active & 0x2) == 0x2) {

//		// clear the block interrupt
//         dw_dmac_clearIrq(ch_t14, IRQ_TYPE_BLOCK);
//    }
//    // SRCTRAN INTERRUPT
//	// not enable in this driver
//    else if((irq_active & 0x4) == 0x4) {
//			
//        // clear the interrupt
//        dw_dmac_clearIrq(ch_t14, IRQ_TYPE_SRCTRAN);
//    }
//    // DSTTRAN INTERRUPT
//	// not enable in this driver
//    else if((irq_active & 0x8) == 0x8) {
//        
//        // clear the interrupt
//        dw_dmac_clearIrq(ch_t14, IRQ_TYPE_DSTTRAN);
//    }
//} 


