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

void sample_efc_t27(void)
{	
	sample_timing_cfg(TPE_PP/BUS_PRD);
	//sample_trm_rld();
	
	//LOCK 状态下写fls_ctrl
//	printf("0.FLS_CTRL LOCKED\r\n");
//	printf("FLS_CONF :0x%x\r\n",EFC->fls_conf);
//	EFC->fls_ctrl = 0x5a<<24 | 0xb0;
//	printf("WRITE FLS_CTRL:0xb0\r\n");
//	printf("READ FLS_CTRL:0x%x\r\n",EFC->fls_ctrl);
//	printf("READ FLS_STAT:0x%x\r\n",EFC->fls_stat);
//	printf("\r\n");
	
	write_ahb32(0xE000ED24,1<<17);
	//使能总线错误
	EFC->fls_conf = BUS_ERR;
	
	//先写LOCK2再写LOCK1
	printf("3.FLS_CTRL UNLOCK ERR CONF\r\n");
	EFC->fls_key0 = FLS_KEY2;
	EFC->fls_key0 = FLS_KEY1;
	
	EFC->fls_ctrl = 0x5a<<24 | 0xf0;
	printf("WRITE FLS_CTRL:0xc0\r\n");
	printf("READ FLS_CTRL:0x%x\r\n",EFC->fls_ctrl);
	printf("READ FLS_CONF:0x%x\r\n",EFC->fls_conf);
	printf("READ FLS_STAT:0x%x\r\n",EFC->fls_stat);
	printf("\r\n");	
}
