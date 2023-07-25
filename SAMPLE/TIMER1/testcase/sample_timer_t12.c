#include "sys.h"
#include "delay.h"
#include "sample_timer.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_timer_t12  
 * Function 单脉冲模式
 * @author  licuihaung
 * @date    2022-5-13
 * @param   void
 * @return  void
*******************************************************************************/
void sample_timer_t12(void)
{	
    TIM1_Int_Init(999,35,TIM_CounterMode_Up,2);  //50/50M=1us  1000*200us=1ms(up) 
	TIM1_PWMOUT_Init(TIM_OCMode_PWM1);
	TIM_SetCompare1(TIM1,300);	//起来3个300us的脉冲
	TIM_SetIC1Prescaler(TIM1,TIM_OCPreload_Disable);  //需关闭预分频功能，不然CCR1为0，单脉冲不成功
	TIM_SelectOnePulseMode(TIM1,TIM_OPMode_Single);  //TIM_OPMode_Repetitive TIM_OPMode_Single

	TIM_Cmd(TIM1, ENABLE);
}



