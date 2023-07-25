#include "sys.h"
#include "delay.h"
#include "sample_adc.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_adc_t9
 * Function �ж�-FIFO underflow
 * @author  licuihuang
 * @date    2022-6-24
 * @param   void
 * @return  void
*******************************************************************************/
void sample_adc_t9(void)
{	
	u32 i=0,j=0,u;
	adc1_nvic_init(21);  //ADC1_IRQHandler
	
	write_ahb32(ADC_CTRL,ADC_CTRL_FIFO_5|ADC_CTRL_underint);   //FIFO��ֵΪ��ʹ�������ж�
	write_ahb32(ADC_CH2,ADC_CH2_CHALEN_1);    //ת��ͨ����
	write_ahb32(ADC_CH0,0x5);                 //��ת����ͨ�� 
	write_ahb32(ADC_MODE,ADC_MODE_SINGLE);    //����ģʽ��Ч������ģʽ�Ῠ��busyλ
	
	while(read_ahb32(ADC_STAT)&ADC_STAT_BUSY);	    //������data ready�ж�ת����Ҫ��data readyλ��һ������
	write_ahb32(ADC_MODE,0);                        //busy��ת3��ͨ�����ң�data readyλ�ж�����ת5��ͨ��
	while((read_ahb32(ADC_STAT)&0xf8)!=0x0)         //�ж�FIFO����ֵ�Ƿ�Ϊ0
	{
		i=read_ahb32(ADC_DATA);
		j=read_ahb32(ADC_STAT);
		printf("data:0x%x stat:0x%x\r\n",i,j);  //ÿ��ӡһ������FIFO����ֵ�ͻ�ݼ�
	}

	printf("data:0x%x ",read_ahb32(ADC_DATA));
	printf("stat:0x%x\r\n",read_ahb32(ADC_STAT));	
	printf("\r\n");
}


