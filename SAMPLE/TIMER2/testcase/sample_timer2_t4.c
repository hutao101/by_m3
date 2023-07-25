#include "sys.h"
#include "delay.h"
#include "sample_timer2.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_timer2_t4
 * Function 输入捕获-单边沿捕获
 * @author  licuihuang
 * @date    2022-7-1
 * @param   void
 * @return  void
*******************************************************************************/
void sample_timer2_t4(void)
{
	TIM2_NVIC_Init(16);
	
	TIM1_Int_Init(199,35999,TIM_CounterMode_Up,0);  //10000/50M=200us  200*200us=40ms(up) 
	TIM2_Int_Init(999,35999,TIM_CounterMode_Up);  
	TIM1_PWMOUT_Init(TIM_OCMode_PWM1);
	TIM2_CaptureIN_Init(TIM_Channel_2,TIM_ICSelection_DirectTI);
	TIM_ITConfig(TIM2, TIM_IT_CC2, ENABLE);	//TIM_IT_Update TIM_IT_CC1

	TIM_SetCompare1(TIM1,100);	
	TIM_Cmd(TIM1, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
	printf("A\r\n");
}

