#include "sys.h"
#include "delay.h"
#include "sample_timer.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_timer_t13   
 * Function 编码器模式:TIM1的CH1接TIM2的CH1，CH2接TIM3的CH1，CH1比CH2提前1/4clk
 * @author  licuihaung
 * @date    2022-5-16
 * @param   void
 * @return  void
*******************************************************************************/
void sample_timer_t13(void)
{
	u32 i,j,k,pol1,pol2,enc;
	for(k=0;k<3;k++)
	{
		switch(k)
		{
			case 0:
				printf("\r\nTIM1 EncoderMode is TI12\r\n");
				enc=TIM_EncoderMode_TI12;
				break;
			case 1:
				printf("\r\nTIM1 EncoderMode is TI1\r\n");
				enc=TIM_EncoderMode_TI1;
				break;
			case 2:
				printf("\r\nTIM1 EncoderMode is TI2\r\n");
				enc=TIM_EncoderMode_TI2;
				break;
			default:
				break;
		}
		for(j=0;j<4;j++)
		{
			TIM2_Int_Init(99,35999,TIM_CounterMode_Up);
			TIM3_Int_Init(99,35999,TIM_CounterMode_Up);
			TIM1_Int_Init(299,0,TIM_CounterMode_Up,0);
			
			TIM2_PWMOUT_Init(TIM_OCMode_PWM2);
			TIM3_PWMOUT_Init(TIM_OCMode_PWM2);
			TIM_SetCompare1(TIM2,50);
			TIM_SetCompare1(TIM3,50);

			TIM1_CaptureIN_Init(TIM_Channel_1|TIM_Channel_2,TIM_ICSelection_DirectTI);
			switch(j)
			{
				case 0:
					printf("IC1/2 polarity is rise/rise:\r\n");
					pol1=TIM_ICPolarity_Rising;
					pol2=TIM_ICPolarity_Rising;
					break;
				case 1:
					printf("IC1/2 polarity is rise/fall:\r\n");
					pol1=TIM_ICPolarity_Rising;
					pol2=TIM_ICPolarity_Falling;
					break;
				case 2:
					printf("IC1/2 polarity is fall/rise:\r\n");
					pol1=TIM_ICPolarity_Falling;
					pol2=TIM_ICPolarity_Rising;
					break;
				case 3:
					printf("IC1/2 polarity is fall/fall:\r\n");
					pol1=TIM_ICPolarity_Falling;
					pol2=TIM_ICPolarity_Falling;
					break;
				default:
					break;
			}
			TIM_EncoderInterfaceConfig(TIM1,enc,pol1,pol2); //???1/2??
			TIM_SetCounter(TIM1,50);
			TIM_Cmd(TIM2,ENABLE);
			delay_ms(25);
			TIM_Cmd(TIM3,ENABLE);
			TIM_Cmd(TIM1,ENABLE);
			 
			for(i=0;i<1000;i++)
			{
				printf("%d ",TIM1->CNT);
			}
			printf("\r\n");
			TIM_Cmd(TIM1,DISABLE);
			TIM_Cmd(TIM2,DISABLE);
			TIM_Cmd(TIM3,DISABLE);
			delay_ms(200);
		}
	}
}

