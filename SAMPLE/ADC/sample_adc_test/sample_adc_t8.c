#include "sys.h"
#include "delay.h"
#include "sample_adc.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_adc_t8
 * Function �ж�-FIFO overflow
 * @author  licuihuang
 * @date    2022-6-24
 * @param   void
 * @return  void
*******************************************************************************/
void sample_adc_t8(void)
{	
	u32 i=0,j=0,u;
	adc1_nvic_init(21);  //ADC1_IRQHandler
	
	write_ahb32(ADC_CTRL,ADC_CTRL_FIFO_3|ADC_CTRL_overint);   //FIFO��ֵ,ʹ�������ж�
	write_ahb32(ADC_CH2,ADC_CH2_CHALEN_1);   //ת��ͨ����
	write_ahb32(ADC_CH0,0x5);                //ת����ͨ��
	write_ahb32(ADC_MODE,ADC_MODE_CONTIN);   //����ģʽ
	
    while(!(read_ahb32(ADC_STAT)&ADC_STAT_ready));	//�ȴ���־��������������
//	while((read_ahb32(ADC_STAT)&ADC_STAT_BUSY));    //����ж�ת���Ƿ����Ҳ�У���֤��ת��һ��ͨ���������������㹻����ʱ
	delay_ms(3);
	write_ahb32(ADC_MODE,0);

    printf("stat:0x%x\r\n",read_ahb32(ADC_STAT));	
	printf("\r\n");
}	
	

	