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

void sample_efc_t28(void)
{	
	u32 i,j,temp;
	
	printf("0x10020100:0x%x\r\n",read_ahb32(0x10020100));
	//使能 EFC 错误中断
	sample_efc_it_cfg(ERR_INTEN);
	sample_efc_nvic_cfg();

#if 1
	sample_unlock_ctrl();
#endif
	
#if 1
	sample_unlock_opt();
#else	
	sample_page_pgm(513,0x12345678);	
#endif	
	
	sample_timing_cfg(TPE_PP/BUS_PRD);
	
	sample_trm_rld();
	
#if 0
	//PWR_MODE_ERR
	printf("ENTER RESET&PD2 Mode:\r\n");
	sample_set_pwrm(eMOD_PD2 | eMOD_RST);
	printf("fls_ctrl:0x%x\r\n",EFC->fls_ctrl);
	printf("fls_stat:0x%x\r\n",EFC->fls_stat);
	printf("fls_trim:0x%x\r\n",EFC->fls_trim);
	printf("\r\n");
#endif	

#if 0
	//OP_MODE_ERR
	while((EFC->fls_stat & FLS_BUSY) != 0);//等待BUSY释放
	sample_set_opm(ePAGE_PGM,0);
	printf("PGM FLS_STAT:0x%x\r\n",EFC->fls_stat);
	while((EFC->fls_stat & CMD_DONE) == 0);//等待CMD_DONE置位	
#endif	

#if 1
	//ARR_ERR
	sample_page_er(512);
	
	printf("FLS_STAT:0x%x\r\n",EFC->fls_stat);
	
	//清除中断
	temp = EFC->fls_stat;
	EFC->fls_stat =temp;
	printf("FLS_STAT:0x%x\r\n",EFC->fls_stat);
	
	//清除中断
	temp = EFC->fls_stat;
	EFC->fls_stat =temp;
	printf("FLS_STAT:0x%x\r\n",EFC->fls_stat);
	
	//清除中断
	temp = EFC->fls_stat;
	EFC->fls_stat =temp;
	printf("FLS_STAT:0x%x\r\n",EFC->fls_stat);
	
	//清除中断
	temp = EFC->fls_stat;
	EFC->fls_stat =temp;
	printf("FLS_STAT:0x%x\r\n",EFC->fls_stat);
	
	printf("FLS_STAT:0x%x\r\n",EFC->fls_stat);
	printf("FLS_STAT:0x%x\r\n",EFC->fls_stat);
	printf("FLS_STAT:0x%x\r\n",EFC->fls_stat);
	printf("FLS_STAT:0x%x\r\n",EFC->fls_stat);
#endif	
	
}
