#include "sys.h"
#include "delay.h"
#include "sample_timer2.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_timer2_t22
 * Function 中断-CC1OF/CC2OF/CC3OF/CC4OF(16)
 * @author  licuihuang
 * @date    2022-7-4
 * @param   void
 * @return  void
*******************************************************************************/
void sample_timer2_t22(void)
{
	TIM2_NVIC_Init(16);
	TIM_ITConfig(TIM2, TIM_IT_CC1OF|TIM_IT_CC2OF|TIM_IT_CC3OF|TIM_IT_CC4OF, ENABLE);//|TIM_IT_CC2OF|TIM_IT_CC3OF|TIM_IT_CC4OF
	
	TIM1_Int_Init(999,3599,TIM_CounterMode_Up,0);  //100ms
	TIM2_Int_Init(9999,3599,TIM_CounterMode_Up);   //1s

	TIM2_CaptureIN_Init(TIM_Channel_1,TIM_ICSelection_DirectTI);  //输入捕获通道选择
	TIM2_CaptureIN_Init(TIM_Channel_2,TIM_ICSelection_DirectTI);  //输入捕获通道选择
	TIM2_CaptureIN_Init(TIM_Channel_3,TIM_ICSelection_DirectTI);  //输入捕获通道选择
	TIM2_CaptureIN_Init(TIM_Channel_4,TIM_ICSelection_DirectTI);  //输入捕获通道选择
	TIM1_PWMOUT_Init(TIM_OCMode_PWM1); 
	TIM_SetCompare1(TIM1,400);	

	TIM_Cmd(TIM1, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
    
//	while(1)
//	{
//		while(!TIM_GetFlagStatus(TIM2,TIM_FLAG_CC1));	
//		while(!TIM_GetFlagStatus(TIM2,TIM_FLAG_CC1OF));	
//		printf("sr:0x%x\r\n",read_ahb32(0x40018010));
//		TIM_ClearFlag(TIM2,TIM_FLAG_CC1);	
//		TIM_ClearFlag(TIM2,TIM_FLAG_CC1OF);
//	}

//	delay_ms(200);
//	printf("sr:0x%x dier:0x%x\r\n",read_ahb32(0x40018010),read_ahb32(0x4001800c));
}	

