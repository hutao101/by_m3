#include "sys.h"
#include "delay.h"
#include "sample_systick.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_systick_t2
 * Function 24位向下计数器
 * @author  licuihuang
 * @date    2022-6-28
 * @param   void
 * @return  void
*******************************************************************************/
void sample_systick_t2(void)
{
    u32 i,u,reload;	
	for(i=0;i<2;i++)
	{
		switch(i)
		{
			case 0:
				printf("Reload value is 0xFFFFFF:\r\n");
				reload=0xFFFFFF;
				break;
			case 1:
				printf("Reload value is 0x1000000:\r\n");
				reload=0x1000000;
				break;
			default:
				break;
		}
		u=SysTick_Config(reload);  //(0x1000000);  //重装载，时钟源AHB（72M），打开中断，使能systick
		SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);  //时钟源选择_Div8

		if(u)
		{
			printf("Reload value exceeded\r\n");
			printf("reload:0x%x value:0x%x\r\n",SysTick->LOAD,SysTick->VAL);
		}
		else
		{
			while(!(SysTick->CTRL&SysTick_CTRL_COUNTFLAG_Msk))   //第一次向下计数到0(进中断)前打印计数值
			{
				printf("0x%x ",SysTick->VAL);
			}
		}
				
		delay_ms(2000);
	    systick_close();
	    printf("\r\n");
	}
}	

