#include "sys.h"
#include "delay.h"
#include "sample_adc.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_adc_t10
 * Function �ж�-data ready
 * @author  licuihuang
 * @date    2022-6-24
 * @param   void
 * @return  void
*******************************************************************************/
void sample_adc_t10(void)
{	
	u32 i=0,j=0,u;
	adc1_nvic_init(21);  //ADC1_IRQHandler
	
	write_ahb32(ADC_CTRL,ADC_CTRL_FIFO_1|ADC_CTRL_datareadyint);   //FIFO��ֵ,ʹ������׼���ж�
	write_ahb32(ADC_CH2,ADC_CH2_CHALEN_1);   //ת��һ��ͨ������ͨ��ת����
	write_ahb32(ADC_CH0,0x5);                //��һ����ת����ͨ�� 0x8398a4
	write_ahb32(ADC_MODE,ADC_MODE_SINGLE);   //����ģʽ
	
    while((read_ahb32(ADC_STAT)&ADC_STAT_BUSY));   //�ȴ�����ת�����
	printf("stat:0x%x\r\n",read_ahb32(ADC_STAT));	
	printf("\r\n");
}


