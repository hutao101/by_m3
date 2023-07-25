#ifndef _SAMPLE_WDT_H_
#define _SAMPLE_WDT_H_

#include "stm32f10x.h"
#include "usart.h"
#include "delay.h"
#include "snps_wdt.h"



void sample_wdt_t1(void);
void sample_wdt_t2(void);

void sample_wdt_t3(void);
void sample_wdt_t4(void);

void sample_wdt_t5(u8 timeout);

#endif 

