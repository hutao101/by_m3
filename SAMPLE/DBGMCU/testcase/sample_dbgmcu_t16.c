#include "sys.h"
#include "delay.h"
#include "sample_dbgmcu.h"
#include "stm32f10x.h"
#include "core_cm3.h"

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

void sample_dbgmcu_t16(void)
{	
	write_ahb32(0x4001b070,0<<24);
	while(1)
	{
		ITM_SendChar('B');
		ITM_SendChar('Y');
		ITM_SendChar('K');
		ITM_SendChar('J');
	}
}
