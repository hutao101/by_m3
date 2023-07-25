#include "sys.h"
#include "delay.h"
#include "sample_iwdg.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_iwdg_t1
 * Function KR=0xCCCC �������Ź�
 * @author  licuihuang
 * @date    2022-6-15
 * @param   void
 * @return  void
*******************************************************************************/
void sample_iwdg_t1(void)
{	
	printf("IWDG RESET!!\r\n");
	
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Disable); //��0�������Ź�
	printf("KR is written to 0:  ");	
	delay_ms(5000);	
	printf("iwdg is no reset\r\n");
	
	IWDG_Enable(); //��0xcccc�������Ź�		
	printf("KR is written to 0xCCCC:  ");	
	delay_ms(5000);
	printf("iwdg is no reset\r\n");

	printf("\r\n"); 
}


