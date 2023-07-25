#include "sys.h"
#include "delay.h"
#include "sample_rtc.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_rtc_t13
 * Function TAMPER输出秒脉冲(校准值为0x7f)
 * @author  licuihuang
 * @date    2022-8-29
 * @param   void
 * @return  void
*******************************************************************************/
void sample_rtc_t13(void)
{	
	u32 reg;	
//	nvic_rtc_init(42);
	write_ahb32(0x4001ae04,0x800);  //后备域写访问
	
	printf("rtc_ctrl:0x%x 0x%x 0x%x\r\n",read_ahb32(RTC_CTRL),read_ahb32(TAMP_CTRL),read_ahb32(TAMP_STAT));
	write_ahb32(RTC_CTRL,0x7fff037f);       //频率为32.768kh计数0x7ff次刚好为1s，打开秒中断使能位
	printf("rtc_ctrl:0x%x 0x%x 0x%x\r\n",read_ahb32(RTC_CTRL),read_ahb32(TAMP_CTRL),read_ahb32(TAMP_STAT));
	
	write_ahb32(RTC_CMR,0x0);
	write_ahb32(RTC_CLR ,0x0);
	write_ahb32(RTC_CPSR,0x0);
	write_ahb32(RTC_CCR,RTC_ENABLE);//|RTC_WRAP_ENABLE|RTC_INT_UNMASK|RTC_INT_ENABLE);   //使能rtc 

	while(1)
	{
		while(!(read_ahb32(RTC_CTRL)&0x800)); 
		printf("rtc_ctrl:0x%x ",read_ahb32(RTC_CTRL));
		write_ahb32(RTC_CTRL,0x7fff0f00);
		printf("0x%x\r\n",read_ahb32(RTC_CTRL));
	}
	printf("\r\n");
}

