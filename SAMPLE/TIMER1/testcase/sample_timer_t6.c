#include "sys.h"
#include "delay.h"
#include "sample_timer.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_timer_t6
 * Function ����Ƚ�-ǿ�����ģʽ
 * @author  licuihuang
 * @date    2022-5-13
 * @param   void
 * @return  void
*******************************************************************************/
void sample_timer_t6(void)
{		
    TIM1_Int_Init(99,499,TIM_CounterMode_Up,2);  //10us*100=1ms
	
	TIM_OC1PolarityConfig(TIM1,TIM_OCPolarity_High);  //����������ã�0���ߵ�ƽ��1���͵�ƽ��
	TIM_CCxCmd(TIM1,TIM_Channel_1,TIM_CCx_Enable);    //����/�Ƚ����ʹ��
	TIM_CtrlPWMOutputs(TIM1,ENABLE);                  //�����ʹ�ܣ��߼���ʱ�����У�
	
    TIM_ForcedOC1Config(TIM1,TIM_ForcedAction_Active);  //ǿ�����Ϊ��Ч��ƽ
    delay_ms(3);
    TIM_ForcedOC1Config(TIM1,TIM_ForcedAction_InActive);  //ǿ�����Ϊ��Ч��ƽ
    TIM_Cmd(TIM1, ENABLE);	
    printf("A\r\n");
}

