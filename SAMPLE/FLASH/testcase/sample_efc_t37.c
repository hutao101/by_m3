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

void sample_efc_t37(void)
{	
	OB_TypeDef opt;
	u32 i;
	
	sample_unlock_ctrl();
	sample_unlock_opt();
	sample_timing_cfg(TPE_PP/BUS_PRD);
	
	sample_trm_rld();
	opt.Data0 = 0xff;
	opt.Data1 = 0xff;
	opt.USER = 0;
	opt.RDP = 0x0;
	opt.WRP0 = 0;
	opt.WRP1 = 0;
	opt.WRP2 = 0;
	opt.WRP3 = 0;

	sample_opt_pgm(opt);

	for(i=0;i<4;i++)
	{
		printf("0x%x:read word:0x%x\r\n",i*4,sample_word_read(0x10020100+4*i));
	}
	
	//¸´Î»flash£¬load option byte
	sample_trm_rld();

	for(i=0;i<4;i++)
	{
		printf("0x%x:read word:0x%x\r\n",i*4,sample_word_read(0x10020100+4*i));
	}
	printf("fls_opt0:0x%x\r\n",EFC->fls_opt0);
	printf("fls_opt1:0x%x\r\n",EFC->fls_opt1);
}
