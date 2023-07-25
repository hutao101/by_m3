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


void sample_efc_t12(void)
{	
	u32 i;
		
	sample_unlock_ctrl();
	sample_unlock_opt();
	sample_timing_cfg(TPE_PP/BUS_PRD);
	
	sample_trm_rld();
	
	//PAGE PGM SM����CHIPֵ��Ϊ0x12345678
	for(i=512;i<520;i++)
	{
		sample_page_pgm(i,0x12345678);	
	}
	
	//�л�����ģʽ
	write_ahb32(EFC_BASE,0x5a000000);
	
	sample_page_vrf(512, 8);

	
	//PAGE ER SM����
	for(i=512;i<520;i++)
	{
		sample_page_er(i);	
	}
	
	sample_page_vrf(512, 8);
}
