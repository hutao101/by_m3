#include "sys.h"
#include "delay.h"
#include "sample_timer.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_timer_t19   
 * Function �жϣ�TIM_IT_Break���жϺ�37��
 * @author  licuihuang
 * @date    2022-5-19
 * @param   void
 * @return  void
*******************************************************************************/
void sample_timer_t19(void)
{	
	TIM1_Int_Init(999,499,TIM_CounterMode_Up,0);  //100*(500/50M)=1ms
	TIM1_BREAK_Init(TIM_OCMode_PWM1,TIM_BreakPolarity_Low,100);  //break_in���ŵ�ƽ���1��0��ֱ�ӽ�GND�ǲ��ܽ��жϵ�
	TIM_ITConfig(TIM1, TIM_IT_Break, ENABLE);
	TIM1_NVIC_Init(37);
	TIM_SetCompare1(TIM1,400);	
	
    TIM_Cmd(TIM1, ENABLE);
//	printf("SR:0x%x\r\n",read_ahb32(0x40011c10));
}

