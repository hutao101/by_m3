#ifndef __SAMPLE_SYSTICK_H__
#define __SAMPLE_SYSTICK_H__

#include "sys.h"
#include "delay.h"
#include "stm32f10x.h"

void systick_close(void);
void systick_nvic_init(u32 channel);

void sample_systick_t1(void);
void sample_systick_t2(void);
void sample_systick_t3(void);
void sample_systick_t4(void);
void sample_systick_t5(void);
void sample_systick_t6(void);
void sample_systick_t7(void);
void sample_systick_t8(void);
void sample_systick_t9(void);

#endif

