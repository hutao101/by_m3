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
void sample_efc_t1_1(void)
{	
	u32 i,j;
	
	sample_unlock_ctrl();
	sample_timing_cfg(TPE_PP/BUS_PRD);
		
	sample_trm_rld();
	
	//PGM PAGE0
	sample_page_pgm(0,0x5A5AA5A5);	
	
	//切换到读模式
	EFC->fls_ctrl = 0x5a000000;
	
	sample_set_pwrm(eMOD_SLP);
	while((EFC->fls_stat & CMD_DONE) == 0);//等待CMD_DONE置位
		
	sample_set_pwrm(eMOD_IDLE);
	printf("fls_stat:0x%x\r\n",EFC->fls_stat);
	while((EFC->fls_stat & CMD_DONE) == 0);//等待CMD_DONE置位
	
	//RD_TURBO
	//切换到读模式
	EFC->fls_ctrl = 0x5a000000;
	
	sample_read_cnf(RD_TURBO);
	printf("READ TURBO\r\n");

	for(i = 0;i < 1;i++)
	{
		printf("%d:----------------------\r\n",i);
		for(j=0;j<64;j++)
		{
			printf("0x%x:read word:0x%x\r\n",j*4,sample_word_read((0x10000000 + (i<<8))+4*j));
		}
	}
}
