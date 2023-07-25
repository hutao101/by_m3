#include "sys.h"
#include "delay.h"
#include "sample_systick.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_systick_t8
 * Function 往AVL写值会清除寄存器
 * @author  licuihuang
 * @date    2022-6-28
 * @param   void
 * @return  void
*******************************************************************************/
void sample_systick_t8(void)
{
	u32 u;	
	u=SysTick_Config(0xFFFFFF);  //重装载，时钟源AHB（72M），打开中断，使能systick
	printf("0x%x\r\n",SysTick->VAL);
	
//	SysTick->CTRL&=~SysTick_CTRL_TICKINT_Msk;  //关闭中断
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);  //时钟源选择_Div8

	SysTick->VAL=0xFFF;
	printf("0x%x\r\n",SysTick->VAL);
	
	while(u)
	{
		printf("Reload value exceeded\r\n");
	}	
	printf("\r\n");
}

