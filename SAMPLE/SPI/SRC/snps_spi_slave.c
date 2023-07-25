/*
 * SNPS SPI slave driver file
 */

#include "snps_spi_common.h"
#include "snps_spi_slave.h"


struct snps_spi_init snps_spi_slave_init_table[2] = {
	/* spi1*/
	{
		.spi_id = 0,
		.base_addr = SPI1_SLAVE_BASE,
		.tmode = SPI_TMOD_TR,
		.work_mode = SPI_MODE0,
		.word_size = SPI_DFS16(8),
		.type = SPI_FRF_SPI,
		.irq_num = SLAVE_SPI1_IRQ,
		.xfer_mode = XFER_POLL_MODE,
	},

	/*spi2*/
	{
		.spi_id = 1,
		.base_addr = SPI2_SLAVE_BASE,
		.tmode = SPI_TMOD_TR,
		.work_mode = SPI_MODE0,
		.word_size = SPI_DFS16(8),
		.type = SPI_FRF_SPI,		
		.irq_num = SLAVE_SPI2_IRQ,
		.xfer_mode = XFER_POLL_MODE,
	}
};

bool snps_spi_slave_output_isenable(struct snps_spi_init *init)
{
	u32 reg;
	reg = read_ahb32(init->base_addr + DW_SPI_CTRL0);	
	if((reg & SPI_SLAVE_DISABLE) == SPI_SLAVE_DISABLE)
		return false;
	else	
		return true;	
}

void snps_spi_slave_output_enable(struct snps_spi_init *init)
{
	u32 reg;
	reg = read_ahb32(init->base_addr + DW_SPI_CTRL0);
	write_ahb32(init->base_addr+DW_SPI_CTRL0, reg & (~SPI_SLAVE_DISABLE) );
} 
void snps_spi_slave_output_disable(struct snps_spi_init *init)
{
	u32 reg;
	reg = read_ahb32(init->base_addr+ DW_SPI_CTRL0);
	write_ahb32(init->base_addr+DW_SPI_CTRL0, reg | SPI_SLAVE_DISABLE);	
} 
 
void snps_spi_deivce_init(struct snps_spi_init *init)
{
	u32 ctl0 = 0;
	u32 base = init->base_addr;

	snps_spi_printf("ip reg base is 0x%x\n", base);
	snps_spi_printf("version is 0x%x\n",read_ahb32(base+ DW_SPI_VERSION));

	snps_spi_disable(base);

	/*initializes the slave mode for the serial transfer*/
	/*1.Write CTRLR0 to match the required transfer*/
	/*STD_SPI_FRF/FRAME_08BITS/Transmit & Receive/MODE0/MOTOROLA_SPI*/
	ctl0 = init->type | init->work_mode | init->word_size | init->tmode;
	write_ahb32(base+ DW_SPI_CTRL0, ctl0);
	snps_spi_printf("addr = 0x%x, val = 0x%x\n",base + DW_SPI_CTRL0, read_ahb32(base+ DW_SPI_CTRL0));

	/*2.Write IMR register to set interrupt masks*/
	write_ahb32(base+ DW_SPI_IMR, 0x00);
	snps_spi_printf("addr = 0x%x, val = 0x%x\n",base + DW_SPI_IMR, read_ahb32(base+ DW_SPI_IMR));

	/*3.Write TXFTLR & RXFTLR to set FIFO Threshold Level*/
	write_ahb32(base+ DW_SPI_TXFTLR, FIFO_DEPTH / 2);
	write_ahb32(base+ DW_SPI_RXFTLR, FIFO_DEPTH / 2 - 1);
	snps_spi_printf("addr = 0x%x, val = 0x%x\n",base + DW_SPI_TXFTLR, read_ahb32(base+ DW_SPI_TXFTLR));
	snps_spi_printf("addr = 0x%x, val = 0x%x\n",base + DW_SPI_RXFTLR, read_ahb32(base+ DW_SPI_RXFTLR));
	
	/*4.enable the slave output*/
	snps_spi_slave_output_enable(init);
}


void snps_spi_slave_receive(struct snps_spi_init *init)
{
	u32 ctl0 = 0;
	u32 base = init->base_addr;
	u8 *rx_buff = init->spi_operation_data.rx_buff;
	int dma_channel;
	struct per2mem_info per2mem;

	snps_spi_disable(base);
	ctl0 = read_ahb32(base+ DW_SPI_CTRL0);
	ctl0 &= ~(SPI_TMOD_MASK | SPI_SPI_FRF_MASK | SPI_DFS_16_MASK);
	ctl0 |= SPI_TMOD_RO;
	ctl0 |= SPI_DFS16(8);
	ctl0 |= SPI_STD_SPI_FRF;
	write_ahb32(base+ DW_SPI_CTRL0, ctl0);
	snps_spi_mask_int(base, 0x1f);

	if(init->xfer_mode ==XFER_IRQ_MODE){
		/*config RX trigger level*/
		if (init->spi_operation_data.rx_len < FIFO_DEPTH / 2)
			write_ahb32(base+ DW_SPI_RXFTLR, init->spi_operation_data.rx_len - 1);
		else
			write_ahb32(base+ DW_SPI_RXFTLR, (FIFO_DEPTH / 2) - 1);
		/*rx_fifo underflow, overflow and full interrupt unmask*/
		snps_spi_unmask_int(base,  SPI_INT_RXFI);
		NVIC_EnableIRQ(init->irq_num);
		snps_spi_enable(base);
		write_ahb32(base+ DW_SPI_DR, 0xff);	
	}
	else if(init->xfer_mode ==XFER_POLL_MODE) {
		snps_spi_enable(base);
		write_ahb32(base+ DW_SPI_DR, 0xff);
		snps_spi_read_data(base, rx_buff, init->spi_operation_data.rx_len);
	}
	else if(init->xfer_mode ==XFER_DMA_MODE){
		write_ahb32(base+DW_SPI_DMACR, RDMAE_EN);
		write_ahb32(base+DW_SPI_DMATDLR, FIFO_DEPTH / 2);
		write_ahb32(base+DW_SPI_DMARDLR, FIFO_DEPTH / 2 - 1);
		//configure the DMA and start
		dma_channel = dw_dmac_getIdleChannel();
		if(dma_channel == -1)
		{
			snps_spi_printf("All the DMA channel is enable, no idle channel!!\n");
			return ;																		   
		}
		per2mem.src_per_addr = base+DW_SPI_DR;
		per2mem.des_mem_addr = (u32)init->spi_operation_data.rx_buff;
		per2mem.trans_length_inbytes = init->spi_operation_data.rx_len;
		if(base == SPI1_SLAVE_BASE)
			per2mem.src_per_handshake = SPI1_RX_DMA_BREQ;
		else if(base == SPI2_SLAVE_BASE)
			per2mem.src_per_handshake = SPI2_RX_DMA_BREQ;
		per2mem.src_per_polarity = 0; // active high
		per2mem.src_per_fifodepth = FIFO_DEPTH;	 	
		per2mem.scatter.enable = 0;
		dmac_per2mem_transfer_init(&per2mem, dma_channel);
		//enable the channel to start the transfer
		dw_dmac_enableChannel(dma_channel);
		snps_spi_enable(base);
#ifndef MASTER_SLAVE_TEST
		//wait for the DMA transfer done
		while( dmac_checkdone(dma_channel) != 1 );
		//close the DMA 
		dmac_ch_close(dma_channel); 
		//clear the data length
	   	init->spi_operation_data.rx_len = 0;
#endif
	}
}

void snps_spi_slave_transmit(struct snps_spi_init *init)
{
	u32 ctl0 = 0;
	u8 *tx_buff = init->spi_operation_data.tx_buff;
	u32 base = init->base_addr;
	int dma_channel;
	struct mem2per_info mem2per;

    snps_spi_disable(base);
	ctl0 = read_ahb32(base+ DW_SPI_CTRL0);
	ctl0 &= ~(SPI_TMOD_MASK | SPI_SPI_FRF_MASK | SPI_DFS_16_MASK);
	ctl0 |= SPI_TMOD_TO;
	ctl0 |= SPI_DFS16(8);
	ctl0 |= SPI_STD_SPI_FRF;
	write_ahb32(base+ DW_SPI_CTRL0, ctl0);
	snps_spi_mask_int(base, 0x1f);
	
	if(init->xfer_mode ==XFER_IRQ_MODE){
		/*config TX trigger level*/
		if (init->spi_operation_data.tx_len  > FIFO_DEPTH)
			write_ahb32(base+ DW_SPI_TXFTLR, FIFO_DEPTH / 2);
		else
			write_ahb32(base+ DW_SPI_TXFTLR, 1);

		/*tx_fifo empty and overflow interrupt umask*/
		snps_spi_unmask_int(base, SPI_INT_TXEI );  	
		/*initerrupt free and request*/
		NVIC_DisableIRQ(init->irq_num);
		NVIC_EnableIRQ(init->irq_num);	
		snps_spi_enable(base);	
	}
	else if(init->xfer_mode == XFER_POLL_MODE){
		snps_spi_enable(base);
		if (init->spi_operation_data.tx_len != 0)
			snps_spi_write_data(base, tx_buff, init->spi_operation_data.tx_len);
		init->spi_operation_data.tx_len = 0;
	}
	else if(init->xfer_mode ==XFER_DMA_MODE){ 
		write_ahb32(base+DW_SPI_DMACR, TDMAE_EN);
		write_ahb32(base+DW_SPI_DMATDLR, FIFO_DEPTH / 2);
		write_ahb32(base+DW_SPI_DMARDLR, FIFO_DEPTH / 2 - 1);
		snps_spi_enable(base);	
		if(init->spi_operation_data.tx_len != 0) {
			//configure the DMA TX and start
			dma_channel = dw_dmac_getIdleChannel();
			if(dma_channel == -1)
			{
				snps_spi_printf("All the DMA channel is enable, no idle channel!!\n");
				return ;																		   
			}
			mem2per.des_per_addr = base+DW_SPI_DR;
			mem2per.src_mem_addr = (u32)init->spi_operation_data.tx_buff;
			mem2per.trans_length_inbytes = init->spi_operation_data.tx_len;
			if(base == SPI1_SLAVE_BASE)
				mem2per.des_per_handshake = SPI1_TX_DMA_BREQ;
			else if(base == SPI2_SLAVE_BASE)
				mem2per.des_per_handshake = SPI2_TX_DMA_BREQ;
			mem2per.des_per_polarity = 0; // active high
			mem2per.des_per_fifodepth = FIFO_DEPTH;	 	
			mem2per.gather.enable = 0;
			dmac_mem2per_transfer_init(&mem2per, dma_channel);
			//enable the channel to start the transfer
			dw_dmac_enableChannel(dma_channel);
#ifndef MASTER_SLAVE_TEST
			//wait for the DMA transfer done
			while( dmac_checkdone(dma_channel) != 1);
			//close the DMA 
			dmac_ch_close(dma_channel); 
			//clear the data length
		   	init->spi_operation_data.tx_len = 0;
#endif
		}
	}
}

void snps_spi_slave_duplex(struct snps_spi_init *init)
{
	u32 len = 0 ;
	u32 ctl0 = 0;
	u8 *tx_buff = init->spi_operation_data.tx_buff;
	u8 *rx_buff = init->spi_operation_data.rx_buff;
	u32 base = init->base_addr;
	int dma_tx_channel;
	int dma_rx_channel;
	struct per2mem_info per2mem;
	struct mem2per_info mem2per;
	u32 i;

	snps_spi_disable(base);
	/*TMOD = TR, STD_SPI*/
	ctl0 = read_ahb32(base+ DW_SPI_CTRL0);
	ctl0 &= ~(SPI_TMOD_MASK | SPI_SPI_FRF_MASK | SPI_DFS_16_MASK);
	ctl0 |= SPI_TMOD_TR;
	ctl0 |= SPI_DFS16(8);
	ctl0 |= SPI_STD_SPI_FRF;
	write_ahb32(base+ DW_SPI_CTRL0, ctl0);
	
	/*config TX trigger level*/
	if (init->spi_operation_data.tx_len  > FIFO_DEPTH)
		write_ahb32(base+ DW_SPI_TXFTLR, FIFO_DEPTH / 2);
	else
		write_ahb32(base+ DW_SPI_TXFTLR, 1);	
	
	/*config RX trigger level*/
	if (init->spi_operation_data.rx_len < FIFO_DEPTH / 2)
		write_ahb32(base+ DW_SPI_RXFTLR, init->spi_operation_data.rx_len - 1);
	else
		write_ahb32(base+ DW_SPI_RXFTLR, (FIFO_DEPTH / 2) - 1);
	
	snps_spi_mask_int(base, 0x1f);
			
	if(init->xfer_mode ==XFER_IRQ_MODE){
		/*tx_fifo empty and overflow interrupt umask*/
		snps_spi_unmask_int(base, SPI_INT_TXEI | SPI_INT_RXFI);  	
		/*initerrupt free and request*/
		NVIC_EnableIRQ(init->irq_num);	
		snps_spi_enable(base);
	}
	else if(init->xfer_mode ==XFER_POLL_MODE){
		snps_spi_enable(base);
		while(init->spi_operation_data.tx_len){
			len =MIN (init->spi_operation_data.tx_len, FIFO_DEPTH/2);
			snps_spi_write_data(base, tx_buff, len);
			snps_spi_read_data(base, rx_buff, len);
			init->spi_operation_data.tx_len -= len;
			tx_buff += len;
			rx_buff += len;
		}
	}
	else if(init->xfer_mode ==XFER_DMA_MODE){		
		write_ahb32(base+DW_SPI_DMACR, RDMAE_EN | TDMAE_EN);
		write_ahb32(base+DW_SPI_DMATDLR, FIFO_DEPTH / 2);
		write_ahb32(base+DW_SPI_DMARDLR, FIFO_DEPTH / 2 - 1);
		snps_spi_enable(base);		
		//configure the DMA TX and start
		if(init->spi_operation_data.tx_len > 0) {
			dma_tx_channel = dw_dmac_getIdleChannel();
			if(dma_tx_channel == -1)
			{
				snps_spi_printf("All the DMA channel is enable, no idle channel!!\n");
				return ;																		   
			}
			mem2per.des_per_addr = base+DW_SPI_DR;
			mem2per.src_mem_addr = (u32)init->spi_operation_data.tx_buff;
			mem2per.trans_length_inbytes = init->spi_operation_data.tx_len;
			if(base == SPI1_SLAVE_BASE)
				mem2per.des_per_handshake = SPI1_TX_DMA_BREQ;
			else if(base == SPI2_SLAVE_BASE)
				mem2per.des_per_handshake = SPI2_TX_DMA_BREQ;
			mem2per.des_per_polarity = 0; // active high
			mem2per.des_per_fifodepth = FIFO_DEPTH;	 	
			mem2per.gather.enable = 0;
			dmac_mem2per_transfer_init(&mem2per, dma_tx_channel);
			//enable the channel to start the transfer
			dw_dmac_enableChannel(dma_tx_channel);
		}
		//configure the DMA RX and start
		dma_rx_channel = dw_dmac_getIdleChannel();
		if(dma_rx_channel == -1)
		{
			snps_spi_printf("All the DMA channel is enable, no idle channel!!\n");
			return ;																		   
		}
		per2mem.src_per_addr = base+DW_SPI_DR;
		per2mem.des_mem_addr = (u32)init->spi_operation_data.rx_buff;
		per2mem.trans_length_inbytes = init->spi_operation_data.rx_len;
		if(base == SPI1_SLAVE_BASE)
			per2mem.src_per_handshake = SPI1_RX_DMA_BREQ;
		else if(base == SPI2_SLAVE_BASE)
			per2mem.src_per_handshake = SPI2_RX_DMA_BREQ;
		per2mem.src_per_polarity = 0; // active high
		per2mem.src_per_fifodepth = FIFO_DEPTH;	 	
		per2mem.scatter.enable = 0;
		dmac_per2mem_transfer_init(&per2mem, dma_rx_channel);  
		//enable the channel to start the transfer
		dw_dmac_enableChannel(dma_rx_channel);
#ifndef MASTER_SLAVE_TEST
		//wait for the DMA transfer done
		while( (dmac_checkdone(dma_tx_channel) != 1) || (dmac_checkdone(dma_rx_channel) != 1) );
		//close the DMA 
		dmac_ch_close(dma_tx_channel);
		dmac_ch_close(dma_rx_channel);
		//clear the data length
	   	init->spi_operation_data.rx_len = 0;
		init->spi_operation_data.tx_len = 0;
		init->spi_operation_data.cmd_len = 0;
#endif
	}

}


void Slave_SPI1_IRQHandler(void) __irq 
{
	u32 len = 0;
	u32 i = 0;
	struct snps_spi_init *init = &snps_spi_slave_init_table[0];
	u32 base = init->base_addr;
	u32 irq_status = read_ahb32(base+ DW_SPI_ISR);
	
	snps_spi_mask_int(base, irq_status);

	read_ahb32(base+ DW_SPI_ICR);

	if (irq_status & SPI_INT_RXFI) {
		len = MIN(init->spi_operation_data.rx_len, FIFO_DEPTH/2);
		for (i = 0; i < len; i++)
			init->spi_operation_data.rx_buff[i] =read_ahb32(base+ DW_SPI_DR);
		init->spi_operation_data.rx_len -= len;
		init->spi_operation_data.rx_buff += len;
		if (init->spi_operation_data.rx_len < FIFO_DEPTH/2 && init->spi_operation_data.rx_len > 0)
			write_ahb32(base+ DW_SPI_RXFTLR,init->spi_operation_data.rx_len - 1);
		if (init->spi_operation_data.rx_len == 0)
			snps_spi_mask_int(base, SPI_INT_RXFI);	
		else
			snps_spi_unmask_int(base, SPI_INT_RXFI);		
	}
	if (irq_status & SPI_INT_TXEI) {
		/*mask tx_fifo_empty interrupt*/
		if(init->spi_operation_data.tx_len >0 )	{
			len = MIN(init->spi_operation_data.tx_len,FIFO_DEPTH/2);
		//	snps_spi_write_data(base,init->spi_operation_data.tx_buff,len);
			for (i = 0; i < len; i++)
				write_ahb32(base+DW_SPI_DR,init->spi_operation_data.tx_buff[i]);
			init->spi_operation_data.tx_buff += len;
			init->spi_operation_data.tx_len -= len;
			if(init->spi_operation_data.tx_len == 0)
				write_ahb32(base+ DW_SPI_TXFTLR, 0);
			snps_spi_unmask_int(base, SPI_INT_TXEI);
		 }else {
			snps_spi_mask_int(base, SPI_INT_TXEI );
		}
	}
}

void Slave_SPI2_IRQHandler(void) __irq 
{
	u32 len = 0;
	u32 i = 0;
	struct snps_spi_init *init = &snps_spi_slave_init_table[1];
	u32 base = init->base_addr;
	u32 irq_status = read_ahb32(base+ DW_SPI_ISR);
	
	snps_spi_mask_int(base, irq_status);

	read_ahb32(base+ DW_SPI_ICR);

	if (irq_status & SPI_INT_RXFI) {
		len = MIN(init->spi_operation_data.rx_len, FIFO_DEPTH/2);
		for (i = 0; i < len; i++)
			init->spi_operation_data.rx_buff[i] =read_ahb32(base+ DW_SPI_DR);
		init->spi_operation_data.rx_len -= len;
		init->spi_operation_data.rx_buff += len;
		if (init->spi_operation_data.rx_len < FIFO_DEPTH/2 && init->spi_operation_data.rx_len > 0)
			write_ahb32(base+ DW_SPI_RXFTLR,init->spi_operation_data.rx_len - 1);
		if (init->spi_operation_data.rx_len == 0)
			snps_spi_mask_int(base, SPI_INT_RXFI);	
		else
			snps_spi_unmask_int(base, SPI_INT_RXFI);		
	}
	if (irq_status & SPI_INT_TXEI) {
		/*mask tx_fifo_empty interrupt*/
		if(init->spi_operation_data.tx_len >0 )	{
			len = MIN(init->spi_operation_data.tx_len,FIFO_DEPTH/2);
		//	snps_spi_write_data(base,init->spi_operation_data.tx_buff,len);
			for (i = 0; i < len; i++)
				write_ahb32(base+DW_SPI_DR,init->spi_operation_data.tx_buff[i]);
			init->spi_operation_data.tx_buff += len;
			init->spi_operation_data.tx_len -= len;
			if(init->spi_operation_data.tx_len == 0)
				write_ahb32(base+ DW_SPI_TXFTLR, 0);
			snps_spi_unmask_int(base, SPI_INT_TXEI);
		 }else {
			snps_spi_mask_int(base, SPI_INT_TXEI );
		}
	}	
}





