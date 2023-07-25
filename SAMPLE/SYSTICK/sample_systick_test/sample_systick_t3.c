#include "sys.h"
#include "delay.h"
#include "sample_systick.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_systick_t3
 * Function COUNTFLAG��־����
 * @author  licuihuang
 * @date    2022-6-28
 * @param   void
 * @return  void
*******************************************************************************/
void sample_systick_t3(void)
{
    u32 i,u;	
    u=SysTick_Config(0xFFFFFF);  //��װ�أ�ʱ��ԴAHB��72M�������жϣ�ʹ��systick
	SysTick->CTRL&=~SysTick_CTRL_TICKINT_Msk;  //�ر��ж�
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);  //ʱ��Դѡ��_Div8

	while(u)
	{
		printf("Reload value exceeded\r\n");
	}
	
	while(!(SysTick->CTRL&SysTick_CTRL_COUNTFLAG_Msk));
	printf("COUNTFLAG is 1\r\n");
	
	SysTick->CTRL;
	if(!(SysTick->CTRL&SysTick_CTRL_COUNTFLAG_Msk))
	{
		printf("COUNTFLAG can auto clear 0 by read it\r\n");
	}
	else
	{
		printf("COUNTFLAG can not auto clear 0 by read it\r\n");
	}
	
  printf("\r\n");
}		

