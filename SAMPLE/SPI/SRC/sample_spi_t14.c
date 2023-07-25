#include "sample_spi.h"

//主从通信 SPI1(stm32) 做主机 SPI2做从机

#define MASTER_BASE_T14  SPI2_MASTER_BASE           //SPI2_MASTER_BASE
#define SLAVE_BASE_T14   SPI2_SLAVE_BASE           //SPI2_SLAVE_BASE


void spi2_slave_cfg_t14(void)
{
	u32 reg = 0;
	u32 base = SLAVE_BASE_T14;
	
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = 26;  //SLAVE_SPI2_IRQ
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
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
	reg = 0x3;  //不屏蔽TXO  TXE
	write_ahb32(base+ DW_SPI_IMR, reg);  
	
	//set FIFO Threshold Level
	write_ahb32(base+ DW_SPI_TXFTLR, 4);
	write_ahb32(base+ DW_SPI_RXFTLR, 3);
	
}


void sample_spi_t14(void)
{
	u8 i = 0;
	
	printf("SLAVE_BASE_T14: 0x%x\r\n",SLAVE_BASE_T14);
	
//	printf_reg4(SLAVE_BASE_T14);
	
	snps_spi_disable(MASTER_BASE_T14);
	snps_spi_disable(SLAVE_BASE_T14);
	
	printf("SLAVE begin cfg!\r\n");
	spi2_slave_cfg_t14();
	
	printf("SPI2 Slave cfg over\r\nnow will enable slave\r\n\r\nTXE interrupt should come!!\r\n");
	snps_spi_enable(SLAVE_BASE_T14);
	
	printf("now will write to slave DR\r\n");

	for(i=0;i<32;i++)
	{
		write_ahb32(SLAVE_BASE_T14+DW_SPI_DR,i+1);
		delay_ms(1);
		printf("SLAVE_TX_FIFO_Level = 0x%x\r\n",read_ahb32(SLAVE_BASE_T14 + DW_SPI_TXFLR));
	}
	
	printf("slave TX data has write 32 data!!\r\n\r\n");
	delay_ms(1000);
	
	printf("now will product TXO interrupt,we will write 33data to slave\r\n\r\n");
	write_ahb32(SLAVE_BASE_T14+DW_SPI_DR,i+1);
	
	delay_ms(10000);
	printf("TXO has occur,TXE、TXO test over\r\n");
	
	printf("\r\n");
	while(1)
	{
		
	}
}





