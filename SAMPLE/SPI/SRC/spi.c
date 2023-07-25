#include "spi.h"


u8 ready_key_flag = 0;

u8 RXF_flag = 0;
u8 TXO_flag = 0;
u8 RXO_flag = 0;

void snps_spi_enable(u32 base)
{
	write_ahb32(base+DW_SPI_SSIENR, 1);
}

void snps_spi_disable(u32 base)
{
	write_ahb32(base+DW_SPI_SSIENR, 0);
}


void printf_reg(u32 addr_base)
{
	u32 base = addr_base;
	
	printf("MASTER_CTRLR0: 0x%x\r\n",read_ahb32(base+ DW_SPI_CTRL0));
	printf("MASTER_CTRLR1: 0x%x\r\n",read_ahb32(base+ DW_SPI_CTRL1));
	printf("MASTER_SSIENR: 0x%x\r\n",read_ahb32(base+ DW_SPI_SSIENR));
	printf("MASTER_MWCR: 0x%x\r\n",read_ahb32(base+ DW_SPI_MWCR));
	printf("MASTER_SER: 0x%x\r\n",read_ahb32(base+ DW_SPI_SER));
	printf("MASTER_BAUDR: 0x%x\r\n",read_ahb32(base+ DW_SPI_BAUDR));
	printf("MASTER_TXFTLR: 0x%x\r\n",read_ahb32(base+ DW_SPI_TXFTLR));
	printf("MASTER_RXFTLR: 0x%x\r\n",read_ahb32(base+ DW_SPI_RXFTLR));
	printf("MASTER_TXFLR: 0x%x\r\n",read_ahb32(base+ DW_SPI_TXFLR));
	printf("MASTER_RXFLR: 0x%x\r\n",read_ahb32(base+ DW_SPI_RXFLR));
	
	printf("MASTER_SR: 0x%x\r\n",read_ahb32(base+ DW_SPI_SR));
	printf("MASTER_IMR: 0x%x\r\n",read_ahb32(base+ DW_SPI_IMR));
	printf("MASTER_ISR: 0x%x\r\n",read_ahb32(base+ DW_SPI_ISR));
	
	printf("MASTER_RISR: 0x%x\r\n",read_ahb32(base+ DW_SPI_RISR));
	printf("MASTER_TXOICR: 0x%x\r\n",read_ahb32(base+ DW_SPI_TXOICR));
	printf("MASTER_RXOICR: 0x%x\r\n",read_ahb32(base+ DW_SPI_RXOICR));
	printf("MASTER_RXUICR: 0x%x\r\n",read_ahb32(base+ DW_SPI_RXUICR));
	printf("MASTER_MSTICR: 0x%x\r\n",read_ahb32(base+ DW_SPI_MSTICR));


	printf("MASTER_ICR: 0x%x\r\n",read_ahb32(base+ DW_SPI_ICR));
	printf("MASTER_DMACR: 0x%x\r\n",read_ahb32(base+ DW_SPI_DMACR));
	printf("MASTER_DMATDLR: 0x%x\r\n",read_ahb32(base+ DW_SPI_DMATDLR));
	printf("MASTER_DMARDLR: 0x%x\r\n",read_ahb32(base+ DW_SPI_DMARDLR));
	printf("MASTER_IDR: 0x%x\r\n",read_ahb32(base+ DW_SPI_IDR));
	printf("MASTER_VERSION: 0x%x\r\n",read_ahb32(base+ DW_SPI_VERSION));
//	printf("MASTER_DR: 0x%x\r\n",read_ahb32(base+ DW_SPI_DR));
	printf("MASTER_RX_SAMPLE_DLY: 0x%x\r\n",read_ahb32(base+ DW_SPI_RX_SAMPLE_DLY));
	printf("MASTER_SPI_CTRLR0: 0x%x\r\n",read_ahb32(base+ DW_SPI_CTRLR0));
	printf("MASTER_TXD_DRIVE_EDGE: 0x%x\r\n",read_ahb32(base+ DW_SPI_TXD_DRIVE_EDGE));
	
	printf("MASTER_RSVD: 0x%x\r\n",read_ahb32(base+ DW_SPI_RSVD));
	
	printf("\r\n");
}



void SPI2M_IRQHandler(void)
{
	u32 status = 0;
	u32 raw_status = 0;
	u32 reg = 0;
	u32 RXFLR = 0;
	u32 RXFTLR = 0;

	
	reg = read_ahb32(SPI2_MASTER_BASE + DW_SPI_RXFLR);
	raw_status = read_ahb32(SPI2_MASTER_BASE+ DW_SPI_RISR);
	status = read_ahb32(SPI2_MASTER_BASE+ DW_SPI_ISR);
	RXFLR = read_ahb32(SPI2_MASTER_BASE + DW_SPI_RXFLR);
  RXFTLR = read_ahb32(SPI2_MASTER_BASE + DW_SPI_RXFTLR);

	
	printf("has come Interrupt!!!!!!!!!!!!!!!!!!!!!!!!\r\n");
	printf("SPI2_RXFLR = %d\r\n",reg);
	printf("SPI2M_IRQ_STATUS = 0x%x\r\n",status);
	printf("SPI2M_IRQ_raw_status = 0x%x\r\n",raw_status);
	
	if((status & 1) == 1)   //TXEIR
	{
		reg = read_ahb32(SPI2_MASTER_BASE+ DW_SPI_IMR);
		reg &= (~1);  //屏蔽TXEIR
		write_ahb32(SPI2_MASTER_BASE+ DW_SPI_IMR,reg);
		printf("\r\nTXE has occur\r\n\r\n");
	}
	
	else if((status & (1<<1)) == (1<<1) ) //TXOIR
	{
		TXO_flag = 1;
		reg = read_ahb32(SPI2_MASTER_BASE+ DW_SPI_TXOICR);  //清TXOIR
		printf("\r\nTX0 has occur\r\n\r\n");
	}
	
	else if((status & (1<<2)) == (1<<2) ) //RXUIR 
	{
		reg = read_ahb32(SPI2_MASTER_BASE+ DW_SPI_RXUICR);  //清RXUIR
		printf("\r\nRXU has occur\r\n\r\n");
	}
	
	else if((status & (1<<3)) == (1<<3) )  //RXOIR
	{
		printf("\r\nRXO has occur!\r\n\r\n");
		reg = read_ahb32(SPI2_MASTER_BASE+ DW_SPI_RXOICR);  //清RXOIR
	}
	
	else if((status & (1<<4)) == (1<<4) )  //RXFIR
	{
		printf("\r\nRXF has occur\r\n\r\n");
	
		if(RXFLR == (RXFTLR+1))
		{
			printf("RXFLR = (RXFTLR+1)\r\n\r\n");
		}
		else
		{
			printf("RXFLR != (RXFTLR+1)\r\n");
		}
		reg = read_ahb32(SPI2_MASTER_BASE+ DW_SPI_IMR);
		reg &= ~(1<<4);  //屏蔽TXEIR
		write_ahb32(SPI2_MASTER_BASE+ DW_SPI_IMR,reg);
	}
	
	else if((status & (1<<5)) == (1<<5) )   //MSTIR
	{
		reg = read_ahb32(SPI2_MASTER_BASE+ DW_SPI_MSTICR);  //清MSTIR
	}		
	printf("SPI2M_IRQ_STATUS = 0x%x\r\n",read_ahb32(SPI2_MASTER_BASE+ DW_SPI_ISR));
}



void SPI2S_IRQHandler(void)
{
	u32 status = 0;
	u32 raw_status = 0;
	u32 reg = 0;
	u8 RXFLR = 0;

	
	status = read_ahb32(SPI2_SLAVE_BASE+ DW_SPI_ISR);
	raw_status = read_ahb32(SPI2_SLAVE_BASE+ DW_SPI_RISR);
	RXFLR = read_ahb32(SPI2_SLAVE_BASE + DW_SPI_RXFLR);
	
	printf("has come Interrupt!!!!!!!!!!!!!!!!!!!!!!!!\r\n");
	printf("SPI2S_IRQ_STATUS = 0x%x\r\n",status);
	printf("SPI2S_IRQ_raw_status = 0x%x\r\n",raw_status);
	printf("RXFLR = %d",RXFLR);
	
	if((status & 1) == 1)   //TXEIR
	{
		
		reg = read_ahb32(SPI2_SLAVE_BASE+ DW_SPI_IMR);
		reg &= (~1);  //屏蔽TXEIR
		write_ahb32(SPI2_SLAVE_BASE+ DW_SPI_IMR,reg);
		printf("\r\nTXE has occur\r\n\r\n");
	}
	
	else if((status & (1<<1)) == (1<<1) ) //TXOIR
	{
		TXO_flag = 1;
		reg = read_ahb32(SPI2_SLAVE_BASE+ DW_SPI_TXOICR);  //清TXOIR
		printf("\r\nTX0 has occur\r\n\r\n");
	}
	
	else if((status & (1<<2)) == (1<<2) ) //RXUIR 
	{
		reg = read_ahb32(SPI2_SLAVE_BASE+ DW_SPI_RXUICR);  //清RXUIR
		printf("\r\nRXU has occur\r\n\r\n");
	}
	
	else if((status & (1<<3)) == (1<<3) )  //RXOIR
	{
		reg = read_ahb32(SPI2_SLAVE_BASE+ DW_SPI_RXOICR);  //清RXOIR
		RXO_flag = 1;
		printf("\r\nRX0 has occur\r\n\r\n");
		printf("SLAVE_RX_FIFO_Level = 0x%x\r\n",RXFLR);
	}
	
	else if((status & (1<<4)) == (1<<4) )  //RXFIR
	{
		reg = read_ahb32(SPI2_SLAVE_BASE+ DW_SPI_IMR);
		reg &= ~(1<<4);  //屏蔽TXEIR
		write_ahb32(SPI2_SLAVE_BASE+ DW_SPI_IMR,reg);
		printf("SLAVE_RX_FIFO_Level = 0x%x\r\n",RXFLR);
		printf("\r\nRXF has occur\r\n\r\n");
		RXF_flag = 1;
	}
	
	else if((status & (1<<5)) == (1<<5) )   //MSTIR
	{
		reg = read_ahb32(SPI2_SLAVE_BASE+ DW_SPI_MSTICR);  //清MSTIR
	}		
	printf("SPI2S_IRQ_STATUS = 0x%x\r\n",read_ahb32(SPI2_SLAVE_BASE+ DW_SPI_ISR));
}

