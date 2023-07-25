#include "sys.h"
#include "delay.h"
#include "sample_timer2.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_timer2_t10  
 * Function 单脉冲模式
 * @author  licuihuang
 * @date    2022-7-4
 * @param   void
 * @return  void
*******************************************************************************/
void sample_timer2_t10(void)
{	
    TIM2_Int_Init(999,36,TIM_CounterMode_Up);  //50/50M=1us  1000*200us=1ms(up) 
	TIM2_PWMOUT_Init(TIM_OCMode_PWM1);
	TIM_SetCompare2(TIM2,300);	//起来3个300us的脉冲
	TIM_SetIC2Prescaler(TIM2,TIM_OCPreload_Disable);  //需关闭预分频功能，不然CCR1为0，单脉冲不成功
	TIM_SelectOnePulseMode(TIM2,TIM_OPMode_Single);  //TIM_OPMode_Repetitive TIM_OPMode_Single

	TIM_Cmd(TIM2, ENABLE);
}

