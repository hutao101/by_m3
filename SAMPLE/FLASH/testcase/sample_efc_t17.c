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

void sample_efc_t17(void)
{	
	u32 i;
	
	sample_unlock_ctrl();
	sample_timing_cfg(TPE_PP/BUS_PRD);
	
	sample_trm_rld();

	//512��PAGE PGM��CHIPֵ��Ϊ0x5A5AA5A5
	for(i=0;i<512;i++)
	{
		sample_page_pgm(i,0x5A5AA5A5);	
	}
	
	//�л�����ģʽ
	write_ahb32(EFC_BASE,0x5a000000);
	sample_page_vrf(0, 512);
	
	//��CHIP
	sample_bulk_er();
	
	//�л�����ģʽ
	write_ahb32(EFC_BASE,0x5a000000);
	sample_page_vrf(0, 512);
	
}
