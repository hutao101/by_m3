#include "snps_dmac.h"   // DMA Controller public header

unsigned char buffer[20] = {0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55}; 


/**********************************************************************/
void dw_dmac_enable(void)
{
	u32 reg;
	reg = read_ahb32(AHB_DMAC_BASE +DMAC_DMA_CFG_REG_L);
	if(reg != 0x1) 
		write_ahb32(AHB_DMAC_BASE +DMAC_DMA_CFG_REG_L, 0x1);
}
/**********************************************************************/
void dw_dmac_disable(void)
{
	u32 reg;
	reg = read_ahb32(AHB_DMAC_BASE +DMAC_DMA_CFG_REG_L);
	if(reg != 0x0) 
		write_ahb32(AHB_DMAC_BASE +DMAC_DMA_CFG_REG_L, 0x0);
}
/**********************************************************************/
bool dw_dmac_isEnabled(void)
{
	u32 reg;
	reg = read_ahb32(AHB_DMAC_BASE +DMAC_DMA_CFG_REG_L);
	if(reg == 0x1) 
		return TRUE;
	else	
		return FALSE;
}
/**********************************************************************/
int dw_dmac_checkChannelBusy(int channel)
{
    u32 reg;
    reg = read_ahb32(AHB_DMAC_BASE +DMAC_CH_EN_REG_L);
    if(reg & (1<<channel) != 0) 
        return 1;
	else
		return 0;
}

/**********************************************************************/
void dw_dmac_enableChannel(int channel)
{
	bool dma_en;
	dma_en =  dw_dmac_isEnabled();
	if(dma_en == 0)
		dw_dmac_enable();
    write_ahb32(AHB_DMAC_BASE +DMAC_CH_EN_REG_L, (1 << channel) | (1 << (channel+8)));
}
/**********************************************************************/
void dw_dmac_disableChannel(int channel)
{
    u32 reg;
    u32 enabled_ch;
	reg = read_ahb32(AHB_DMAC_BASE +DMAC_CH_EN_REG_L);
	enabled_ch = reg & (1<< channel);
	if(enabled_ch != 0x0) 
		write_ahb32(AHB_DMAC_BASE +DMAC_CH_EN_REG_L,  reg & (1 << (channel+8)));
}
/**********************************************************************/
bool dw_dmac_isChannelEnabled(int channel)
{
    u32 reg;
    reg = read_ahb32(AHB_DMAC_BASE +DMAC_CH_EN_REG_L);
	if((reg & (1<<channel)) != 0)
		return TRUE;
	else 
		return FALSE;
}
/**********************************************************************/
void dw_dmac_clearIrq(int channel, int irq_type)
{
	u32 reg;
	// clear TFR 
	if((irq_type & IRQ_TYPE_TFR) == IRQ_TYPE_TFR){
	   reg = read_ahb32(AHB_DMAC_BASE + DMAC_RAW_TFR_L);
	   if((reg & (1 << channel)) != 0x0)
	   		write_ahb32(AHB_DMAC_BASE + DMAC_CLEAR_TFR_L , 1 << channel);	
	}
	//clear block
	if((irq_type & IRQ_TYPE_BLOCK) == IRQ_TYPE_BLOCK)
	{
	   reg = read_ahb32(AHB_DMAC_BASE + DMAC_RAW_BLOCK_L);
	   if((reg & (1 << channel)) != 0x0)
	   		write_ahb32(AHB_DMAC_BASE + DMAC_CLEAR_BLOCK_L , 1 << channel);	
	}
	//clear srctran
   	if((irq_type & IRQ_TYPE_SRCTRAN) == IRQ_TYPE_SRCTRAN)
	{
	   reg = read_ahb32(AHB_DMAC_BASE + DMAC_RAW_SRCTRAN_L);
	   if((reg & (1 << channel)) != 0x0)
	   		write_ahb32(AHB_DMAC_BASE + DMAC_CLEAR_SRCTRAN_L , 1 << channel);	
	}
	//clear dsttran
	if((irq_type & IRQ_TYPE_DSTTRAN) == IRQ_TYPE_DSTTRAN)
	{
	   reg = read_ahb32(AHB_DMAC_BASE + DMAC_RAW_DSTTRAN_L);
	   if((reg & (1 << channel)) != 0x0)
	   		write_ahb32(AHB_DMAC_BASE + DMAC_CLEAR_DSTTRAN_L , 1 << channel);	
	}
	//clear error
	if((irq_type & IRQ_TYPE_ERROR) == IRQ_TYPE_ERROR)
	{
	   reg = read_ahb32(AHB_DMAC_BASE + DMAC_RAW_ERR_L);
	   if((reg & (1 << channel)) != 0x0)
	   		write_ahb32(AHB_DMAC_BASE + DMAC_CLEAR_ERR_L , 1 << channel);	
	}
}

/**
******************************************************************************************
**	 function for memory to memory transfer 
**   source TR_WIDTH: 32bits
**   destination TR_WIDTH: 32 bits
******************************************************************************************
**/
int dmac_mem2per_channel_cfg(int channel)
{
    u32 reg_value; 
	//configure the source address 
	write_ahb32(AHB_DMAC_BASE +DMAC_SAR_L(channel),(u32)buffer);
	//configure the destination address
	write_ahb32(AHB_DMAC_BASE +DMAC_DAR_L(channel), 0x40018c00+0x04);
	//configure the CTL_L
	reg_value =   (1 << 20)	   										// memory to peripheral ,control by ahb-dmac
				 | (0 << 17)                                	 	// gather enable
				 | (0 << 18)										//scatter disable
				 | (0 << 11)| (1 << 14)		                        //source burst length is 16	and dest burst length is half of the peripheral fifo depth
				 | (2 << 7) | (0 << 9)		 						//increment the source and no change destination address
				 | (0 << 4) | (0 << 1)								//source and destination transfer width is 8bits
				 | 0;				   								//interrupt enable 
	write_ahb32(AHB_DMAC_BASE + DMAC_CTL_L(channel), reg_value);
	//configure the CTL_L	 
	reg_value =  12;	   										 
	write_ahb32(AHB_DMAC_BASE + DMAC_CTL_H(channel), reg_value);
	//configure the CFG_L
	reg_value =    (0 << 19)| (0 << 18)								//handshake polarrity, src default 0, dest base on setting	   										// memory to memory ,control by ahb-dmac
				 | (0 << 11)| (0 << 10)								//handshake select hardware
				 | (0 << 8 ) 										//DMA transfer from the source is not sspended
				 | (channel << 5);				    				//channel prority is set to the channel index,means the channel 7 is the hishgest channel
	write_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(channel), reg_value);
	//configure the CFG_H
	reg_value =    (0 << 7)| (3 << 11)								//USART2 TX-- 3
				 | (1 << 2) 										//prot non-cache, non-buff,prvilge data
				 | (0 << 1 ) 										//Space/data available for single transfer 
				 | 0 ;				    							//source data pre-fetching is enable
	write_ahb32(AHB_DMAC_BASE + DMAC_CFG_H(channel), reg_value);

	return 0;
}

int dmac_per2mem_channel_cfg(int channel)
{
    u32 reg_value; 
	//configure the source address 
	write_ahb32(AHB_DMAC_BASE +DMAC_SAR_L(channel),0x40018c00+0x04);
	//configure the destination address
	write_ahb32(AHB_DMAC_BASE +DMAC_DAR_L(channel),(u32)buffer);
	//configure the CTL_L
	reg_value =   (2 << 20)	   										// peripheral to memory,control by ahb-dmac
				 | (0 << 17)                                	 	// gather enable
				 | (0 << 18)										//scatter disable
				 | (2 << 11)| (0 << 14)		                        //source burst length is 16	and dest burst length is half of the peripheral fifo depth
				 | (0 << 7) | (2 << 9)		 						//increment the source and no change destination address
				 | (0 << 4) | (2 << 1)								//source and destination transfer width is 8bits
				 | 0;				   								//interrupt enable 
	write_ahb32(AHB_DMAC_BASE + DMAC_CTL_L(channel), reg_value);
	//configure the CTL_H	 
	reg_value =  11;	   										 
	write_ahb32(AHB_DMAC_BASE + DMAC_CTL_H(channel), reg_value);
	//configure the CFG_L
	reg_value =    (0 << 19)| (0 << 18)								//handshake polarrity, src default 0, dest base on setting	   										// memory to memory ,control by ahb-dmac
				 | (0 << 11)| (0 << 10)								//handshake select hardware
				 | (0 << 8 ) 										//DMA transfer from the source is not sspended
				 | (channel << 5);				    				//channel prority is set to the channel index,means the channel 7 is the hishgest channel
	write_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(channel), reg_value);
	//configure the CFG_H
	reg_value =    (2 << 7)| (0 << 11)								//USART2 RX -- 2
				 | (1 << 2) 										//prot non-cache, non-buff,prvilge data
				 | (0 << 1 ) 										//Space/data available for single transfer 
				 | 0  ;				    							//source data pre-fetching is enable
	write_ahb32(AHB_DMAC_BASE + DMAC_CFG_H(channel), reg_value);


	return 0;
}


