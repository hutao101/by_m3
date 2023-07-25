#include "sys.h"
#include "delay.h"
#include "sample_rtc.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_rtc_t8
 * Function 中断(rtc_second_42)
 * @author  licuihuang
 * @date    2022-7-19
 * @param   void
 * @return  void
*******************************************************************************/
void sample_rtc_t8(void)
{	
	nvic_rtc_init(42);
	write_ahb32(0x4001ae04,0x800);  //后备域写访问
	
	write_ahb32(RTC_CMR,0x0);
	write_ahb32(RTC_CPSR,0x0);
	write_ahb32(RTC_CLR ,0x0);    //因rtc一直开着，所以重装载后需立即使能秒中断，要不打印计数值会有偏差
	write_ahb32(RTC_CTRL,0x7fff0400);   //秒中断使能，时间为1s
	write_ahb32(RTC_CCR,RTC_ENABLE);   //使能rtc RTC_WRAP_ENABLE|RTC_INT_UNMASK|RTC_INT_ENABLE|


/*******清秒中断测试***********/	
//	while(!(read_ahb32(RTC_CTRL)&0x800));  //等待秒标志为1
//	while(read_ahb32(RTC_CTRL)&0x800)
//	{
//		write_ahb32(RTC_CTRL,0x7fff0400);  //对RTC_CRTL[11]标志位写0清不了秒标志
//		read_ahb32(RTC_CTRL);              //读RTC_CRTL寄存器清不了秒标志
//		write_ahb32(RTC_CTRL,0x7fff0800);  //对RTC_CRTL[10]秒中断使能位写0可清秒标志
//		write_ahb32(RTC_CTRL,0x7fff0c00);  //对RTC_CRTL[11]标志位写1可清秒标志,正确做法
//		printf("second:0x%x\r\n",read_ahb32(RTC_CTRL));  //若是标志位为0将会打印此句
//	}
//	printf("second:0x%x\r\n",read_ahb32(RTC_CTRL));  //若是标志位为0将会打印此句
/*******清秒中断测试***********/		

  printf("\r\n");
}

