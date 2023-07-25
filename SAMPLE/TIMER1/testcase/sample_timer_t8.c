#include "sys.h"
#include "delay.h"
#include "sample_timer.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_timer_t8   
 * Function 输出比较-PWM1模式
 * @author  licuihuang
 * @date    2022-5-13
 * @param   void
 * @return  void
*******************************************************************************/
void sample_timer_t8(void)
{
//	u32 i;
//	TIM1_Int_Init(199,499,TIM_CounterMode_Up,0); //40ms,50MHZ
//	TIM1_PWMOUT_Init(TIM_OCMode_PWM1);
//	
//	TIM_SetCompare1(TIM1,99);
//	TIM_Cmd(TIM1, ENABLE);
//	printf("A\r\n");


//以下为测试remap引脚内容	
	printf("remap:0x%x ",read_ahb32(0x4001b070));
	write_ahb32(0x4001b06c,0x00f);      //TIM PIN
	write_ahb32(0x4001b070,0x40);      //REMAPE
	printf("remap:0x%x\r\n",read_ahb32(0x4001b070));
    printf("pin mux:0x%x ",read_ahb32(0x4001b018));
	printf("tim pin:0x%x\r\n\r\n",read_ahb32(0x4001b06c));
//BREAK功能
//	TIM1_Int_Init(99,359,TIM_CounterMode_Up,0);
//	TIM1_PWMOUT_Init(TIM_OCMode_PWM1);	
//	TIM_SetCompare1(TIM1,40);	
//	TIM_Cmd(TIM1, ENABLE);	
//	delay_ms(1000);
//	TIM1_BREAK_Init(TIM_OCMode_PWM1,TIM_BreakPolarity_Low,100);
	
//ETR功能	
//	TIM1_Int_Init(99,359,TIM_CounterMode_Up,0);  //100*(500/50M)=1ms
//	TIM1_PWMOUT_Init(TIM_OCMode_PWM1);	
//	TIM_SetCompare1(TIM1,40);	
//    TIM_Cmd(TIM1, ENABLE);
//	delay_ms(1000);
//	TIM_ClearOC1Ref(TIM1,TIM_OCClear_Enable);
	  
//	TIM2_Int_Init(99,359,TIM_CounterMode_Up);  //100*(500/50M)=1ms
//	TIM2_PWMOUT_Init(TIM_OCMode_PWM1);	 
//	TIM_SetCompare2(TIM2,40);	
//    TIM_Cmd(TIM2, ENABLE);	
//	delay_ms(1000);
//	TIM_ClearOC2Ref(TIM2,TIM_OCClear_Enable); 

//	TIM3_Int_Init(99,359,TIM_CounterMode_Up);  //100*(500/50M)=1ms
//	TIM3_PWMOUT_Init(TIM_OCMode_PWM1);	 
//	TIM_SetCompare1(TIM3,40);	
//    TIM_Cmd(TIM3, ENABLE);	
//	delay_ms(1000);
//	TIM_ClearOC1Ref(TIM3,TIM_OCClear_Enable);
	
//引脚波形
	TIM1_Int_Init(999,359,TIM_CounterMode_Up,0); //40ms,50MHZ
	TIM1_PWMOUT_Init(TIM_OCMode_PWM1);	
	TIM_SetCompare1(TIM1,599);
	TIM_SetCompare2(TIM1,499);
	TIM_SetCompare3(TIM1,399);
	TIM_SetCompare4(TIM1,299);
	TIM_Cmd(TIM1, ENABLE);

//	TIM2_Int_Init(999,359,TIM_CounterMode_Up); //40ms,50MHZ
//	TIM2_PWMOUT_Init(TIM_OCMode_PWM1);	
//	TIM_SetCompare1(TIM2,399);
//	TIM_SetCompare2(TIM2,499);
//	TIM_SetCompare3(TIM2,599);
//	TIM_SetCompare4(TIM2,699);
//	TIM_Cmd(TIM2, ENABLE);

//	TIM3_Int_Init(999,359,TIM_CounterMode_Up); //40ms,50MHZ
//	TIM3_PWMOUT_Init(TIM_OCMode_PWM1);	
//	TIM_SetCompare1(TIM3,399);
//	TIM_SetCompare2(TIM3,499);
//	TIM_SetCompare3(TIM3,599);
//	TIM_SetCompare4(TIM3,699);
//	TIM_Cmd(TIM3, ENABLE);

//	TIM4_Int_Init(999,359,TIM_CounterMode_Up); //40ms,50MHZ
//	TIM4_PWMOUT_Init(TIM_OCMode_PWM1);	
//	TIM_SetCompare1(TIM4,399);
//	TIM_SetCompare2(TIM4,499);
//	TIM_SetCompare3(TIM4,599);
//	TIM_SetCompare4(TIM4,699);
//	TIM_Cmd(TIM4, ENABLE);  
}

