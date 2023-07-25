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
void sample_efc_t33(void)
{	
	sample_unlock_ctrl();
	//sample_timing_cfg(TPE_PP/BUS_PRD);
	
	printf("ENTER PD1 Mode:\r\n");
	sample_set_pwrm(eMOD_PD1);
	printf("fls_ctrl:0x%x\r\n",EFC->fls_ctrl);
	printf("fls_stat:0x%x\r\n",EFC->fls_stat);
	printf("fls_trim:0x%x\r\n",EFC->fls_trim);
	printf("\r\n");

//	printf("ENTER PD2 Mode:\r\n");
//	sample_set_pwrm(eMOD_PD2);
//	printf("fls_ctrl:0x%x\r\n",EFC->fls_ctrl);
//	printf("fls_stat:0x%x\r\n",EFC->fls_stat);
//	printf("fls_trim:0x%x\r\n",EFC->fls_trim);
//	printf("\r\n");
//	
//	printf("ENTER RST Mode:\r\n");
//	sample_set_pwrm(eMOD_RST);
//	printf("fls_ctrl:0x%x\r\n",EFC->fls_ctrl);
//	printf("fls_stat:0x%x\r\n",EFC->fls_stat);
//	printf("fls_trim:0x%x\r\n",EFC->fls_trim);
//	printf("\r\n");

	printf("ENTER SLP Mode:\r\n");
	sample_set_pwrm(eMOD_SLP);
	printf("fls_ctrl:0x%x\r\n",EFC->fls_ctrl);
	printf("fls_stat:0x%x\r\n",EFC->fls_stat);
	printf("fls_trim:0x%x\r\n",EFC->fls_trim);
	printf("\r\n");

	printf("ENTER IDLE Mode:\r\n");
	sample_set_pwrm(eMOD_IDLE);
	printf("fls_ctrl:0x%x\r\n",EFC->fls_ctrl);
	printf("fls_stat:0x%x\r\n",EFC->fls_stat);
	printf("fls_trim:0x%x\r\n",EFC->fls_trim);
	printf("\r\n");

	printf("TRIM DATA Reload:\r\n");
	sample_set_opm(eTRM_DATA_RLD,0);
	while(EFC->fls_stat & FLS_BUSY);//等待操作完成
	printf("fls_ctrl:0x%x\r\n",EFC->fls_ctrl);
	printf("fls_stat:0x%x\r\n",EFC->fls_stat);
	printf("fls_trim:0x%x\r\n",EFC->fls_trim);
	printf("\r\n");
	
	printf("ENTER RESET&PD2 Mode:\r\n");
	sample_set_pwrm(eMOD_PD2 | eMOD_RST);
	printf("fls_ctrl:0x%x\r\n",EFC->fls_ctrl);
	printf("fls_stat:0x%x\r\n",EFC->fls_stat);
	printf("fls_trim:0x%x\r\n",EFC->fls_trim);
	printf("\r\n");

	
}
