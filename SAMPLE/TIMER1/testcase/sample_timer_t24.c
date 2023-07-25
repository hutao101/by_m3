#include "sys.h"
#include "delay.h"
#include "sample_timer.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_timer_t24  
 * Function TIM3_CH3/CH4Òý½Å²âÊÔ
 * @author  licuihuang
 * @date    2022-5-13
 * @param   void
 * @return  void
*******************************************************************************/
void sample_timer_t24(void)
{	
	printf("remap:0x%x tim pin:0x%x\r\n",read_ahb32(0x4001b070),read_ahb32(0x4001b06c));   
	write_ahb32(0x4001b070,0xc00);         //pin remap
	write_ahb32(0x4001b06c,0x0f00);         //tim pin
	printf("remap:0x%x tim pin:0x%x\r\n",read_ahb32(0x4001b070),read_ahb32(0x4001b06c)); 

	TIM3_Int_Init(999,359,TIM_CounterMode_Up); 
	TIM3_PWMOUT_Init(TIM_OCMode_PWM1);	
//	TIM_SetCompare1(TIM3,399);
//	TIM_SetCompare2(TIM3,499);
	TIM_SetCompare3(TIM3,599);
	TIM_SetCompare4(TIM3,699);
	TIM_Cmd(TIM3, ENABLE);

	printf("\r\n");
}

