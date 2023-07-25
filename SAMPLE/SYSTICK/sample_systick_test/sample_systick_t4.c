#include "sys.h"
#include "delay.h"
#include "sample_systick.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_systick_t4
 * Function COUNTFLAG��־
 * @author  licuihuang
 * @date    2022-6-28
 * @param   void
 * @return  void
*******************************************************************************/
extern u32 systick_t4;
void sample_systick_t4(void)
{
    u32 u;	
	systick_t4 = 1;
    u=SysTick_Config(0xFFFFFF);  //��װ�أ�ʱ��ԴAHB��72M�������жϣ�ʹ��systick
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);  //ʱ��Դѡ��_Div8

	while(u)
	{
		printf("Reload value exceeded\r\n");
	}
	
    printf("\r\n");
}


