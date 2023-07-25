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
void sample_efc_t46(void)
{	
	u32 i,j;
	
	sample_unlock_ctrl();
	sample_timing_cfg(TPE_PP/BUS_PRD);
		
	sample_trm_rld();
	
	//pre+er
	sample_page_er(0);
	
	//CLEAR PAGE BUFFER
	op_page_clr(0);

	//write page latch all
	while((EFC->fls_stat & FLS_BUSY) != 0);//等待BUSY释放
	sample_set_opm(eWR_PAGE_ALL,0);
	write_ahb32(FLSAH_BASE+ (0 << 8)+2,0x12345678);
	while((EFC->fls_stat & CMD_DONE) == 0);//等待CMD_DONE置位

	//PGM operate
	sample_timing_cfg(TPE_P/BUS_PRD);
	op_pgm(ePAGE_PGM,0);
		
	for(i=0;i<2;i++)
	{
		printf("%d:----------------------\r\n",i);
		for(j=0;j<64;j++)
		{
			printf("0x%x:read word:0x%x\r\n",(j*4),sample_word_read((0x10000000 + (i<<8))+(4*j)));
		}
	}
}
