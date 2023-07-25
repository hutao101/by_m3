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
void sample_efc_t44(void)
{	
	u32 i,j;
	
	sample_unlock_ctrl();
	sample_timing_cfg(TPE_PP/BUS_PRD);
		
	sample_trm_rld();
	sample_page_pgm(0, 0x0055aaff);
	
	for(i=0;i<1;i++)
	{
		printf("%d:----------------------\r\n",i);
		for(j=0;j<64;j++)
		{
			printf("0x%x:read word:0x%x\r\n",(j*4),sample_word_read((0x10000000 + (i<<8))+(4*j)));
		}
	}
	
	//CLEAR PAGE BUFFER
	op_page_clr(0);

	//write page latch word2word
	op_page_write_byte((0<<8),0x12345678);

	//PGM operate
	sample_timing_cfg(TPE_P/BUS_PRD);
	op_pgm(ePAGE_PGM,0);
		
	for(i=0;i<1;i++)
	{
		printf("%d:----------------------\r\n",i);
		for(j=0;j<64;j++)
		{
			printf("0x%x:read word:0x%x\r\n",(j*4),sample_word_read((0x10000000 + (i<<8))+(4*j)));
		}
	}

}
