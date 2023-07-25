/*
 * SNPS SPI common file
 */

#include "snps_spi_common.h"

void snps_spi_delay(u32 delay)
{
 	u32 i;
	for(i=delay*1000;i >0; i--)	  ;
}

void snps_spi_mask_int(u32 base, u32 mask)
{
	u32 new_mask = 0;
	new_mask = read_ahb32(base + DW_SPI_IMR) & (~mask);
	write_ahb32(base + DW_SPI_IMR, new_mask);
}

void snps_spi_unmask_int(u32 base, u32 mask)
{
	u32 new_mask = 0;

	new_mask = read_ahb32(base + DW_SPI_IMR) | mask;
	write_ahb32(base + DW_SPI_IMR, new_mask);
}

u32 snps_spi_check_states(u32 base, u32 offset, u32 flag, u8 level)
{
	if (level) {
		if (read_ahb32(base+offset) & flag)
			return 0;
	    else 
			return 1;
	} else {
		if ((read_ahb32(base+offset) & flag) == 0)
			return 0;
		else 
			return 1;
	}
}

void snps_spi_enable(u32 base)
{
	write_ahb32(base+DW_SPI_SSIENR, 1);
}

void snps_spi_disable(u32 base)
{
	write_ahb32(base+DW_SPI_SSIENR, 0);
}


u32 snps_spi_IsBusy(u32 base)
{
	u32 status;

	status =  read_ahb32(base+DW_SPI_SR);
	if((status & 0x1) ==0x1) // spi-ssi is busy
		 return 1;
	else 					 //spi-ssi is idle
		return 0;
}

u32 snps_spi_write_data(u32 base, u8 *prdata, u32 len)
{
	u32 tx_len = len;
	u32 write_len = 0;
	u8 *tx_buff = prdata;
	u32 status = 0;
	u32 i;
		  
	while (tx_len > 0) { 
		 status =  read_ahb32(base+DW_SPI_SR);
	     if((status & SR_TF_EMPT) ==SR_TF_EMPT)
		 {
		 	write_len = MIN(tx_len,FIFO_DEPTH /2 );
			for(i=0;i< write_len ; i++)
			{
				write_ahb32(base+DW_SPI_DR, *tx_buff);
				tx_len -= 1; 
			    tx_buff += 1;
			}	
		 }
	}
	return 0;
}

u32 snps_spi_read_data(u32 base, u8 *prdata, u32 len)
{
	u32 rx_len = len;
	u8 *ptr = prdata;
	u32 status = 0;

	while(rx_len>0){
		status =  read_ahb32(base+DW_SPI_SR);
		if((status&SR_RF_NOT_EMPT) == SR_RF_NOT_EMPT)
		{
		  *ptr = read_ahb32(base+DW_SPI_DR);
		  ptr++;
		  rx_len -- ;
		}
	}
	
	return 0;
}

void snps_spi_xfer_done(struct snps_spi_init *init)
{
	
	while((init->spi_operation_data.tx_len !=0) || (init->spi_operation_data.cmd_len !=0) || (init->spi_operation_data.rx_len !=0));
	
	while(snps_spi_IsBusy(init->base_addr));
	
	snps_spi_disable(init->base_addr);
	
	if(init->xfer_mode == XFER_IRQ_MODE)
		NVIC_DisableIRQ(init->irq_num);
}

