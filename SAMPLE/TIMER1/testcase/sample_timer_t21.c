#include "sys.h"
#include "delay.h"
#include "sample_timer2.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_timer_t21
 * Function 中断-CC1OF/CC2OF/CC3OF/CC4OF(38/39/40/41)
 * @author  licuihuang
 * @date    2022-5-10
 * @param   void
 * @return  void
*******************************************************************************/
void sample_timer_t21(void)
{
	TIM1_NVIC_Init(41);
	TIM_ITConfig(TIM1, TIM_IT_CC1OF|TIM_IT_CC2OF|TIM_IT_CC3OF|TIM_IT_CC4OF, ENABLE);//|TIM_IT_CC2OF|TIM_IT_CC3OF|TIM_IT_CC4OF
	
	TIM1_Int_Init(9999,3599,TIM_CounterMode_Up,0);  //100ms
	TIM2_Int_Init(999,3599,TIM_CounterMode_Up);     //1s

	TIM1_CaptureIN_Init(TIM_Channel_1,TIM_ICSelection_DirectTI);  //输入捕获通道选择
	TIM1_CaptureIN_Init(TIM_Channel_2,TIM_ICSelection_DirectTI);  //输入捕获通道选择
	TIM1_CaptureIN_Init(TIM_Channel_3,TIM_ICSelection_DirectTI);  //输入捕获通道选择
	TIM1_CaptureIN_Init(TIM_Channel_4,TIM_ICSelection_DirectTI);  //输入捕获通道选择
	TIM2_PWMOUT_Init(TIM_OCMode_PWM1); 
	TIM_SetCompare1(TIM2,400);	

	TIM_Cmd(TIM1, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
}

