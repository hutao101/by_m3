#include "sys.h"
#include "delay.h"
#include "sample_rtc.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_rtc_t10
 * Function TAMPER输出闹钟脉冲
 * @author  licuihuang
 * @date    2022-7-19
 * @param   void
 * @return  void
*******************************************************************************/
void sample_rtc_t10(void)
{	
	u32 reg;	
//	nvic_rtc_init(19);
	write_ahb32(0x4001ae04,0x800);  //后备域写访问
	
	printf("rtc_ctrl:0x%x 0x%x 0x%x\r\n",read_ahb32(RTC_CTRL),read_ahb32(TAMP_CTRL),read_ahb32(TAMP_STAT));
	write_ahb32(RTC_CTRL,0x7fff0100);       //频率为32.768kh计数0x7ff次刚好为1s，打开秒中断使能位
	printf("rtc_ctrl:0x%x 0x%x 0x%x\r\n",read_ahb32(RTC_CTRL),read_ahb32(TAMP_CTRL),read_ahb32(TAMP_STAT));
	
	write_ahb32(RTC_CMR,0xffff);
	write_ahb32(RTC_CLR ,0x0);
	write_ahb32(RTC_CPSR,0x0);
	write_ahb32(RTC_CCR,RTC_ENABLE|RTC_WRAP_ENABLE|RTC_INT_UNMASK|RTC_INT_ENABLE);   //使能rtc 

	while(1)
	{
		while(!(read_ahb32(RTC_STAT)&0x1)); 
		printf("RTC_STAT:0x%x\r\n",read_ahb32(RTC_STAT));
		read_ahb32(RTC_EOI);
	}	
    printf("\r\n");
}

