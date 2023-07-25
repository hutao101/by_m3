#include "sample_dma.h"
//m 2 m 32bit

/****************************************************************************
 * @brief   sample_dma_t1
 * Function test the SRAM to SRAM dma transfer of 32bit for per ch
 * @author  he
 * @date    2022-3-31
 * @param   void (but notice the param channel which in this function which can be 0-7)
 * @return void
******************************************************************************/

//men2men����
void sample_dma_t1(void)
{
	struct mem2mem_info mem2mem_info_cfg ;
	u32 i = 0;
	u32 des_addr = 0x20004B00;
	int channel = 0;
	
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
	mem2mem_info_cfg.des_addr = des_addr;
	mem2mem_info_cfg.des_width = 2;
	mem2mem_info_cfg.src_width = 2;
	mem2mem_info_cfg.block_size = 13;   //Block_TS  һ�������������
	mem2mem_info_cfg.trans_length_inbytes = 52;   //�����ĳ��ȣ��ֽ�Ϊ��λ��
	mem2mem_info_cfg.scatter .enable  = 0;
	mem2mem_info_cfg.gather .enable   = 0;
	mem2mem_info_cfg.int_enable = 1; 
	
	dmac_mem2mem_transfer(&mem2mem_info_cfg,channel);
	
	
	mem2mem_info_cfg.src_addr = 0x20004B00;
	mem2mem_info_cfg.des_addr = 0x20004C00;
	dmac_mem2mem_transfer(&mem2mem_info_cfg,channel+1);	
	
	printf("\r\n0x20004C00��ֵ��\r\n");
	for(i=0;i<13;i++)
	{
		printf("0x%x",read_ahb32(0x20004C00+i*4));
	}
//	printf("\r\nͨ��ʹ��λֵ��0x%x",read_ahb32(AHB_DMAC_BASE+DMAC_CH_EN_REG_L));
	printf("\r\n");
	printf("\r\nͨ��ʹ��λֵ��0x%x",read_ahb32(AHB_DMAC_BASE+DMAC_CH_EN_REG_L));

}






