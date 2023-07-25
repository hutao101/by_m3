#include "sys.h"
#include "delay.h"
#include "sample_efc.h"
#include "stm32f10x.h"
#include "usart.h"
/*******************************************************************************
 * @brief   sample_efc_test_t13
 * Function eflash test
 * @author  taohu
 * @date    2022-06-23
 * @param   
		
 * @return void
*******************************************************************************/   
void sample_efc_t13(void)
{	
	u32 i;
	
	sample_unlock_ctrl();
	sample_timing_cfg(TPE_PP/BUS_PRD);
	
	sample_trm_rld();
	
	//512��SECTOR PGM��CHIPֵ��Ϊ0xff00ff00
	for(i=0;i<64;i++)
	{
		sample_sector_pgm(i,0xff00ff00);
	}
	
	//�л�����ģʽ
	write_ahb32(EFC_BASE,0x5a000000);
	sample_page_vrf(0, 1);

	
	//PAGE ER SM����
	for(i=0;i < 64;i++)
	{
		sample_sector_er(i);
	}
	
	//�л�����ģʽ
	write_ahb32(EFC_BASE,0x5a000000);
	sample_page_vrf(0, 512);
}
