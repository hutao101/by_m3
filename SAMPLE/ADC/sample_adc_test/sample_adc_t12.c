#include "sys.h"
#include "delay.h"
#include "sample_adc.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_adc_t12
 * Function 正弦波频率测试-连续模式
 * @author  licuihuang
 * @date    2022-7-14
 * @param   void
 * @return  void
*******************************************************************************/
void sample_adc_t12(void)
{	
	printf("continue mode\r\n");
	printf("sample:2ms");	
	
	write_ahb32(ADC_CTRL,0);
	write_ahb32(ADC_CTRL,ADC_CTRL_FIFO_1|0x3);   //FIFO阈值 ADC_CTRL_FIFO_6
//  write_ahb32(ADC_CH2,ADC_CH2_CHALEN_1);       //转换通道个数
//  write_ahb32(ADC_CH0,0x1);                    //需转换的通道

	SysTick_Config(72000);
	write_ahb32(ADC_MODE,ADC_MODE_CONTIN);       //连续模式
	
//	while(!(read_ahb32(ADC_STAT)&ADC_STAT_ready)); //不能用ADC_STAT_BUSY标志来判断，转完第一个通道此标志就为0了
	printf("\r\n");
}	
	
	