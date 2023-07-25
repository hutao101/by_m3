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


void sample_efc_t40(void)
{	
	u32 i;
	for(i=0;i<17;i++)
	{
		printf("0x%x:0x%x\r\n",i*4,read_ahb32(EFC_BASE+i*4));
	}
	
}
