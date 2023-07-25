#include "sys.h"
#include "delay.h"
#include "sample_timer2.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_timer_t16   
 * Function 霍尔传感器(tim2的CH1接tim3的CH1,CH3作触发输出,观察tim1的CH1输出情况)
 * @author  licuihuang
 * @date    2022-8-24
 * @param   void
 * @return  void
*******************************************************************************/
void sample_timer2_t16(void)
{
	u32 i=3; 
//	for(i=2;i<4;i++) //CH2/3接连做触发输出跑出来波形不对
	{
	//	TIM_GPIO_Init();
		TIM_OCInitTypeDef  TIM_OCInitStructure={0};
		TIM1_Int_Init(1999,3599,TIM_CounterMode_Up,0);	//5000/50M=100us  1000*100us=100ms(up)
		TIM3_Int_Init(4999,3599,TIM_CounterMode_Up);  //5000/50M=100us  5000*100us=500ms(up)
		TIM2_Int_Init(5799,3599,TIM_CounterMode_Up);  //5000/50M=100us  5800*100us=580ms(up)	
		TIM1_NVIC_Init(35);
		TIM_ITConfig(TIM1, TIM_IT_COM, ENABLE);//TIM_IT_COM TIM_IT_Trigger

		TIM3_PWMOUT_Init(TIM_OCMode_PWM2);
		TIM_SetCompare1(TIM3,2000);
		TIM_SetIC1Prescaler(TIM3,TIM_OCPreload_Disable);  //需关闭预分频功能，不然CCR1为0，单脉冲不成功
		TIM_SelectOnePulseMode(TIM3,TIM_OPMode_Single); 
		
		TIM2_CaptureIN_Init(TIM_Channel_1,TIM_ICSelection_TRC);	

		TIM_SelectHallSensor(TIM2,ENABLE);//TIM_CR2_TI1S);    //异或时只有1通道有波形，可以把这个屏蔽然后打开tim通道2看波形
		TIM_SelectInputTrigger(TIM2, TIM_TS_TI1F_ED);//TIM_TS_TI1F_ED);	  //三通道异或做触发输入 TIM_TS_TI2FP2
		TIM_SelectSlaveMode(TIM2, TIM_SlaveMode_Reset);  // 选择从模式: 复位模式
		
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;//TIM_OCMode_Active;//TIM_ForcedAction_InActive;//TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
		switch(i)
		{
			case 1:   //通道1做捕获输模式，就不用作触发输出了
				printf("\r\nTIM2 CH1 as trigger output:\r\n");
				TIM_OC1Init(TIM2, &TIM_OCInitStructure);
				TIM_SelectOCxM(TIM2, TIM_Channel_1, TIM_OCMode_PWM2);
				TIM_SetCompare1(TIM2,1500);
			    TIM_SelectOutputTrigger(TIM2,TIM_TRGOSource_OC1Ref);
				break;
			case 2: 
				printf("\r\nTIM2 CH2 as trigger output:\r\n");
				TIM_OC2Init(TIM2, &TIM_OCInitStructure);
				TIM_SelectOCxM(TIM2, TIM_Channel_2, TIM_OCMode_PWM2);
				TIM_SetCompare2(TIM2,1500);
			    TIM_SelectOutputTrigger(TIM2,TIM_TRGOSource_OC2Ref);
				break;
			case 3: 
				printf("\r\nTIM2 CH3 as trigger output:\r\n");
				TIM_OC3Init(TIM2, &TIM_OCInitStructure);
				TIM_SelectOCxM(TIM2, TIM_Channel_3, TIM_OCMode_PWM2);
				TIM_SetCompare3(TIM2,1500);
			    TIM_SelectOutputTrigger(TIM2,TIM_TRGOSource_OC3Ref);
				break;
			default:
				break;
		}		
//		TIM2_PWMOUT_Init(TIM_OCMode_PWM2);
//		TIM_SelectOCxM(TIM2, TIM_Channel_2, TIM_OCMode_PWM2);
//		TIM_CCxCmd(TIM2, TIM_Channel_2, TIM_CCx_Enable);	//不应该使能，异或是在引脚发生，输出会反馈给输入
//		TIM_SetCompare2(TIM2,1500);
		
		TIM_SelectInputTrigger(TIM1, TIM_TS_ITR1);	  //TIM1作为从模式，TIM3作为主模式时应选ITR2
//		TIM_SelectOutputTrigger(TIM2,TIM_TRGOSource_OC2Ref);   //选择PWM参考信号0C2REF作为触发
		TIM_SelectSlaveMode(TIM1, TIM_SlaveMode_Reset);
		TIM_SelectMasterSlaveMode(TIM2,TIM_MasterSlaveMode_Enable);  //开启主从模式	
		
		TIM1_PWMOUT_Init(TIM_OCMode_PWM1);
		TIM_SetCompare1(TIM1,400);	
		TIM_CCPreloadControl(TIM1, ENABLE);  //把CC1E CC1NE OC1M预装载使能打开
		TIM_SelectCOM(TIM1,ENABLE);  //在com或TRGI上升沿更新捕获/比较控制位
		
		TIM_Cmd(TIM1, ENABLE);
		TIM_Cmd(TIM3, ENABLE);
		TIM_Cmd(TIM2, ENABLE);

		delay_ms(1000);
		TIM_Cmd(TIM1, DISABLE);
		TIM_Cmd(TIM3, DISABLE);
		TIM_Cmd(TIM2, DISABLE);
	}
}	
	
	   