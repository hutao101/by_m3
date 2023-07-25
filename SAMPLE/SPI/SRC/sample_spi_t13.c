#include "sample_spi.h"

//主从通信 SPI1做从机 SPI2做主机

#define MASTER_BASE_T13  SPI2_MASTER_BASE           //SPI2_MASTER_BASE
#define SLAVE_BASE_T13   SPI2_SLAVE_BASE           //SPI1_SLAVE_BASE

u8 TXO_flag_t13 = 0;


void spi2_master_cfg_t13(void)
{
	u32 reg = 0;
	u32 base = MASTER_BASE_T13;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = 5;  //MASTER_SPI2_IRQ
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
	//set ctrl0
	reg = read_ahb32(base + DW_SPI_CTRL0);
	reg &= ~(3<<21); //Standard SPI Frame Format
	reg &= ~(3<<8);  //TMOD = 00 T&R
	reg |= (1<<7);  //SCPOL = 1
	reg |= (1<<6);  //SCPH = 1
	reg &= ~(3<<4);  //FRF = SPI
	reg |= 7;         //Data Frame size = 8
	write_ahb32(base+ DW_SPI_CTRL0, reg);
	
	//set BAUDR
	reg  = read_ahb32(base + DW_SPI_BAUDR);
	reg |= 72;  // SCLK_OUT = 36mhz/72 =500khz
	write_ahb32(base+ DW_SPI_BAUDR, reg);
	
	//set IMR
	reg = 0x1c;  //屏蔽TXE TXO
	write_ahb32(base+ DW_SPI_IMR, reg);  //
	
	//set FIFO Threshold Level
	write_ahb32(base+ DW_SPI_TXFTLR, 4);
	write_ahb32(base+ DW_SPI_RXFTLR, 3);
	
	//set SER (slave enable reg)
	reg = 1;
	write_ahb32(base+DW_SPI_SER, reg);  //ser = 1
}




void sample_spi_t13(void)
{
	u8 i = 0;
	
	printf("MST_BASE_T13: 0x%x\r\n",MASTER_BASE_T13);
		
	snps_spi_disable(MASTER_BASE_T13);
	snps_spi_disable(SLAVE_BASE_T13);
	
	printf("master begin cfg!\r\n");
	spi2_master_cfg_t13();
	
	snps_spi_enable(MASTER_BASE_T13);
	
	printf("now will write to master DR\r\n");
	
	for(i=0;i<32;i++)
	{
		if(TXO_flag_t13 == 0)
		{
			write_ahb32(MASTER_BASE_T13+DW_SPI_DR,i+1);
			delay_ms(20);
		}
	}

	printf("MASTER_RX_FIFO_Level = 0x%x\r\n",read_ahb32(MASTER_BASE_T13 + DW_SPI_RXFLR));
	
	while( (read_ahb32(MASTER_BASE_T13 + DW_SPI_SR) & (1<<2)) != (1<<2) );  //Tx fifo is empty?
	while((read_ahb32(MASTER_BASE_T13 + DW_SPI_SR) & 1) == 1);  //is busy?
	delay_ms(1000);
	
	printf("\r\n");
	printf("now 32 data has writen and will write 33!!!\r\n");
	
	
	write_ahb32(MASTER_BASE_T13+DW_SPI_DR,i+1);  //写第33个数据 以此来得到第33个接收的数据
	
	delay_ms(1);
	
	printf("\r\nnow read to product RXU\r\n");
	
	printf("\r\n");
	
	delay_ms(10000);
	for(i=0;i<32;i++)
	{
		delay_ms(1);
		printf("MASTER_RX_FIFO = 0x%x\r\n",read_ahb32(MASTER_BASE_T13 + DW_SPI_DR));
	}
	
	printf("32 data has read!!\r\n\r\n");
	printf("MASTER_RX_FIFO_Level = 0x%x\r\n",read_ahb32(MASTER_BASE_T13 + DW_SPI_RXFLR));
	printf("now will read 33!!\r\n");
	printf("MASTER_RX_FIFO = 0x%x\r\n",read_ahb32(MASTER_BASE_T13 + DW_SPI_DR));

	while(1)
	{
		
	}
}





