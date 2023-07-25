#include "sys.h"
#include "delay.h"
#include "sample_rtc.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_rtc_t9
 * Function TAMPER���������
 * @author  licuihuang
 * @date    2022-7-19
 * @param   void
 * @return  void
*******************************************************************************/
void sample_rtc_t9(void)
{	
	u32 reg;	
//	nvic_rtc_init(42);
	write_ahb32(0x4001ae04,0x800);  //����д����
	
	printf("rtc_ctrl:0x%x 0x%x 0x%x\r\n",read_ahb32(RTC_CTRL),read_ahb32(TAMP_CTRL),read_ahb32(TAMP_STAT));
	write_ahb32(RTC_CTRL,0x7fff0300);       //Ƶ��Ϊ32.768kh����0x7ff�θպ�Ϊ1s���������ж�ʹ��λ
	printf("rtc_ctrl:0x%x 0x%x 0x%x\r\n",read_ahb32(RTC_CTRL),read_ahb32(TAMP_CTRL),read_ahb32(TAMP_STAT));
	
	write_ahb32(RTC_CMR,0x0);
	write_ahb32(RTC_CLR ,0x0);
	write_ahb32(RTC_CPSR,0x0);
//	write_ahb32(RTC_CCR,RTC_ENABLE);//ʹ��rtc 
	
	printf("ccr:0x%x\r\n",read_ahb32(RTC_CCR));
	delay_ms(1000);
	printf("ccr:0x%x\r\n",read_ahb32(RTC_CCR));

	while(1)
	{
		while(!(read_ahb32(RTC_CTRL)&0x800));    //�ж����־
		reg=read_ahb32(RTC_CTRL);
		printf("rtc_ctrl:0x%x ",reg);
		write_ahb32(RTC_CTRL,reg|0x800);         //�����־
		printf("0x%x\r\n",read_ahb32(RTC_CTRL));
	}
	printf("\r\n");
}

