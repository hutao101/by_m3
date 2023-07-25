#include "sys.h"
#include "delay.h"
#include "sample_systick.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_systick_t6
 * Function 外部参考时钟做时钟源
 * @author  licuihuang
 * @date    2022-6-28
 * @param   void
 * @return  void
*******************************************************************************/
void sample_systick_t6(void)
{
  u32 u;	
  u=SysTick_Config(0xFFFFFF);  //重装载，时钟源AHB（72M），打开中断，使能systick
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);  //时钟源选择_Div8

	while(u)
	{
		printf("Reload value exceeded\r\n");
	}
	
  printf("\r\n");
}

