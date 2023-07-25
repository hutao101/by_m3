#include "sys.h"
#include "delay.h"
#include "sample_efc.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_efc_test_t22
 * Function eflash test
 * @author  taohu
 * @date    2022-06-23
 * @param   
		
 * @return void
*******************************************************************************/   


void sample_efc_t32(void)
{	
	u32 i;

	sample_unlock_ctrl();
	sample_timing_cfg(TPE_PP/BUS_PRD);
	
	sample_trm_rld();

	//512次PAGE PGM，CHIP值都为
	for(i=513;i<520;i++)
	{
		sample_page_pgm(i,0x12345678);		
	}
	
	printf("fls_stat:0x%x\r\n",EFC->fls_stat);

	sample_page_vrf(513, 8);
	
	printf("fls_stat:0x%x\r\n",EFC->fls_stat);
}
