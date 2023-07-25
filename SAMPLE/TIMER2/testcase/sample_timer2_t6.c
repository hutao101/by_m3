#include "sys.h"
#include "delay.h"
#include "sample_timer2.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_time2_t6
 * Function 输出比较-强制输出模式
 * @author  licuihuang
 * @date    2022-7-4
 * @param   void
 * @return  void
*******************************************************************************/
void sample_timer2_t6(void)
{		
    TIM2_Int_Init(99,499,TIM_CounterMode_Up);  //10us*100=1ms	
	TIM_OC2PolarityConfig(TIM2,TIM_OCPolarity_High);  //输出极性设置（0：高电平，1：低电平）
	TIM_CCxCmd(TIM2,TIM_Channel_2,TIM_CCx_Enable);    //捕获/比较输出使能
//	  TIM_CtrlPWMOutputs(TIM2,ENABLE);                  //主输出使能（高级定时器独有）
	
    TIM_ForcedOC2Config(TIM2,TIM_ForcedAction_Active);  //强制输出为有效电平
    delay_ms(3);
    TIM_ForcedOC2Config(TIM2,TIM_ForcedAction_InActive);  //强制输出为无效电平
    TIM_Cmd(TIM2, ENABLE);	
    printf("A\r\n");
}



