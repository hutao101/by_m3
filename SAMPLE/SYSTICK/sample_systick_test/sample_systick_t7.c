#include "sys.h"
#include "delay.h"
#include "sample_systick.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_systick_t7
 * Function 定时器开启后重装载有效
 * @author  licuihuang
 * @date    2022-6-28
 * @param   void
 * @return  void
*******************************************************************************/
void sample_systick_t7(void)
{
	u32 u;	
	u=SysTick_Config(0xFFFFFF);  //重装载，时钟源AHB（72M），打开中断，使能systick
	printf("0x%x\r\n",SysTick->LOAD);
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);  //时钟源选择_Div8

	SysTick->LOAD=0xFFFFF;
	printf("0x%x\r\n",SysTick->LOAD);

	while(u)
	{
		printf("Reload value exceeded\r\n");
	}				
	printf("\r\n");
}

