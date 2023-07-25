#include "sample_dma.h"
// p2m 32bit

int dmac_32bit_iic2mem_cfg(int channel)
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
				 | (0 << 4) | (2 << 1)								//src 8bit des 32bit
				 | 0;				   								//interrupt enable 
	write_ahb32(AHB_DMAC_BASE + DMAC_CTL_L(channel), reg_value);
	//configure the CTL_H	 
	reg_value =  13;	   										 
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

/****************************************************************************
 * @brief   sample_dma_t2
 * Function test the IIC to SRAM dma transfer ,IIC is 8bit£¬SRAM is 32bit for per ch
 * @author  he
 * @date    2022-3-31
 * @param   
		channel:DMA CH of 0-7
 * @return void
******************************************************************************/

void sample_dma_t2(int channel)
{
	//IIC1_DMA Ö§³Ö
	write_ahb32(I2C2_BASE + I2C_DMA_CR, 0x3); //0x1 rdmae  0x2 Tmdae
//write_ahb32(I2C1_BASE+ I2C_DMA_RDLR, I2C_FIFO_DEPTH/2-1);
//write_ahb32(I2C1_BASE + I2C_DMA_TDLR, I2C_FIFO_DEPTH/2);
	
	sample_i2c_t1_toDma_t2();
	
	dw_dmac_clearIrq(channel, IRQ_TYPE_ALL);
	dmac_32bit_iic2mem_cfg(channel);
	
	printf("DMAC_CTL_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_L(channel)));
	printf("DMAC_CTL_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_H(channel)));

	printf("DMAC_CFG_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(channel)));
	printf("DMAC_CFG_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_H(channel)));
	
	dw_dmac_enableChannel(channel);
	

	
	
	while(1)
	{
		delay_ms(10000);
		printf("\r\n0x20004B00£º0x%x\r\n",read_ahb32(0x20004B00));
		printf("\r\n0x20004B04£º0x%x\r\n",read_ahb32(0x20004B04));
		printf("\r\n0x20004B08£º0x%x\r\n",read_ahb32(0x20004B08));
		printf("\r\n0x20004B0C£º0x%x\r\n",read_ahb32(0x20004B0C));
	}
}




