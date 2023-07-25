#include "sys.h"
#include "delay.h"
#include "sample_rtc.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_rtc_t11
 * Function TAMPER输出校准时钟
 * @author  licuihuang
 * @date    2022-7-19
 * @param   void
 * @return  void
*******************************************************************************/
void sample_rtc_t11(void)
{	
	u32 i;	
//	nvic_rtc_init(19);
	write_ahb32(0x4001ae04,0x800);  //后备域写访问
	
	printf("rtc_ctrl:0x%x 0x%x 0x%x\r\n",read_ahb32(RTC_CTRL),read_ahb32(TAMP_CTRL),read_ahb32(TAMP_STAT));
	write_ahb32(RTC_CTRL,0x7fff0080);       //频率为32.768kh计数0x7ff次刚好为1s，打开秒中断使能位
	printf("rtc_ctrl:0x%x 0x%x 0x%x\r\n",read_ahb32(RTC_CTRL),read_ahb32(TAMP_CTRL),read_ahb32(TAMP_STAT));
	
	write_ahb32(RTC_CMR,0x0);
	write_ahb32(RTC_CLR ,0x0);//0xfff);
	write_ahb32(RTC_CPSR,0x0);
	write_ahb32(RTC_CCR,RTC_ENABLE);   //使能rtc 

	delay_ms(100); 
    printf("rtc_ctrl:0x%x\r\n",read_ahb32(RTC_CTRL));	
    printf("\r\n");
}


