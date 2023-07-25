#include "sys.h"
#include "delay.h"
#include "sample_rtc.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_rtc_t6
 * Function 中断(rtc_19)
 * @author  licuihuang
 * @date    2022-7-19
 * @param   void
 * @return  void
*******************************************************************************/
void sample_rtc_t6(void)
{	
	u32 reg;	
	nvic_rtc_init(19);
	write_ahb32(0x4001ae04,0x800);      //后备域写访问
	
	write_ahb32(RTC_CMR,0x1ffff);       //计数到0x1ffff进入匹配中断(4s)
	write_ahb32(RTC_CLR,0x0);           //从0开始向上计数
	write_ahb32(RTC_CTRL,0x7fff0400);   //使能秒中断  测试秒中断能不能进rtc全局中断函数
//	write_ahb32(RTC_CPSR,0x0);          //无此reg
	write_ahb32(RTC_CCR,RTC_WRAP_ENABLE|RTC_INT_UNMASK|RTC_INT_ENABLE); //使能匹配中断
	reg=read_ahb32(RTC_CCR);
	write_ahb32(RTC_CCR,reg|RTC_ENABLE);

	delay_ms(5000);                     //判断能不能出中断函数
	printf("cmr:0x%x clr:0x%x cpsr:0x%x ccr:0x%x\r\n",read_ahb32(RTC_CMR),\
	read_ahb32(RTC_CLR),read_ahb32(RTC_CPSR),read_ahb32(RTC_CCR));
	
//	printf("stat:0x%x raw:0x%x\r\n",read_ahb32(RTC_STAT),read_ahb32(RTC_RSTAT));
    printf("\r\n");
}

      
