#include "sys.h"
#include "delay.h"
#include "sample_rtc.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_rtc_t1
 * Function rtc向上计数
 * @author  licuihuang
 * @date    2022-7-16
 * @param   void
 * @return  void
*******************************************************************************/
void sample_rtc_t1(void)
{
	write_ahb32(0x4001ae04,0x800);  //后备域写访问
	
	write_ahb32(RTC_CMR,0x0);        
	write_ahb32(RTC_CLR,0xffffefff);
	write_ahb32(RTC_CPSR,0x0);       //无此reg
	write_ahb32(RTC_CCR,RTC_ENABLE); //RTC_ENABLE无效

	printf("cmr:0x%x clr:0x%x cpsr:0x%x ccr:0x%x\r\n",read_ahb32(RTC_CMR),\
	read_ahb32(RTC_CLR),read_ahb32(RTC_CPSR),read_ahb32(RTC_CCR));
    printf("CCVR:");
	while(1)
	{
		printf("0x%x ",read_ahb32(RTC_CCVR));
	}

    printf("\r\n\r\n"); 

}


