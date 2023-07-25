#include "sys.h"
#include "delay.h"
#include "sample_timer.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_timer_t7
 * Function ����Ƚ�-����ģʽ����ת��
 * @author  licuihuang
 * @date    2022-5-13
 * @param   void
 * @return  void
*******************************************************************************/
void sample_timer_t7(void)
{
	TIM1_Int_Init(99,499,TIM_CounterMode_Up,2);  

	TIM_SetCompare1(TIM1,30);	
	TIM_OC1PolarityConfig(TIM1,TIM_OCPolarity_High);
	TIM_SelectOCxM(TIM1,TIM_Channel_1,TIM_OCMode_Toggle);  //CNT=CCR1ʱ��ת
	TIM_CCxCmd(TIM1,TIM_Channel_1,TIM_CCx_Enable);         //����/�Ƚ����ʹ��
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);                   //�����ʹ�ܣ��߼���ʱ�����У�
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //ccr1Ԥװ��ʹ��(�費�趼��)
    TIM_Cmd(TIM1, ENABLE);
}


