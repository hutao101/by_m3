#include "sys.h"
#include "delay.h"
#include "sample_timer.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_timer_t11   
 * Function ���OCxREF�źţ�TIM1_CH1�����߼�������ͨ��0��TIM1_ETR����VCC��Ĭ�ϣ�ʱ�������㣬����GND�����������
 * @author  licuihaung
 * @date    2022-5-11
 * @param   void
 * @return  void
*******************************************************************************/
void sample_timer_t11(void)
{
	TIM1_Int_Init(99,359,TIM_CounterMode_Up,0);  //100*(500/50M)=1ms
	TIM1_PWMOUT_Init(TIM_OCMode_PWM1);
	
	TIM_ClearOC1Ref(TIM1,TIM_OCClear_Enable);
	TIM_SetCompare1(TIM1,40);	
    TIM_Cmd(TIM1, ENABLE);

	printf("A\r\n");
}



