#include "sys.h"
#include "delay.h"
#include "sample_rtc.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_rtc_t8
 * Function �ж�(rtc_second_42)
 * @author  licuihuang
 * @date    2022-7-19
 * @param   void
 * @return  void
*******************************************************************************/
void sample_rtc_t8(void)
{	
	nvic_rtc_init(42);
	write_ahb32(0x4001ae04,0x800);  //����д����
	
	write_ahb32(RTC_CMR,0x0);
	write_ahb32(RTC_CPSR,0x0);
	write_ahb32(RTC_CLR ,0x0);    //��rtcһֱ���ţ�������װ�غ�������ʹ�����жϣ�Ҫ����ӡ����ֵ����ƫ��
	write_ahb32(RTC_CTRL,0x7fff0400);   //���ж�ʹ�ܣ�ʱ��Ϊ1s
	write_ahb32(RTC_CCR,RTC_ENABLE);   //ʹ��rtc RTC_WRAP_ENABLE|RTC_INT_UNMASK|RTC_INT_ENABLE|


/*******�����жϲ���***********/	
//	while(!(read_ahb32(RTC_CTRL)&0x800));  //�ȴ����־Ϊ1
//	while(read_ahb32(RTC_CTRL)&0x800)
//	{
//		write_ahb32(RTC_CTRL,0x7fff0400);  //��RTC_CRTL[11]��־λд0�岻�����־
//		read_ahb32(RTC_CTRL);              //��RTC_CRTL�Ĵ����岻�����־
//		write_ahb32(RTC_CTRL,0x7fff0800);  //��RTC_CRTL[10]���ж�ʹ��λд0�������־
//		write_ahb32(RTC_CTRL,0x7fff0c00);  //��RTC_CRTL[11]��־λд1�������־,��ȷ����
//		printf("second:0x%x\r\n",read_ahb32(RTC_CTRL));  //���Ǳ�־λΪ0�����ӡ�˾�
//	}
//	printf("second:0x%x\r\n",read_ahb32(RTC_CTRL));  //���Ǳ�־λΪ0�����ӡ�˾�
/*******�����жϲ���***********/		

  printf("\r\n");
}

