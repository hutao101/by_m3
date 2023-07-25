#include "sys.h"
#include "delay.h"
#include "sample_rtc.h"
#include "stm32f10x.h"

void nvic_rtc_init(u32 irqchannel)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = irqchannel; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
}


void RTC_IRQHandler(void)  //只对匹配中断有效
{
/****只判断秒标志位不清匹配中断则出不来中断函数****/
//	if(read_ahb32(RTC_CTRL)&0x800)
//	{
//		printf("rtc second irq\r\n");
//		write_ahb32(RTC_CTRL,0x7fff0c00);  //对秒标志写1清标志	
//	}
/**************************************************/
		
	if(read_ahb32(RTC_STAT))
	{
		printf("rtc match irq\r\n");
//		write_ahb32(RTC_CCR,reg1|RTC_INT_MASK);   //屏蔽中断，只进中断一次
		read_ahb32(RTC_EOI);   //清匹配标志位,隔一段时间进中断打印一次
	}
}


void RTC_SEC_IRQHandler(void)
{
	u32 r0,r1;
    r1=read_ahb32(RTC_CCVR);	
	r0=read_ahb32(RTC_CTRL);
	if(read_ahb32(RTC_CTRL)&0x800)
	{
		printf("0x%x ",r1);
		printf("rtc second irq\r\n");
		write_ahb32(RTC_CTRL,r0|0x800);   //对秒标志写1清标志,正确做法		
	}
}

