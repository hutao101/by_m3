#include "sys.h"
#include "delay.h"
#include "sample_rtc.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_rtc_t12
 * Function RTC_CMR与RTC_CPCVR关系
 * @author  licuihuang
 * @date    2022-7-19
 * @param   void
 * @return  void
*******************************************************************************/
void sample_rtc_t12(void)
{	
	u32 reg;
	nvic_rtc_init(19);
	write_ahb32(0x4001ae04,0x800);  //后备域写访问
	
	write_ahb32(0x4001a804,0x0);//ffff);  //CMR
	write_ahb32(RTC_CLR,0x0);
	write_ahb32(RTC_CPSR,0x0);
	write_ahb32(0x4001a824,0xffff);  //CPCVR
	write_ahb32(RTC_CCR,RTC_PSCLR_ENABLE|RTC_WRAP_ENABLE|RTC_INT_UNMASK|RTC_INT_ENABLE); //RTC_ENABLE
	reg=read_ahb32(RTC_CCR);
    write_ahb32(RTC_CCR,reg|RTC_ENABLE); 
	
    printf("cmr:0x%x cpcvr:0x%x\r\n",read_ahb32(0x4001a804),read_ahb32(0x4001a824));		
    printf("\r\n");
}


