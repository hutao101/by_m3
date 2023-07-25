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
void sample_efc_t2(void)
{	
	u32 i,j;
	
	sample_unlock_ctrl();
	sample_timing_cfg(TPE_PP/BUS_PRD);
	
	sample_trm_rld();
	
	
	//默认参数读
	for(i=0;i<8;i++)
	{
		printf("%d:----------------------\r\n",i);
		for(j=0;j<64;j++)
		{
			printf("0x%x:read word:0x%x\r\n",j*4,sample_word_read((0x10020000 + (i<<8))+4*j));
		}
	}

	//DIS_PREF
	sample_read_cnf(DIS_PREF);
	printf("DIS_PREF\r\n");
	for(i=0;i<8;i++)
	{
		printf("%d:----------------------\r\n",i);
		for(j=0;j<64;j++)
		{
			printf("0x%x:read word:0x%x\r\n",j*4,sample_word_read((0x10000000 + (i<<8))+4*j));
		}
	}
	
	//RD_TURBO
	sample_read_cnf(RD_TURBO);
	printf("READ TURBO\r\n");
	for(i = 0;i < 8;i++)
	{
		printf("%d:----------------------\r\n",i);
		for(j=0;j<64;j++)
		{
			printf("0x%x:read word:0x%x\r\n",j*4,sample_word_read((0x10000000 + (i<<8))+4*j));
		}
	}

	//RD_NOWAIT
	sample_read_cnf(RD_NOWAIT);
	printf("READ NOWAIT\r\n");
	for(i=0;i<8;i++)
	{
		printf("%d:----------------------\r\n",i);
		for(j=0;j<64;j++)
		{
			printf("0x%x:read word:0x%x\r\n",j*4,sample_word_read((0x10000000 + (i<<8))+4*j));
		}
	}
	
	//默认参数读
	sample_read_cnf(0);
	for(i=0;i<8;i++)
	{
		printf("%d:----------------------\r\n",i);
		for(j=0;j<64;j++)
		{
			printf("0x%x:read word:0x%x\r\n",j*4,sample_word_read((0x10020000 + (i<<8))+4*j));
		}
	}
	
}
