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


void sample_efc_t10(void)
{	
	u32 i,j;
		
	sample_unlock_ctrl();
	sample_unlock_opt();
	sample_timing_cfg(TPE_PP/BUS_PRD);
	
	sample_trm_rld();
	

	//sector PGM SM区域，CHIP值都为0x0055aaff	
	sample_bank_pgm(2,0x12345678);
	
	//默认参数读
	for(i=512;i<520;i++)
	{
		printf("%d:----------------------\r\n",i);
		for(j=0;j<64;j++)
		{
			printf("0x%x:read word:0x%x\r\n",j*4,sample_word_read((0x10000000 + (i<<8))+4*j));
		}
	}
	
	for(i=0;i<1;i++)
	{
		printf("%d:----------------------\r\n",i);
		for(j=0;j<64;j++)
		{
			printf("0x%x:read word:0x%x\r\n",j*4,sample_word_read((0x10000000 + (i<<8))+4*j));
		}
	}
}
