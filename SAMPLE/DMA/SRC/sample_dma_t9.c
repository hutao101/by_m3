#include "sample_dma.h"
//m2p 32bit

int dmac_32bit_mem2iic_cfg(int channel)
{
    u32 reg_value; 
	//configure the source address 
	write_ahb32(AHB_DMAC_BASE +DMAC_SAR_L(channel),0x20004A00);
	//configure the destination address
	write_ahb32(AHB_DMAC_BASE +DMAC_DAR_L(channel), I2C1_BASE+I2C_DATA_CMD);
	//configure the CTL_L
	reg_value =   (1 << 20)	   										// memory to peripheral ,control by ahb-dmac
				 | (0 << 17)                                	 	// gather enable
				 | (0 << 18)										//scatter disable
				 | (1 << 11)| (1 << 14)		         //source burst length is 4	and dest also is 4 
				 | (2 << 7) | (0 << 9)		 						//increment the source and no change destination address
				 | (2 << 4) | (0 << 1)								//source and destination transfer width is 8bits
				 | 0;				   								//interrupt enable 
	write_ahb32(AHB_DMAC_BASE + DMAC_CTL_L(channel), reg_value);
	//configure the CTL_L	 
	reg_value =  2;	   										 
	write_ahb32(AHB_DMAC_BASE + DMAC_CTL_H(channel), reg_value);
	//configure the CFG_L
	reg_value =    (0 << 19)| (0 << 18)								//handshake polarrity, src default 0, dest base on setting	   										// memory to memory ,control by ahb-dmac
				 | (0 << 11)| (0 << 10)								//handshake select hardware
				 | (0 << 8 ) 										//DMA transfer from the source is not sspended
				 | (channel << 5);				    				//channel prority is set to the channel index,means the channel 7 is the hishgest channel
	write_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(channel), reg_value);
	//configure the CFG_H
	reg_value =    (0 << 7)| (I2C1_TX_DMA_BREQ << 11)								//USART3 TX-- 5
				 | (1 << 2) 										//prot non-cache, non-buff,prvilge data
				 | (0 << 1 ) 										//Space/data available for single transfer 
				 | 0  ;				    							//source data pre-fetching is enable
	write_ahb32(AHB_DMAC_BASE + DMAC_CFG_H(channel), reg_value);


	return 0;
}

/****************************************************************************
 * @brief   sample_dma_t9
 * Function test the SRAM to IIC dma transfer,SRAM is 32 bit,IIC is 8 bit for per ch
 * @author  he
 * @date    2022-3-31
 * @param   
		channel:DMA通道
 * @return void
******************************************************************************/

void sample_dma_t9(int channel)
{
	//注意：需要先使能IIC，再往I2C_DATA_CMD寄存器写数据
	sample_i2c_t1_toDMA_t3();
	
	write_ahb32(0x20004A00,0xAABBCCDD);
	write_ahb32(0x20004A04,0x01020304);
	dw_dmac_clearIrq(channel, IRQ_TYPE_ALL);
	dmac_32bit_mem2iic_cfg(channel);
	
	printf("DMAC_CTL_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_L(channel)));
	printf("DMAC_CTL_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_H(channel)));

	printf("DMAC_CFG_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(channel)));
	printf("DMAC_CFG_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_H(channel)));
	

	dw_dmac_enableChannel(channel);
	
	//IIC1_DMA 支持
	write_ahb32(I2C1_BASE + I2C_DMA_CR, 0x3); //0x1 rdmae  0x2 Tmdae
	//write_ahb32(I2C1_BASE+ I2C_DMA_RDLR, I2C_FIFO_DEPTH/2-1);
	//write_ahb32(I2C1_BASE + I2C_DMA_TDLR, I2C_FIFO_DEPTH/2);
	
	while(1)
	{
		delay_ms(10000);
		printf("\r\n\r\nI2C2_BASE+I2C_DATA_CMD：0x%x\r\n",read_ahb32(I2C2_BASE+I2C_DATA_CMD));
		printf("StatusBlock_L：0x%x\r\n",         read_ahb32(AHB_DMAC_BASE+DMAC_STATUS_BLOCK_L));
		printf("RwaBlock_L：0x%x\r\n",            read_ahb32(AHB_DMAC_BASE+DMAC_RAW_BLOCK_L));
		printf("StatusSrcTran_L：0x%x\r\n",       read_ahb32(AHB_DMAC_BASE+DMAC_STATUS_SRCTRAN_L));
		printf("RawSrcTran_L：0x%x\r\n",          read_ahb32(AHB_DMAC_BASE+DMAC_RAW_SRCTRAN_L));
		printf("StatusDesTran_L：0x%x\r\n",       read_ahb32(AHB_DMAC_BASE+DMAC_STATUS_DSTTRAN_L));
		printf("RawDesTran_L：0x%x\r\n",          read_ahb32(AHB_DMAC_BASE+DMAC_RAW_DSTTRAN_L));
		printf("DMAC_CFG_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(channel)));
	}
}

/****************************************************************************
 * @brief   sample_dma_t9_before_iic_enable
 * Function test the SRAM to IIC dma transfer ,to check the DMA  can handshack with IIC before IIC enable
 *	SRAM is 32 bit,IIC is 8 bit for per ch
 * @author  he
 * @date    2022-4-18
 * @param   
		channel:DMA通道
 * @return void
******************************************************************************/

void sample_dma_t9_before_iic_enable(int channel)
{
	//注意：需要先使能IIC，再往I2C_DATA_CMD寄存器写数据
//	sample_i2c_t1_toDMA_t3();
	u8 i = 0;

	
	printf("I2C1_BASE+I2C_ENABLE：0x%x\r\n",read_ahb32(I2C1_BASE+I2C_ENABLE));
	printf("I2C1_BASE+I2C_ENABLE_STATUS：0x%x\r\n",read_ahb32(I2C1_BASE+I2C_ENABLE_STATUS));
	printf("I2C1_BASE+I2C_CON：0x%x\r\n",read_ahb32(I2C1_BASE+I2C_CON));
	printf("I2C2_BASE+I2C_CON：0x%x\r\n",read_ahb32(I2C2_BASE+I2C_CON));
	
	printf("I2C2_BASE+I2C_ENABLE：0x%x\r\n",read_ahb32(I2C2_BASE+I2C_ENABLE));
	printf("I2C2_BASE+I2C_ENABLE_STATUS：0x%x\r\n",read_ahb32(I2C2_BASE+I2C_ENABLE_STATUS));
	printf("I2C1_BASE+I2C_DMA_CR：0x%x\r\n",read_ahb32(I2C1_BASE+I2C_DMA_CR));
	
	write_ahb32(0x20004A00,0xAABBCCDD);
	write_ahb32(0x20004A04,0x01020304);
	
	//close IIC 确保IIC 被关闭
	sample_i2c_t1_toDMA_t3_disable_iic();
	printf("\r\nI2C1_BASE+I2C_ENABLE_STATUS：0x%x\r\n",read_ahb32(I2C1_BASE+I2C_ENABLE_STATUS));
	//sample_i2c_t1_toDMA_t3();
	
	dw_dmac_clearIrq(channel, IRQ_TYPE_ALL);
//	dw_dmac_unmaskIrq(channel,IRQ_TYPE_ALL);
	
	
	
	printf("\r\n\r\n");printf("\r\n\r\n");
	
	printf("I2C1_BASE+I2C_CON：0x%x\r\n",read_ahb32(I2C1_BASE+I2C_CON));
	printf("I2C2_BASE+I2C_CON：0x%x\r\n",read_ahb32(I2C2_BASE+I2C_CON));
	printf("I2C1_BASE+I2C_STATUS: 0x%x\r\n",read_ahb32(I2C1_BASE+I2C_STATUS));
	printf("I2C2_BASE+I2C_STATUS: 0x%x\r\n",read_ahb32(I2C2_BASE+I2C_STATUS));
	printf("\r\n");
	write_ahb32(I2C1_BASE + I2C_DMA_CR, 0x3);//2
	
	printf("I2C1_BASE+I2C_CON：0x%x\r\n",read_ahb32(I2C1_BASE+I2C_CON));
	printf("I2C2_BASE+I2C_CON：0x%x\r\n",read_ahb32(I2C2_BASE+I2C_CON));
	printf("I2C1_BASE+I2C_STATUS: 0x%x\r\n",read_ahb32(I2C1_BASE+I2C_STATUS));
	printf("I2C2_BASE+I2C_STATUS: 0x%x\r\n",read_ahb32(I2C2_BASE+I2C_STATUS));
	printf("\r\n");
	dmac_32bit_mem2iic_cfg(channel);
	
	printf("I2C1_BASE+I2C_CON：0x%x\r\n",read_ahb32(I2C1_BASE+I2C_CON));
	printf("I2C2_BASE+I2C_CON：0x%x\r\n",read_ahb32(I2C2_BASE+I2C_CON));
	printf("I2C1_BASE+I2C_STATUS: 0x%x\r\n",read_ahb32(I2C1_BASE+I2C_STATUS));
	printf("I2C2_BASE+I2C_STATUS: 0x%x\r\n",read_ahb32(I2C2_BASE+I2C_STATUS));
	printf("\r\n");
	dw_dmac_enableChannel(channel);//1
	
	printf("I2C1_BASE+I2C_CON：0x%x\r\n",read_ahb32(I2C1_BASE+I2C_CON));
	printf("I2C2_BASE+I2C_CON：0x%x\r\n",read_ahb32(I2C2_BASE+I2C_CON));
	printf("I2C1_BASE+I2C_STATUS: 0x%x\r\n",read_ahb32(I2C1_BASE+I2C_STATUS));
	printf("I2C2_BASE+I2C_STATUS: 0x%x\r\n",read_ahb32(I2C2_BASE+I2C_STATUS));
	printf("\r\n");
	sample_i2c_t1_toDMA_t3();//3
	
	printf("I2C1_BASE+I2C_CON：0x%x\r\n",read_ahb32(I2C1_BASE+I2C_CON));
	printf("I2C2_BASE+I2C_CON：0x%x\r\n",read_ahb32(I2C2_BASE+I2C_CON));
	printf("I2C1_BASE+I2C_STATUS: 0x%x\r\n",read_ahb32(I2C1_BASE+I2C_STATUS));
	printf("I2C2_BASE+I2C_STATUS: 0x%x\r\n",read_ahb32(I2C2_BASE+I2C_STATUS));
	printf("\r\n\r\n");printf("\r\n\r\n");
	
//不出差231、132、312、321
//出错 123、213、

	printf("DMA ENABLE,IIC_DMA DISABLE\r\n");
	printf("StatusBlock_L：0x%x\r\n",         read_ahb32(AHB_DMAC_BASE+DMAC_STATUS_BLOCK_L));
	printf("RwaBlock_L：0x%x\r\n",            read_ahb32(AHB_DMAC_BASE+DMAC_RAW_BLOCK_L));
	printf("StatusSrcTran_L：0x%x\r\n",       read_ahb32(AHB_DMAC_BASE+DMAC_STATUS_SRCTRAN_L));
	printf("RawSrcTran_L：0x%x\r\n",          read_ahb32(AHB_DMAC_BASE+DMAC_RAW_SRCTRAN_L));
	printf("StatusDesTran_L：0x%x\r\n",       read_ahb32(AHB_DMAC_BASE+DMAC_STATUS_DSTTRAN_L));
	printf("RawDesTran_L：0x%x\r\n",          read_ahb32(AHB_DMAC_BASE+DMAC_RAW_DSTTRAN_L));
	printf("DMAC_CFG_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(channel)));
	printf("\r\n\r\n");
	
	//使能IIC1_DMA 
//	write_ahb32(I2C1_BASE + I2C_DMA_CR, 0x3); //0x1 rdmae  0x2 Tmdae

	delay_ms(20000);//延时是为了给DMA足够的时间更新状态，时间太短DMA块状态可能更新不了
	printf("DNA ENABLE、IIC_DMA enable、IIC disable after delay 2s\r\n");
	printf("StatusBlock_L：0x%x\r\n",         read_ahb32(AHB_DMAC_BASE+DMAC_STATUS_BLOCK_L));
	printf("RwaBlock_L：0x%x\r\n",            read_ahb32(AHB_DMAC_BASE+DMAC_RAW_BLOCK_L));
	printf("StatusSrcTran_L：0x%x\r\n",       read_ahb32(AHB_DMAC_BASE+DMAC_STATUS_SRCTRAN_L));
	printf("RawSrcTran_L：0x%x\r\n",          read_ahb32(AHB_DMAC_BASE+DMAC_RAW_SRCTRAN_L));
	printf("StatusDesTran_L：0x%x\r\n",       read_ahb32(AHB_DMAC_BASE+DMAC_STATUS_DSTTRAN_L));
	printf("RawDesTran_L：0x%x\r\n",          read_ahb32(AHB_DMAC_BASE+DMAC_RAW_DSTTRAN_L));
	printf("DMAC_CFG_L:0x%x",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(channel)));
	printf("\r\n");
	
//	sample_i2c_t1_toDMA_t3();
	
	printf("DMA使能后，IIC_DMA使能后,IIC使能后的状态\r\n");
	printf("StatusBlock_L：0x%x\r\n",         read_ahb32(AHB_DMAC_BASE+DMAC_STATUS_BLOCK_L));
	printf("RwaBlock_L：0x%x\r\n",            read_ahb32(AHB_DMAC_BASE+DMAC_RAW_BLOCK_L));
	printf("StatusSrcTran_L：0x%x\r\n",       read_ahb32(AHB_DMAC_BASE+DMAC_STATUS_SRCTRAN_L));
	printf("RawSrcTran_L：0x%x\r\n",          read_ahb32(AHB_DMAC_BASE+DMAC_RAW_SRCTRAN_L));
	printf("StatusDesTran_L：0x%x\r\n",       read_ahb32(AHB_DMAC_BASE+DMAC_STATUS_DSTTRAN_L));
	printf("RawDesTran_L：0x%x\r\n",          read_ahb32(AHB_DMAC_BASE+DMAC_RAW_DSTTRAN_L));
	printf("DMAC_CFG_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(channel)));
	
	printf("\r\n\r\n");
	for(i=0;i<8;i++)
	{
		delay_ms(100);
		printf("I2C2_BASE+I2C_DATA_CMD：0x%x\r\n",read_ahb32(I2C2_BASE+I2C_DATA_CMD));
	}
	
	delay_ms(50000);
	printf("\r\n\r\n");
	printf("StatusBlock_L：0x%x\r\n",         read_ahb32(AHB_DMAC_BASE+DMAC_STATUS_BLOCK_L));
	printf("RwaBlock_L：0x%x\r\n",            read_ahb32(AHB_DMAC_BASE+DMAC_RAW_BLOCK_L));
	printf("StatusSrcTran_L：0x%x\r\n",       read_ahb32(AHB_DMAC_BASE+DMAC_STATUS_SRCTRAN_L));
	printf("RawSrcTran_L：0x%x\r\n",          read_ahb32(AHB_DMAC_BASE+DMAC_RAW_SRCTRAN_L));
	printf("StatusDesTran_L：0x%x\r\n",       read_ahb32(AHB_DMAC_BASE+DMAC_STATUS_DSTTRAN_L));
	printf("RawDesTran_L：0x%x\r\n",          read_ahb32(AHB_DMAC_BASE+DMAC_RAW_DSTTRAN_L));
	printf("DMAC_CFG_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(channel)));
	
	printf("DMAC_CTL_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_L(channel)));
	printf("DMAC_CTL_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_H(channel)));

	printf("DMAC_CFG_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(channel)));
	printf("DMAC_CFG_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_H(channel)));
	
	printf("I2C1_BASE+I2C_ENABLE：0x%x\r\n",read_ahb32(I2C1_BASE+I2C_ENABLE));
	printf("I2C1_BASE+I2C_ENABLE_STATUS：0x%x\r\n",read_ahb32(I2C1_BASE+I2C_ENABLE_STATUS));
	printf("I2C1_BASE+I2C_CON：0x%x\r\n",read_ahb32(I2C1_BASE+I2C_CON));
	printf("I2C2_BASE+I2C_CON：0x%x\r\n",read_ahb32(I2C2_BASE+I2C_CON));
	printf("I2C1_BASE+I2C_STATUS: 0x%x\r\n",read_ahb32(I2C1_BASE+I2C_STATUS));
	printf("I2C2_BASE+I2C_STATUS: 0x%x\r\n",read_ahb32(I2C2_BASE+I2C_STATUS));
	
	printf("I2C2_BASE+I2C_ENABLE：0x%x\r\n",read_ahb32(I2C2_BASE+I2C_ENABLE));
	printf("I2C2_BASE+I2C_ENABLE_STATUS：0x%x\r\n",read_ahb32(I2C2_BASE+I2C_ENABLE_STATUS));
	printf("I2C1_BASE+I2C_DMA_CR：0x%x\r\n",read_ahb32(I2C1_BASE+I2C_DMA_CR));
	printf("\r\n\r\n\r\n");
	
	while(1)
	{
	}
}

/****************************************************************************
 * @brief   sample_dma_t3
 * Function test the SRAM to IIC dma transfer,SRAM is 32 bit,IIC is 8 bit for 8 ch
 * @author  he
 * @date    2022-3-31
 * @param  void 
 * @return void
******************************************************************************/
void sample_dma_t9_8ch(void)
{
	u32 channel = 0;
	u8 i = 0;
	//注意：需要先使能IIC，再往I2C_DATA_CMD寄存器写数据
	sample_i2c_t1_toDMA_t3();
	
	write_ahb32(0x20004A00,0xAABBCCDD);
	write_ahb32(0x20004A04,0x01020304);
	for(channel=0;channel<8;channel++)
	{
		dw_dmac_clearIrq(channel, IRQ_TYPE_ALL);
		dmac_32bit_mem2iic_cfg(channel);
		
		printf("DMAC_CTL_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_L(channel)));
		printf("DMAC_CTL_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_H(channel)));

		printf("DMAC_CFG_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(channel)));
		printf("DMAC_CFG_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_H(channel)));
		

		dw_dmac_enableChannel(channel);
		
		//IIC1_DMA 支持
		write_ahb32(I2C1_BASE + I2C_DMA_CR, 0x3); //0x1 rdmae  0x2 Tmdae
		//write_ahb32(I2C1_BASE+ I2C_DMA_RDLR, I2C_FIFO_DEPTH/2-1);
		//write_ahb32(I2C1_BASE + I2C_DMA_TDLR, I2C_FIFO_DEPTH/2);

		for(i=0;i<8;i++)
		{
			delay_ms(100);
			printf("\r\nI2C2_BASE+I2C_DATA_CMD：0x%x",read_ahb32(I2C2_BASE+I2C_DATA_CMD));
		}
		printf("\r\n\r\n");
		write_ahb32(I2C1_BASE + I2C_DMA_CR, 0x0); //0x1 rdmae  0x2 Tmdae
		dw_dmac_disableChannel(channel);
		delay_ms(500);
		
	}
}
