#include "sample_dma.h"
//p2m 8bit  中断测试

u32 ch_t16 = 0;

//正常 配置，（测试除了错误中断其他的传输类型能否进中断）
int dmac_8bit_iic2mem_cfg_t16(int channel)
{
    u32 reg_value; 
	//configure the source address 
	write_ahb32(AHB_DMAC_BASE +DMAC_SAR_L(channel),I2C2_BASE+I2C_DATA_CMD);
	//configure the destination address
	write_ahb32(AHB_DMAC_BASE +DMAC_DAR_L(channel),0x20004B00);
	//configure the CTL_L
	reg_value =   (2 << 20)	   										// peripheral to memory,control by ahb-dmac
				 | (0 << 17)                                	 	// gather enable
				 | (0 << 18)										//scatter disable
				 | (2 << 11)| (0 << 14)		       //des 8  src 1    //source burst length is 16	and dest burst length is half of the peripheral fifo depth
				 | (0 << 7) | (2 << 9)		 						//increment the source and no change destination address
				 | (0 << 4) | (0 << 1)								//src 8bit des 16bit
				 | 1;				   								//interrupt enable 
	write_ahb32(AHB_DMAC_BASE + DMAC_CTL_L(channel), reg_value);
	//configure the CTL_H	 
	reg_value =  16;	   										 
	write_ahb32(AHB_DMAC_BASE + DMAC_CTL_H(channel), reg_value);
	//configure the CFG_L
	reg_value =    (0 << 19)| (0 << 18)								//handshake polarrity, src default 0, dest base on setting	   										// memory to memory ,control by ahb-dmac
				 | (0 << 11)| (0 << 10)								//handshake select hardware
				 | (0 << 8 ) 										//DMA transfer from the source is not sspended
				 | (channel << 5);				    				//channel prority is set to the channel index,means the channel 7 is the hishgest channel
	write_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(channel), reg_value);
	//configure the CFG_H
	reg_value =    (I2C2_RX_DMA_BREQ << 7)| (0 << 11)								//USART3 RX -- 4
				 | (1 << 2) 										//prot non-cache, non-buff,prvilge data
				 | (0 << 1 ) 										//Space/data available for single transfer 
				 | 0  ;				    							//source data pre-fetching is enable
	write_ahb32(AHB_DMAC_BASE + DMAC_CFG_H(channel), reg_value);


	return 0;
}


////src 128bit 测试是否会发生错误中断
//int dmac_8bit_iic2mem_cfg_t16(int channel)
//{
//    u32 reg_value; 
//	//configure the source address 
//	write_ahb32(AHB_DMAC_BASE +DMAC_SAR_L(channel),I2C2_BASE+I2C_DATA_CMD);
//	//configure the destination address
//	write_ahb32(AHB_DMAC_BASE +DMAC_DAR_L(channel),0x20004B00);
//	//configure the CTL_L
//	reg_value =   (2 << 20)	   										// peripheral to memory,control by ahb-dmac
//				 | (0 << 17)                                	 	// gather enable
//				 | (0 << 18)										//scatter disable
//				 | (2 << 11)| (0 << 14)		       //des 8  src 1    //source burst length is 16	and dest burst length is half of the peripheral fifo depth
//				 | (0 << 7) | (2 << 9)		 						//increment the source and no change destination address
//				 | (4 << 4) | (0 << 1)								//src 128bit des 8bit
//				 | 1;				   								//interrupt enable 
//	write_ahb32(AHB_DMAC_BASE + DMAC_CTL_L(channel), reg_value);
//	//configure the CTL_H	 
//	reg_value =  1;	   										 
//	write_ahb32(AHB_DMAC_BASE + DMAC_CTL_H(channel), reg_value);
//	//configure the CFG_L
//	reg_value =    (0 << 19)| (0 << 18)								//handshake polarrity, src default 0, dest base on setting	   										// memory to memory ,control by ahb-dmac
//				 | (0 << 11)| (0 << 10)								//handshake select hardware
//				 | (0 << 8 ) 										//DMA transfer from the source is not sspended
//				 | (channel << 5);				    				//channel prority is set to the channel index,means the channel 7 is the hishgest channel
//	write_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(channel), reg_value);
//	//configure the CFG_H
//	reg_value =    (I2C2_RX_DMA_BREQ << 7)| (0 << 11)								//USART3 RX -- 4
//				 | (1 << 2) 										//prot non-cache, non-buff,prvilge data
//				 | (0 << 1 ) 										//Space/data available for single transfer 
//				 | 0  ;				    							//source data pre-fetching is enable
//	write_ahb32(AHB_DMAC_BASE + DMAC_CFG_H(channel), reg_value);


//	return 0;
//}

/****************************************************************************
 * @brief   sample_dma_t16
 * Function test the IIC to SRAM dma transfer of des and src is 8bit for per ch，to check 
 * whether Interrupt is ok ,this test not include err Interrupt
 * @author  he
 * @date    2022-04-21
 * @param   
		channel:DMA通道 of 0-7
 * @return void
******************************************************************************/

void sample_dma_t16(int channel)
{

	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = 1;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 		//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure); //根据指定的参数初始化NVIC寄存器
	
	dw_dmac_clearIrq(channel, IRQ_TYPE_ALL);
	dw_dmac_unmaskIrq(channel,IRQ_TYPE_ALL);
	dmac_8bit_iic2mem_cfg_t16(channel);
	
	printf("DMAC_CTL_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_L(channel)));
	printf("DMAC_CTL_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_H(channel)));

	printf("DMAC_CFG_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(channel)));
	printf("DMAC_CFG_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_H(channel)));
	
	dw_dmac_enableChannel(channel);
	
	sample_i2c_t1_toDma_t16();
	
	//IIC1_DMA 支持
	write_ahb32(I2C2_BASE + I2C_DMA_CR, 0x3); //0x1 rdmae  0x2 Tmdae

	
	delay_ms(1000);
	printf("\r\n0x20004B00：0x%x\r\n",read_ahb32(0x20004B00));
	printf("\r\n0x20004B04：0x%x\r\n",read_ahb32(0x20004B04));
	printf("\r\n0x20004B08：0x%x\r\n",read_ahb32(0x20004B08));
	printf("\r\n0x20004B0C：0x%x\r\n",read_ahb32(0x20004B0C));


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
//        dw_dmac_clearIrq(ch_t16, IRQ_TYPE_ERROR);
//    }
//    // TFR INTERRUPT
//	// not enable in this driver
//   	else if((irq_active & IRQ_TYPE_TFR) == IRQ_TYPE_TFR) {
//        
// 	    // clear the interrupt
//        dw_dmac_clearIrq(ch_t16, IRQ_TYPE_TFR);
//    }   
//    // BLOCK INTERRUPT
//    else if((irq_active & 0x2) == 0x2) {

//		// clear the block interrupt
//         dw_dmac_clearIrq(ch_t16, IRQ_TYPE_BLOCK);
//    }
//    // SRCTRAN INTERRUPT
//	// not enable in this driver
//    else if((irq_active & 0x4) == 0x4) {
//			
//        // clear the interrupt
//        dw_dmac_clearIrq(ch_t16, IRQ_TYPE_SRCTRAN);
//    }
//    // DSTTRAN INTERRUPT
//	// not enable in this driver
//    else if((irq_active & 0x8) == 0x8) {
//        
//        // clear the interrupt
//        dw_dmac_clearIrq(ch_t16, IRQ_TYPE_DSTTRAN);
//    }
//} 



