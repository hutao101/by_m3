#include "sys.h"
#include "delay.h"
#include "sample_iwdg.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_iwdg_t3
 * Function KR=0xAAAA重装载计数器，PR值只要写访问写入就生效
 * @author  licuihuang
 * @date    2022-6-15
 * @param   void
 * @return  void
*******************************************************************************/
void sample_iwdg_t3(void)
{
	printf("iwdg reset!!");
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	sample_iwdg_init(IWDG_Prescaler_64,0xFFF);  //3.3S
	
	IWDG_Enable(); //启动看门狗
	IWDG_ReloadCounter();
	printf("\r\n");
}


