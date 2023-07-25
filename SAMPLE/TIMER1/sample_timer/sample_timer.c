#include "sample_timer.h"
#include "stm32f10x.h"

#define read_ahb32(mreg)	    *(volatile unsigned int *)(mreg)
#define write_ahb32(mreg,val)   *(volatile unsigned int *)(mreg) = (val)
	
u16 IC2Value = 0;
u16 IC1Value = 0;
float DutyCycle = 0;
float Frequency = 0;

//void TIM_GPIO_Init(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //TIM1_CH1:PA8
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //TIM2_CH1:PA0
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; //TIM1_CH1N:PB13
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO
//	
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;  //TIM1_BKIN:PB12
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_Init(GPIOB, &GPIO_InitStructure);
////	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //TIM1_CH2：PA9
//////	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //浮空输入
////	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
////	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO
//}
      

void TIM2_Int_Init(u16 arr,u16 psc,u16 countmode)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler = psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = countmode;//TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
}


void TIM3_Int_Init(u16 arr,u16 psc,u16 countmode)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler = psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = countmode;//TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
}


void TIM4_Int_Init(u16 arr,u16 psc,u16 countmode)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler = psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = countmode;//TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
}



void TIM1_Int_Init(u16 arr,u16 psc,u16 countmode,u8 rcr)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler = psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = countmode;//TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseStructure.TIM_RepetitionCounter = rcr;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
}


void TIM1_PWMOUT_Init(u16 ocmode)
{  
	TIM_OCInitTypeDef  TIM_OCInitStructure = {0};
	
	TIM_OCInitStructure.TIM_OCMode = ocmode;//TIM_OCMode_Active;//TIM_ForcedAction_InActive;//TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM OC2
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);
}


void TIM1_COMPLEOUT_Init(u16 ocmode,u16 deadtime)
{  
	TIM_OCInitTypeDef  TIM_OCInitStructure = {0};
	TIM_BDTRInitTypeDef TIM_BDTRInitStructure = {0};
	
	TIM_OCInitStructure.TIM_OCMode = ocmode;//TIM_OCMode_Active;//TIM_ForcedAction_InActive;//TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:CH1与CH1N输出极性相同时得到互补波形
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM OC2
	
	TIM_BDTRInitStructure.TIM_DeadTime = deadtime;
	TIM_BDTRConfig(TIM1,&TIM_BDTRInitStructure);
	TIM_CtrlPWMOutputs(TIM1,ENABLE);  //MOE位设置
}


void TIM1_BREAK_Init(u16 ocmode,u16 breakpol,u16 breaktim)
{  
	TIM_OCInitTypeDef  TIM_OCInitStructure = {0};
	TIM_BDTRInitTypeDef TIM_BDTRInitStructure = {0};
	
	TIM_OCInitStructure.TIM_OCMode = ocmode;//TIM_OCMode_Active;//TIM_ForcedAction_InActive;//TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性高
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set; //CH1输出空闲状态电平与CH1N相反，要不两通道都输出0
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low; //Low High
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Set;
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM OC2
	
//	TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Disable;//TIM_AutomaticOutput_Enable;  //设置为1时可将MOE位在下次更新事件时自动置1
	TIM_BDTRInitStructure.TIM_Break = TIM_Break_Enable;
	TIM_BDTRInitStructure.TIM_BreakPolarity = breakpol;  //当刹车输入引脚输入低电平时刹车功能启动，引脚默认高电平
//	TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;     //锁定设置
	TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;  //可不设置，定时器不工作时有效
	TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;  //可不设置，定时器不工作时有效
	TIM_BDTRInitStructure.TIM_DeadTime = breaktim;
	TIM_BDTRConfig(TIM1,&TIM_BDTRInitStructure);
	TIM_CtrlPWMOutputs(TIM1,ENABLE);  //MOE位设置
}


void TIM2_PWMOUT_Init(u16 ocmode)
{  
	TIM_OCInitTypeDef  TIM_OCInitStructure = {0};
	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//TIM_OCMode_Active;//TIM_ForcedAction_InActive;//TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM OC2	
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);
}


void TIM3_PWMOUT_Init(u16 ocmode)
{  
	TIM_OCInitTypeDef  TIM_OCInitStructure = {0};
	 
	TIM_OCInitStructure.TIM_OCMode = ocmode;//TIM_OCMode_Active;//TIM_ForcedAction_InActive;//TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
//	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:高
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM OC2	
	TIM_OC2Init(TIM3, &TIM_OCInitStructure); 
	TIM_OC3Init(TIM3, &TIM_OCInitStructure); 
	TIM_OC4Init(TIM3, &TIM_OCInitStructure); 	
}


void TIM4_PWMOUT_Init(u16 ocmode)
{  
	TIM_OCInitTypeDef  TIM_OCInitStructure = {0};
	 
	TIM_OCInitStructure.TIM_OCMode = ocmode;//TIM_OCMode_Active;//TIM_ForcedAction_InActive;//TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:高
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM OC2	
 	TIM_OC2Init(TIM4, &TIM_OCInitStructure); 
	TIM_OC3Init(TIM4, &TIM_OCInitStructure); 
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);
}


void TIM1_PWMIN_Init(u16 channel,u16 icselect)
{
	TIM_ICInitTypeDef TIM_ICInitStructure;
//	GPIO_InitTypeDef GPIO_InitStructure;
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //TIM1_CH1：PA8
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //浮空输入
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICSelection = icselect;//TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICFilter = 0x0;
	TIM_PWMIConfig(TIM1, &TIM_ICInitStructure); 
	
	// 当工作做PWM输入模式时,只需要设置触发信号的那一路即可（用于测量周期）
	// 另外一路（用于测量占空比）会由硬件自带设置，不需要再配置		
//	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
//  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;
//  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_IndirectTI;
//  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
//  TIM_ICInitStructure.TIM_ICFilter = 0x0;
//  TIM_PWMIConfig(ADVANCE_TIM, &TIM_ICInitStructure);
//	TIM_ICInit(TIM1,&TIM_ICInitStructure);
    TIM_SelectInputTrigger(TIM1, TIM_TS_TI1FP1);	 // 选择输入捕获的触发信号		
    TIM_SelectSlaveMode(TIM1, TIM_SlaveMode_Reset);  // 选择从模式: 复位模式,PWM输入模式时,从模式必须工作在复位模式，当捕获开始时,计数器CNT会被复位
    TIM_SelectMasterSlaveMode(TIM1,TIM_MasterSlaveMode_Enable);  //开启主从模式
}


void TIM1_CaptureIN_Init(u16 channel,u16 icselect)
{
	TIM_ICInitTypeDef TIM_ICInitStructure;
//	GPIO_InitTypeDef GPIO_InitStructure;
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //TIM1_CH1：PA8
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //浮空输入
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO
	
   	TIM_ICInitStructure.TIM_Channel = channel;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;  //选择极性时就把捕获使能开了CCER[0]
    TIM_ICInitStructure.TIM_ICSelection = icselect;//TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 0x0;
  	TIM_ICInit(TIM1,&TIM_ICInitStructure);	
}


void TIM3_CaptureIN_Init(u16 channel,u16 icselect)
{
	TIM_ICInitTypeDef TIM_ICInitStructure;
//		GPIO_InitTypeDef GPIO_InitStructure;
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//		
//		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; 
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //浮空输入
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//		GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO

	TIM_ICStructInit(&TIM_ICInitStructure);  //结构体初始化，将所有成员填充为复位值，防止某些未赋值的参数自动配置为随机值
	TIM_ICInitStructure.TIM_Channel = channel;
	TIM_ICInitStructure.TIM_ICSelection = icselect;//TIM_ICSelection_TRC TIM_ICSelection_DirectTI
	TIM_ICInit(TIM3,&TIM_ICInitStructure);
}


void TIM4_CaptureIN_Init(u16 channel,u16 icselect)
{
	TIM_ICInitTypeDef TIM_ICInitStructure;
//	GPIO_InitTypeDef GPIO_InitStructure;
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //TIM1_CH1：PA8
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //浮空输入
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO
	
	TIM_ICStructInit(&TIM_ICInitStructure);  //结构体初始化，将所有成员填充为复位值，防止某些未赋值的参数自动配置为随机值
   	TIM_ICInitStructure.TIM_Channel = channel;
//    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;  //选择极性时就把捕获使能开了CCER[0]
    TIM_ICInitStructure.TIM_ICSelection = icselect;
//    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
//    TIM_ICInitStructure.TIM_ICFilter = 0x0;
  	TIM_ICInit(TIM4,&TIM_ICInitStructure);	
}


void TIM2_CaptureIN_Init(u16 channel,u16 icselect)
{
    TIM_ICInitTypeDef TIM_ICInitStructure;
//	GPIO_InitTypeDef GPIO_InitStructure;
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //TIM1_CH1：PA8
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //浮空输入
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO
	
	TIM_ICStructInit(&TIM_ICInitStructure);  //结构体初始化，将所有成员填充为复位值，防止某些未赋值的参数自动配置为随机值
   	TIM_ICInitStructure.TIM_Channel = channel;
//    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;  //选择极性时就把捕获使能开了CCER[0]
    TIM_ICInitStructure.TIM_ICSelection = icselect;
//    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
//    TIM_ICInitStructure.TIM_ICFilter = 0x0;
  	TIM_ICInit(TIM2,&TIM_ICInitStructure);	
}


void TIM4_ENCODER_Init(void)                      
{            
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStruct;
    TIM_ICInitTypeDef TIM_ICInitStruct;          
     
    TIM_TimeBaseStructInit(&TIM_TimeBaseStruct);    
    TIM_TimeBaseStruct.TIM_Prescaler = 11;          
    TIM_TimeBaseStruct.TIM_Period = 65;       
    TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;      
    TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up; 
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStruct); 

    TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI2,TIM_ICPolarity_Rising, TIM_ICPolarity_Rising); //TIM_ICPolarity_Rising  TIM_ICPolarity_Falling TIM_ICPolarity_BothEdge
    TIM_ICStructInit(&TIM_ICInitStruct);  
    TIM_ICInitStruct.TIM_ICFilter = 0; 
    TIM_ICInit(TIM4, &TIM_ICInitStruct);
    TIM_Cmd(TIM4,ENABLE);               
} 



void TIM1_NVIC_Init(u32 irqchannel)
{	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	
	NVIC_InitStructure.NVIC_IRQChannel = irqchannel;//TIM1_CC_IRQn;  //TIM中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器	
}



void TIM3_NVIC_Init(void)
{	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	
	NVIC_InitStructure.NVIC_IRQChannel = 17;//TIM1_CC_IRQn;  //TIM中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器	
}


u16 i;
void TIM1_IRQHandler(void) //6
{	
//	printf("TIM1 IRQ\r\n");
//	  if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
//		{
//			printf("  UP IRQ!!\r\n");
//		  TIM_ClearITPendingBit(TIM1, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
//		}
//		if (TIM_GetITStatus(TIM1, TIM_IT_CC1) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
//		{
////			printf("  CC1 IRQ!!\r\n");
//		  TIM_ClearITPendingBit(TIM1, TIM_IT_CC1  );  //清除TIMx的中断待处理位:TIM 中断源 
//					
///********输入捕获-PWM*********/	
			IC1Value = TIM_GetCapture1(TIM1);
			IC2Value = TIM_GetCapture2(TIM1);
//			printf("cnt:%d  capture1:%d  capture2:%d\r\n",TIM1->CNT,TIM_GetCapture1(TIM1),TIM_GetCapture2(TIM1));
			if (IC1Value != 0) // 注意：捕获寄存器CCR1和CCR2的值在计算占空比和频率的时候必须加1
			{   
				DutyCycle = (float)((IC2Value+1) * 100) / (IC1Value+1);  /* 占空比计算 */
				Frequency = (36000000/(9999+1))/(float)(IC1Value+1);   /* 频率计算 */
				printf("占空比：%0.2f%%   频率：%0.2fHz\r\n",DutyCycle,Frequency);
			}
			else
			{
				DutyCycle = 0;
				Frequency = 0;  
			}
/********输入捕获-PWM*********/		
		
/********输入捕获-单边沿*********/
//		  if(i == 0)
//			{
//		    IC1Value = TIM_GetCapture1(TIM1);				
//				Frequency = (float)(IC1Value+1);//(72000000/(71+1));
//				printf("IC1Value:%d  clk:%f\r\n",IC1Value,Frequency);				
//				i++;
//			}
//			else
//			{				
//				Frequency = (float)(36000000/((9999+1)*((TIM_GetCapture1(TIM1)+1)-(IC1Value+1))));
//				printf("capture:%d  IC1Value:%d  Frequency:%fHz\r\n",TIM_GetCapture1(TIM1),IC1Value,Frequency);
//				IC1Value = TIM_GetCapture1(TIM1);
//			}
/********输入捕获-单边沿*********/
//		}
}

	
void TIM3_IRQHandler(void) //17
{	
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
		printf("  UP IRQ!!\r\n");
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
	}
	if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
		printf("  CC1 IRQ!!\r\n");
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC1  );  //清除TIMx的中断待处理位:TIM 中断源 
	}
}

	
void TIM1_UP_IRQHandler(void) //6
{	
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
		printf("  UP IRQ!!\r\n");
	    TIM_ClearITPendingBit(TIM1, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
	}
}

	
u16 i=0;	
void TIM1_CC1_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM1, TIM_IT_CC1) != RESET) 
	{
		printf("  CC1 IRQ!!\r\n");
		TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);
/********输入捕获-PWM*********/	
//			IC1Value = TIM_GetCapture1(TIM1);
//			IC2Value = TIM_GetCapture2(TIM1);
////	  	printf("  CC1 IRQ!!\r\n");
//		  printf("0x%x 0x%x\r\n",IC1Value,IC2Value);
////			printf("capture1:%d  capture2:%d\r\n",TIM_GetCapture1(TIM1),TIM_GetCapture2(TIM1));
//			if (IC1Value != 0) // 注意：捕获寄存器CCR1和CCR2的值在计算占空比和频率的时候必须加1
//			{   
//				DutyCycle = (float)((IC2Value+1) * 100) / (IC1Value+1);  /* 占空比计算 */
//				Frequency = (36000000/(9999+1))/(float)(IC1Value+1);   /* 频率计算 */
//				printf("占空比：%0.2f%%   频率：%0.2fHz\r\n",DutyCycle,Frequency);
//			}
//			else
//			{
//				DutyCycle = 0;
//				Frequency = 0;  
//			}
/********输入捕获-PWM*********/		
		
/********输入捕获-单边沿*********/
//		  if(i == 0)
//			{
//		    IC1Value = TIM_GetCapture1(TIM1);				
//				printf("IC1Value:%d\r\n",IC1Value);				
//				i++;
//			}
//			else
//			{			
//        IC2Value = TIM_GetCapture1(TIM1);					
//				Frequency = (36000000/(9999+1))/(float)((IC2Value+1)-(IC1Value+1));
//				printf("IC2Value:%d  IC1Value:%d  fre:%f\r\n",IC2Value,IC1Value,Frequency);
//				IC1Value = IC2Value;
//			}
/********输入捕获-单边沿*********/
//       printf("tim1 cc1:%d\r\n",TIM1->CNT);
			}	  
}


void TIM1_CC2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM1, TIM_IT_CC2) != RESET) 
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_CC2);
	  	printf("  CC2 IRQ!!\r\n");	
	}
}


void TIM1_CC3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM1, TIM_IT_CC3) != RESET) 
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_CC3);
	  	printf("  CC3 IRQ!!\r\n");	
	}
}


void TIM1_CC4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM1, TIM_IT_CC4) != RESET) 
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_CC4);
	  	printf("  CC4 IRQ!!\r\n");	
	}
}

void TIM1_BK_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM1, TIM_IT_Break) != RESET) 
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_Break);
	  	printf("  BREAK IRQ!!\r\n");
	    printf("CR1:0x%x CCER:0x%x BDTR:0x%x\r\n",read_ahb32(0x40011c00),read_ahb32(0x40011c20),read_ahb32(0x40011c44));		
	}
}


void TIM1_TRG_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM1, TIM_IT_Trigger) != RESET) 
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_Trigger);
	  	printf("  Trigger IRQ!!\r\n");	
	}
}


void TIM1_COM_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM1, TIM_IT_COM) != RESET) 
	{
		TIM_SelectOCxM(TIM1, TIM_Channel_1, TIM_OCMode_Active);
    	TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Enable);	
		TIM_ClearITPendingBit(TIM1, TIM_IT_COM);
	  	printf("  COM IRQ!!\r\n");	
	}
}


void TIM1_CC1OF_IRQHandler(void)
{
	if(TIM_GetFlagStatus(TIM1,TIM_FLAG_CC1OF))
	{
		printf("TIM1 CC1OF IRQ!!\r\n");
		TIM_ClearITPendingBit(TIM1, ~TIM_IT_CC1OF); //对CC1OF写0清标志位
	}
}

void TIM1_CC2OF_IRQHandler(void)
{
	if(TIM_GetFlagStatus(TIM1,TIM_FLAG_CC2OF))
	{
		printf("TIM1 CC2OF IRQ!!\r\n");
		TIM_ClearITPendingBit(TIM1, ~TIM_IT_CC2OF); //对CC1OF写0清标志位
	}
}

void TIM1_CC3OF_IRQHandler(void)
{
	if(TIM_GetFlagStatus(TIM1,TIM_FLAG_CC3OF))
	{
		printf("TIM1 CC3OF IRQ!!\r\n");
		TIM_ClearITPendingBit(TIM1, ~TIM_IT_CC3OF); //对CC1OF写0清标志位
	}
}

void TIM1_CC4OF_IRQHandler(void)
{
	if(TIM_GetFlagStatus(TIM1,TIM_FLAG_CC4OF))
	{
		printf("TIM1 CC4OF IRQ!!\r\n");
		TIM_ClearITPendingBit(TIM1, ~TIM_IT_CC4OF); //对CC1OF写0清标志位
	}
}

