#include "sys.h"
#include "delay.h"
#include "sample_timer.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_timer_t4
 * Function 输入捕获-单边沿捕获
 * @author  licuihuang
 * @date    2022-5-13
 * @param   void
 * @return  void
*******************************************************************************/
void sample_timer_t4(void)
{
//从tim3引入波形
//	TIM1_NVIC_Init(6);
//	
//  TIM3_Int_Init(199,9999,TIM_CounterMode_Up);  //10000/50M=200us  200*200us=40ms(up) 
//	TIM1_Int_Init(999,9999,TIM_CounterMode_Up,2);  
//	TIM3_PWMOUT_Init(TIM_OCMode_PWM1);
//	TIM1_CaptureIN_Init(TIM_Channel_1,TIM_ICSelection_DirectTI);
//	TIM_ITConfig(TIM1, TIM_IT_CC1, ENABLE);	

//	TIM_SetCompare1(TIM3,100);	
//    TIM_Cmd(TIM1, ENABLE);
//	TIM_Cmd(TIM3, ENABLE);
//	printf("A\r\n");

	
//从tim2引入波形
	TIM1_NVIC_Init(6);
	
	TIM2_Int_Init(199,9999,TIM_CounterMode_Up);  //10000/50M=200us  200*200us=40ms(up) 
	TIM1_Int_Init(999,9999,TIM_CounterMode_Up,2);  
	TIM2_PWMOUT_Init(TIM_OCMode_PWM1);
	TIM1_CaptureIN_Init(TIM_Channel_1,TIM_ICSelection_DirectTI);
	TIM_ITConfig(TIM1, TIM_IT_CC1, ENABLE);	

	TIM_SetCompare1(TIM2,100);	
    TIM_Cmd(TIM1, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
	printf("A\r\n");	
}


