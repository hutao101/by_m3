#include "sys.h"
#include "delay.h"
#include "sample_timer.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_timer_t18   
 * Function ÖÐ¶Ï£ºTIM_IT_Update/TIM_IT_CC1/CC2/CC3/CC4£¨ÖÐ¶ÏºÅ30/31/32/33/34£©
 * @author  licuihuang
 * @date    2022-5-19
 * @param   void
 * @return  void
*******************************************************************************/
void sample_timer_t18(void)
{
    u32 i,j,irqchannel,it;
	for(j=0;j<5;j++)
	{
		switch(j)
		{
			case 0:
				printf("test TIM1_IT_Update:\r\n");
				it = TIM_IT_Update;
			    irqchannel = 30;
			    break;
			case 1:
				printf("test TIM1_IT_CC1:\r\n");
				it = TIM_IT_CC1;
			    irqchannel = 31;
			    break;
			case 2:
				printf("test TIM1_IT_CC2:\r\n");
				it = TIM_IT_CC2;
			    irqchannel = 32;
			    break;
			case 3:
				printf("test TIM1_IT_CC3:\r\n");
				it = TIM_IT_CC3;
			    irqchannel = 33;
			    break;
			case 4:
				printf("test TIM1_IT_CC4:\r\n");
				it = TIM_IT_CC4;
			    irqchannel = 34;
			    break;
			default:
				break;				
		}
		
		TIM1_Int_Init(999,3599,TIM_CounterMode_Up,0);  //1000*(5000/50M)=100ms
		TIM1_PWMOUT_Init(TIM_OCMode_PWM1);
		TIM_ITConfig(TIM1, it, ENABLE);
		TIM1_NVIC_Init(irqchannel);
		TIM_SetCompare1(TIM1,400);
		TIM_SetCompare2(TIM1,500);
		TIM_SetCompare3(TIM1,600);
		TIM_SetCompare4(TIM1,700);		
		TIM_Cmd(TIM1, ENABLE);
		
		delay_ms(500);
		TIM_Cmd(TIM1, DISABLE);
		TIM_ITConfig(TIM1, it, DISABLE);
		printf("\r\n");
	}
}

