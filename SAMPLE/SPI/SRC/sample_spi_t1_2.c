#include "sample_spi.h"

//����ͨ�� SPI2���ӻ� SPI1������

#define MASTER_BASE_T1_2  SPI2_MASTER_BASE           //SPI2_MASTER_BASE
#define SLAVE_BASE_T1_2   SPI2_SLAVE_BASE           //SPI1_SLAVE_BASE


void spi2_slave_cfg_t1_2(void)
{
	u32 reg = 0;
	u32 base = SLAVE_BASE_T1_2;
	
	//set ctrl0
	reg = read_ahb32(base + DW_SPI_CTRL0);
	reg &= ~(3<<21); //Standard SPI Frame Format
	reg &= ~(1<<10); //Slave Output Enable
	reg |= (2<<8);  //TMOD = 02 only R
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


void sample_spi_t1_2(void)
{
	printf("SLAVE_BASE_T1_2: 0x%x\r\n",SLAVE_BASE_T1_2);
	
//	printf_reg(SLAVE_BASE_T1_2);
	
	snps_spi_disable(MASTER_BASE_T1_2);
	snps_spi_disable(SLAVE_BASE_T1_2);
	
	printf("slave begin cfg!\r\n");
	spi2_slave_cfg_t1_2();
	
	snps_spi_enable(SLAVE_BASE_T1_2);
	
	printf("will write to slave\r\n");
  //��slaveд������
	write_ahb32(SLAVE_BASE_T1_2+DW_SPI_DR,0xbb);
	printf("SPI1_SLAVE_TX_FIFO_Level = 0x%x\r\n",read_ahb32(SLAVE_BASE_T1_2 + DW_SPI_TXFLR));
	while((read_ahb32(SLAVE_BASE_T1_2 + DW_SPI_SR) & 4) == 4)
	{
		printf("writing the data to SLave TX FIFO\r\n");
	}
	printf("data has write to SLave TX FIFO!!\r\n");
	printf("SPI1_SLAVE_TX_FIFO_Level = 0x%x\r\n",read_ahb32(SLAVE_BASE_T1_2 + DW_SPI_TXFLR));
	

	printf("\r\n");
//	printf_reg(SLAVE_BASE_T1_2);
	
	while(1)
	{
//		delay_ms(2000);
		if(read_ahb32(SLAVE_BASE_T1_2 + DW_SPI_RXFLR) != 0 )
		{
			printf("DW_SLA_DR: 0x%x\r\n",read_ahb32(SLAVE_BASE_T1_2 + DW_SPI_DR));
		}
	}
}






