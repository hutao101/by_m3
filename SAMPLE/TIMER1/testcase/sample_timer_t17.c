#include "sys.h"
#include "delay.h"
#include "sample_timer.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_timer_t17   
 * Function 主从模式-触发模式
 * @author  licuihuang
 * @date    2022-5-14
 * @param   void
 * @return  void
*******************************************************************************/
void sample_timer_t17(void)
{
//  TIM_GPIO_Init();	
//	TIM1_Int_Init(99,49,TIM_CounterMode_Up,2);  
//    TIM3_Int_Init(999,49,TIM_CounterMode_Up);	 //10000/50M=200us  100*200us=20ms(up)
//	TIM1_PWMOUT_Init(TIM_OCMode_PWM1);
//	TIM3_PWMOUT_Init(TIM_OCMode_PWM1);  //把此句屏蔽则TIM1波形不能正常输出
//	TIM_SetCompare1(TIM1,20);	
//	TIM_SetCompare1(TIM3,550);	
//	
//	TIM_SelectInputTrigger(TIM1, TIM_TS_ITR2);	 
//	TIM_SelectOutputTrigger(TIM3,TIM_TRGOSource_OC1Ref);   //选择PWM参考信号0C1REF作为触发
//    TIM_SelectSlaveMode(TIM1, TIM_SlaveMode_Trigger);    //选择从模式: 触发模式
//    TIM_SelectMasterSlaveMode(TIM3,TIM_MasterSlaveMode_Enable);  //开启主从模式

////	TIM_Cmd(TIM1, ENABLE);  //不用使能TIM1
//	TIM_SetIC1Prescaler(TIM3,TIM_OCPreload_Disable);  //需关闭预分频功能，不然CCR1为0，单脉冲不成功
//	TIM_SelectOnePulseMode(TIM3,TIM_OPMode_Single);  //TIM_OPMode_Repetitive TIM_OPMode_Single
//	TIM_Cmd(TIM3, ENABLE);
	
	
//tim3被禁，用tim2
	TIM1_Int_Init(99,49,TIM_CounterMode_Up,2);  
    TIM2_Int_Init(999,49,TIM_CounterMode_Up);	 //10000/50M=200us  100*200us=20ms(up)
	TIM1_PWMOUT_Init(TIM_OCMode_PWM1);
	TIM2_PWMOUT_Init(TIM_OCMode_PWM1);  //把此句屏蔽则TIM1波形不能正常输出
	TIM_SetCompare1(TIM1,20);	
	TIM_SetCompare1(TIM2,550);	
	
	TIM_SelectInputTrigger(TIM1, TIM_TS_ITR1);	 
	TIM_SelectOutputTrigger(TIM2,TIM_TRGOSource_OC1Ref);   //选择PWM参考信号0C1REF作为触发
    TIM_SelectSlaveMode(TIM1, TIM_SlaveMode_Trigger);      //选择从模式: 触发模式
    TIM_SelectMasterSlaveMode(TIM2,TIM_MasterSlaveMode_Enable);  //开启主从模式

//	TIM_Cmd(TIM1, ENABLE);  //不用使能TIM1
	TIM_SetIC1Prescaler(TIM2,TIM_OCPreload_Disable);  //需关闭预分频功能，不然CCR1为0，单脉冲不成功
	TIM_SelectOnePulseMode(TIM2,TIM_OPMode_Single);   //TIM_OPMode_Repetitive TIM_OPMode_Single
	TIM_Cmd(TIM2, ENABLE);
}

