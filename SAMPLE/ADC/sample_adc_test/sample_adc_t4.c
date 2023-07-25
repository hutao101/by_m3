#include "sys.h"
#include "delay.h"
#include "sample_adc.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_adc_t4
 * Function ���������
 * @author  licuihuang
 * @date    2022-6-15
 * @param   void
 * @return  void
*******************************************************************************/
void sample_adc_t4(void)
{	
	u32 i=0,j=0,u;
	for(u=0x0;u<0x1f;u=u+0x1)   //��ӡ��ͨ����ʼֵ
	{
		printf("u:0x%x ",u);
		write_ahb32(ADC_CTRL,ADC_CTRL_FIFO_1|ADC_CTRL_LEFT);   //FIFO��ֵ
		write_ahb32(ADC_CH2,ADC_CH2_CHALEN_1);                 //��ת����ͨ����
		write_ahb32(ADC_CH0,u);                                //��ת����ͨ����
		write_ahb32(ADC_MODE,ADC_MODE_SINGLE);                 //ADC_MODE_SINGLE|ADC_MODE_CONTIN|ADC_MODE_SCAN|ADC_MODE_DISCON);   //����ģʽ
		
//    while(1)                                         
	{
		while((read_ahb32(ADC_STAT)&ADC_STAT_BUSY));	//�ȴ�ת�����
		while((read_ahb32(ADC_STAT)&0xf8)!=0x0)         //�ж�FIFO����ֵ�Ƿ�Ϊ0
		{
			i=read_ahb32(ADC_DATA);
			j=read_ahb32(ADC_STAT);
			printf("data:0x%x stat:0x%x\r\n",i,j);      //ÿ��ӡһ������FIFO����ֵ�ͻ�ݼ�
		}
		printf("\r\n");
	
	//    if(!read_ahb32(ADC_MODE))
	//	{
	//		write_ahb32(ADC_MODE,ADC_MODE_SINGLE);         //����ģʽת����ͨ�����Զ��ر�
	//	}	  
	}
	}

	printf("ctrl:0x%x mode:0x%x stat:0x%x ch0:0x%x ch1:0x%x ch2:0x%x\r\n",read_ahb32(ADC_CTRL),read_ahb32(ADC_MODE),\
	read_ahb32(ADC_STAT),read_ahb32(ADC_CH0),read_ahb32(ADC_CH1),read_ahb32(ADC_CH2));	
	printf("\r\n");
}	
	

	