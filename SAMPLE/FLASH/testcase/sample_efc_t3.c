#include "sys.h"
#include "delay.h"
#include "sample_efc.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_efc_test_t1
 * Function eflash test
 * @author  taohu
 * @date    2022-06-23
 * @param   
		
 * @return void
*******************************************************************************/   
void sample_efc_t3(void)
{	
	u32 i;
	
	sample_unlock_ctrl();
	sample_timing_cfg(TPE_PP/BUS_PRD);
	
	sample_trm_rld();
	

	//512次PAGE PGM，CHIP值都为0x5A5AA5A5
	for(i=0;i<512;i++)
	{
		sample_page_pgm(i,0x5A5AA5A5);	
	}
	
	//默认参数读
	//切换到读模式
	write_ahb32(EFC_BASE,0x5a000000);
	
	sample_page_vrf(0, 512);
}
