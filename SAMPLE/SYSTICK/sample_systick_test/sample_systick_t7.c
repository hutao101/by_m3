#include "sys.h"
#include "delay.h"
#include "sample_systick.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_systick_t7
 * Function ��ʱ����������װ����Ч
 * @author  licuihuang
 * @date    2022-6-28
 * @param   void
 * @return  void
*******************************************************************************/
void sample_systick_t7(void)
{
	u32 u;	
	u=SysTick_Config(0xFFFFFF);  //��װ�أ�ʱ��ԴAHB��72M�������жϣ�ʹ��systick
	printf("0x%x\r\n",SysTick->LOAD);
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);  //ʱ��Դѡ��_Div8

	SysTick->LOAD=0xFFFFF;
	printf("0x%x\r\n",SysTick->LOAD);

	while(u)
	{
		printf("Reload value exceeded\r\n");
	}				
	printf("\r\n");
}

