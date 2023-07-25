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


void sample_efc_t20(void)
{	
	u32 i=0,j;
	
	sample_unlock_ctrl();
	sample_timing_cfg(TPE_PP/BUS_PRD);
	
	sample_trm_rld();

	//512次PAGE PGM，CHIP值都为0x12345678
	//for(i=0;i<512;i++)
	{
		sample_page_pgm(0,0x5500AAFF);	
	}
	
	//默认参数读
	//for(i=0;i<512;i++)
	{
		printf("%d:----------------------\r\n",i);
		for(j=0;j<64;j++)
		{
			printf("0x%x:read word:0x%x\r\n",j*4,sample_word_read(0x10000000+4*j));
		}
	}
	
}
