#include "sample_spi.h"

//����ͨ�� SPI1���ӻ� SPI2������
//t18dma m to p

#define MASTER_BASE_T19  SPI2_MASTER_BASE           //SPI2_MASTER_BASE
#define SLAVE_BASE_T19   SPI2_SLAVE_BASE           //SPI1_SLAVE_BASE

u8 TXO_flag_t19 = 0;
u8 ch_t19 = 1; //DMAʹ��ͨ��1 (0-7)
u8 num_t19 = 6;

int dmac_8bit_spi2m2mem_cfg_t19(int channel)
{
  u32 reg_value; 
	NVIC_InitTypeDef NVIC_InitStructure;
	
	
	NVIC_InitStructure.NVIC_IRQChannel = 1;  //DMA IRQ
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	//configure the source address 
	write_ahb32(AHB_DMAC_BASE +DMAC_SAR_L(channel),MASTER_BASE_T19+DW_SPI_DR);
	//configure the destination address
	write_ahb32(AHB_DMAC_BASE +DMAC_DAR_L(channel),0x20004A00);
	//configure the CTL_L
	reg_value =   (2 << 20)	   										// peripheral to memory,control by ahb-dmac
				 | (0 << 17)                                	 	// gather enable
				 | (0 << 18)										//scatter disable
				 | (2 << 11)| (0 << 14)		       //des 8  src 1    //source burst length is 16	and dest burst length is half of the peripheral fifo depth
				 | (0 << 7) | (2 << 9)		 						//increment the source and no change destination address
				 | (0 << 4) | (0 << 1)								//src 8bit des 16bit
				 | 1;				   								//interrupt enable 
	write_ahb32(AHB_DMAC_BASE + DMAC_CTL_L(channel), reg_value);
	//configure the CTL_H	 
	reg_value =  num_t19;	   										 
	write_ahb32(AHB_DMAC_BASE + DMAC_CTL_H(channel), reg_value);
	//configure the CFG_L
	reg_value =    (0 << 19)| (0 << 18)								//handshake polarrity, src default 0, dest base on setting	   										// memory to memory ,control by ahb-dmac
				 | (0 << 11)| (0 << 10)								//handshake select hardware
				 | (0 << 8 ) 										//DMA transfer from the source is not sspended
				 | (channel << 5);				    				//channel prority is set to the channel index,means the channel 7 is the hishgest channel
	write_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(channel), reg_value);
	//configure the CFG_H
	reg_value =    (SPI2_M_RX_DMA_BREQ << 7)| (0 << 11)								//
				 | (1 << 2) 										//prot non-cache, non-buff,prvilge data
				 | (0 << 1 ) 										//Space/data available for single transfer 
				 | 0  ;				    							//source data pre-fetching is enable
	write_ahb32(AHB_DMAC_BASE + DMAC_CFG_H(channel), reg_value);


	return 0;
}



void spi2_master_cfg_t19(void)
{
	u32 reg = 0;
	u32 base = MASTER_BASE_T19;

	
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
//	reg = 0x11;  //������TXE RXF
//	write_ahb32(base+ DW_SPI_IMR, reg);  //
	write_ahb32(base+ DW_SPI_IMR, 0);  //  ���������ж�
	
	//set FIFO Threshold Level
	write_ahb32(base+ DW_SPI_TXFTLR, 1);
	write_ahb32(base+ DW_SPI_RXFTLR, 4);
	
  //set SER (slave enable reg)
	reg = 1;
	write_ahb32(base+DW_SPI_SER, reg);  //ser = 1
}



void sample_spi_t19(void)
{
	u8 i = 0;
	
	printf("MST_BASE_T19: 0x%x\r\n",MASTER_BASE_T19);
	
//	printf_reg9(MASTER_BASE_T19);
	
	snps_spi_disable(MASTER_BASE_T19);
	snps_spi_disable(SLAVE_BASE_T19);
	
	/***********SPI CFG*************/
	printf("master begin cfg!\r\n");
	spi2_master_cfg_t19();
	
	printf("SPI WILL ENABLE\r\n");
	snps_spi_enable(MASTER_BASE_T19);
	
	/***********DMA CFG*************/
	dw_dmac_clearIrq(ch_t19, IRQ_TYPE_ALL);
	dw_dmac_unmaskIrq(ch_t19,IRQ_TYPE_ALL);
	dmac_8bit_spi2m2mem_cfg_t19(ch_t19);
	dw_dmac_enableChannel(ch_t19);
	
	/***********handshack CFG*************/
	write_ahb32(MASTER_BASE_T19 + DW_SPI_DMACR, 0x3); //0x1 rdmae  0x2 Tmdae
	
	//write data
	for(i=0;i<6;i++)
	{
		write_ahb32(MASTER_BASE_T19+DW_SPI_DR,i+1);
	}
	
	printf("DMA_CFG_L = 0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(ch_t19)));
	printf("DMA_CTL_H = 0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_H(ch_t19)));
	printf("DMA_CH_EN_L = 0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CH_EN_REG_L));
	
	while( (read_ahb32(MASTER_BASE_T19 + DW_SPI_SR) & (1<<2)) != (1<<2) );  //Tx fifo is empty?
	while((read_ahb32(MASTER_BASE_T19 + DW_SPI_SR) & 1) == 1);  //is busy?
	
	delay_ms(1000);

	
	printf("transfer over!!\r\n");
  printf("MASTER_TX_FIFO_Level = 0x%x\r\n",read_ahb32(MASTER_BASE_T19 + DW_SPI_TXFLR));
	printf("MASTER_RX_FIFO_Level = 0x%x\r\n",read_ahb32(MASTER_BASE_T19 + DW_SPI_RXFLR));
	
	printf("\r\n");
//	printf_reg9(MASTER_BASE_T19);
	
	delay_ms(10000);
	for(i=0;i<num_t19;i++)
	{
		delay_ms(1);
		printf("0x20004A00+%d = 0x%x\r\n",i,read_ahb08(0x20004A00 + i));
	}
	while(1)
	{
		
	}
}





