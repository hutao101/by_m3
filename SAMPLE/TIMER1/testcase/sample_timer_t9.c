#include "sys.h"
#include "delay.h"
#include "sample_timer.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_timer_t9   
 * Function 死区可编程的互补输出
 * @author  licuihuang
 * @date    2022-5-13
 * @param   void
 * @return  void
*******************************************************************************/
void sample_timer_t9(void)
{
	TIM1_Int_Init(99,359,TIM_CounterMode_Up,0);  //100*(360/36M)=1ms
	TIM1_COMPLEOUT_Init(TIM_OCMode_PWM1,120);  //120*(1/36M)=2us  120=0x77
	
	TIM_SetCompare1(TIM1,40);	
    TIM_Cmd(TIM1, ENABLE);
	printf("A\r\n");
}


