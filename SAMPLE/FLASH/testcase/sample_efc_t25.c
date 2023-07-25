#include "sys.h"
#include "delay.h"
#include "sample_efc.h"
#include "stm32f10x.h"
#include "usart.h"
/*******************************************************************************
 * @brief   sample_efc_test_t25
 * Function eflash test
 * @author  taohu
 * @date    2022-06-23
 * @param   
		
 * @return void
*******************************************************************************/   

void sample_efc_t25(void)
{	
	u32 i,temp;
	for(i=0;i<17;i++)
	{
		printf("0x%x:0x%x\r\n",i*4,read_ahb32(EFC_BASE+i*4));
	}
	printf("\r\n");
	
	//Ê¹ÄÜ EFC ´íÎóÖÐ¶Ï
//	sampel_efc_it_cfg(ERR_INTEN);
//	sample_efc_nvic_cfg();
	
	sample_timing_cfg(TPE_PP/BUS_PRD);
	//sample_trm_rld();
	
	//Çå³ý×´Ì¬¼Ä´æÆ÷
	temp = EFC->fls_stat;
	EFC->fls_stat = temp;
	//LOCK ×´Ì¬ÏÂÐ´fls_ctrl
	printf("0.FLS_CTRL LOCKED\r\n");
	printf("FLS_CONF :0x%x\r\n",EFC->fls_conf);
	EFC->fls_ctrl = 0x5a<<24 | 0xb0;
	printf("WRITE FLS_CTRL:0xb0\r\n");
	printf("READ FLS_CTRL:0x%x\r\n",EFC->fls_ctrl);
	printf("READ FLS_STAT:0x%x\r\n",EFC->fls_stat);
	printf("\r\n");
	
	//Çå³ý×´Ì¬¼Ä´æÆ÷
	temp = EFC->fls_stat;
	EFC->fls_stat = temp;
	//UNLOCK×´Ì¬ÏÂÐ´fls_ctrl
	printf("1.FLS_CTRL UNLOCK\r\n");
	EFC->fls_key0 = FLS_KEY1;
	EFC->fls_key0 = FLS_KEY2;
	EFC->fls_ctrl = 0x5a<<24 | 0xb0;
	printf("WRITE FLS_CTRL:0xb0\r\n");
	printf("READ FLS_CTRL:0x%x\r\n",EFC->fls_ctrl);
	printf("READ FLS_CONF:0x%x\r\n",EFC->fls_conf);
	printf("READ FLS_STAT:0x%x\r\n",EFC->fls_stat);
	printf("\r\n");
	
	//Çå³ý×´Ì¬¼Ä´æÆ÷
	temp = EFC->fls_stat;
	EFC->fls_stat = temp;
	//SW LOCK fls_ctrl
	printf("2.FLS_CTRL SW LOCK\r\n");
	EFC->fls_conf |= ((u32)1<<31);
	EFC->fls_ctrl = 0x5a<<24 | 0xf0;
	printf("WRITE FLS_CTRL:0xf0\r\n");
	printf("READ FLS_CTRL:0x%x\r\n",EFC->fls_ctrl);
	printf("READ FLS_CONF:0x%x\r\n",EFC->fls_conf);
	printf("READ FLS_STAT:0x%x\r\n",EFC->fls_stat);
	printf("\r\n");
	
	//Çå³ý×´Ì¬¼Ä´æÆ÷
	temp = EFC->fls_stat;
	EFC->fls_stat = temp;
	//ÏÈÐ´LOCK2ÔÙÐ´LOCK1
	printf("3.FLS_CTRL UNLOCK ERR CONF\r\n");
	EFC->fls_key0 = FLS_KEY2;
	EFC->fls_key0 = FLS_KEY1;
	EFC->fls_ctrl = 0x5a<<24 | 0xc0;
	printf("WRITE FLS_CTRL:0xc0\r\n");
	printf("READ FLS_CTRL:0x%x\r\n",EFC->fls_ctrl);
	printf("READ FLS_CONF:0x%x\r\n",EFC->fls_conf);
	printf("READ FLS_STAT:0x%x\r\n",EFC->fls_stat);
	printf("\r\n");
	
	i = EFC->fls_stat;
	EFC->fls_stat = i;
	printf("READ FLS_STAT:0x%x\r\n",EFC->fls_stat);
}
