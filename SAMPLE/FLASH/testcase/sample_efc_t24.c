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

void sample_efc_t24(void)
{	
	OB_TypeDef opt;
	u32 i,temp;
	
	sample_unlock_ctrl();
	sample_unlock_opt();
	sample_timing_cfg(TPE_PP/BUS_PRD);

	sample_trm_rld();
	opt.Data0 = 0xff;
	opt.Data1 = 0xff;
	opt.USER = 0;
	opt.RDP = 0x00;
	opt.WRP0 = 0xAA;
	opt.WRP1 = 0xff;
	opt.WRP2 = 0xff;
	opt.WRP3 = 0xff;
	
	sample_opt_pgm(opt);
	
	//切换到读模式
	write_ahb32(0x4000200C,0x5a000000);

	for(i=0;i<4;i++)
	{
		printf("0x%x:read word:0x%x\r\n",i*4,sample_word_read(0x10020100+4*i));
	}
	
	//reset flash
	sample_trm_rld();
	sample_stat_clr();
	
	//512次PAGE PGM，CHIP值都为0x5A5AA5A5
	for(i=0;i<512;i++)
	{
		sample_page_pgm(i,0x5A5AA5A5);	
		
		printf("fls_stat:0x%x\r\n",EFC->fls_stat); //读取FLASH状态
		temp = EFC->fls_stat;
		EFC->fls_stat = temp;
	}
	
	
	//校验数据是否写入
	sample_page_vrf(0, 512);
}
