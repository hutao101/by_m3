#include "sys.h"
#include "delay.h"
#include "sample_adc.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_adc_t8
 * Function 中断-FIFO overflow
 * @author  licuihuang
 * @date    2022-6-24
 * @param   void
 * @return  void
*******************************************************************************/
void sample_adc_t8(void)
{	
	u32 i=0,j=0,u;
	adc1_nvic_init(21);  //ADC1_IRQHandler
	
	write_ahb32(ADC_CTRL,ADC_CTRL_FIFO_3|ADC_CTRL_overint);   //FIFO阈值,使能上溢中断
	write_ahb32(ADC_CH2,ADC_CH2_CHALEN_1);   //转换通道数
	write_ahb32(ADC_CH0,0x5);                //转换的通道
	write_ahb32(ADC_MODE,ADC_MODE_CONTIN);   //连续模式
	
    while(!(read_ahb32(ADC_STAT)&ADC_STAT_ready));	//等待标志起来，不读数据
//	while((read_ahb32(ADC_STAT)&ADC_STAT_BUSY));    //这句判断转换是否完成也行，保证已转完一个通道，加上下面有足够的延时
	delay_ms(3);
	write_ahb32(ADC_MODE,0);

    printf("stat:0x%x\r\n",read_ahb32(ADC_STAT));	
	printf("\r\n");
}	
	

	