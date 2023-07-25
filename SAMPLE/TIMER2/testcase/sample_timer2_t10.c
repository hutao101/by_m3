#include "sys.h"
#include "delay.h"
#include "sample_timer2.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_timer2_t10  
 * Function ������ģʽ
 * @author  licuihuang
 * @date    2022-7-4
 * @param   void
 * @return  void
*******************************************************************************/
void sample_timer2_t10(void)
{	
    TIM2_Int_Init(999,36,TIM_CounterMode_Up);  //50/50M=1us  1000*200us=1ms(up) 
	TIM2_PWMOUT_Init(TIM_OCMode_PWM1);
	TIM_SetCompare2(TIM2,300);	//����3��300us������
	TIM_SetIC2Prescaler(TIM2,TIM_OCPreload_Disable);  //��ر�Ԥ��Ƶ���ܣ���ȻCCR1Ϊ0�������岻�ɹ�
	TIM_SelectOnePulseMode(TIM2,TIM_OPMode_Single);  //TIM_OPMode_Repetitive TIM_OPMode_Single

	TIM_Cmd(TIM2, ENABLE);
}

