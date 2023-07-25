#include "sample_spi.h"

//主从通信 SPI2做从机 SPI1(stm)做主机

#define MASTER_BASE_T7  SPI2_MASTER_BASE          
#define SLAVE_BASE_T7   SPI2_SLAVE_BASE           


void spi2_slave_cfg_t7(void)
{
	u32 reg = 0;
	u32 base = SLAVE_BASE_T7;
	
	//set ctrl0
	reg = read_ahb32(base + DW_SPI_CTRL0);
	reg &= ~(3<<21); //Standard SPI Frame Format
	reg &= ~(1<<10); //Slave Output Enable
	reg &= ~(3<<8);  //TMOD = 00 T&R ,01 T
	reg &= ~(1<<7);  //SCPOL = 0
	reg &= ~(1<<6);  //SCPH = 0
	reg &= ~(3<<4);  //FRF = SPI
	#if 0
	reg |= 0x7;         //Data Frame size = 8
	#else 
	reg |= 0xf;         //Data Frame size = 16
	#endif
	
	write_ahb32(base+ DW_SPI_CTRL0, reg);
	
	//set IMR
	reg = 0;
	write_ahb32(base+ DW_SPI_IMR, reg);  //屏蔽所有中断
	
	//set FIFO Threshold Level
	write_ahb32(base+ DW_SPI_TXFTLR, 4);
	write_ahb32(base+ DW_SPI_RXFTLR, 3);
	
}


void sample_spi_t7(void)
{
	u32 i = 0;
	
	printf("SPI2 T7 begin\r\n");
	printf("SLAVE_BASE_T7: 0x%x\r\n",SLAVE_BASE_T7);
	
//	printf_reg(SLAVE_BASE_T7);
	
	snps_spi_disable(MASTER_BASE_T7);
	snps_spi_disable(SLAVE_BASE_T7);
	
	printf("slave begin cfg!\r\n");
	spi2_slave_cfg_t7();
	
	snps_spi_enable(SLAVE_BASE_T7);
	
	printf("will write to slave\r\n");
  //给slave写入数据
	
	write_ahb32(SLAVE_BASE_T7+DW_SPI_DR,0xaaaa);
	write_ahb32(SLAVE_BASE_T7+DW_SPI_DR,0xbbbb);
	write_ahb32(SLAVE_BASE_T7+DW_SPI_DR,0xcccc);
	write_ahb32(SLAVE_BASE_T7+DW_SPI_DR,0xdddd);
	write_ahb32(SLAVE_BASE_T7+DW_SPI_DR,0xeeee);
	write_ahb32(SLAVE_BASE_T7+DW_SPI_DR,0xffff);

	printf("SPI1_SLAVE_TX_FIFO_Level = 0x%x\r\n",read_ahb32(SLAVE_BASE_T7 + DW_SPI_TXFLR));
	while((read_ahb32(SLAVE_BASE_T7 + DW_SPI_SR) & 4) == 4)
	{
		printf("writing the data to SLave TX FIFO\r\n");
	}
	printf("data has write to SLave TX FIFO!!\r\n");
	printf("SPI1_SLAVE_TX_FIFO_Level = 0x%x\r\n",read_ahb32(SLAVE_BASE_T7 + DW_SPI_TXFLR));
	
	printf("\r\n");
//	printf_reg(SLAVE_BASE_T7);
	printf("SLAVE_TXFLR: 0x%x\r\n",read_ahb32(SLAVE_BASE_T7+ DW_SPI_TXFLR));
	while(1)
	{
		if(read_ahb32(SLAVE_BASE_T7 + DW_SPI_RXFLR) != 0 )
		{
//			printf("DW_SLA_DR: 0x%x\r\n",read_ahb32(SLAVE_BASE_T7 + DW_SPI_DR));
//			printf("SLAVE_TXFLR: 0x%x\r\n",read_ahb32(SLAVE_BASE_T7+ DW_SPI_TXFLR));
			write_ahb32(SLAVE_BASE_T7+DW_SPI_DR,i+1);
			i++;
			if(i%3 == 0)
			{
				printf("DW_SLA_DR: 0x%x\r\n",read_ahb32(SLAVE_BASE_T7 + DW_SPI_DR));
			}
		}
	}
}






