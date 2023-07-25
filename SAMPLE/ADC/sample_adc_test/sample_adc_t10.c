#include "sys.h"
#include "delay.h"
#include "sample_adc.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_adc_t10
 * Function 中断-data ready
 * @author  licuihuang
 * @date    2022-6-24
 * @param   void
 * @return  void
*******************************************************************************/
void sample_adc_t10(void)
{	
	u32 i=0,j=0,u;
	adc1_nvic_init(21);  //ADC1_IRQHandler
	
	write_ahb32(ADC_CTRL,ADC_CTRL_FIFO_1|ADC_CTRL_datareadyint);   //FIFO阈值,使能数据准备中断
	write_ahb32(ADC_CH2,ADC_CH2_CHALEN_1);   //转换一个通道（单通道转换）
	write_ahb32(ADC_CH0,0x5);                //第一个需转换的通道 0x8398a4
	write_ahb32(ADC_MODE,ADC_MODE_SINGLE);   //单次模式
	
    while((read_ahb32(ADC_STAT)&ADC_STAT_BUSY));   //等待数据转换完成
	printf("stat:0x%x\r\n",read_ahb32(ADC_STAT));	
	printf("\r\n");
}


