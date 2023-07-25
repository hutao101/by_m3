#include "sys.h"
#include "delay.h"
#include "sample_adc.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_adc_t9
 * Function 中断-FIFO underflow
 * @author  licuihuang
 * @date    2022-6-24
 * @param   void
 * @return  void
*******************************************************************************/
void sample_adc_t9(void)
{	
	u32 i=0,j=0,u;
	adc1_nvic_init(21);  //ADC1_IRQHandler
	
	write_ahb32(ADC_CTRL,ADC_CTRL_FIFO_5|ADC_CTRL_underint);   //FIFO阈值为，使能下溢中断
	write_ahb32(ADC_CH2,ADC_CH2_CHALEN_1);    //转换通道数
	write_ahb32(ADC_CH0,0x5);                 //需转换的通道 
	write_ahb32(ADC_MODE,ADC_MODE_SINGLE);    //单次模式有效，连续模式会卡在busy位
	
	while(read_ahb32(ADC_STAT)&ADC_STAT_BUSY);	    //不能用data ready判断转换，要不data ready位会一起起来
	write_ahb32(ADC_MODE,0);                        //busy会转3个通道左右，data ready位判断至少转5个通道
	while((read_ahb32(ADC_STAT)&0xf8)!=0x0)         //判断FIFO计数值是否为0
	{
		i=read_ahb32(ADC_DATA);
		j=read_ahb32(ADC_STAT);
		printf("data:0x%x stat:0x%x\r\n",i,j);  //每打印一次数据FIFO计数值就会递减
	}

	printf("data:0x%x ",read_ahb32(ADC_DATA));
	printf("stat:0x%x\r\n",read_ahb32(ADC_STAT));	
	printf("\r\n");
}


