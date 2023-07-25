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

void sample_efc_t26(void)
{	
	u32 i,temp;
	for(i=0;i<17;i++)
	{
		printf("0x%x:0x%x\r\n",i*4,read_ahb32(EFC_BASE+i*4));
	}
	printf("\r\n");
	
	sample_timing_cfg(TPE_PP/BUS_PRD);
	sample_trm_rld();
	sample_unlock_ctrl();
	

	//LOCK ×´Ì¬ÏÂ¶ÁÈ¡×´Ì¬ÐÅÏ¢
	printf("0.FLS_OPT LOCKED\r\n");
	printf("FLS_CONF :0x%x\r\n",EFC->fls_conf);
	printf("READ FLS_STAT:0x%x\r\n",EFC->fls_stat);
	printf("\r\n");
	//Çå³ý×´Ì¬¼Ä´æÆ÷
	temp = EFC->fls_stat;
	EFC->fls_stat = temp;

	//UNLOCK×´Ì¬ÏÂ¶ÁÈ¡×´Ì¬ÐÅÏ¢
	printf("1.FLS_OPT UNLOCK\r\n");
	EFC->fls_key1 = FLS_KEY1;
	EFC->fls_key1 = FLS_KEY2;
	printf("READ FLS_CONF:0x%x\r\n",EFC->fls_conf);
	printf("READ FLS_STAT:0x%x\r\n",EFC->fls_stat);
	printf("\r\n");
	//Çå³ý×´Ì¬¼Ä´æÆ÷
	temp = EFC->fls_stat;
	EFC->fls_stat = temp;
	
	//SW LOCK fls_ctrl
	printf("2.FLS_CTRL SW LOCK\r\n");
	EFC->fls_conf |= ((u32)1<<30);
	printf("READ FLS_CONF:0x%x\r\n",EFC->fls_conf);
	printf("READ FLS_STAT:0x%x\r\n",EFC->fls_stat);
	printf("\r\n");
	//Çå³ý×´Ì¬¼Ä´æÆ÷
	temp = EFC->fls_stat;
	EFC->fls_stat = temp;
	
	//ÏÈÐ´LOCK2ÔÙÐ´LOCK1
	printf("3.FLS_CTRL UNLOCK ERR CONF\r\n");
	EFC->fls_key1 = FLS_KEY2;
	EFC->fls_key1 = FLS_KEY1;
	printf("READ FLS_CONF:0x%x\r\n",EFC->fls_conf);
	printf("READ FLS_STAT:0x%x\r\n",EFC->fls_stat);
	printf("\r\n");
	
}
