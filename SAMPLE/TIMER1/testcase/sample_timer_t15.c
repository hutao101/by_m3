#include "sys.h"
#include "delay.h"
#include "sample_timer.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_timer_t15   
 * Function ����ģʽ-��λģʽ
 * @author  licuihuang
 * @date    2022-5-14
 * @param   void
 * @return  void
*******************************************************************************/
void sample_timer_t15(void)
{
//  TIM_GPIO_Init();	
//	TIM3_Int_Init(999,4999,TIM_CounterMode_Up);  //7199/72M=100us  100*1000us=100ms(up)
//    TIM1_Int_Init(149,4999,TIM_CounterMode_Up,0);	  
//	TIM1_PWMOUT_Init(TIM_OCMode_PWM1);
//	TIM3_PWMOUT_Init(TIM_OCMode_PWM1);
//	TIM_SetCompare1(TIM3,500);	
//	TIM_SetCompare1(TIM1,70);	
//	
//	TIM_SelectInputTrigger(TIM1, TIM_TS_ITR2);	           //TIM1��Ϊ��ģʽ��TIM3��Ϊ��ģʽʱӦѡITR2
//	TIM_SelectOutputTrigger(TIM3,TIM_TRGOSource_OC1Ref);   //ѡ��PWM�ο��ź�0C1REF��Ϊ����
//    TIM_SelectSlaveMode(TIM1, TIM_SlaveMode_Reset);      // ѡ���ģʽ: ��λģʽ
//    TIM_SelectMasterSlaveMode(TIM3,TIM_MasterSlaveMode_Enable);  //��������ģʽ

//	TIM_Cmd(TIM1, ENABLE);
//  
//	delay_ms(20);
//	TIM_SetIC1Prescaler(TIM3,TIM_OCPreload_Disable);  //��ر�Ԥ��Ƶ���ܣ���ȻCCR1Ϊ0�������岻�ɹ�
//	TIM_SelectOnePulseMode(TIM3,TIM_OPMode_Single);   //TIM_OPMode_Repetitive TIM_OPMode_Single
//	TIM_Cmd(TIM3, ENABLE);

	
//tim3��������tim2
	TIM2_Int_Init(999,4999,TIM_CounterMode_Up);  //7199/72M=100us  100*1000us=100ms(up)
    TIM1_Int_Init(149,4999,TIM_CounterMode_Up,0);	  
	TIM1_PWMOUT_Init(TIM_OCMode_PWM1);
	TIM2_PWMOUT_Init(TIM_OCMode_PWM1);
	TIM_SetCompare1(TIM2,500);	
	TIM_SetCompare1(TIM1,70);	
	
	TIM_SelectInputTrigger(TIM1, TIM_TS_ITR1);	           //TIM1��Ϊ��ģʽ��TIM2��Ϊ��ģʽʱӦѡITR1
	TIM_SelectOutputTrigger(TIM2,TIM_TRGOSource_OC1Ref);   //ѡ��PWM�ο��ź�0C1REF��Ϊ����
    TIM_SelectSlaveMode(TIM1, TIM_SlaveMode_Reset);        // ѡ���ģʽ: ��λģʽ
    TIM_SelectMasterSlaveMode(TIM2,TIM_MasterSlaveMode_Enable);  //��������ģʽ

	TIM_Cmd(TIM1, ENABLE);
  
	delay_ms(20);
	TIM_SetIC1Prescaler(TIM2,TIM_OCPreload_Disable);  //��ر�Ԥ��Ƶ���ܣ���ȻCCR1Ϊ0�������岻�ɹ�
	TIM_SelectOnePulseMode(TIM2,TIM_OPMode_Single);   //TIM_OPMode_Repetitive TIM_OPMode_Single
	TIM_Cmd(TIM2, ENABLE);
}

