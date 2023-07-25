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


void sample_efc_t29(void)
{	
	u32 i;
	
	//使能 EFC 错误中断
	sample_efc_it_cfg(CMD_INTEN);
	sample_efc_nvic_cfg();
	
	sample_unlock_ctrl();
	sample_timing_cfg(TPE_PP/BUS_PRD);
	
	sample_trm_rld();
	

	//512次PAGE PGM，CHIP值都为0x12345678
	for(i=0;i<512;i++)
	{
		sample_page_pgm(i,0x5A5AA5A5);	
	}
	
	//默认参数读
	//切换到读模式
	write_ahb32(0x1003000C,0x5a000000);
	
	sample_page_vrf(0, 512);	
}
