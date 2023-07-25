#include "sys.h"
#include "delay.h"
#include "sample_efc.h"
#include "stm32f10x.h"
#include "usart.h"
/*******************************************************************************
 * @brief   sample_efc_test_t21
 * Function eflash test
 * @author  taohu
 * @date    2022-06-23
 * @param   
		
 * @return void
*******************************************************************************/   
void sample_efc_t23(void)
{	
	OB_TypeDef opt;
	u32 i,temp;
	
	sample_unlock_ctrl();
	sample_unlock_opt();
	sample_timing_cfg(TPE_PP/BUS_PRD);

	sample_trm_rld();
	opt.Data0 = 0xff;
	opt.Data1 = 0xff;
	opt.USER = 1<<3;
	opt.RDP = 0xA5;
	opt.WRP0 = 0;
	opt.WRP1 = 0;
	opt.WRP2 = 0;
	opt.WRP3 = 0;
	
	sample_opt_pgm(opt);
	
	//EFC->fls_conf = 1<<5;
	//ÇÐ»»µ½¶ÁÄ£Ê½
	write_ahb32(0x4000200C,0x5a000000);

	for(i=0;i<4;i++)
	{
		printf("0x%x:read word:0x%x\r\n",i*4,sample_word_read(0x10020100+4*i));
	}
	
	//reset flash
	sample_trm_rld();
//	sample_unlock_ctrl();
//	sample_unlock_opt();
//	sample_timing_cfg(TPE_PP/BUS_PRD);
	
	printf("opt0:0x%x\r\n",read_ahb32(EFC_BASE+0x3C));
	
	//clear status bit
	printf("fls_stat:0x%x\r\n",EFC->fls_stat);
	printf("fls_conf:0x%x\r\n",EFC->fls_conf);
	temp = EFC->fls_stat;
	EFC->fls_stat = temp;
	printf("fls_stat:0x%x\r\n",EFC->fls_stat);
	
	printf("read protect area:\r\n");
	
	for(i=0;i<64;i++)
	{
		printf("0x%x:read word:0x%x\r\n",i*4,sample_word_read(0x10000000+4*i));
	}
	
	printf("fls_stat:0x%x\r\n",EFC->fls_stat);
}
