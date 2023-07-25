#include "sample_spi.h"

//主从通信 SPI2做从机 SPI1做主机  

/****************  t2 case 没有跑  只有壳子  需要重新写 *********************/
#define MASTER_BASE_T2  SPI2_MASTER_BASE          
#define SLAVE_BASE_T2   SPI2_SLAVE_BASE           



void spi2_master_cfg_t2(void)
{
	u32 reg = 0;
	u32 base = MASTER_BASE_T2;
	
	//set ctrl0
	reg = read_ahb32(base + DW_SPI_CTRL0);
	reg &= ~(3<<21); //Standard SPI Frame Format
	reg &= ~(3<<8);  //TMOD = 00 T&R
	reg &= ~(1<<7);  //SCPOL = 0
	reg &= ~(1<<6);  //SCPH = 0
	reg &= ~(3<<4);  //FRF = SPI
	reg |= 7;         //Data Frame size = 8
	write_ahb32(base+ DW_SPI_CTRL0, reg);
	
	//set BAUDR
	reg  = read_ahb32(base + DW_SPI_BAUDR);
	reg |= 72;  // SCLK_OUT = 36mhz/72 =500khz
	write_ahb32(base+ DW_SPI_BAUDR, reg);
	
	//set IMR
	reg = 0;
	write_ahb32(base+ DW_SPI_IMR, reg);  //屏蔽所有中断
	
	//set FIFO Threshold Level
	write_ahb32(base+ DW_SPI_TXFTLR, 4);
	write_ahb32(base+ DW_SPI_RXFTLR, 3);
	
	//set SER (slave enable reg)
	reg = 1;
	write_ahb32(base+DW_SPI_SER, reg);  //ser = 1
}

void spi2_slave_cfg_t2(void)
{
	u32 reg = 0;
	u32 base = SLAVE_BASE_T2;
	
	//set ctrl0
	reg = read_ahb32(base + DW_SPI_CTRL0);
	reg &= ~(3<<21); //Standard SPI Frame Format
	reg &= ~(1<<10); //Slave Output Enable
	reg &= ~(3<<8);  //TMOD = 00 T&R
	reg &= ~(1<<7);  //SCPOL = 0
	reg &= ~(1<<6);  //SCPH = 0
	reg &= ~(3<<4);  //FRF = SPI
	reg |= 7;         //Data Frame size = 8
	write_ahb32(base+ DW_SPI_CTRL0, reg);
	
	//set IMR
	reg = 0;
	write_ahb32(base+ DW_SPI_IMR, reg);  //屏蔽所有中断
	
	//set FIFO Threshold Level
	write_ahb32(base+ DW_SPI_TXFTLR, 4);
	write_ahb32(base+ DW_SPI_RXFTLR, 3);
	
}

void sample_spi_t2(void)
{
	printf("MST_BASE_T2: 0x%x\r\n",MASTER_BASE_T2);
	
	printf_reg(MASTER_BASE_T2);
	
	snps_spi_disable(MASTER_BASE_T2);
	snps_spi_disable(SLAVE_BASE_T2);
	
	printf("master begin cfg!\r\n");
	spi2_master_cfg_t2();
	
	
	snps_spi_enable(MASTER_BASE_T2);
	
//	printf("MASTER_SSIENR: 0x%x\r\n",read_ahb32(MASTER_BASE_T2+ DW_SPI_SSIENR));
	
	printf("now will write to master DR\r\n");
	write_ahb32(MASTER_BASE_T2+DW_SPI_DR,0x11);
	while((read_ahb32(MASTER_BASE_T2 + DW_SPI_SR) & 1) == 1);
//	printf("MASTER_TX_FIFO_Level = 0x%x\r\n",read_ahb32(MASTER_BASE_T2 + DW_SPI_TXFLR));
	
	while((read_ahb32(MASTER_BASE_T2 + DW_SPI_SR) & 1) == 1); //检测是否busy
	delay_ms(1000);
	printf("transfer over!!\r\n");
//	printf("MASTER_TX_FIFO_Level = 0x%x\r\n",read_ahb32(MASTER_BASE_T2 + DW_SPI_TXFLR));
	
//	printf("MASTER_BASE_T2 + DW_SPI_SR = 0x%x\r\n",read_ahb32(MASTER_BASE_T2 + DW_SPI_SR));
//	printf("master RX_FIFO = 0x%x\r\n",read_ahb32(MASTER_BASE_T2+DW_SPI_DR));
	
	printf("\r\n");
	printf_reg(MASTER_BASE_T2);
	
	while(1);
}




