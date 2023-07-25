#include "sample_spi.h"

//����ͨ�� SPI2���ӻ� SPI1������

#define MASTER_BASE_T1_1  SPI2_MASTER_BASE           //SPI2_MASTER_BASE
#define SLAVE_BASE_T1_1   SPI2_SLAVE_BASE           //SPI1_SLAVE_BASE



void spi2_slave_cfg_t1_1(void)
{
	u32 reg = 0;
	u32 base = SLAVE_BASE_T1_1;
	
	//set ctrl0
	reg = read_ahb32(base + DW_SPI_CTRL0);
	reg &= ~(3<<21); //Standard SPI Frame Format
	reg &= ~(1<<10); //Slave Output Enable
	reg |= (1<<8);  //TMOD = 01 only T
	reg &= ~(1<<7);  //SCPOL = 0
	reg &= ~(1<<6);  //SCPH = 0
	reg &= ~(3<<4);  //FRF = SPI
	reg |= 7;         //Data Frame size = 8
	write_ahb32(base+ DW_SPI_CTRL0, reg);
	
	//set IMR
	reg = 0;
	write_ahb32(base+ DW_SPI_IMR, reg);  //���������ж�
	
	//set FIFO Threshold Level
	write_ahb32(base+ DW_SPI_TXFTLR, 4);
	write_ahb32(base+ DW_SPI_RXFTLR, 3);
	
}


void sample_spi_t1_1(void)
{
	u32 i = 0;
	printf("SPI2 T1_1 begin \r\n");
	printf("SLAVE_BASE_T1_1: 0x%x\r\n",SLAVE_BASE_T1_1);
	
//	printf_reg(SLAVE_BASE_T1_1);
	
	snps_spi_disable(MASTER_BASE_T1_1);
	snps_spi_disable(SLAVE_BASE_T1_1);
	
	printf("slave begin cfg!\r\n");
	spi2_slave_cfg_t1_1();
	
	snps_spi_enable(SLAVE_BASE_T1_1);
	
	printf("will write to slave\r\n");
  //��slaveд������
	write_ahb32(SLAVE_BASE_T1_1+DW_SPI_DR,0xAA);
	write_ahb32(SLAVE_BASE_T1_1+DW_SPI_DR,0xBB);
	write_ahb32(SLAVE_BASE_T1_1+DW_SPI_DR,0xCC);
	write_ahb32(SLAVE_BASE_T1_1+DW_SPI_DR,0xDD);
	write_ahb32(SLAVE_BASE_T1_1+DW_SPI_DR,0xEE);
	write_ahb32(SLAVE_BASE_T1_1+DW_SPI_DR,0xFF);
	
	printf("SPI1_SLAVE_TX_FIFO_Level = 0x%x\r\n",read_ahb32(SLAVE_BASE_T1_1 + DW_SPI_TXFLR));
	while((read_ahb32(SLAVE_BASE_T1_1 + DW_SPI_SR) & 4) == 4)
	{
		printf("writing the data to SLave TX FIFO\r\n");
	}
	printf("data has write to SLave TX FIFO!!\r\n");
	printf("SPI1_SLAVE_TX_FIFO_Level = 0x%x\r\n",read_ahb32(SLAVE_BASE_T1_1 + DW_SPI_TXFLR));
	
	printf("now wait for master transfer, if no any printf ,the case will success\r\n");
	printf("\r\n");
//	printf_reg(SLAVE_BASE_T1_1);
	
	while(1)
	{
		if(read_ahb32(SLAVE_BASE_T1_1 + DW_SPI_RXFLR) != 0 )
		{
			write_ahb32(SLAVE_BASE_T1_1+DW_SPI_DR,i+1);
			i++;
			printf("SPI2_RXFLR: 0x%x\r\n",read_ahb32(SLAVE_BASE_T1_1+ DW_SPI_RXFLR));
			printf("DW_SLA_DR: 0x%x\r\n",read_ahb32(SLAVE_BASE_T1_1 + DW_SPI_DR));

		}
	}
}






