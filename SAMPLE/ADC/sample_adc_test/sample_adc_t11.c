#include "sys.h"
#include "delay.h"
#include "sample_adc.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_adc_t11
 * Function 正弦波频率测试-单次模式
 * @author  licuihuang
 * @date    2022-7-13
 * @param   void
 * @return  void
*******************************************************************************/
extern u32 single_mode;

void sample_adc_t11(void)
{	
	printf("single mode\r\n");
	printf("sample:2ms");	 
	single_mode = 1;
	
	write_ahb32(ADC_CTRL,0);
	write_ahb32(ADC_CTRL,ADC_CTRL_FIFO_1|0x3);   //FIFO阈值 ADC_CTRL_FIFO_6
//  write_ahb32(ADC_CH2,ADC_CH2_CHALEN_1);   //转换通道个数
//  write_ahb32(ADC_CH0,0x3);                //需转换的通道

	SysTick_Config(72000);
	write_ahb32(ADC_MODE,ADC_MODE_SINGLE);
	printf("\r\n");
}	
	
	
