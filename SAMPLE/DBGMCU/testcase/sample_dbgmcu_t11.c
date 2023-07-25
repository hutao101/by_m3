#include "sys.h"
#include "delay.h"
#include "sample_dbgmcu.h"
#include "stm32f10x.h"
#include "stm32f10x.h"

u32 data[10] = {0};
/*******************************************************************************
 * @brief   sample_uart_test_t1
 * Function DWT compare
 * @author  taohu
 * @date    2022-06-11
 * @param   
		
 * @return void
*******************************************************************************/   
//DWT_COMP0-3   0x20/30/40/50
//DWT_MASK0-3   0x24/34/44/54
//DWT_FUNC0-3   0x28/38/48/58

void sample_dbgmcu_t11(void)
{	
	u32 i=0;
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk | CoreDebug_DEMCR_MON_EN_Msk;
	
	write_ahb32(DWT_BASE+0x28, 0);
	write_ahb32(DWT_BASE+0x20, (u32)&data[8]);
	write_ahb32(DWT_BASE+0x24, 2);
	write_ahb32(DWT_BASE+0x28, 6);
	
	write_ahb32(DWT_BASE+0x38, 0);
	write_ahb32(DWT_BASE+0x30, (u32)&data[3]);
	write_ahb32(DWT_BASE+0x34, 2);
	write_ahb32(DWT_BASE+0x38, 6);
	
	
	write_ahb32(DWT_BASE+0x48, 0);
	write_ahb32(DWT_BASE+0x40, (u32)&data[6]);
	write_ahb32(DWT_BASE+0x44, 2);
	write_ahb32(DWT_BASE+0x48, 6);
	
	write_ahb32(DWT_BASE+0x58, 0);
	write_ahb32(DWT_BASE+0x50, (u32)&data[2]);
	write_ahb32(DWT_BASE+0x54, 2);
	write_ahb32(DWT_BASE+0x58, 6);
	
	for(i=0;i<10;i++)
	{
		data[i] =i;
		printf("data[%d]=%d\r\n",i,i);
		delay_ms(10);
	}
}
