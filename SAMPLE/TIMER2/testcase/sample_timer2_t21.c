#include "sys.h"
#include "delay.h"
#include "sample_timer2.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_timer2_t21   
 * Function 中断：TIM_IT_Trigger（中断号16）
 * @author  licuihuang
 * @date    2022-7-4
 * @param   void
 * @return  void
*******************************************************************************/
void sample_timer2_t21(void)
{
//  TIM_GPIO_Init();	
	TIM2_NVIC_Init(16);
	TIM2_Int_Init(99,35,TIM_CounterMode_Up);  
    TIM1_Int_Init(999,35,TIM_CounterMode_Up,2);	 //10000/50M=200us  100*200us=20ms(up)
	TIM1_PWMOUT_Init(TIM_OCMode_PWM1);
	TIM2_PWMOUT_Init(TIM_OCMode_PWM1);
	TIM_SetCompare2(TIM2,20);	
	TIM_SetCompare1(TIM1,550);	
	
	TIM_SelectInputTrigger(TIM2, TIM_TS_ITR0);	 
	TIM_SelectOutputTrigger(TIM1,TIM_TRGOSource_OC1Ref);   //选择PWM参考信号0C1REF作为触发
    TIM_SelectSlaveMode(TIM2, TIM_SlaveMode_Trigger);  // 选择从模式: 触发模式
    TIM_SelectMasterSlaveMode(TIM1,TIM_MasterSlaveMode_Enable);  //开启主从模式

//	TIM_Cmd(TIM2, ENABLE);  //不用使能TIM2
	TIM_SetIC1Prescaler(TIM1,TIM_OCPreload_Disable);  //需关闭预分频功能，不然CCR1为0，单脉冲不成功
	TIM_SelectOnePulseMode(TIM1,TIM_OPMode_Single);  //TIM_OPMode_Repetitive TIM_OPMode_Single
	TIM_ITConfig(TIM2, TIM_IT_Trigger, ENABLE); //触发中断/COM中断通过TRGI上升沿产生

	TIM_Cmd(TIM1, ENABLE);
}

