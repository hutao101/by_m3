#include "sys.h"
#include "delay.h"
#include "sample_systick.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_systick_t1
 * Function 校准
 * @author  licuihuang
 * @date    2022-6-28
 * @param   void
 * @return  void
*******************************************************************************/
void sample_systick_t1(void)
{
	u32 u;		
	u=SysTick_Config(45000);  //重装载，时钟源AHB（72M），打开中断，使能systick
	SysTick->CTRL&=~SysTick_CTRL_TICKINT_Msk;  //关闭中断
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);  //时钟源选择_Div8

	while(u)
	{
		printf("Reload value exceeded\r\n");
	}
  
	while(!(SysTick->CTRL&SysTick_CTRL_COUNTFLAG_Msk));
	printf("calib:0x%x\r\n",SysTick->CALIB);	
	printf("\r\n");
}


