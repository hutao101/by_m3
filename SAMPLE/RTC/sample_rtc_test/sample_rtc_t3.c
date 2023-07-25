#include "sys.h"
#include "delay.h"
#include "sample_rtc.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_rtc_t3
 * Function 预分频计数器再编程(CPSR小于等于CPCVR)
 * @author  licuihuang
 * @date    2022-7-19
 * @param   void
 * @return  void
*******************************************************************************/
void sample_rtc_t3(void)
{
  u32 reg;	
	write_ahb32(0x4001ae04,0x800);  //后备域写访问
	
	write_ahb32(RTC_CMR,0x0);
	write_ahb32(RTC_CLR,0xffff);
	write_ahb32(RTC_CPSR,0xfff);
	write_ahb32(RTC_CCR,RTC_PSCLR_ENABLE|RTC_ENABLE); //RTC_ENABLE

	printf("cmr:0x%x clr:0x%x cpsr:0x%x ccr:0x%x\r\n",read_ahb32(RTC_CMR),\
	read_ahb32(RTC_CLR),read_ahb32(RTC_CPSR),read_ahb32(RTC_CCR));
	
	printf("CPCVR:\r\n");
  while(1)
	{
		reg=read_ahb32(RTC_CPCVR);
		printf("0x%x ",reg);
		if(reg==0xfff)
		{
			write_ahb32(RTC_CPSR,0x9ff);
		}
	}
	
  printf("\r\n");
}


