#include "sample_spi.h"

//主从通信 SPI2做从机 SPI1做主机 测试从机DMA

#define MASTER_BASE_T19_1  SPI2_MASTER_BASE           //SPI2_MASTER_BASE
#define SLAVE_BASE_T19_1   SPI2_SLAVE_BASE           //SPI1_SLAVE_BASE

u8 ch_t19_1 = 1; //DMA使用通道1 (0-7)
u8 num_t19_1 = 6;

u8 data_buffer_t19_1[10] = {1,2,3,4,5,6,7,8,9,10};



int dmac_8bit_mem2spi2m_cfg_t19_1(int channel)
{
  u32 reg_value; 
	u8 i = 0;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	
	NVIC_InitStructure.NVIC_IRQChannel = 1;  //DMA IRQ
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
	for(i=0;i<num_t19_1;i++)
	{	
		write_ahb32(0x20004A00+i,i+1);
		printf("0x20004A00+ 0x%x = 0x%x\r\n",i,read_ahb32(0x20004A00 + i));
	}
	
	//configure the source address 
	write_ahb32(AHB_DMAC_BASE +DMAC_SAR_L(channel),(u32)data_buffer_t19_1);
	//configure the destination address
	write_ahb32(AHB_DMAC_BASE +DMAC_DAR_L(channel), SLAVE_BASE_T19_1+DW_SPI_DR);
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
	reg_value = num_t19_1;	   										 
	write_ahb32(AHB_DMAC_BASE + DMAC_CTL_H(channel), reg_value);
	//configure the CFG_L
	reg_value =    (0 << 19)| (0 << 18)								//handshake polarrity, src default 0, dest base on setting	   										// memory to memory ,control by ahb-dmac
				 | (0 << 11)| (0 << 10)								//handshake select hardware
				 | (0 << 8 ) 										//DMA transfer from the source is not sspended
				 | (channel << 5);				    				//channel prority is set to the channel index,means the channel 7 is the hishgest channel
	write_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(channel), reg_value);
	//configure the CFG_H
	
	//MCU_CFG[9]=1
	reg_value = read_ahb32(MCU_CFG);
	reg_value |= 1<<9;
	write_ahb32(MCU_CFG, reg_value);  
	
	reg_value =    (0 << 7)| (SPI2_M_TX_DMA_BREQ << 11)								//SPI2 M/S TX--9
				 | (1 << 2) 										//prot non-cache, non-buff,prvilge data
				 | (0 << 1 ) 										//Space/data available for single transfer 
				 | 0  ;				    							//source data pre-fetching is enable
	write_ahb32(AHB_DMAC_BASE + DMAC_CFG_H(channel), reg_value);

	return 0;
}


void spi2_slave_cfg_t19_1(void)
{
	u32 reg = 0;
	u32 base = SLAVE_BASE_T19_1;
	
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


void sample_spi_t19_1(void)
{
	printf("SLAVE_BASE_T19_1: 0x%x\r\n",SLAVE_BASE_T19_1);
		
	snps_spi_disable(MASTER_BASE_T19_1);
	snps_spi_disable(SLAVE_BASE_T19_1);
	
	/**************SPI2_S CFG*************/
	printf("slave begin cfg!\r\n");
	spi2_slave_cfg_t19_1();
	snps_spi_enable(SLAVE_BASE_T19_1);
	
	/**************DMA CFG********************/
	dw_dmac_clearIrq(ch_t19_1, IRQ_TYPE_ALL);
	dw_dmac_unmaskIrq(ch_t19_1,IRQ_TYPE_ALL);
	dmac_8bit_mem2spi2m_cfg_t19_1(ch_t19_1);
	dw_dmac_enableChannel(ch_t19_1);
	
	/***********handshack CFG*************/
	write_ahb32(SLAVE_BASE_T19_1 + DW_SPI_DMACR, 0x3); //0x1 rdmae  0x2 Tmdae	
	
	printf("DMA_CFG_L = 0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(ch_t19_1)));
	printf("DMA_CTL_H = 0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_H(ch_t19_1)));
	printf("DMA_CH_EN_L = 0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CH_EN_REG_L));
	printf("SPI2_SLAVE_TX_FIFO_Level = 0x%x\r\n",read_ahb32(SLAVE_BASE_T19_1 + DW_SPI_TXFLR));
	printf("SPI2_SLAVE_RX_FIFO_Level = 0x%x\r\n",read_ahb32(SLAVE_BASE_T19_1 + DW_SPI_RXFLR));

	printf("begin trans!\r\n\r\n");
//	printf_reg9_1(SLAVE_BASE_T19_1);
	
	delay_ms(50000);
	
	while(1)
	{
		if(read_ahb32(SLAVE_BASE_T19_1 + DW_SPI_RXFLR) != 0 )
		{
			printf("DW_SLA_DR: 0x%x\r\n",read_ahb32(SLAVE_BASE_T19_1 + DW_SPI_DR));
			printf("SPI2_SLAVE_RX_FIFO_Level = 0x%x\r\n",read_ahb32(SLAVE_BASE_T19_1 + DW_SPI_RXFLR));
		}
	}
}








