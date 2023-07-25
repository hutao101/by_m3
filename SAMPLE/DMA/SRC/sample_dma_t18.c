#include "sample_dma.h"
 
//2022.06.20����IIC2 DMA ���������Ⱥ�˳�� t18
//p2m 8bit  IIC1 to IIC2

//���� sample_i2c_t1.C�е�/*********i2c1 to i2c2********/  ����/*******i2c2 to i2c1**********/ 



int dmac_8bit_iic2mem_cfg(int channel)
{
    u32 reg_value; 
	//configure the source address 
	write_ahb32(AHB_DMAC_BASE +DMAC_SAR_L(channel),I2C2_BASE+I2C_DATA_CMD);
	//configure the destination address
	write_ahb32(AHB_DMAC_BASE +DMAC_DAR_L(channel),0x20004B00);
	//configure the CTL_L
	reg_value =   (2 << 20)	   										// peripheral to memory,control by ahb-dmac
				 | (0 << 17)                                	 	// gather enable
				 | (0 << 18)										//scatter disable
				 | (2 << 11)| (0 << 14)		       //des 8  src 1    //source burst length is 16	and dest burst length is half of the peripheral fifo depth
				 | (0 << 7) | (2 << 9)		 						//increment the source and no change destination address
				 | (0 << 4) | (0 << 1)								//src 8bit des 16bit
				 | 0;				   								//interrupt enable 
	write_ahb32(AHB_DMAC_BASE + DMAC_CTL_L(channel), reg_value);
	//configure the CTL_H	 
	reg_value =  1;	   										 
	write_ahb32(AHB_DMAC_BASE + DMAC_CTL_H(channel), reg_value);
	//configure the CFG_L
	reg_value =    (0 << 19)| (0 << 18)								//handshake polarrity, src default 0, dest base on setting	   										// memory to memory ,control by ahb-dmac
				 | (0 << 11)| (0 << 10)								//handshake select hardware
				 | (0 << 8 ) 										//DMA transfer from the source is not sspended
				 | (channel << 5);				    				//channel prority is set to the channel index,means the channel 7 is the hishgest channel
	write_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(channel), reg_value);
	//configure the CFG_H
	reg_value =    (I2C2_RX_DMA_BREQ << 7)| (0 << 11)								//USART3 RX -- 4
				 | (1 << 2) 										//prot non-cache, non-buff,prvilge data
				 | (0 << 1 ) 										//Space/data available for single transfer 
				 | 0  ;				    							//source data pre-fetching is enable
	write_ahb32(AHB_DMAC_BASE + DMAC_CFG_H(channel), reg_value);


	return 0;
}


/****************************************************************************
 * @brief   sample_dma_t18
 * Function test the IIC to SRAM dma transfer of des is 8bit for per ch
 * @author  he
 * @date    2022-04-02
 * @param   
		channel:DMAͨ�� of 0-7
 * @return void
******************************************************************************/

void sample_dma_t18(int channel)
{
	
/*begin***************************������DMA Ȼ������IIC���֡�IIC2 ***********************************/
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = 1;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 		//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure); //����ָ���Ĳ�����ʼ��NVIC�Ĵ���
	
	ch_irq = channel;
	
	iic_close();
	write_ahb32(I2C2_BASE + I2C_DMA_CR, 0x0); //���ȹر�IIC����ʹ��
	
	dw_dmac_clearIrq(channel, IRQ_TYPE_ALL);
	dw_dmac_unmaskIrq(channel,IRQ_TYPE_ALL);
	dmac_8bit_iic2mem_cfg(channel);
	
	printf("DMAC_CTL_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_L(channel)));
	printf("DMAC_CTL_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_H(channel)));

	printf("DMAC_CFG_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(channel)));
	printf("DMAC_CFG_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_H(channel)));
	
	dw_dmac_enableChannel(channel);
	
	//IIC1_DMA ֧��
	write_ahb32(I2C2_BASE + I2C_DMA_CR, 0x1); //0x1 rdmae  0x2 Tmdae

	
	sample_i2c_t1();
	write_ahb32(I2C1_BASE+I2C_DATA_CMD, 0x77);
	printf("transter finish\r\n");
	delay_ms(2);
	printf("0x%x  \r\n\r\n",read_ahb32(I2C2_BASE +  I2C_DATA_CMD));
	
	
	delay_ms(2);
	printf("\r\n0x20004B00��0x%x\r\n",read_ahb32(0x20004B00));
	printf("\r\n0x20004B04��0x%x\r\n",read_ahb32(0x20004B04));
	printf("\r\n0x20004B08��0x%x\r\n",read_ahb32(0x20004B08));
	printf("\r\n0x20004B0C��0x%x\r\n",read_ahb32(0x20004B0C));
	
	printf("DMAC_CTL_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_L(channel)));
	printf("DMAC_CTL_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_H(channel)));

	printf("DMAC_CFG_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(channel)));
	printf("DMAC_CFG_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_H(channel)));
	while(1);
/*end***************************������DMA Ȼ������IIC2���֡�IIC2 ***********************************/


///*begin***************************������IIC2���֡�IIC2 Ȼ������DMA ***********************************/
//	NVIC_InitTypeDef NVIC_InitStructure;
//	
//	NVIC_InitStructure.NVIC_IRQChannel = 1;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 		//IRQͨ��ʹ��
//	NVIC_Init(&NVIC_InitStructure); //����ָ���Ĳ�����ʼ��NVIC�Ĵ���
//	
//	iic_close();
//	ch_irq = channel;
//	
//	write_ahb32(I2C2_BASE + I2C_DMA_CR, 0x0); //���ȹر�IIC����ʹ��
//	
//		//IIC1_DMA ֧��
//	write_ahb32(I2C2_BASE + I2C_DMA_CR, 0x1); //0x1 rdmae  0x2 Tmdae

//	
//	sample_i2c_t1();
//	
//	dw_dmac_clearIrq(channel, IRQ_TYPE_ALL);
//	dw_dmac_unmaskIrq(channel,IRQ_TYPE_ALL);
//	dmac_8bit_iic2mem_cfg(channel);
//	
//	printf("DMAC_CTL_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_L(channel)));
//	printf("DMAC_CTL_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_H(channel)));

//	printf("DMAC_CFG_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(channel)));
//	printf("DMAC_CFG_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_H(channel)));
//	
//	dw_dmac_enableChannel(channel);
//	
//	write_ahb32(I2C1_BASE+I2C_DATA_CMD, 0x77);
//	printf("transter finish\r\n");
//	delay_ms(2);
//	printf("0x%x  \r\n\r\n",read_ahb32(I2C2_BASE +  I2C_DATA_CMD));
//	
//	delay_ms(2);
//	printf("\r\n0x20004B00��0x%x\r\n",read_ahb32(0x20004B00));
//	printf("\r\n0x20004B04��0x%x\r\n",read_ahb32(0x20004B04));
//	printf("\r\n0x20004B08��0x%x\r\n",read_ahb32(0x20004B08));
//	printf("\r\n0x20004B0C��0x%x\r\n",read_ahb32(0x20004B0C));
//		
//	printf("DMAC_CTL_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_L(channel)));
//	printf("DMAC_CTL_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_H(channel)));

//	printf("DMAC_CFG_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(channel)));
//	printf("DMAC_CFG_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_H(channel)));
//	printf("\r\n\r\n");
//	while(1);
///*end***************************������DMA Ȼ������IIC2 ***********************************/


/*begin***************************������DMA Ȼ������IIC������ʹ�� ********************/
//	NVIC_InitTypeDef NVIC_InitStructure;
//	
//	NVIC_InitStructure.NVIC_IRQChannel = 1;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 		//IRQͨ��ʹ��
//	NVIC_Init(&NVIC_InitStructure); //����ָ���Ĳ�����ʼ��NVIC�Ĵ���
//	
//	iic_close();
//	
//	ch_irq = channel;
//	
//	write_ahb32(I2C2_BASE + I2C_DMA_CR, 0x0); //���ȹر�IIC����ʹ��
//	
//	dw_dmac_clearIrq(channel, IRQ_TYPE_ALL);
//	dw_dmac_unmaskIrq(channel,IRQ_TYPE_ALL);
//	dmac_8bit_iic2mem_cfg(channel);
//	
//	printf("DMAC_CTL_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_L(channel)));
//	printf("DMAC_CTL_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_H(channel)));

//	printf("DMAC_CFG_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(channel)));
//	printf("DMAC_CFG_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_H(channel)));
//	
//	dw_dmac_enableChannel(channel);
//	
//	sample_i2c_t1();
//	
//	//IIC1_DMA ֧��
//	write_ahb32(I2C2_BASE + I2C_DMA_CR, 0x1); //0x1 rdmae  0x2 Tmdae

//	write_ahb32(I2C1_BASE+I2C_DATA_CMD, 0x77);
//	printf("transter finish\r\n");
//	delay_ms(2);
//	printf("0x%x  \r\n\r\n",read_ahb32(I2C2_BASE +  I2C_DATA_CMD));
//	
//	delay_ms(2);
//	printf("\r\n0x20004B00��0x%x\r\n",read_ahb32(0x20004B00));
//	printf("\r\n0x20004B04��0x%x\r\n",read_ahb32(0x20004B04));
//	printf("\r\n0x20004B08��0x%x\r\n",read_ahb32(0x20004B08));
//	printf("\r\n0x20004B0C��0x%x\r\n",read_ahb32(0x20004B0C));
//		
//	printf("DMAC_CTL_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_L(channel)));
//	printf("DMAC_CTL_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_H(channel)));

//	printf("DMAC_CFG_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(channel)));
//	printf("DMAC_CFG_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_H(channel)));
//	printf("\r\n\r\n");
//	while(1);
/*end***************************������DMA Ȼ������IIC2 ***********************************/
}



