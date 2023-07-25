#include "sys.h"
#include "delay.h"
#include "sample_timer.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_timer_t10   
 * Function …≤≥µ ‰»Î
 * @author  licuihuang
 * @date    2022-5-11
 * @param   void
 * @return  void
*******************************************************************************/
void sample_timer_t10(void)
{		
	TIM1_Int_Init(99,359,TIM_CounterMode_Up,0);  
	TIM1_BREAK_Init(TIM_OCMode_PWM1,TIM_BreakPolarity_Low,100);	    
	TIM_SetCompare1(TIM1,40);	
	printf("CR1:0x%x CCER:0x%x BDTR:0x%x\r\n",read_ahb32(0x40011c00),read_ahb32(0x40011c20),read_ahb32(0x40011c44));
		
    TIM_Cmd(TIM1, ENABLE);
	delay_ms(1500);
	printf("CR1:0x%x CCER:0x%x BDTR:0x%x\r\n",read_ahb32(0x40011c00),read_ahb32(0x40011c20),read_ahb32(0x40011c44));
	printf("A\r\n");
}


