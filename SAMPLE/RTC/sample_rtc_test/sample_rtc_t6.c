#include "sys.h"
#include "delay.h"
#include "sample_rtc.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_rtc_t6
 * Function �ж�(rtc_19)
 * @author  licuihuang
 * @date    2022-7-19
 * @param   void
 * @return  void
*******************************************************************************/
void sample_rtc_t6(void)
{	
	u32 reg;	
	nvic_rtc_init(19);
	write_ahb32(0x4001ae04,0x800);      //����д����
	
	write_ahb32(RTC_CMR,0x1ffff);       //������0x1ffff����ƥ���ж�(4s)
	write_ahb32(RTC_CLR,0x0);           //��0��ʼ���ϼ���
	write_ahb32(RTC_CTRL,0x7fff0400);   //ʹ�����ж�  �������ж��ܲ��ܽ�rtcȫ���жϺ���
//	write_ahb32(RTC_CPSR,0x0);          //�޴�reg
	write_ahb32(RTC_CCR,RTC_WRAP_ENABLE|RTC_INT_UNMASK|RTC_INT_ENABLE); //ʹ��ƥ���ж�
	reg=read_ahb32(RTC_CCR);
	write_ahb32(RTC_CCR,reg|RTC_ENABLE);

	delay_ms(5000);                     //�ж��ܲ��ܳ��жϺ���
	printf("cmr:0x%x clr:0x%x cpsr:0x%x ccr:0x%x\r\n",read_ahb32(RTC_CMR),\
	read_ahb32(RTC_CLR),read_ahb32(RTC_CPSR),read_ahb32(RTC_CCR));
	
//	printf("stat:0x%x raw:0x%x\r\n",read_ahb32(RTC_STAT),read_ahb32(RTC_RSTAT));
    printf("\r\n");
}

      
