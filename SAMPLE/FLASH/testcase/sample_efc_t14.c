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

void sample_efc_t14(void)
{		
	sample_unlock_ctrl();
	sample_unlock_opt();
	sample_timing_cfg(TPE_PP/BUS_PRD);
	
	sample_trm_rld();
	
	//SECTOR PGM SM区域，CHIP值都为0x12345678
	sample_sector_pgm(64,0x12345678);	

	//切换到读模式
	write_ahb32(EFC_BASE,0x5a000000);
	
	sample_page_vrf(512, 8);
	sample_page_vrf(0, 8);
	
	//SECTOR ER SM区域
	sample_sector_er(64);	

	sample_page_vrf(512, 8);
	sample_page_vrf(0, 8);	
}
