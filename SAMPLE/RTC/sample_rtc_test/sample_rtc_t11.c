#include "sys.h"
#include "delay.h"
#include "sample_rtc.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_rtc_t11
 * Function TAMPER���У׼ʱ��
 * @author  licuihuang
 * @date    2022-7-19
 * @param   void
 * @return  void
*******************************************************************************/
void sample_rtc_t11(void)
{	
	u32 i;	
//	nvic_rtc_init(19);
	write_ahb32(0x4001ae04,0x800);  //����д����
	
	printf("rtc_ctrl:0x%x 0x%x 0x%x\r\n",read_ahb32(RTC_CTRL),read_ahb32(TAMP_CTRL),read_ahb32(TAMP_STAT));
	write_ahb32(RTC_CTRL,0x7fff0080);       //Ƶ��Ϊ32.768kh����0x7ff�θպ�Ϊ1s�������ж�ʹ��λ
	printf("rtc_ctrl:0x%x 0x%x 0x%x\r\n",read_ahb32(RTC_CTRL),read_ahb32(TAMP_CTRL),read_ahb32(TAMP_STAT));
	
	write_ahb32(RTC_CMR,0x0);
	write_ahb32(RTC_CLR ,0x0);//0xfff);
	write_ahb32(RTC_CPSR,0x0);
	write_ahb32(RTC_CCR,RTC_ENABLE);   //ʹ��rtc 

	delay_ms(100); 
    printf("rtc_ctrl:0x%x\r\n",read_ahb32(RTC_CTRL));	
    printf("\r\n");
}


