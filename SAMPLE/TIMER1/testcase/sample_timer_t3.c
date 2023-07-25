#include "sys.h"
#include "delay.h"
#include "sample_timer.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_timer_t3
 * Function 计数模式-中心对齐
 * @author  licuihuang
 * @date    2022-5-13
 * @param   void
 * @return  void
*******************************************************************************/
void sample_timer_t3(void)
{
    u32 i;  
	TIM1_NVIC_Init(30);
	TIM1_Int_Init(199,9999,TIM_CounterMode_CenterAligned1,2);  //10000/50M=200us  200*200us=40ms(up) 
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);	
    TIM_Cmd(TIM1, ENABLE); 
	
	printf("ARR:%d  PSC:%d  RCR:%d\r\n",TIM1->ARR,TIM1->PSC,TIM1->RCR);
	for(i=0;i<=500;i++)
	{
		printf("%d  ",TIM_GetCounter(TIM1));
    }
}

