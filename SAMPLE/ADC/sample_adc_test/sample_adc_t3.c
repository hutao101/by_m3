#include "sys.h"
#include "delay.h"
#include "sample_adc.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_adc_t3
 * Function ��ͨ��ת��
 * @author  licuihuang
 * @date    2022-6-15
 * @param   void
 * @return  void
*******************************************************************************/
void sample_adc_t3(void)
{	
	write_ahb32(ADC_CTRL,ADC_CTRL_FIFO_5);   //FIFO��ֵ
	write_ahb32(ADC_CH2,ADC_CH2_CHALEN_2);   //ת��ͨ������ͨ��ת����
	write_ahb32(ADC_CH0,0x32);               //��ת����ͨ������
	write_ahb32(ADC_MODE,ADC_MODE_SCAN);     //ɨ��ģʽ
	
    printf("ctrl:0x%x mode:0x%x\r\n",read_ahb32(ADC_CTRL),read_ahb32(ADC_MODE));	
    while((read_ahb32(ADC_STAT)&ADC_STAT_underflow) == RESET)	//��FIFOû������ʱ
	{
		printf("data:0x%x stat:0x%x\r\n",read_ahb32(ADC_DATA),read_ahb32(ADC_STAT));
	}
	printf("0x%x 0x%x\r\n",read_ahb32(ADC_DATA),read_ahb32(ADC_STAT));
	
	printf("ctrl:0x%x mode:0x%x stat:0x%x ch0:0x%x ch1:0x%x ch2:0x%x\r\n",read_ahb32(ADC_CTRL),read_ahb32(ADC_MODE),\
	read_ahb32(ADC_STAT),read_ahb32(ADC_CH0),read_ahb32(ADC_CH1),read_ahb32(ADC_CH2));
	
	printf("\r\n");
}	
	


	