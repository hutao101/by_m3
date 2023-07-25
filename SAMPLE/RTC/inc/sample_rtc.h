#ifndef __SAMPLE_RTC_H__
#define __SAMPLE_RTC_H__

#include "sys.h"
#include "delay.h"
#include "stm32f10x.h"

#define true    TRUE
#define false   FALSE

#define RTC_CTRL            BKP_BASE+0x28
#define TAMP_CTRL           BKP_BASE+0x30
#define TAMP_STAT           BKP_BASE+0x34

#define RTC_CCVR			    	RTC_BASE+0x00  //Current Counter Value Register
#define RTC_CMR					    RTC_BASE+0x04  // Counter Match Register 
#define RTC_CLR					    RTC_BASE+0x08  // Counter Load Register 
#define RTC_CCR					    RTC_BASE+0x0c  // Counter Control Register 
#define RTC_STAT			    	RTC_BASE+0x10  // Interrupt Status Register 
#define RTC_RSTAT			    	RTC_BASE+0x14  // Interrupt Raw Status Register 
#define RTC_EOI					    RTC_BASE+0x18  // End of Interrupt Register 
#define RTC_COMP_VERSION		RTC_BASE+0x1C  // Component Version Register 
#define RTC_CPSR				    RTC_BASE+0x20  // Counter PreScaler Register 
#define RTC_CPCVR				    RTC_BASE+0x24  // Current Prescaler Counter Value Register 


/* Bit map for RTC_CCR */
#define RTC_INT_ENABLE			(1)
#define RTC_INT_DISABLE			(0)
#define RTC_INT_MASK			(1 << 1)
#define RTC_INT_UNMASK			(0 << 1)
#define RTC_ENABLE				(1 << 2)
#define RTC_DISABLE				(0 << 2)
#define RTC_WRAP_ENABLE			(1 << 3)
#define RTC_WRAP_DISABLE		(0 << 3)
#define RTC_PSCLR_ENABLE		(1 << 4)
#define RTC_PSCLR_DISABLE		(0 << 4)


void nvic_rtc_init(u32 irqchannel);
	
void sample_rtc_t1(void);
void sample_rtc_t2(void);
void sample_rtc_t3(void);
void sample_rtc_t4(void);	
void sample_rtc_t5(void);
void sample_rtc_t6(void);		
void sample_rtc_t7(void);		
void sample_rtc_t8(void);			
void sample_rtc_t9(void);			
void sample_rtc_t10(void);
void sample_rtc_t11(void);
void sample_rtc_t12(void);
void sample_rtc_t13(void);

#endif

