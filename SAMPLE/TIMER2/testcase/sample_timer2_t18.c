#include "sys.h"
#include "delay.h"
#include "sample_timer2.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_timer2_t18   
 * Function 主从模式-门控模式
 * @author  licuihuang
 * @date    2022-7-4
 * @param   void
 * @return  void
*******************************************************************************/
void sample_timer2_t18(void)
{
//  TIM_GPIO_Init();	
 	TIM2_Int_Init(99,36,TIM_CounterMode_Up);  
    TIM1_Int_Init(999,36,TIM_CounterMode_Up,2);	 //10000/50M=200us  100*200us=20ms(up)
	TIM1_PWMOUT_Init(TIM_OCMode_PWM1);
	TIM2_PWMOUT_Init(TIM_OCMode_PWM1);
	TIM_SetCompare2(TIM2,20);	
	TIM_SetCompare1(TIM1,550);	
	
	TIM_SelectInputTrigger(TIM2, TIM_TS_ITR0);	 
	TIM_SelectOutputTrigger(TIM1,TIM_TRGOSource_OC1Ref);   //选择PWM参考信号0C1REF作为触发
    TIM_SelectSlaveMode(TIM2, TIM_SlaveMode_Gated);  // 选择从模式: 门控模式
    TIM_SelectMasterSlaveMode(TIM1,TIM_MasterSlaveMode_Enable);  //开启主从模式

	TIM_Cmd(TIM1, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
}

