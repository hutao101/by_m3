#include "sys.h"
#include "delay.h"
#include "sample_timer.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_timer_t25
 * Function 计数模式-向上计数
 * @author  licuihuang
 * @date    2022-5-10
 * @param   void
 * @return  void
*******************************************************************************/
void sample_timer_t25(void)
{
	TIM1_Int_Init(1,0,TIM_CounterMode_Up,0);  
	TIM1_PWMOUT_Init(TIM_OCMode_PWM1);
	TIM_SetCompare1(TIM1,1);
//	TIM_SelectOnePulseMode(TIM1,TIM_OPMode_Single);
    TIM_Cmd(TIM1, ENABLE); 
	
	printf("ARR:%d  PSC:%d  RCR:%d\r\n",TIM1->ARR,TIM1->PSC,TIM1->RCR);
	printf("\r\n");
}

