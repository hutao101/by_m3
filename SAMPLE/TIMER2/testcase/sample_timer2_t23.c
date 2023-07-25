#include "sys.h"
#include "delay.h"
#include "sample_timer2.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_timer_t23
 * Function 输入捕获滤波
 * @author  licuihuang
 * @date    2022-5-10
 * @param   void
 * @return  void
*******************************************************************************/
void sample_timer2_t23(void)
{
	u16 arr=2400;
	TIM2_NVIC_Init(16);
	
    TIM1_Int_Init(arr,0,TIM_CounterMode_Up,0);  //10000/50M=200us  100*200us=20ms(up) 
	TIM2_Int_Init(arr*18,0,TIM_CounterMode_Up);  //更改fDTS
	TIM1_PWMOUT_Init(TIM_OCMode_PWM1);
	TIM2_CaptureIN_Init(TIM_Channel_2,TIM_ICSelection_DirectTI);  //更改滤波值
	TIM_ITConfig(TIM2, TIM_IT_CC2, ENABLE);
	
	TIM_SetCompare1(TIM1,arr/2);	
    TIM_Cmd(TIM1, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
}

