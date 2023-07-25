#include "sample_dma.h"


//m2p 8bit  IIC2->IIC1


int dmac_8bit_mem2iic_cfg_t21(int channel)
{
    u32 reg_value; 
	//configure the source address 
	write_ahb32(AHB_DMAC_BASE +DMAC_SAR_L(channel),0x20004A00);
	//configure the destination address
	write_ahb32(AHB_DMAC_BASE +DMAC_DAR_L(channel), I2C2_BASE+I2C_DATA_CMD);
	//configure the CTL_L
	reg_value =   (1 << 20)	   										// memory to peripheral ,control by ahb-dmac
				 | (0 << 17)                                	 	// gather enable
				 | (0 << 18)										//scatter disable
				 | (1 << 11)| (1 << 14)		         //source burst length is 4	and dest also is 4 
				 | (2 << 7) | (0 << 9)		 						//increment the source and no change destination address
				 | (0 << 4) | (0 << 1)								//source and destination transfer width is 8bits
				 | 0;				   								//interrupt enable 
	write_ahb32(AHB_DMAC_BASE + DMAC_CTL_L(channel), reg_value);
	//configure the CTL_L	 
	reg_value =  7;	   										 
	write_ahb32(AHB_DMAC_BASE + DMAC_CTL_H(channel), reg_value);
	//configure the CFG_L
	reg_value =    (0 << 19)| (0 << 18)								//handshake polarrity, src default 0, dest base on setting	   										// memory to memory ,control by ahb-dmac
				 | (0 << 11)| (0 << 10)								//handshake select hardware
				 | (0 << 8 ) 										//DMA transfer from the source is not sspended
				 | (channel << 5);				    				//channel prority is set to the channel index,means the channel 7 is the hishgest channel
	write_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(channel), reg_value);
	//configure the CFG_H
	reg_value =    (0 << 7)| (I2C2_TX_DMA_BREQ << 11)								//USART3 TX-- 5
				 | (1 << 2) 										//prot non-cache, non-buff,prvilge data
				 | (0 << 1 ) 										//Space/data available for single transfer 
				 | 0  ;				    							//source data pre-fetching is enable
	write_ahb32(AHB_DMAC_BASE + DMAC_CFG_H(channel), reg_value);


	return 0;
}

/*******************************************************************************
 * @brief   sample_i2c_t1
* Function  Master Mode-i2c1 as a transmitter(addr:7 bit)
 * @author  licuihaung
 * @date    2022-3
 * @param   void
 * @return  void
*******************************************************************************/ 
void sample_i2c_t1_dmat21(void)
{
		
    u32 ret,i;
		u32 byte_receive_data ;
		u32 byte_write_data = 0x12;
		u32 status1,status2;
	
		struct snps_i2c_info i2c1_info ;
		struct snps_i2c_info i2c2_info ;
		struct i2c_cfg_param icp; 
	   
    printf("0x%x 0x%x\r\n",read_ahb32(I2C1_BASE+I2C_ENABLE),read_ahb32(I2C2_BASE+I2C_ENABLE));
	
		i2c1_info.base_address = I2C1_BASE;  //all
		i2c1_info.clk_freq = I2C_PCLK;  //set_speed
		i2c1_info.xfer_param.target_dev_addr = SLAVE_ADDRESS;  //set_addr
		i2c1_info.xfer_param.len = 1;  //控制read write个数
		i2c1_info.xfer_param.buffer = &byte_write_data;  //&byte_write_data; 

		i2c2_info.base_address = I2C2_BASE;
		i2c2_info.clk_freq = I2C_PCLK;
		i2c2_info.xfer_param.target_dev_addr = SLAVE_ADDRESS;
		i2c2_info.xfer_param.len = 1;
		i2c2_info.xfer_param.buffer = &byte_receive_data; //&byte_receive_data;
		
		icp.xfer_mode = I2C_POLLING_MODE;
		icp.dev_addr_as_slave = SLAVE_ADDRESS;
		icp.speed = I2C_SPEED_MODE_STANDARD;  
		printf("0x%x 0x%x\r\n",read_ahb32(I2C1_BASE+I2C_ENABLE),read_ahb32(I2C2_BASE+I2C_ENABLE));
		
		snps_i2c_close(&i2c1_info);
    snps_i2c_close(&i2c2_info);
	  write_ahb32(I2C1_BASE+I2C_ENABLE, 0x0);
    write_ahb32(I2C2_BASE+I2C_ENABLE, 0x0);

/*********i2c1 to i2c2********/ 
//		printf("0x%x 0x%x\r\n",read_ahb32(I2C1_BASE+I2C_ENABLE),read_ahb32(I2C2_BASE+I2C_ENABLE));
//	  snps_i2c_config(&i2c1_info, &icp);    //start i2c  传输模式/启主禁从/速度/TAR
//		printf("0x%x 0x%x\r\n",read_ahb32(I2C1_BASE+I2C_ENABLE),read_ahb32(I2C2_BASE+I2C_ENABLE));
//		snps_i2c_slave_config(&i2c2_info, &icp);  //start i2c
//		printf("0x%x 0x%x\r\n",read_ahb32(I2C1_BASE+I2C_ENABLE),read_ahb32(I2C2_BASE+I2C_ENABLE));
//		printf("TAR: 0x%x ,SAR : 0x%x \r\n",read_ahb32(i2c1_info.base_address +  I2C_TAR),read_ahb32(i2c2_info.base_address +  I2C_SAR));

////    write_ahb32(I2C1_BASE+I2C_DATA_CMD, 0x77);
//    printf("transter finish\r\n");
//		delay_ms(2);
//		printf("0x%x  \r\n\r\n",read_ahb32(i2c2_info.base_address +  I2C_DATA_CMD));
/*********i2c1 to i2c2********/ 

/*********i2c2 to i2c1********/   
		printf("0x%x 0x%x\r\n",read_ahb32(I2C1_BASE+I2C_ENABLE),read_ahb32(I2C2_BASE+I2C_ENABLE));
	  snps_i2c_config(&i2c2_info, &icp);    //start i2c  传输模式/启主禁从/速度/TAR
		printf("0x%x 0x%x\r\n",read_ahb32(I2C1_BASE+I2C_ENABLE),read_ahb32(I2C2_BASE+I2C_ENABLE));
		snps_i2c_slave_config(&i2c1_info, &icp);  //start i2c
		printf("0x%x 0x%x\r\n",read_ahb32(I2C1_BASE+I2C_ENABLE),read_ahb32(I2C2_BASE+I2C_ENABLE));
		printf("TAR: 0x%x ,SAR : 0x%x \r\n",read_ahb32(i2c2_info.base_address +  I2C_TAR),read_ahb32(i2c1_info.base_address +  I2C_SAR));
		
		write_ahb32(I2C1_BASE+I2C_ENABLE, 0x1);
    write_ahb32(I2C2_BASE+I2C_ENABLE, 0x1);
//    write_ahb32(I2C2_BASE+I2C_DATA_CMD, 0x77);
//    printf("transter finish\r\n");
//		delay_ms(2);
//		printf("0x%x  \r\n\r\n",read_ahb32(i2c1_info.base_address +  I2C_DATA_CMD));
/*********i2c2 to i2c1********/

}


/****************************************************************************
 * @brief   sample_dma_t21
 * Function test the SRAM to IIC dma transfer of SRAM and IIC are 8bit for per ch
 * @author  he
 * @date    2022-3-31
 * @param   
		channel:DMA CH 0-7
 * @return void
******************************************************************************/

void sample_dma_t21(int channel)
{
/*begin*****************顺序：IIC配置以及使能,DMA配置及使能、握手*******************/
	u8 i = 0;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = 1;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 		//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure); //根据指定的参数初始化NVIC寄存器
	
	iic_close();
	write_ahb32(I2C2_BASE + I2C_DMA_CR, 0x0); //0x1 rdmae  0x2 Tmdae
	ch_irq = channel;
	
	//注意：需要先使能IIC，再往I2C_DATA_CMD寄存器写数据
	sample_i2c_t1_dmat21();
	
	write_ahb32(0x20004A00,0xAABBCCDD);
	write_ahb32(0x20004A04,0x01020304);
	
	dw_dmac_clearIrq(channel, IRQ_TYPE_ALL);
	dw_dmac_unmaskIrq(channel,IRQ_TYPE_ALL);
	dmac_8bit_mem2iic_cfg_t21(channel);
	
	printf("DMAC_CTL_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_L(channel)));
	printf("DMAC_CTL_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_H(channel)));

	printf("DMAC_CFG_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(channel)));
	printf("DMAC_CFG_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_H(channel)));
	

	dw_dmac_enableChannel(channel);
	
	//IIC1_DMA 支持
	write_ahb32(I2C2_BASE + I2C_DMA_CR, 0x3); //0x1 rdmae  0x2 Tmdae

	printf("DMAC_CTL_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_L(channel)));
	printf("DMAC_CTL_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_H(channel)));

	printf("DMAC_CFG_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(channel)));
	printf("DMAC_CFG_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_H(channel)));
	
	for(i=0;i<7;i++)
	{
		printf("\r\n\r\nI2C1_BASE+I2C_DATA_CMD：0x%x\r\n",read_ahb32(I2C1_BASE+I2C_DATA_CMD));
	}
	
	while(1);
/*end*****************顺序：IIC配置以及使能,DMA配置及使能、握手*******************/


///*begin*****************顺序：DMA配置及使能、IIC配置以及使能、握手*******************/
//	u8 i = 0;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	
//	NVIC_InitStructure.NVIC_IRQChannel = 1;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 		//IRQ通道使能
//	NVIC_Init(&NVIC_InitStructure); //根据指定的参数初始化NVIC寄存器
//	
//	ch_irq = channel;
//	write_ahb32(I2C2_BASE + I2C_DMA_CR, 0x0); //0x1 rdmae  0x2 Tmdae
//	iic_close();
//	
//	write_ahb32(0x20004A00,0xAABBCCDD);
//	write_ahb32(0x20004A04,0x01020304);
//	
//	dw_dmac_clearIrq(channel, IRQ_TYPE_ALL);
//	dw_dmac_unmaskIrq(channel,IRQ_TYPE_ALL);
//	dmac_8bit_mem2iic_cfg_t21(channel);
//	
//	printf("DMAC_CTL_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_L(channel)));
//	printf("DMAC_CTL_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_H(channel)));

//	printf("DMAC_CFG_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(channel)));
//	printf("DMAC_CFG_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_H(channel)));
//	
//	dw_dmac_enableChannel(channel);
//	
//	
//	sample_i2c_t1_dmat21();
//	
//	//IIC1_DMA 支持
//	write_ahb32(I2C2_BASE + I2C_DMA_CR, 0x3); //0x1 rdmae  0x2 Tmdae

//	printf("DMAC_CTL_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_L(channel)));
//	printf("DMAC_CTL_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_H(channel)));

//	printf("DMAC_CFG_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(channel)));
//	printf("DMAC_CFG_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_H(channel)));
//	
//	for(i=0;i<7;i++)
//	{
//		printf("\r\n\r\nI2C1_BASE+I2C_DATA_CMD：0x%x\r\n",read_ahb32(I2C1_BASE+I2C_DATA_CMD));
//	}
//	
//	while(1);
///*end*****************顺序：DMA配置及使能、IIC配置以及使能、握手*******************/

/*begin*****************顺序：DMA配置及使能、握手、IIC配置以及使能、*******************/
//	u8 i = 0;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	
//	NVIC_InitStructure.NVIC_IRQChannel = 1;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 		//IRQ通道使能
//	NVIC_Init(&NVIC_InitStructure); //根据指定的参数初始化NVIC寄存器
//	
//	ch_irq = channel;
//	write_ahb32(I2C2_BASE + I2C_DMA_CR, 0x0); //0x1 rdmae  0x2 Tmdae
//	iic_close();
//	
//	write_ahb32(0x20004A00,0xAABBCCDD);
//	write_ahb32(0x20004A04,0x01020304);
//	
//	dw_dmac_clearIrq(channel, IRQ_TYPE_ALL);
//	dw_dmac_unmaskIrq(channel,IRQ_TYPE_ALL);
//	dmac_8bit_mem2iic_cfg_t21(channel);
//	
//	printf("DMAC_CTL_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_L(channel)));
//	printf("DMAC_CTL_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_H(channel)));

//	printf("DMAC_CFG_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(channel)));
//	printf("DMAC_CFG_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_H(channel)));
//	
//	dw_dmac_enableChannel(channel);
//	
//	//IIC1_DMA 支持
//	write_ahb32(I2C2_BASE + I2C_DMA_CR, 0x3); //0x1 rdmae  0x2 Tmdae
//	
//	sample_i2c_t1_dmat21();
//	
//	printf("DMAC_CTL_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_L(channel)));
//	printf("DMAC_CTL_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_H(channel)));

//	printf("DMAC_CFG_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(channel)));
//	printf("DMAC_CFG_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_H(channel)));
//	
//	for(i=0;i<7;i++)
//	{
//		printf("\r\n\r\nI2C1_BASE+I2C_DATA_CMD：0x%x\r\n",read_ahb32(I2C1_BASE+I2C_DATA_CMD));
//	}
//	
//	while(1);
/*end*****************顺序：DMA配置及使能、IIC配置以及使能、握手*******************/
	
}



