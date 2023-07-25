#include "sys.h"
#include "delay.h"
#include "sample_timer2.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_timer2_t3
 * Function 计数模式-中心对齐
 * @author  licuihuang
 * @date    2022-7-1
 * @param   void
 * @return  void
*******************************************************************************/
void sample_timer2_t3(void)
{
    u32 i;  
	TIM2_NVIC_Init(16);
	TIM2_Int_Init(199,9999,TIM_CounterMode_CenterAligned1);  //10000/50M=200us  200*200us=40ms(up) 
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);	
    TIM_Cmd(TIM2, ENABLE); 
	
	printf("ARR:%d  PSC:%d\r\n",TIM2->ARR,TIM2->PSC);
	for(i=0;i<=500;i++)
	{
		printf("%d  ",TIM_GetCounter(TIM2));
    }
}

