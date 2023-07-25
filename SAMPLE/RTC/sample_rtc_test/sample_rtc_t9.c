#include "sys.h"
#include "delay.h"
#include "sample_rtc.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_rtc_t9
 * Function TAMPER输出秒脉冲
 * @author  licuihuang
 * @date    2022-7-19
 * @param   void
 * @return  void
*******************************************************************************/
void sample_rtc_t9(void)
{	
	u32 reg;	
//	nvic_rtc_init(42);
	write_ahb32(0x4001ae04,0x800);  //后备域写访问
	
	printf("rtc_ctrl:0x%x 0x%x 0x%x\r\n",read_ahb32(RTC_CTRL),read_ahb32(TAMP_CTRL),read_ahb32(TAMP_STAT));
	write_ahb32(RTC_CTRL,0x7fff0300);       //频率为32.768kh计数0x7ff次刚好为1s，不打开秒中断使能位
	printf("rtc_ctrl:0x%x 0x%x 0x%x\r\n",read_ahb32(RTC_CTRL),read_ahb32(TAMP_CTRL),read_ahb32(TAMP_STAT));
	
	write_ahb32(RTC_CMR,0x0);
	write_ahb32(RTC_CLR ,0x0);
	write_ahb32(RTC_CPSR,0x0);
//	write_ahb32(RTC_CCR,RTC_ENABLE);//使能rtc 
	
	printf("ccr:0x%x\r\n",read_ahb32(RTC_CCR));
	delay_ms(1000);
	printf("ccr:0x%x\r\n",read_ahb32(RTC_CCR));

	while(1)
	{
		while(!(read_ahb32(RTC_CTRL)&0x800));    //判断秒标志
		reg=read_ahb32(RTC_CTRL);
		printf("rtc_ctrl:0x%x ",reg);
		write_ahb32(RTC_CTRL,reg|0x800);         //清秒标志
		printf("0x%x\r\n",read_ahb32(RTC_CTRL));
	}
	printf("\r\n");
}

