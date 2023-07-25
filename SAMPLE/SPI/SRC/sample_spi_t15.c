#include "sample_spi.h"

//主从通信 SPI1(stm32) 做主机 SPI2做从机

#define MASTER_BASE_T15  SPI2_MASTER_BASE           //SPI2_MASTER_BASE
#define SLAVE_BASE_T15   SPI2_SLAVE_BASE           //SPI2_SLAVE_BASE



void spi2_slave_cfg_t15(void)
{
	u32 reg = 0;
	u32 base = SLAVE_BASE_T15;
	
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
	reg = 0x1C;  //不屏蔽RXF RXO RXU
	write_ahb32(base+ DW_SPI_IMR, reg);  
	
	//set FIFO Threshold Level
	write_ahb32(base+ DW_SPI_TXFTLR, 4);
	write_ahb32(base+ DW_SPI_RXFTLR, 31-1);
	
}


void sample_spi_t15(void)
{
	u8 i = 0;
	
	printf("SLAVE_BASE_T15: 0x%x\r\n",SLAVE_BASE_T15);
	
	snps_spi_disable(MASTER_BASE_T15);
	snps_spi_disable(SLAVE_BASE_T15);
	
	printf("SLAVE begin cfg!\r\n");
	spi2_slave_cfg_t15();
	
//	printf_reg(SLAVE_BASE_T15);
	
	snps_spi_enable(SLAVE_BASE_T15);
	
	printf("now will write to slave DR\r\n");

	for(i=0;i<32;i++)
	{
		write_ahb32(SLAVE_BASE_T15+DW_SPI_DR,i+1);
		delay_ms(1);
//		printf("SLAVE_TX_FIFO_Level = 0x%x\r\n",read_ahb32(SLAVE_BASE_T15 + DW_SPI_TXFLR));
	}
	
	printf("slave TX data has write 32 data!!\r\n\r\n");

	
	printf("wait for RXF\r\n");
	while(!RXF_flag);
	printf("SLAVE_RX_FIFO_Level = 0x%x\r\n",read_ahb32(SLAVE_BASE_T15 + DW_SPI_RXFLR));
	printf("RXF interrupt has occur\r\n\r\n");
	
	printf("wait for RXO\r\n");
	printf("SLAVE_RX_FIFO_Level = 0x%x\r\n",read_ahb32(SLAVE_BASE_T15 + DW_SPI_RXFLR));
	i = read_ahb32(SLAVE_BASE_T15 + DW_SPI_RXFLR);
	while(!RXO_flag)
	{
		if(read_ahb32(SLAVE_BASE_T15 + DW_SPI_RXFLR) != i)
		{
			printf("SLAVE_RX_FIFO_Level = %d\r\n",read_ahb32(SLAVE_BASE_T15 + DW_SPI_RXFLR));
			i = read_ahb32(SLAVE_BASE_T15 + DW_SPI_RXFLR);
		}
	}
	printf("RXO interrupt has occur\r\n\r\n");
	
	printf("now read RXFIFO to product RXU!\r\n");

	i = read_ahb32(SLAVE_BASE_T15 + DW_SPI_RXFLR);
	while(i--)
	{
		read_ahb32(SLAVE_BASE_T15 + DW_SPI_DR);
//		printf("SLAVE_RX_FIFO = 0x%x\r\n",read_ahb32(SLAVE_BASE_T15 + DW_SPI_DR));
	}

	printf("now RXFIFO has read over!\r\n");
	printf("we will raed again to product RXU!\r\n");
	
	printf("SLAVE_RX_FIFO = 0x%x\r\n",read_ahb32(SLAVE_BASE_T15 + DW_SPI_DR));
	
	printf("test over\r\n");
	
	while(1)
	{
		
	}
}




