#include "sys.h"
#include "delay.h"
#include "sample_timer.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_timer_t16   
 * Function ����ģʽ-�ſ�ģʽ
 * @author  licuihuang
 * @date    2022-5-14
 * @param   void
 * @return  void
*******************************************************************************/
void sample_timer_t16(void)
{
//  TIM_GPIO_Init();	
//	TIM1_Int_Init(99,49,TIM_CounterMode_Up,2);  
//    TIM3_Int_Init(999,49,TIM_CounterMode_Up);	 //10000/50M=200us  100*200us=20ms(up)
//	TIM1_PWMOUT_Init(TIM_OCMode_PWM1);
//	TIM3_PWMOUT_Init(TIM_OCMode_PWM1);
//	TIM_SetCompare1(TIM1,20);	
//	TIM_SetCompare1(TIM3,550);	
//	
//	TIM_SelectInputTrigger(TIM1, TIM_TS_ITR2);	 
//	TIM_SelectOutputTrigger(TIM3,TIM_TRGOSource_OC1Ref);   //ѡ��PWM�ο��ź�0C1REF��Ϊ����
//    TIM_SelectSlaveMode(TIM1, TIM_SlaveMode_Gated);      //ѡ���ģʽ: �ſ�ģʽ
//    TIM_SelectMasterSlaveMode(TIM3,TIM_MasterSlaveMode_Enable);  //��������ģʽ

//	TIM_Cmd(TIM1, ENABLE);
//	TIM_Cmd(TIM3, ENABLE);
	
	
//tim3��������tim2
	TIM1_Int_Init(99,49,TIM_CounterMode_Up,2);  
    TIM2_Int_Init(999,49,TIM_CounterMode_Up);	
	TIM1_PWMOUT_Init(TIM_OCMode_PWM1);
	TIM2_PWMOUT_Init(TIM_OCMode_PWM1);
	TIM_SetCompare1(TIM1,20);	
	TIM_SetCompare1(TIM2,550);	
	
	TIM_SelectInputTrigger(TIM1, TIM_TS_ITR1);	 
	TIM_SelectOutputTrigger(TIM2,TIM_TRGOSource_OC1Ref);   //ѡ��PWM�ο��ź�0C1REF��Ϊ����
    TIM_SelectSlaveMode(TIM1, TIM_SlaveMode_Gated);        //ѡ���ģʽ: �ſ�ģʽ
    TIM_SelectMasterSlaveMode(TIM2,TIM_MasterSlaveMode_Enable);  //��������ģʽ

	TIM_Cmd(TIM1, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
}

