#include "sample_spi.h"

//主从通信 SPI1做从机 SPI2做主机
//t18dma m to p

#define MASTER_BASE_T18  SPI2_MASTER_BASE           //SPI2_MASTER_BASE
#define SLAVE_BASE_T18   SPI2_SLAVE_BASE           //SPI1_SLAVE_BASE

u8 TXO_flag_t18 = 0;
u8 ch_t18 = 1; //DMA使用通道1 (0-7)
u8 num = 6;

u8 data_buffer[10] = {1,2,3,4,5,6,7,8,9,10};


int dmac_8bit_mem2spi2m_cfg_t18(int channel)
{
  u32 reg_value; 
	NVIC_InitTypeDef NVIC_InitStructure;
	
	
	NVIC_InitStructure.NVIC_IRQChannel = 1;  //DMA IRQ
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
	
	//configure the source address 
	write_ahb32(AHB_DMAC_BASE +DMAC_SAR_L(channel),(u32)data_buffer);
	//configure the destination address
	write_ahb32(AHB_DMAC_BASE +DMAC_DAR_L(channel), MASTER_BASE_T18+DW_SPI_DR);
	//configure the CTL_L
	reg_value =   (1 << 20)	   										// memory to peripheral ,control by ahb-dmac
				 | (0 << 17)                                	 	// gather enable
				 | (0 << 18)										//scatter disable
				 | (1 << 11)| (1 << 14)		         //source burst length is 4	and dest also is 4 
				 | (2 << 7) | (0 << 9)		 						//increment the source and no change destination address
				 | (0 << 4) | (0 << 1)								//source and destination transfer width is 8bits
				 | 1;				   								//interrupt enable 
	write_ahb32(AHB_DMAC_BASE + DMAC_CTL_L(channel), reg_value);
	//configure the CTL_L	 
	reg_value = num;	   										 
	write_ahb32(AHB_DMAC_BASE + DMAC_CTL_H(channel), reg_value);
	//configure the CFG_L
	reg_value =    (0 << 19)| (0 << 18)								//handshake polarrity, src default 0, dest base on setting	   										// memory to memory ,control by ahb-dmac
				 | (0 << 11)| (0 << 10)								//handshake select hardware
				 | (0 << 8 ) 										//DMA transfer from the source is not sspended
				 | (channel << 5);				    				//channel prority is set to the channel index,means the channel 7 is the hishgest channel
	write_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(channel), reg_value);
	//configure the CFG_H
	reg_value =    (0 << 7)| (SPI2_M_TX_DMA_BREQ << 11)								//SPI2 M TX-- 5
				 | (1 << 2) 										//prot non-cache, non-buff,prvilge data
				 | (0 << 1 ) 										//Space/data available for single transfer 
				 | 0  ;				    							//source data pre-fetching is enable
	write_ahb32(AHB_DMAC_BASE + DMAC_CFG_H(channel), reg_value);

	return 0;
}



void spi2_master_cfg_t18(void)
{
	u32 reg = 0;
	u32 base = MASTER_BASE_T18;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	
//	NVIC_InitStructure.NVIC_IRQChannel = 5;  //MASTER_SPI2_IRQ
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
//	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
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
//	reg = 0x11;  //不屏蔽TXE RXF
//	write_ahb32(base+ DW_SPI_IMR, reg);  //
	write_ahb32(base+ DW_SPI_IMR, 0);  //  屏蔽所有中断
	
	//set FIFO Threshold Level
	write_ahb32(base+ DW_SPI_TXFTLR, 1);
	write_ahb32(base+ DW_SPI_RXFTLR, 4);
	
  //set SER (slave enable reg)
	reg = 1;
	write_ahb32(base+DW_SPI_SER, reg);  //ser = 1
}




/***********************************
经0706测试，SPI 配置存在先后顺序问题

*************************************/
void sample_spi_t18(void)
{
	u8 i = 0;
	
	printf("MST_BASE_T18: 0x%x\r\n",MASTER_BASE_T18);
	
//	printf_reg8(MASTER_BASE_T18);
	
	snps_spi_disable(MASTER_BASE_T18);
	snps_spi_disable(SLAVE_BASE_T18);
	

	printf("MASTER_SSIENR: 0x%x\r\n",read_ahb32(MASTER_BASE_T18+ DW_SPI_SSIENR));
	
	/***********DMA CFG*************/
	printf("\r\nDMA begin cfg\r\n");
	dw_dmac_clearIrq(ch_t18, IRQ_TYPE_ALL);
	dw_dmac_unmaskIrq(ch_t18,IRQ_TYPE_ALL);
	dmac_8bit_mem2spi2m_cfg_t18(ch_t18);
	dw_dmac_enableChannel(ch_t18);
	
	/***********handshack CFG*************/
	write_ahb32(MASTER_BASE_T18 + DW_SPI_DMACR, 0x3); //0x1 rdmae  0x2 Tmdae
	
	/***********SPI CFG*************/
	printf("master begin cfg!\r\n");
	spi2_master_cfg_t18();
	
	printf("SPI WILL ENABLE\r\n");
	snps_spi_enable(MASTER_BASE_T18);
	
	printf("DMA_CFG_L = 0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(ch_t18)));
	printf("DMA_CTL_H = 0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_H(ch_t18)));
	printf("DMA_CH_EN_L = 0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CH_EN_REG_L));
	
	while( (read_ahb32(MASTER_BASE_T18 + DW_SPI_SR) & (1<<2)) != (1<<2) );  //Tx fifo is empty?
	while((read_ahb32(MASTER_BASE_T18 + DW_SPI_SR) & 1) == 1);  //is busy?
	
	delay_ms(1000);

	
	printf("transfer over!!\r\n");
  printf("MASTER_TX_FIFO_Level = 0x%x\r\n",read_ahb32(MASTER_BASE_T18 + DW_SPI_TXFLR));
	printf("MASTER_RX_FIFO_Level = 0x%x\r\n",read_ahb32(MASTER_BASE_T18 + DW_SPI_RXFLR));
	
	printf("\r\n");

	
	delay_ms(10000);
	for(i=0;i<num;i++)
	{
		delay_ms(1);
		printf("MASTER_RX_FIFO = 0x%x\r\n",read_ahb32(MASTER_BASE_T18 + DW_SPI_DR));
	}
	while(1)
	{
		
	}
}





