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

void sample_efc_t34(void)
{	
	
	sample_unlock_ctrl();
	sample_timing_cfg(TPE_PP/BUS_PRD);
	sample_trm_rld();
	
	//PGM前未做ER
	while((EFC->fls_stat & FLS_BUSY) != 0);//等待BUSY释放
	sample_set_opm(ePAGE_PGM,0);
	printf("PGM FLS_STAT:0x%x\r\n",EFC->fls_stat);
	while((EFC->fls_stat & CMD_DONE) == 0);//等待CMD_DONE置位

}
