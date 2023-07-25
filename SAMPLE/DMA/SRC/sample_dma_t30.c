#include "sample_dma.h"

u8 ch_t30 = 1;
u32 dma_buffer[10]={0};

int dmac_8bit_spi2m2mem_cfg_t30(int channel)
{
  u32 reg_value; 
	NVIC_InitTypeDef NVIC_InitStructure;
	
	
	NVIC_InitStructure.NVIC_IRQChannel = 1;  //DMA IRQ
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
	//configure the source address 
	write_ahb32(AHB_DMAC_BASE +DMAC_SAR_L(channel),ADC_DATA);
	//configure the destination address
	write_ahb32(AHB_DMAC_BASE +DMAC_DAR_L(channel),(u32)dma_buffer);
	//configure the CTL_L
	reg_value =   (2 << 20)	   										// peripheral to memory,control by ahb-dmac
				 | (0 << 17)                                	 	// gather enable
				 | (0 << 18)										//scatter disable
				 | (1 << 11)| (1 << 14)		       //des msize=4  src msize = 4   
				 | (0 << 7) | (2 << 9)		 						//increment the destination and no change source address
				 | (2 << 4) | (2<< 1)								//src 32bit des 32bit
				 | 1;				   								//interrupt enable 
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
	printf("MCU_CFG = 00x%x\r\n",read_ahb32(MCU_CFG));
	
	
	
	reg_value =    (14 << 7)| (0 << 11)								//14 ADC1 
				 | (1 << 2) 										//prot non-cache, non-buff,prvilge data
				 | (0 << 1 ) 										//Space/data available for single transfer 
				 | 0  ;				    							//source data pre-fetching is enable
	write_ahb32(AHB_DMAC_BASE + DMAC_CFG_H(channel), reg_value);


	return 0;
}

void sample_adc_for_dma_t30(void)
{	
u32 i;
	u32 reg = 0;
	
	write_ahb32(ADC_CTRL,0);
	
	//DMA ENABLE
	reg = read_ahb32(ADC_CTRL);
	reg |= 1<<24;
	write_ahb32(ADC_CTRL,reg);
	
//	//FIFO阈值
	reg = read_ahb32(ADC_CTRL);
	reg |= ADC_CTRL_FIFO_1;
	reg |= 0x3;
	write_ahb32(ADC_CTRL,reg);	
	
//	delay_ms(1000);
//	write_ahb32(ADC_CTRL,ADC_CTRL_FIFO_1|0x3|1<<24);   //FIFO阈值
	
//	printf("\r\nADC_CTRL = 0x%x\r\n\r\n",read_ahb32(ADC_CTRL));


	write_ahb32(ADC_MODE,ADC_MODE_SINGLE);//ADC_MODE_SINGLE|ADC_MODE_CONTIN|ADC_MODE_SCAN|ADC_MODE_DISCON);   //单次模式
	
//  while((read_ahb32(ADC_STAT)&ADC_STAT_BUSY));	//等待转换完成

}	


void sample_dma_t30(void)
{
	u32 reg = 0;
	
	//预置SRAM
	write_ahb32(0x20004A00,0xffffffff);
	write_ahb32(0x20004A04,0xffffffff);
	write_ahb32(0x20004A08,0xffffffff);
	
//	/**************handshack CFG***********/
//	printf("ADC & DMA handshack CFG!\r\n");
//	reg = read_ahb32(ADC_CTRL);
//	reg |= 1<<24;  //DMA mode enable
//	write_ahb32(ADC_CTRL,reg);
//	printf("ADC_CTRL: DMA MODE enable = 0x%x\r\n",read_ahb32(ADC_CTRL));
	

		


	/*****************DMA CFG*********************/
	printf("\r\nDMA begin CFG! \r\n");
	dw_dmac_clearIrq(ch_t30, IRQ_TYPE_ALL);
	dw_dmac_unmaskIrq(ch_t30,IRQ_TYPE_ALL);
	dmac_8bit_spi2m2mem_cfg_t30(ch_t30);
	dw_dmac_enableChannel(ch_t30);
	
	/************ADC CFG***********/
	printf("\r\nADC begin CFG!\r\n");
	sample_adc_for_dma_t30();
	
	delay_ms(500);
	printf("\r\nall CFG over!\r\n");
	
	printf("DMA_CFG_L = 0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(ch_t30)));
	printf("DMA_CTL_H = 0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_H(ch_t30)));
	printf("DMA_CH_EN_L = 0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CH_EN_REG_L));
	
	printf("dma_buffer[0] = 0x%x\r\n",dma_buffer[0]);
	printf("ADC_DATA = 0x%x\r\n",read_ahb32(ADC_DATA));
	while(1);
	
	while(1)
	{
		if(read_ahb32(0x20004A00) != 0xffffffff)
		{
			printf("0x20004A00 = 0x%x\r\n",read_ahb32(0x20004A00));
			printf("0x20004A04 = 0x%x\r\n",read_ahb32(0x20004A04));
			printf("0x20004A08 = 0x%x\r\n",read_ahb32(0x20004A08));
			printf("ADC_DATA = 0x%x\r\n",read_ahb32(ADC_DATA));
		}
		delay_ms(5000);

	}
}



//////////////////////////////////////////////////////////////////////////////////////////////////
//公共函数名，用不到的时候需要注释
/**********************************************************************/
void DMA_IRQHandler(void)
{
    u32  irq_active;
    // read the interrupt status register
    irq_active = read_ahb32(AHB_DMAC_BASE +DMAC_STATUS_INT_L);
		
		printf("STATUS_INT_L: 0x%x\r\n",irq_active);

    // ERR INTERRUPT
    if((irq_active & IRQ_TYPE_ERROR) == IRQ_TYPE_ERROR) {

        // clear the interrupt
        dw_dmac_clearIrq(ch_t30, IRQ_TYPE_ERROR);
    }
    // TFR INTERRUPT
	// not enable in this driver
   	else if((irq_active & IRQ_TYPE_TFR) == IRQ_TYPE_TFR) {
        
 	    // clear the interrupt
        dw_dmac_clearIrq(ch_t30, IRQ_TYPE_TFR);
    }   
    // BLOCK INTERRUPT
    else if((irq_active & 0x2) == 0x2) {

		// clear the block interrupt
         dw_dmac_clearIrq(ch_t30, IRQ_TYPE_BLOCK);
    }
    // SRCTRAN INTERRUPT
	// not enable in this driver
    else if((irq_active & 0x4) == 0x4) {
			
        // clear the interrupt
        dw_dmac_clearIrq(ch_t30, IRQ_TYPE_SRCTRAN);
    }
    // DSTTRAN INTERRUPT
	// not enable in this driver
    else if((irq_active & 0x8) == 0x8) {
        
        // clear the interrupt
        dw_dmac_clearIrq(ch_t30, IRQ_TYPE_DSTTRAN);
    }
} 
