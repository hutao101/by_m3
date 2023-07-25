#include "sys.h"
#include "delay.h"
#include "sample_timer.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_timer_t22
 * Function 输入捕获-PWM1模式
 * @author  licuihuang
 * @date    2022-5-10
 * @param   void
 * @return  void
*******************************************************************************/
void sample_timer_t22(void)
{
	TIM1_NVIC_Init(31);
	TIM1_Int_Init(1999,4999,TIM_CounterMode_Up,0);  //10000/50M=200us  200*200us=40ms(up) 
	TIM_ITConfig(TIM1, TIM_IT_CC1, ENABLE);	
	
	TIM1_PWMOUT_Init(TIM_OCMode_PWM1);
	TIM_SetCounter(TIM1,0);
	TIM_SetCompare1(TIM1,999);
    TIM_Cmd(TIM1, ENABLE);

	printf("\r\n");
}

