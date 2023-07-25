
#include "snps_ahb_dma.h"   // DMA Controller public header

u8 ch_irq = 0;

struct snps_ahb_dmac snps_ahb_dmac ={
  	.irq_num = 1,
	.dmac_int_en = 1,  //default enable the interrupt
};

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
		return true;
	else	
		return false;
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
	if(dma_en == false)
		dw_dmac_enable();
    write_ahb32(AHB_DMAC_BASE +DMAC_CH_EN_REG_L, (1 << channel) | (1 << (channel+8)));
	
		read_ahb32(AHB_DMAC_BASE+DMAC_CH_EN_REG_L);
//		printf("this is get ch whether is busy\r\n");
//		printf("\r\n通道使能位值：0x%x\r\n",read_ahb32(AHB_DMAC_BASE+DMAC_CH_EN_REG_L));
//		printf("\r\n通道：0x%x\r\n",channel);
	
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
		return true;
	else 
		return false;
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
/**********************************************************************/
void dw_dmac_maskIrq(int channel, int irq_type)
{
    u32 reg;
	// clear TFR 
	if((irq_type & IRQ_TYPE_TFR) == IRQ_TYPE_TFR){
	   reg = read_ahb32(AHB_DMAC_BASE + DMAC_MASK_TFR_L);
	   if((reg & (1 << channel)) != 0x0)
	   		write_ahb32(AHB_DMAC_BASE + DMAC_MASK_TFR_L , reg & (1 << (channel+8)));	
	}
	//clear block
	if((irq_type & IRQ_TYPE_BLOCK) == IRQ_TYPE_BLOCK)
	{
	   reg = read_ahb32(AHB_DMAC_BASE + DMAC_MASK_BLOCK_L);
	   if((reg & (1 << channel)) != 0x0)
	   		write_ahb32(AHB_DMAC_BASE + DMAC_MASK_BLOCK_L , reg & (1 << (channel+8)));	
	}
	//clear srctran
   	if((irq_type & IRQ_TYPE_SRCTRAN) == IRQ_TYPE_SRCTRAN)
	{
	   reg = read_ahb32(AHB_DMAC_BASE + DMAC_MASK_SRCTRAN_L);
	   if((reg & (1 << channel)) != 0x0)
	   		write_ahb32(AHB_DMAC_BASE + DMAC_MASK_SRCTRAN_L , reg & (1 << (channel+8)));	
	}
	//clear dsttran
	if((irq_type & IRQ_TYPE_DSTTRAN) == IRQ_TYPE_DSTTRAN)
	{
	   reg = read_ahb32(AHB_DMAC_BASE + DMAC_MASK_DSTTRAN_L);
	   if((reg & (1 << channel)) != 0x0)
	   		write_ahb32(AHB_DMAC_BASE + DMAC_MASK_DSTTRAN_L , reg & (1 << (channel+8)));	
	}
	//clear error
	if((irq_type & IRQ_TYPE_ERROR) == IRQ_TYPE_ERROR)
	{
	   reg = read_ahb32(AHB_DMAC_BASE + DMAC_MASK_ERR_L);
	   if((reg & (1 << channel)) != 0x0)
	   		write_ahb32(AHB_DMAC_BASE + DMAC_MASK_ERR_L , reg & (1 << (channel+8)));
	}
}

/**********************************************************************/
void dw_dmac_unmaskIrq(int channel, int irq_type)
{
    u32 reg;
	// clear TFR 
	if((irq_type & IRQ_TYPE_TFR) == IRQ_TYPE_TFR){
	   reg = read_ahb32(AHB_DMAC_BASE + DMAC_MASK_TFR_L);
	   if((reg & (1 << channel)) == 0x0)
	   		write_ahb32(AHB_DMAC_BASE + DMAC_MASK_TFR_L , reg | (1 << channel) | (1 << channel+8));	
	}
	//clear block
	if((irq_type & IRQ_TYPE_BLOCK) == IRQ_TYPE_BLOCK)
	{
	   reg = read_ahb32(AHB_DMAC_BASE + DMAC_MASK_BLOCK_L);
	   if((reg & (1 << channel)) == 0x0)
	   		write_ahb32(AHB_DMAC_BASE + DMAC_MASK_BLOCK_L , reg | (1 << channel) | (1 << channel+8));	
	}
	//clear srctran
   	if((irq_type & IRQ_TYPE_SRCTRAN) == IRQ_TYPE_SRCTRAN)
	{
	   reg = read_ahb32(AHB_DMAC_BASE + DMAC_MASK_SRCTRAN_L);
	   if((reg & (1 << channel)) == 0x0)
	   		write_ahb32(AHB_DMAC_BASE + DMAC_MASK_SRCTRAN_L , reg | (1 << channel) | (1 << channel+8));	
	}
	//clear dsttran
	if((irq_type & IRQ_TYPE_DSTTRAN) == IRQ_TYPE_DSTTRAN)
	{
	   reg = read_ahb32(AHB_DMAC_BASE + DMAC_MASK_DSTTRAN_L);
	   if((reg & (1 << channel)) == 0x0)
	   		write_ahb32(AHB_DMAC_BASE + DMAC_MASK_DSTTRAN_L , reg | (1 << channel) | (1 << channel+8));	
	}
	//clear error
	if((irq_type & IRQ_TYPE_ERROR) == IRQ_TYPE_ERROR)
	{
	   reg = read_ahb32(AHB_DMAC_BASE + DMAC_MASK_ERR_L);
	   if((reg & (1 << channel)) == 0x0)
	   		write_ahb32(AHB_DMAC_BASE + DMAC_MASK_ERR_L , reg | (1 << channel) | (1 << channel+8));
	}
}
/**********************************************************************/
int dw_dmac_getIdleChannel(void)
{
	u32 reg;
	int i=0;
	reg = read_ahb32(AHB_DMAC_BASE+DMAC_CH_EN_REG_L);
	//scan from the high priority channel 7
	while(1)
	{
	   if((reg & (0x80 >> i ))  == 0x0)
	   		return 7-i;
	   else{
	   		if( i>=8 )
				break;
			else
				i++;	
	   }
	}
	return -1;
}
int dmac_checkdone(int channel)
{
	if(snps_ahb_dmac.channel_table[channel].transfer_status ==1)
		return 1;
	else 
		return 0;
}

/**
******************************************************************************************
**	 function for memory to memory transfer 
**   source TR_WIDTH: 32bits
**   destination TR_WIDTH: 32 bits
******************************************************************************************
**/
int dmac_mem2mem_channel_cfg(struct mem2mem_info *mem2mem, int channel)
{
    u32 reg_value;  
	//configure the source address 
	write_ahb32(AHB_DMAC_BASE +DMAC_SAR_L(channel), mem2mem->src_addr);
	//configure the destination address
	write_ahb32(AHB_DMAC_BASE +DMAC_DAR_L(channel), mem2mem->des_addr);
	//configure the CTL_L
	reg_value =   (0 << 20)	   										// memory to memory ,control by ahb-dmac
				 | ((mem2mem->gather.enable) ? (1 << 17) : 0)	 	// gather enable
				 | ((mem2mem->scatter.enable) ? (1 << 18) : 0)		//scatter enable
				 | (3 << 11)| (3 << 14)								//burst length is 16 (MSIZE)
				 | (0 << 7) | (0 << 9)		 						//increment the source and destination address
				 | (mem2mem->src_width << 4) | (mem2mem->des_width <<1 )									//source and destination transfer width is 32bits
				 | ((mem2mem->int_enable) ? 1:0);				    //interrupt enable 
	write_ahb32(AHB_DMAC_BASE + DMAC_CTL_L(channel), reg_value);	
	//configure the CTL_L	 
	reg_value =  mem2mem->block_size ;	   										 
	write_ahb32(AHB_DMAC_BASE + DMAC_CTL_H(channel), reg_value);
	//configure the CFG_L
	reg_value =    (0 << 19)| (0 << 18)								//handshake polarrity, default 0	   										// memory to memory ,control by ahb-dmac
		#if 0                                       //handshake select, default 0(硬件握手)
				 | (1 << 11)| (1 << 10)								
		#else
				 | (0 << 11)| (0 << 10)
		#endif
				 | (0 << 8 ) 										//DMA transfer from the source is not sspended
				 | (channel << 5);				    				//channel prority is set to the channel index,means the channel 7 is the hishgest channel
	write_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(channel), reg_value);
	//configure the CFG_H
	reg_value =    (0 << 11)| (0 << 7)								//handshake req source number,default 0，硬件握手需配置，软件握手不用管
				 | (1 << 2) 										//prot non-cache, non-buff,prvilge data
				 | (0 << 1 ) 										//Space/data available for single transfer 
				 | 0  ;				    							//source data pre-fetching is enable
	write_ahb32(AHB_DMAC_BASE + DMAC_CFG_H(channel), reg_value);
	//configure the gather if enable
	if(mem2mem->gather.enable == 1) {
		reg_value =  ((mem2mem->gather.section_length_inbytes /4 ) << 20 )	|	( mem2mem->gather.section_offset_inbytes / 4);
		write_ahb32(AHB_DMAC_BASE + DMAC_SGR_L(channel), reg_value);				
	}
	//configure the scatter if enable
	if(mem2mem->scatter.enable == 1) {
	  	reg_value =  ((mem2mem->scatter.section_length_inbytes /4 ) << 20 )	|	( mem2mem->scatter.section_offset_inbytes / 4);
		write_ahb32(AHB_DMAC_BASE + DMAC_DSR_L(channel), reg_value);
	}
	return 0;
}



/**
******************************************************************************************
**	 function for peripheral to memory transfer 
**   source TR_WIDTH: 32bits
**   destination TR_WIDTH: 32 bits
******************************************************************************************
**/
int dmac_per2mem_channel_cfg(struct per2mem_info *per2mem, int channel)
{
    u32 reg_value;  
	//configure the source address 
	write_ahb32(AHB_DMAC_BASE +DMAC_SAR_L(channel), per2mem->src_per_addr);
	//configure the destination address
	write_ahb32(AHB_DMAC_BASE +DMAC_DAR_L(channel), per2mem->des_mem_addr);
	//configure the CTL_L
	reg_value =   (2 << 20)	   										// peripheral to memory ,control by ahb-dmac
				 | (0 << 17)	 								    // gather disable
				 | ((per2mem->scatter.enable) ? (1 << 18) : 0)		//scatter enable
				 | (3 << 11)| ((per2mem->src_per_fifodepth/2) << 14)//dest burst length is 16, src is the hlaf of the peripheral fifo depth
				 | (0 << 7) | (2 << 9)		 						//no change the source and increment destination address
				 | (per2mem->src_width << 4) | (per2mem->des_width <<1 )									//source and destination transfer width is 32bits
				 | ((per2mem->int_enable) ? 1:0);				    //interrupt enable 
	write_ahb32(AHB_DMAC_BASE + DMAC_CTL_L(channel), reg_value);	
	//configure the CTL_L	 
	reg_value =  per2mem->block_size ;	   										 
	write_ahb32(AHB_DMAC_BASE + DMAC_CTL_H(channel), reg_value);
	//configure the CFG_L
	reg_value =    (per2mem->src_per_polarity << 19)| (0 << 18)	    //handshake polarrity, dest default 0, src base on setting	   										// memory to memory ,control by ahb-dmac
				 | (0 << 11)| (0 << 10)								////handshake select hardware
				 | (0 << 8 ) 										//DMA transfer from the source is not sspended
				 | (channel << 5);				    				//channel prority is set to the channel index,means the channel 7 is the hishgest channel
	write_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(channel), reg_value);
	//configure the CFG_H
	reg_value =    (per2mem->src_per_handshake << 7)| (0 << 11)	     //handshake des req source number,default 0, src base on the setting
				 | (1 << 2) 										//prot non-cache, non-buff,prvilge data
				 | (0 << 1 ) 										//Space/data available for single transfer 
				 | 0  ;				    							//source data pre-fetching is enable
	write_ahb32(AHB_DMAC_BASE + DMAC_CFG_H(channel), reg_value);
	//configure the scatter if enable
	if(per2mem->scatter.enable == 1) {
	  	reg_value =  ((per2mem->scatter.section_length_inbytes /4 ) << 20 )	|	( per2mem->scatter.section_offset_inbytes / 4);
		write_ahb32(AHB_DMAC_BASE + DMAC_DSR_L(channel), reg_value);
	}
	return 0;
}

/**
******************************************************************************************
**	 function for memory to memory transfer 
**   source TR_WIDTH: 32bits
**   destination TR_WIDTH: 32 bits
******************************************************************************************
**/
int dmac_mem2per_channel_cfg(struct mem2per_info *mem2per, int channel)
{
    u32 reg_value;  
	//configure the source address 
	write_ahb32(AHB_DMAC_BASE +DMAC_SAR_L(channel), mem2per->src_mem_addr);
	//configure the destination address
	write_ahb32(AHB_DMAC_BASE +DMAC_DAR_L(channel), mem2per->des_per_addr);
	//configure the CTL_L
	reg_value =   (1 << 20)	   										// memory to peripheral ,control by ahb-dmac
				 | ((mem2per->gather.enable) ? (1 << 17) : 0)	 	// gather enable
				 | (0 << 18)										//scatter disable
				 | ((mem2per->des_per_fifodepth/2) << 11)| (3 << 14)//source burst length is 16	and dest burst length is half of the peripheral fifo depth
				 | (2 << 7) | (2 << 9)		 						//increment the source and no change destination address
				 | (mem2per->src_width << 4) | (mem2per->des_width <<1 )									//source and destination transfer width is 8bits
				 | ((mem2per->int_enable) ? 1:0);				    //interrupt enable 
	write_ahb32(AHB_DMAC_BASE + DMAC_CTL_L(channel), reg_value);
	//configure the CTL_L	 
	reg_value =  mem2per->block_size ;	   										 
	write_ahb32(AHB_DMAC_BASE + DMAC_CTL_H(channel), reg_value);
	//configure the CFG_L
	reg_value =    (0 << 19)| (mem2per->des_per_polarity << 18)		//handshake polarrity, src default 0, dest base on setting	   										// memory to memory ,control by ahb-dmac
				 | (0 << 11)| (0 << 10)								//handshake select hardware
				 | (0 << 8 ) 										//DMA transfer from the source is not sspended
				 | (channel << 5);				    				//channel prority is set to the channel index,means the channel 7 is the hishgest channel
	write_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(channel), reg_value);
	//configure the CFG_H
	reg_value =    (0 << 7)| (mem2per->des_per_handshake << 11)		//handshake src req source number,default 0, dest base on the settin
				 | (1 << 2) 										//prot non-cache, non-buff,prvilge data
				 | (0 << 1 ) 										//Space/data available for single transfer 
				 | 0  ;				    							//source data pre-fetching is enable
	write_ahb32(AHB_DMAC_BASE + DMAC_CFG_H(channel), reg_value);
	//configure the gather if enable
	if(mem2per->gather.enable == 1) {
		reg_value =  ((mem2per->gather.section_length_inbytes /4 ) << 20 )	|	( mem2per->gather.section_offset_inbytes / 4);
		write_ahb32(AHB_DMAC_BASE + DMAC_SGR_L(channel), reg_value);				
	}
	return 0;
}


/**
******************************************************************************************
**	 function for peripheral to mem transfer 
**   source TR_WIDTH: 32bits
**   destination TR_WIDTH: 32 bits
******************************************************************************************
**/
int dmac_mem2mem_transfer(struct mem2mem_info *mem2mem,int channel)
{
	
	struct snps_ahb_dmac *dmac = &snps_ahb_dmac;

	//scan one idle channel from the high priority to low

	if(channel == -1)
	{
		snps_dma_printf("All the DMA channel is enable, no idle channel!!\n");
		return 1;																		   
	}
	//clear all the status register of select channel
	dw_dmac_clearIrq(channel, IRQ_TYPE_ALL);

   	//unmask the interrupt when enable 
	if(mem2mem->int_enable)	{
	   dw_dmac_unmaskIrq(channel,IRQ_TYPE_BLOCK|IRQ_TYPE_ERROR);
	   dmac->channel_table[channel].channel_int_en = 1;
	}
	else 
		dmac->channel_table[channel].channel_int_en = 0;

	//configure the data transfer using the dile channel
	if(mem2mem->trans_length_inbytes == 0)
		return 1;
	else{
	    dmac->channel_table[channel].trans_type = DMA_TRANS_TYPE_MEM2MEM;
		dmac->channel_table[channel].total_bytes = mem2mem->trans_length_inbytes;
		dmac->channel_table[channel].trans_bytes = 0;
		dmac->channel_table[channel].transfer_status = 0;		 	 
		if(mem2mem->trans_length_inbytes >= MEM2MEM_MAX_BLK_BYTES)  
		{
			mem2mem->block_size = 128;
			dmac->channel_table[channel].block_size = 128;
		}
		else {
			mem2mem->block_size = (mem2mem->trans_length_inbytes+3)/4;
			dmac->channel_table[channel].block_size = (mem2mem->trans_length_inbytes+3)/4;
		}
				
		//configure the select channel
		dmac_mem2mem_channel_cfg(mem2mem, channel);
		//enable the channel to start the transfer
		dw_dmac_enableChannel(channel);
	}
	return 0;
}

/**
******************************************************************************************
**	 function for memory to peripheral transfer 
**   source TR_WIDTH: 32bits
**   destination TR_WIDTH: 32 bits
******************************************************************************************
**/
int dmac_mem2per_transfer(struct mem2per_info *mem2per,int channel)
{

	struct snps_ahb_dmac *dmac = &snps_ahb_dmac;

	//scan one idle channel from the high priority to low
//	channel = dw_dmac_getIdleChannel();
//	if(channel == -1)
//	{
//		snps_dma_printf("All the DMA channel is enable, no idle channel!!\n");
//		return 1;																		   
//	}
	//clear all the status register of select channel
	dw_dmac_clearIrq(channel, IRQ_TYPE_ALL);

   	//unmask the interrupt when enable 
	if(mem2per->int_enable)	{
	   dw_dmac_unmaskIrq(channel,IRQ_TYPE_BLOCK|IRQ_TYPE_ERROR);
	   dmac->channel_table[channel].channel_int_en = 1;
	}
	else 
		dmac->channel_table[channel].channel_int_en = 0;

	//configure the data transfer using the dile channel
	if(mem2per->trans_length_inbytes == 0)
		return 1;
	else{
		dmac->channel_table[channel].trans_type = DMA_TRANS_TYPE_MEM2PER;
		dmac->channel_table[channel].total_bytes = mem2per->trans_length_inbytes;
		dmac->channel_table[channel].trans_bytes = 0;
		dmac->channel_table[channel].transfer_status = 0;		 	 
		if(mem2per->trans_length_inbytes >= MEM2MEM_MAX_BLK_BYTES)  
		{
			mem2per->block_size = 128;
			dmac->channel_table[channel].block_size = 128;
		}
		else {
			mem2per->block_size = (mem2per->trans_length_inbytes+3)/4;
			dmac->channel_table[channel].block_size = (mem2per->trans_length_inbytes+3)/4;
		}
		//configure the select channel
		dmac_mem2per_channel_cfg(mem2per, channel);

		//enable the channel to start the transfer
		dw_dmac_enableChannel(channel);
	}
	return 0;
}

/**
******************************************************************************************
**	 function for peripheral to memory transfer 
**   source TR_WIDTH: 32bits
**   destination TR_WIDTH: 32 bits
******************************************************************************************
**/
int dmac_per2mem_transfer(struct per2mem_info *per2mem,int channel)
{

	struct snps_ahb_dmac *dmac = &snps_ahb_dmac;

	//scan one idle channel from the high priority to low
//	channel = dw_dmac_getIdleChannel();
//	if(channel == -1)
//	{
//		snps_dma_printf("All the DMA channel is enable, no idle channel!!\n");
//		return 1;																		   
//	}
	//clear all the status register of select channel
	dw_dmac_clearIrq(channel, IRQ_TYPE_ALL);

   	//unmask the interrupt when enable 
	if(per2mem->int_enable)	{
	   dw_dmac_unmaskIrq(channel,IRQ_TYPE_BLOCK|IRQ_TYPE_ERROR);
	   dmac->channel_table[channel].channel_int_en = 1;
	}
	else 
		dmac->channel_table[channel].channel_int_en = 0;

	//configure the data transfer using the dile channel
	if(per2mem->trans_length_inbytes == 0)
		return 1;
	else{
		dmac->channel_table[channel].trans_type = DMA_TRANS_TYPE_PER2MEM;
		dmac->channel_table[channel].total_bytes = per2mem->trans_length_inbytes;
		dmac->channel_table[channel].trans_bytes = 0;
		dmac->channel_table[channel].transfer_status = 0;		 	 
		if(per2mem->trans_length_inbytes >= MEM2MEM_MAX_BLK_BYTES)  
		{
			per2mem->block_size = 128;
			dmac->channel_table[channel].block_size = 128;
		}
		else {
			per2mem->block_size = (per2mem->trans_length_inbytes+3)/4;
			dmac->channel_table[channel].block_size = (per2mem->trans_length_inbytes+3)/4;
		}
		//configure the select channel
		dmac_per2mem_channel_cfg(per2mem, channel);

		//enable the channel to start the transfer
		dw_dmac_enableChannel(channel);
	}
	return 0;
}



/**********************************************************************/
void dw_dmac_init(void)
{
	int i;
	struct snps_ahb_dmac *dmac = &snps_ahb_dmac;
    // Disable the DMA controller
    dw_dmac_disable();	
	//initial each channel
	for(i=0; i < 8 ; i++) {
		// Disable all DMA channel
		dw_dmac_disableChannel(1);
		// Mask all channel interrupts
	 	dw_dmac_maskIrq(i,IRQ_TYPE_ALL);
		// Clear any pending interrupts
		dw_dmac_clearIrq(i,IRQ_TYPE_ALL);
		//init the global dma structure
		dmac->channel_table[i].trans_type = DMA_TRANS_TYPE_MEM2MEM;
		dmac->channel_table[i].total_bytes = 0;
		dmac->channel_table[i].trans_bytes = 0;
		dmac->channel_table[i].transfer_status = 0;
		dmac->channel_table[i].channel_int_en  =0 ;
		dmac->channel_table[i].block_size = 128 ;
	}
	// Enable the DMA controller
    dw_dmac_enable();
} 


void DMA_IRQHandler(void)
{
    u32  irq_active;
    // read the interrupt status register
    irq_active = read_ahb32(AHB_DMAC_BASE +DMAC_STATUS_INT_L);
		
		printf("STATUS_INT_L: 0x%x\r\n",irq_active);

    // ERR INTERRUPT
    if((irq_active & IRQ_TYPE_ERROR) == IRQ_TYPE_ERROR) {

        // clear the interrupt
        dw_dmac_clearIrq(ch_irq, IRQ_TYPE_ERROR);
    }
    // TFR INTERRUPT
	// not enable in this driver
   	else if((irq_active & IRQ_TYPE_TFR) == IRQ_TYPE_TFR) {
        
 	    // clear the interrupt
        dw_dmac_clearIrq(ch_irq, IRQ_TYPE_TFR);
    }   
    // BLOCK INTERRUPT
    else if((irq_active & 0x2) == 0x2) {

		// clear the block interrupt
         dw_dmac_clearIrq(ch_irq, IRQ_TYPE_BLOCK);
    }
    // SRCTRAN INTERRUPT
	// not enable in this driver
    else if((irq_active & 0x4) == 0x4) {
			
        // clear the interrupt
        dw_dmac_clearIrq(ch_irq, IRQ_TYPE_SRCTRAN);
    }
    // DSTTRAN INTERRUPT
	// not enable in this driver
    else if((irq_active & 0x8) == 0x8) {
        
        // clear the interrupt
        dw_dmac_clearIrq(ch_irq, IRQ_TYPE_DSTTRAN);
    }
} 


