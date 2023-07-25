#include "sys.h"
#include "delay.h"
#include "sample_timer2.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_timer2_t9   
 * Function ���OCxREF�źţ�TIM2_CH2�����߼�������ͨ��0��TIM2_ETR����VCC��Ĭ�ϣ�ʱ�������㣬����GND�����������
 * @author  licuihuang
 * @date    2022-7-4
 * @param   void
 * @return  void
*******************************************************************************/
void sample_timer2_t9(void)
{
	TIM2_Int_Init(99,359,TIM_CounterMode_Up);  //100*(500/50M)=1ms
	TIM2_PWMOUT_Init(TIM_OCMode_PWM1);
	TIM_SetCompare2(TIM2,40);	
    TIM_Cmd(TIM2, ENABLE);

	delay_ms(1000);
	TIM_ClearOC2Ref(TIM2,TIM_OCClear_Enable);
	printf("A\r\n");
}

