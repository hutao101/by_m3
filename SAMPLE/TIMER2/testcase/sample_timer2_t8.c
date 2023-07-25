#include "sys.h"
#include "delay.h"
#include "sample_timer2.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_timer2_t8   
 * Function 输出比较-PWM1模式
 * @author  licuihuang
 * @date    2022-7-4
 * @param   void
 * @return  void
*******************************************************************************/
void sample_timer2_t8(void)
{
	TIM2_Int_Init(1999,359,TIM_CounterMode_Up); //20ms,36MHZ
	TIM2_PWMOUT_Init(TIM_OCMode_PWM1);
	
	TIM_SetCompare1(TIM2,99);
	TIM_SetCompare2(TIM2,499); //5ms
	TIM_SetCompare3(TIM2,999);
	TIM_SetCompare4(TIM2,1599);
	TIM_Cmd(TIM2, ENABLE);

	printf("A\r\n");
}

