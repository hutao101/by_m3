#include "sys.h"
#include "delay.h"
#include "sample_timer2.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_timer2_t20   
 * Function ÖÐ¶Ï£ºTIM_IT_Update/TIM_IT_CC1/CC2/CC3/CC4£¨ÖÐ¶ÏºÅ16£©
 * @author  licuihuang
 * @date    2022-7-4
 * @param   void
 * @return  void
*******************************************************************************/
void sample_timer2_t20(void)
{
    u32 i;	
	TIM2_NVIC_Init(16);
	TIM2_Int_Init(999,4999,TIM_CounterMode_Up);  //1000*(5000/50M)=100ms
	TIM2_PWMOUT_Init(TIM_OCMode_PWM1);
	TIM_ITConfig(TIM2, TIM_IT_Update|TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4, ENABLE);
	
	TIM_SetCompare1(TIM2,400);
	TIM_SetCompare2(TIM2,500);
	TIM_SetCompare3(TIM2,600);
	TIM_SetCompare4(TIM2,700);
	
    TIM_Cmd(TIM2, ENABLE);
}

