#include "sys.h"
#include "delay.h"
#include "sample_adc.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_adc_t1
 * Function ��ͨ��ת��
 * @author  licuihuang
 * @date    2022-6-14
 * @param   void
 * @return  void
*******************************************************************************/
void sample_adc_t1(void)
{	
	u32 i=0,j=0,u,VAL1,VAL2;
	SysTick->VAL = 0x0;
    for(u=0x0;u<0x1f;u=u+0x1)   //��ӡ��ͨ����ʼֵ
	{
		printf("u:0x%x ",u);
		write_ahb32(ADC_CTRL,0);
		write_ahb32(ADC_CTRL,ADC_CTRL_FIFO_1);   //FIFO��ֵ
		write_ahb32(ADC_CH2,ADC_CH2_CHALEN_1);   //��ת����ͨ����
		write_ahb32(ADC_CH0,u);                  //��ת����ͨ����
		write_ahb32(ADC_MODE,ADC_MODE_SINGLE);   //����ģʽ
		
		while((read_ahb32(ADC_STAT)&ADC_STAT_BUSY));   //�ȴ�ת�����
		while((read_ahb32(ADC_STAT)&0xf8)!=0x0)        //�ж�FIFO����ֵ�Ƿ�Ϊ0
		{
			i=read_ahb32(ADC_DATA);
	//		j=read_ahb32(ADC_STAT);
			printf("0x%x ",i);   
	//		printf("data:0x%x stat:0x%x\r\n",i,j);  //ÿ��ӡһ������FIFO����ֵ�ͻ�ݼ�
		}
		
		while(read_ahb32(ADC_MODE));    //�ȴ�����ģʽ�ر�
    }	
	printf("\r\n");
}	


	