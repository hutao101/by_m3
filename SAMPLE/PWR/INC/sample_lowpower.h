#ifndef _SAMPLE_LOWPOWER_H_
#define _SAMPLE_LOWPOWER_H_
#include "stm32f10x.h"
#include "sample_lowpower.h"
#include "usart.h"
#include "delay.h"
#include "sample_dma.h"
#include "sample_wdt.h"
#include "sample_rtc.h"
#include "sample_timer2.h"
#include "sample_exti.h"
#include "sample_iwdg.h"


#define PwrCtrl_base 0x4001AE00

#define GPIO_BASE_A  0x40010000
#define GPIO_BASE_B  0x40010400
#define GPIO_BASE_C  0x40010800
#define GPIO_BASE_D  0x40010C00

#define GPIO_SWPORTA_DR 0x0
#define GPIO_SWPORTA_DDR  0x4
#define GPIO_SWPORTA_CTL  0x8
#define GPIO_EXT_PORTA 0x50

#define GPIO_INTEN  0x30
#define GPIO_INTMASK 0x34
#define GPIO_INTTYPE_LEVEL 0x38
#define GPIO_INT_POLARITY 0x3c
#define GPIO_INTSTATUS 0x40
#define GPIO_PORTA_EOI 0x4c
#define GPIO_RAW_INTSTATUS 0x44

void sample_lowpower_t1(void);
void sample_lowpower_t2(void);
void sample_lowpower_t3(void);



#endif

