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

void sample_efc_t31(void)
{	
	u32 i,j;

	//sample_unlock_ctrl();
	sample_timing_cfg(TPE_PP/BUS_PRD);
	
	sample_trm_rld();

	//512��PAGE PGM��CHIPֵ��Ϊ
	for(i=0;i<512;i++)
	{
		sample_page_pgm(i,0x12345678);		
	}
	
	printf("fls_stat:0x%x\r\n",EFC->fls_stat);

	for(j=0;j<64;j++)
	{
		printf("0x%x:read word:0x%x\r\n",j*4,sample_word_read(0x10000000+4*j));
	}
	printf("fls_stat:0x%x\r\n",EFC->fls_stat);
	
}
