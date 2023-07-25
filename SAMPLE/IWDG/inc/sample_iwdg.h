#ifndef __SAMPLE_IWDG_H__
#define __SAMPLE_IWDG_H__

#include "sys.h"
#include "delay.h"
#include "stm32f10x.h"

#define RCC_CTRL   (PWR_BASE+0x0)
#define PWR_CTRL   (PWR_BASE+0x4)
#define PWR_STAT   (PWR_BASE+0x8)
#define RST_STAT   (PWR_BASE+0xC)

#define PWR_CTRL_clesta     0x8
#define PWR_CTRL_clewake    0x4
#define PWR_CTRL_standby    0x2 
#define PWR_CTRL_stop       0x0
#define PWR_CTRL_lowvolt    0x1

#define RTC_CMR					    RTC_BASE+0x04  // Counter Match Register 
#define RTC_CLR					    RTC_BASE+0x08  // Counter Load Register 
#define RTC_CCR					    RTC_BASE+0x0c  // Counter Control Register 
#define RTC_CTRL                    BKP_BASE+0x28
#define RTC_STAT			    	RTC_BASE+0x10
#define RTC_EOI					    RTC_BASE+0x18

#define RTC_INT_ENABLE			(1)
#define RTC_INT_DISABLE			(0)
#define RTC_INT_MASK			(1 << 1)
#define RTC_INT_UNMASK			(0 << 1)
#define RTC_ENABLE				(1 << 2)
#define RTC_DISABLE				(0 << 2)
#define RTC_WRAP_ENABLE			(1 << 3)
#define RTC_WRAP_DISABLE		(0 << 3)


#define GPIO_BASE_A  0x40010000
#define GPIO_INTEN  0x30
#define GPIO_INTMASK 0x34
#define GPIO_INTTYPE_LEVEL 0x38
#define GPIO_INT_POLARITY 0x3c
#define GPIO_INTSTATUS 0x40
#define GPIO_PORTA_EOI 0x4c

void iwdg_exit_init(void);
void iwdg_rtc_maskirq_init(void);
void sample_iwdg_init(u16 pre,u16 reload);
void sample_iwdg_feed(void);
void standby_iwdg_init(void);
void stop_iwdg_init(void);
	
void sample_iwdg_t1(void);
void sample_iwdg_t2(void);
void sample_iwdg_t3(void);
void sample_iwdg_t4(void);
void sample_iwdg_t5(void);		
void sample_iwdg_t6(void);		
void sample_iwdg_t7(void);			
#endif

