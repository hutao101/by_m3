#include "sys.h"
#include "delay.h"
#include "sample_rtc.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_rtc_t7
 * Function RTC_CCR[7:5]保护级别
 * @author  licuihuang
 * @date    2022-7-19
 * @param   void
 * @return  void
*******************************************************************************/
void sample_rtc_t7(void)
{	
	u32 i,j,k,reg;		
	write_ahb32(0x4001ae04,0x800);  //后备域写访问
	
	write_ahb32(RTC_CMR,0x0);
	write_ahb32(RTC_CLR,0x0);
	write_ahb32(RTC_CPSR,0x0);
	write_ahb32(RTC_CCR,0xe0|RTC_ENABLE);  //保护级别设置与RTC使能
	
	printf("ccr:0x%x\r\n",read_ahb32(RTC_CCR));
    printf("\r\n");
}


