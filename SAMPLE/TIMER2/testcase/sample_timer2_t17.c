#include "sys.h"
#include "delay.h"
#include "sample_timer2.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_timer2_t17   
 * Function ����ģʽ-��λģʽ
 * @author  licuihuang
 * @date    2022-7-4
 * @param   void
 * @return  void
*******************************************************************************/
void sample_timer2_t17(void)
{
//  TIM_GPIO_Init();	
	TIM1_Int_Init(999,3599,TIM_CounterMode_Up,2);  //7199/72M=100us  100*1000us=100ms(up)
    TIM2_Int_Init(149,3599,TIM_CounterMode_Up);	  
	TIM1_PWMOUT_Init(TIM_OCMode_PWM1);
	TIM2_PWMOUT_Init(TIM_OCMode_PWM1);
	TIM_SetCompare1(TIM1,500);	
	TIM_SetCompare2(TIM2,70);	
	
	TIM_SelectInputTrigger(TIM2, TIM_TS_ITR0);	  //TIM1��Ϊ��ģʽ��TIM3��Ϊ��ģʽʱӦѡITR2
	TIM_SelectOutputTrigger(TIM1,TIM_TRGOSource_OC1Ref);   //ѡ��PWM�ο��ź�0C1REF��Ϊ����
    TIM_SelectSlaveMode(TIM2, TIM_SlaveMode_Reset);  // ѡ���ģʽ: ��λģʽ
    TIM_SelectMasterSlaveMode(TIM1,TIM_MasterSlaveMode_Enable);  //��������ģʽ

	TIM_Cmd(TIM2, ENABLE);
  
	TIM_SetIC1Prescaler(TIM1,TIM_OCPreload_Disable);  //��ر�Ԥ��Ƶ���ܣ���ȻCCR1Ϊ0�������岻�ɹ�
	TIM_SelectOnePulseMode(TIM1,TIM_OPMode_Single);  //TIM_OPMode_Repetitive TIM_OPMode_Single
	TIM_Cmd(TIM1, ENABLE);
}

