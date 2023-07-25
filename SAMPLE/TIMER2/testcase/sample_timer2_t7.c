#include "sys.h"
#include "delay.h"
#include "sample_timer2.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_timer2_t7
 * Function 输出比较-正常模式（翻转）
 * @author  licuihuang
 * @date    2022-5-13
 * @param   void
 * @return  void
*******************************************************************************/
void sample_timer2_t7(void)
{
	TIM2_Int_Init(99,359,TIM_CounterMode_Up);  //10us*100=1ms
	TIM_SetCompare2(TIM2,30);	
	TIM_OC2PolarityConfig(TIM2,TIM_OCPolarity_High);
	TIM_SelectOCxM(TIM2,TIM_Channel_2,TIM_OCMode_Inactive);  //CNT=CCR1时翻转 TIM_OCMode_Active TIM_OCMode_Inactive TIM_OCMode_Toggle
	TIM_CCxCmd(TIM2,TIM_Channel_2,TIM_CCx_Enable);  //捕获/比较输出使能
	
//	TIM_CtrlPWMOutputs(TIM1,ENABLE);    //主输出使能（高级定时器独有）
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);  //ccr1预装载使能(设不设都行)
    TIM_Cmd(TIM2, ENABLE);
}


