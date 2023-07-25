#include "sys.h"
#include "delay.h"
#include "sample_adc.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_adc_t5
 * Function ����ģʽ
 * @author  licuihuang
 * @date    2022-6-15
 * @param   void
 * @return  void
*******************************************************************************/
void sample_adc_t5(void)
{	
	u32 i=0,j=0,u;
	write_ahb32(ADC_CTRL,0);
    write_ahb32(ADC_CTRL,ADC_CTRL_FIFO_10);   //FIFO��ֵ ADC_CTRL_FIFO_6
	write_ahb32(ADC_CH2,ADC_CH2_CHALEN_1);   //ת��ͨ������
	write_ahb32(ADC_CH0,0x0);                //��ת����ͨ��
	write_ahb32(ADC_MODE,ADC_MODE_CONTIN);//ADC_MODE_SINGLE|ADC_MODE_CONTIN|ADC_MODE_SCAN|ADC_MODE_DISCON);   //ģʽ
	
	while(!(read_ahb32(ADC_STAT)&ADC_STAT_ready)); //������ADC_STAT_BUSY��־���жϣ�ת���һ��ͨ���˱�־��Ϊ0��
	write_ahb32(ADC_MODE,0);				
	while((read_ahb32(ADC_STAT)&0xf8)!=0x0)  //�ж�FIFO����ֵ�Ƿ�Ϊ0
	{
		u=read_ahb32(ADC_STAT);
		printf("stat:0x%x ",u);  //ÿ��ӡһ������FIFO����ֵ�ͻ�ݼ�
		i=read_ahb32(ADC_DATA);
		printf("data:0x%x ",i);
		j=read_ahb32(ADC_STAT);
		printf("stat:0x%x\r\n",j);  //ÿ��ӡһ������FIFO����ֵ�ͻ�ݼ�
	}

	read_ahb32(ADC_DATA);
	printf("stat:0x%x\r\n",read_ahb32(ADC_STAT));
	read_ahb32(ADC_STAT);
	read_ahb32(ADC_DATA);
	printf("stat:0x%x\r\n",read_ahb32(ADC_STAT));
	printf("\r\n");
}	
	

