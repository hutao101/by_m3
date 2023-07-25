#include "sys.h"
#include "delay.h"
#include "sample_adc.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_adc_t13
 * Function FIFO运行
 * @author  licuihuang
 * @date    2022-6-14
 * @param   void
 * @return  void
*******************************************************************************/
void sample_adc_t13(void)
{	
	u32 i=0,j=0;
	write_ahb32(ADC_CTRL,ADC_CTRL_FIFO_10);   //FIFO阈值
	write_ahb32(ADC_CH2,ADC_CH2_CHALEN_6);    //转换通道个数
	write_ahb32(ADC_CH0,0x321441);            //需转换的通道
	write_ahb32(ADC_MODE,ADC_MODE_SCAN|ADC_MODE_SINGLE);
	
	while((read_ahb32(ADC_STAT)&ADC_STAT_BUSY));	//等待转换完成	
	if((read_ahb32(ADC_STAT)&0xf8)!=0x0)            //判断FIFO计数值是否为0
	{
		for(i=0;i<2;i++)
		{
			printf("data:0x%x ",read_ahb32(ADC_DATA));
			printf("state:0x%x\r\n",read_ahb32(ADC_STAT));		
		}
	}

//	printf("data:0x%x ",read_ahb32(ADC_DATA));
	printf("state:0x%x\r\n",read_ahb32(ADC_STAT));	
	
	write_ahb32(ADC_CH2,ADC_CH2_CHALEN_2);
	write_ahb32(ADC_CH0,0x20); 
	write_ahb32(ADC_MODE,ADC_MODE_SCAN|ADC_MODE_SINGLE);
	while(read_ahb32(ADC_STAT)&ADC_STAT_BUSY);	    //等待转换完成
	while(read_ahb32(ADC_MODE));
	
	for(j=0;j<7;j++)
		{
			printf("data:0x%x ",read_ahb32(ADC_DATA));
			printf("state:0x%x\r\n",read_ahb32(ADC_STAT));		
		}
}	


	