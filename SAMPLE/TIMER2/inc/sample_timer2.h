#ifndef __SAMPLE_TIMER2_H__
#define __SAMPLE_TIMER2_H__

#include "stdio.h"	
#include "sys.h" 
#include "stm32f10x.h"

//void TIM_GPIO_Init(void);
void TIM4_Int_Init(u16 arr,u16 psc,u16 countmode);
void TIM3_Int_Init(u16 arr,u16 psc,u16 countmode);
void TIM2_Int_Init(u16 arr,u16 psc,u16 countmode);
void TIM1_Int_Init(u16 arr,u16 psc,u16 countmode,u8 rcr);

void TIM1_PWMOUT_Init(u16 ocmode);
void TIM2_PWMOUT_Init(u16 ocmode);
void TIM3_PWMOUT_Init(u16 ocmode);
void TIM4_PWMOUT_Init(u16 ocmode);

void TIM1_PWMIN_Init(u16 channel,u16 icselect);
void TIM2_PWMIN_Init(u16 channel,u16 icselect);
void TIM1_CaptureIN_Init(u16 channel,u16 icselect);
void TIM2_CaptureIN_Init(u16 channel,u16 icselect);
void TIM3_CaptureIN_Init(u16 channel,u16 icselect);
void TIM4_CaptureIN_Init(u16 channel,u16 icselect);

void TIM1_COMPLEOUT_Init(u16 ocmode,u16 deadtime);
void TIM1_BREAK_Init(u16 ocmode,u16 breakpol,u16 breaktim);
void TIM1_NVIC_Init(u32 irqchannel);
void TIM2_NVIC_Init(u32 irqchannel);

void TIM4_ENCODER_Init(void); 

void sample_timer2_t1(void);
void sample_timer2_t2(void);
void sample_timer2_t3(void);
void sample_timer2_t4(void);
void sample_timer2_t5(void);
void sample_timer2_t6(void);
void sample_timer2_t7(void);
void sample_timer2_t8(void);
void sample_timer2_t9(void);
void sample_timer2_t10(void);
void sample_timer2_t11(void);
void sample_timer2_t12(void);
void sample_timer2_t13(void);
void sample_timer2_t14(void);
void sample_timer2_t14_1(void);
void sample_timer2_t14_2(void);
void sample_timer2_t15(void);
void sample_timer2_t16(void);
void sample_timer2_t16_1(void);
void sample_timer2_t16_2(void);
void sample_timer2_t16_3(void);
void sample_timer2_t16_4(void);
void sample_timer2_t17(void);
void sample_timer2_t18(void);
void sample_timer2_t19(void);
void sample_timer2_t20(void);
void sample_timer2_t21(void);
void sample_timer2_t22(void);
void sample_timer2_t23(void);
#endif

