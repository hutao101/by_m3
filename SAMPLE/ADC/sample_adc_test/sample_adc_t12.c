#include "sys.h"
#include "delay.h"
#include "sample_adc.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_adc_t12
 * Function ���Ҳ�Ƶ�ʲ���-����ģʽ
 * @author  licuihuang
 * @date    2022-7-14
 * @param   void
 * @return  void
*******************************************************************************/
void sample_adc_t12(void)
{	
	printf("continue mode\r\n");
	printf("sample:2ms");	
	
	write_ahb32(ADC_CTRL,0);
	write_ahb32(ADC_CTRL,ADC_CTRL_FIFO_1|0x3);   //FIFO��ֵ ADC_CTRL_FIFO_6
//  write_ahb32(ADC_CH2,ADC_CH2_CHALEN_1);       //ת��ͨ������
//  write_ahb32(ADC_CH0,0x1);                    //��ת����ͨ��

	SysTick_Config(72000);
	write_ahb32(ADC_MODE,ADC_MODE_CONTIN);       //����ģʽ
	
//	while(!(read_ahb32(ADC_STAT)&ADC_STAT_ready)); //������ADC_STAT_BUSY��־���жϣ�ת���һ��ͨ���˱�־��Ϊ0��
	printf("\r\n");
}	
	
	