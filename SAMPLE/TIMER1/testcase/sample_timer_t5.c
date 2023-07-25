#include "sys.h"
#include "delay.h"
#include "sample_timer.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_timer_t5
 * Function 输入捕获-PWM1模式
 * @author  licuihuang
 * @date    2022-5-10
 * @param   void
 * @return  void
*******************************************************************************/
void sample_timer_t5(void)
{
//从tim3引入波形
//	TIM1_NVIC_Init(31);
//	
//    TIM3_Int_Init(99,9999,TIM_CounterMode_Up);  //10000/50M=200us  100*200us=20ms(up) 
//	TIM1_Int_Init(999,9999,TIM_CounterMode_Up,2);
//	TIM3_PWMOUT_Init(TIM_OCMode_PWM1);
//	TIM1_PWMIN_Init(TIM_OCMode_PWM1,TIM_ICSelection_DirectTI);
//	TIM_ITConfig(TIM1, TIM_IT_CC1, ENABLE);
//	
//	TIM_SetCompare1(TIM3,40);	
//    TIM_Cmd(TIM1, ENABLE);
//	TIM_Cmd(TIM3, ENABLE);
//	printf("A\r\n");

	
//从tim2引入波形	
	TIM1_NVIC_Init(31);
	
    TIM2_Int_Init(99,9999,TIM_CounterMode_Up);  //10000/50M=200us  100*200us=20ms(up) 
	TIM1_Int_Init(999,9999,TIM_CounterMode_Up,2);
	TIM2_PWMOUT_Init(TIM_OCMode_PWM1);
	TIM1_PWMIN_Init(TIM_OCMode_PWM1,TIM_ICSelection_DirectTI);
	TIM_ITConfig(TIM1, TIM_IT_CC1, ENABLE);
	
	TIM_SetCompare1(TIM2,40);	
    TIM_Cmd(TIM1, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
	printf("A\r\n");
}

