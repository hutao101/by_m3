#include "sample_dma.h"
//m2m 8bit
//测试8通道SRAM2SRAM 8bit传输中断是否正常,以及可测的中断顺序是否正确。（包来错误中断）

u8 ch_t15 = 0;



//men2men测试
int dmac_8bit_mem2mem_cfg_t15(struct mem2mem_info *mem2mem, int channel)
{
    u32 reg_value;  
	//configure the source address 
	write_ahb32(AHB_DMAC_BASE +DMAC_SAR_L(channel), mem2mem->src_addr);
	//configure the destination address
	write_ahb32(AHB_DMAC_BASE +DMAC_DAR_L(channel), mem2mem->des_addr);
	//configure the CTL_L
	reg_value =   (0 << 20)	   										// memory to memory ,control by ahb-dmac
				 | ((mem2mem->gather.enable) ? (1 << 17) : 0)	 	// gather enable
				 | ((mem2mem->scatter.enable) ? (1 << 18) : 0)		//scatter enable
				 | (3 << 11)| (3 << 14)								//burst length is 16 (MSIZE)
				 | (0 << 7) | (0 << 9)		 						//increment the source and destination address
				 | (mem2mem->src_width << 4) | (mem2mem->des_width <<1 )									//source and destination transfer width is 32bits
				 | ((mem2mem->int_enable) ? 1:0);				    //interrupt enable 
	write_ahb32(AHB_DMAC_BASE + DMAC_CTL_L(channel), reg_value);	
	//configure the CTL_L	 
	reg_value =  mem2mem->block_size ;	   										 
	write_ahb32(AHB_DMAC_BASE + DMAC_CTL_H(channel), reg_value);
	//configure the CFG_L
	reg_value =    (0 << 19)| (0 << 18)								//handshake polarrity, default 0	   										// memory to memory ,control by ahb-dmac
		#if 0                                       //handshake select, default 0(硬件握手)
				 | (1 << 11)| (1 << 10)								
		#else
				 | (0 << 11)| (0 << 10)
		#endif
				 | (0 << 8 ) 										//DMA transfer from the source is not sspended
				 | (channel << 5);				    				//channel prority is set to the channel index,means the channel 7 is the hishgest channel
	write_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(channel), reg_value);
	//configure the CFG_H
	reg_value =    (0 << 11)| (0 << 7)								//handshake req source number,default 0，硬件握手需配置，软件握手不用管
				 | (1 << 2) 										//prot non-cache, non-buff,prvilge data
				 | (0 << 1 ) 										//Space/data available for single transfer 
				 | 0  ;				    							//source data pre-fetching is enable
	write_ahb32(AHB_DMAC_BASE + DMAC_CFG_H(channel), reg_value);
	//configure the gather if enable

	return 0;
}

/****************************************************************************
 * @brief   sample_dma_t15
 * Function test the SRAM to SRAM dma transfer of 8 bit for per ch,to check 
 * whether Interrupt is ok ,this test include err Interrupt
 * @author  he
 * @date    2022-04-02
 * @param   
		channel:DMA通道 of 0
 * @return void
******************************************************************************/
void sample_dma_t15(int channel)
{
	
	NVIC_InitTypeDef NVIC_InitStructure;
	
	struct mem2mem_info mem2mem_info_cfg ;
	u32 i = 0;
	u32 j =0;
	printf("DmaIDReg: 0x%x\r\n",read_ahb32(AHB_DMAC_BASE+0x3a8));
	NVIC_InitStructure.NVIC_IRQChannel = 1;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 		//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure); //根据指定的参数初始化NVIC寄存器
	
	for(j=0;j<6;j++)
	{
		for(i=0;i<200;i++)  //1200
		{
			write_ahb32(0x20004A00+j*200+i,i);
		}
	}
	
	delay_ms(20);
	mem2mem_info_cfg.src_addr = 0x20004A00;  //0x338 为ClearTfr_L,不可读
	mem2mem_info_cfg.des_addr = 0x20004C00;
#if 1
	mem2mem_info_cfg.des_width = 0;             
	mem2mem_info_cfg.src_width = 0;
	mem2mem_info_cfg.block_size = 16;   //Block_TS  一个块的数据数量
#else
	mem2mem_info_cfg.des_width = 4;     //128位位宽传输，实测也能正常传输，应该是分批传输        
	mem2mem_info_cfg.src_width = 4;
	mem2mem_info_cfg.block_size = 2;   //Block_TS  一个块的数据数量
#endif
	mem2mem_info_cfg.scatter .enable  = 0;
	mem2mem_info_cfg.gather .enable   = 0;
	mem2mem_info_cfg.int_enable = 1; 
	
	dw_dmac_clearIrq(channel, IRQ_TYPE_ALL);
	dw_dmac_unmaskIrq(channel,IRQ_TYPE_ALL);
	
	dmac_8bit_mem2mem_cfg_t15(&mem2mem_info_cfg, channel);
	dw_dmac_enableChannel(channel);
	
	printf("\r\n0x20004B00的值：\r\n");
	for(i=0;i<40;i++)
	{
		printf("0x%x\r\n",read_ahb32(0x20004B00+i));
	}
	printf("\r\n通道使能位值：0x%x",read_ahb32(AHB_DMAC_BASE+DMAC_CH_EN_REG_L));
	printf("\r\n");
	delay_ms(40000);
	printf("STATUS_INT_L: 0x%x\r\n",read_ahb32(AHB_DMAC_BASE +DMAC_STATUS_INT_L));
	
	printf("DMAC_CTL_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_L(0)));
	printf("DMAC_CTL_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_H(0)));

	printf("DMAC_CFG_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(0)));
	printf("DMAC_CFG_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_H(0)));
	printf("通道使能位值：0x%x\r\n",read_ahb32(AHB_DMAC_BASE+DMAC_CH_EN_REG_L));

}

//公用的中断函数 谁用谁打开
////////////////////////////////////////////////////////////////////////////////////////////////////
///**********************************************************************/
void DMA_IRQHandler(void)
{
    u32  irq_active;

    // read the interrupt status register
    irq_active = read_ahb32(AHB_DMAC_BASE +DMAC_STATUS_INT_L);
		
			printf("STATUS_INT_L: 0x%x\r\n",irq_active);


    // ERR INTERRUPT
    if((irq_active & IRQ_TYPE_ERROR) == IRQ_TYPE_ERROR) {

        // clear the interrupt
        dw_dmac_clearIrq(ch_t15, IRQ_TYPE_ERROR);
    }
    // TFR INTERRUPT
	// not enable in this driver
   	else if((irq_active & IRQ_TYPE_TFR) == IRQ_TYPE_TFR) {
        
 	    // clear the interrupt
        dw_dmac_clearIrq(ch_t15, IRQ_TYPE_TFR);
    }   
    // BLOCK INTERRUPT
    else if((irq_active & 0x2) == 0x2) {

		// clear the block interrupt
         dw_dmac_clearIrq(ch_t15, IRQ_TYPE_BLOCK);
    }
    // SRCTRAN INTERRUPT
	// not enable in this driver
    else if((irq_active & 0x4) == 0x4) {
			
        // clear the interrupt
        dw_dmac_clearIrq(ch_t15, IRQ_TYPE_SRCTRAN);
    }
    // DSTTRAN INTERRUPT
	// not enable in this driver
    else if((irq_active & 0x8) == 0x8) {
        
        // clear the interrupt
        dw_dmac_clearIrq(ch_t15, IRQ_TYPE_DSTTRAN);
    }
		
		printf("STATUS_INT_L: 0x%x\r\n",read_ahb32(AHB_DMAC_BASE +DMAC_STATUS_INT_L));
} 
