#include "sys.h"
#include "delay.h"
#include "sample_timer2.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_timer_t5
 * Function 输入捕获-PWM1模式
 * @author  licuihuang
 * @date    2022-7-1
 * @param   void
 * @return  void
*******************************************************************************/
void sample_timer2_t5(void)
{
	TIM2_NVIC_Init(16);	
    TIM1_Int_Init(99,9999,TIM_CounterMode_Up,0);  //10000/50M=200us  100*200us=20ms(up) 
	TIM2_Int_Init(999,9999,TIM_CounterMode_Up);
	TIM1_PWMOUT_Init(TIM_OCMode_PWM1);
	TIM2_PWMIN_Init(TIM_Channel_2,TIM_ICSelection_DirectTI);
	TIM_ITConfig(TIM2, TIM_IT_CC2OF, ENABLE);
	
	TIM_SetCompare1(TIM1,40);	
    TIM_Cmd(TIM1, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
	
	printf("A\r\n");
}

