#include "sample_dma.h"
//m2m 8bit
//测试8通道SRAM2SRAM 8bit传输是否正常

//men2men测试
int dmac_8bit_mem2mem_cfg(struct mem2mem_info *mem2mem, int channel)
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
	if(mem2mem->gather.enable == 1) {
		reg_value =  ((mem2mem->gather.section_length_inbytes /4 ) << 20 )	|	( mem2mem->gather.section_offset_inbytes / 4);
		write_ahb32(AHB_DMAC_BASE + DMAC_SGR_L(channel), reg_value);				
	}
	//configure the scatter if enable
	if(mem2mem->scatter.enable == 1) {
	  	reg_value =  ((mem2mem->scatter.section_length_inbytes /4 ) << 20 )	|	( mem2mem->scatter.section_offset_inbytes / 4);
		write_ahb32(AHB_DMAC_BASE + DMAC_DSR_L(channel), reg_value);
	}
	return 0;
}

/****************************************************************************
 * @brief   sample_dma_t5
 * Function test the SRAM to SRAM dma transfer of 8 bit for per ch
 * @author  he
 * @date    2022-04-02
 * @param   
		channel:DMA通道 of 0-7
 * @return void
******************************************************************************/
void sample_dma_t5(int channel)
{
	struct mem2mem_info mem2mem_info_cfg ;
	u32 i = 0;
	
	write_ahb32(0x20004A00,0xffffffff);
	write_ahb32(0x20004A04,0xffffffff);
	write_ahb32(0x20004A08,0xffffffff);
	write_ahb32(0x20004A0c,0xffffffff);
	write_ahb32(0x20004A10,0x0);
	write_ahb32(0x20004A14,0x0);
	write_ahb32(0x20004A18,0x0);
	write_ahb32(0x20004A1c,0x0);
	write_ahb32(0x20004A20,0xffffffff);
	write_ahb32(0x20004A24,0xffffffff);
	write_ahb32(0x20004A28,0xffffffff);
	write_ahb32(0x20004A2c,0xffffffff);
	write_ahb32(0x20004A30,0xf0f0f0f0);
	
	mem2mem_info_cfg.src_addr = 0x20004A00;
	mem2mem_info_cfg.des_addr = 0x20004B00;
	mem2mem_info_cfg.des_width = 0;
	mem2mem_info_cfg.src_width = 0;
	mem2mem_info_cfg.block_size = 7;   //Block_TS  一个块的数据数量
	mem2mem_info_cfg.trans_length_inbytes = 7;   //传输块的长度（字节为单位）
	mem2mem_info_cfg.scatter .enable  = 0;
	mem2mem_info_cfg.gather .enable   = 0;
	mem2mem_info_cfg.int_enable = 1; 
	
	printf("DMAC_CTL_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_L(channel)));
	printf("DMAC_CTL_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_H(channel)));

	printf("DMAC_CFG_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(channel)));
	printf("DMAC_CFG_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_H(channel)));
	
	dmac_8bit_mem2mem_cfg(&mem2mem_info_cfg, channel);
	dw_dmac_enableChannel(channel);
	
	printf("DMAC_CTL_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_L(channel)));
	printf("DMAC_CTL_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_H(channel)));

	printf("DMAC_CFG_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(channel)));
	printf("DMAC_CFG_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_H(channel)));
	
//	mem2mem_info_cfg.src_addr = 0x20004B00;
//	mem2mem_info_cfg.des_addr = 0x20004C00;
//	dmac_mem2mem_transfer(&mem2mem_info_cfg,channel+1);	
	
	printf("\r\n0x20004B00的值：\r\n");
	for(i=0;i<13;i++)
	{
		printf("0x%x",read_ahb32(0x20004B00+i*4));
	}
//	printf("\r\n通道使能位值：0x%x",read_ahb32(AHB_DMAC_BASE+DMAC_CH_EN_REG_L));
	printf("\r\n");


}

