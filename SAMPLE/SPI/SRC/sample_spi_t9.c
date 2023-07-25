#include "sample_spi.h"

//����ͨ�� SPI1���ӻ� SPI2������

#define MASTER_BASE_T9  SPI2_MASTER_BASE           //SPI2_MASTER_BASE
#define SLAVE_BASE_T9   SPI2_SLAVE_BASE           //SPI1_SLAVE_BASE





void spi2_master_cfg_t9(void)
{
	u32 reg = 0;
	u32 base = MASTER_BASE_T9;
	
	//set ctrl0
	reg = read_ahb32(base + DW_SPI_CTRL0);
	reg &= ~(3<<21); //Standard SPI Frame Format
	reg &= ~(3<<8);  //TMOD = 00 T&R
	reg &= ~(1<<7);  //SCPOL = 0
	reg |= (1<<6);  //SCPH = 1
	reg &= ~(3<<4);  //FRF = SPI
	reg |= 7;         //Data Frame size = 8
	write_ahb32(base+ DW_SPI_CTRL0, reg);
	
	//set BAUDR
	reg  = read_ahb32(base + DW_SPI_BAUDR);
	reg |= 72;  // SCLK_OUT = 36mhz/72 =500khz
	write_ahb32(base+ DW_SPI_BAUDR, reg);
	
	//set IMR
	reg = 0;
	write_ahb32(base+ DW_SPI_IMR, reg);  //���������ж�
	
	//set FIFO Threshold Level
	write_ahb32(base+ DW_SPI_TXFTLR, 4);
	write_ahb32(base+ DW_SPI_RXFTLR, 3);
	
	//set SER (slave enable reg)
	reg = 1;
	write_ahb32(base+DW_SPI_SER, reg);  //ser = 1
}


void sample_spi_t9(void)
{
	printf("MST_BASE_T9: 0x%x\r\n",MASTER_BASE_T9);
	
//	printf_reg(MASTER_BASE_T9);
	
	snps_spi_disable(MASTER_BASE_T9);
	snps_spi_disable(SLAVE_BASE_T9);
	
	printf("master begin cfg!\r\n");
	spi2_master_cfg_t9();
	
	snps_spi_enable(MASTER_BASE_T9);
	
	
	printf("now will write to master DR\r\n");
	write_ahb32(MASTER_BASE_T9+DW_SPI_DR,0xAA);
	while( (read_ahb32(MASTER_BASE_T9 + DW_SPI_SR) & (1<<3)) != (1<<3) );
	printf("DW_MST_DR: 0x%x\r\n",read_ahb32(MASTER_BASE_T9 + DW_SPI_DR));
	write_ahb32(MASTER_BASE_T9+DW_SPI_DR,0xBB);
	while( (read_ahb32(MASTER_BASE_T9 + DW_SPI_SR) & (1<<3)) != (1<<3) );
	printf("DW_MST_DR: 0x%x\r\n",read_ahb32(MASTER_BASE_T9 + DW_SPI_DR));
	write_ahb32(MASTER_BASE_T9+DW_SPI_DR,0xCC);
	while( (read_ahb32(MASTER_BASE_T9 + DW_SPI_SR) & (1<<3)) != (1<<3) );
	printf("DW_MST_DR: 0x%x\r\n",read_ahb32(MASTER_BASE_T9 + DW_SPI_DR));
	write_ahb32(MASTER_BASE_T9+DW_SPI_DR,0xDD);
	while( (read_ahb32(MASTER_BASE_T9 + DW_SPI_SR) & (1<<3)) != (1<<3) );
	printf("DW_MST_DR: 0x%x\r\n",read_ahb32(MASTER_BASE_T9 + DW_SPI_DR));
	write_ahb32(MASTER_BASE_T9+DW_SPI_DR,0xEE);
	while( (read_ahb32(MASTER_BASE_T9 + DW_SPI_SR) & (1<<3)) != (1<<3) );
	printf("DW_MST_DR: 0x%x\r\n",read_ahb32(MASTER_BASE_T9 + DW_SPI_DR));
	write_ahb32(MASTER_BASE_T9+DW_SPI_DR,0xFF);
	while( (read_ahb32(MASTER_BASE_T9 + DW_SPI_SR) & (1<<3)) != (1<<3) );
	printf("DW_MST_DR: 0x%x\r\n",read_ahb32(MASTER_BASE_T9 + DW_SPI_DR));
	
	while( (read_ahb32(MASTER_BASE_T9 + DW_SPI_SR) & (1<<2)) != (1<<2) );  //Tx fifo is empty?
	while((read_ahb32(MASTER_BASE_T9 + DW_SPI_SR) & 1) == 1);  //is busy?
	delay_ms(1000);
	
	
	printf("transfer over!!\r\n");
  printf("MASTER_TX_FIFO_Level = 0x%x\r\n",read_ahb32(MASTER_BASE_T9 + DW_SPI_TXFLR));
	printf("MASTER_RX_FIFO_Level = 0x%x\r\n",read_ahb32(MASTER_BASE_T9 + DW_SPI_RXFLR));
	
	printf("\r\n");
//	printf_reg(MASTER_BASE_T9);

	while(1)
	{

	}
}






