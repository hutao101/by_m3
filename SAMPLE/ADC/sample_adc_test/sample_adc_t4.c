#include "sys.h"
#include "delay.h"
#include "sample_adc.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_adc_t4
 * Function 数据左对齐
 * @author  licuihuang
 * @date    2022-6-15
 * @param   void
 * @return  void
*******************************************************************************/
void sample_adc_t4(void)
{	
	u32 i=0,j=0,u;
	for(u=0x0;u<0x1f;u=u+0x1)   //打印各通道初始值
	{
		printf("u:0x%x ",u);
		write_ahb32(ADC_CTRL,ADC_CTRL_FIFO_1|ADC_CTRL_LEFT);   //FIFO阈值
		write_ahb32(ADC_CH2,ADC_CH2_CHALEN_1);                 //需转换的通道数
		write_ahb32(ADC_CH0,u);                                //需转换的通道号
		write_ahb32(ADC_MODE,ADC_MODE_SINGLE);                 //ADC_MODE_SINGLE|ADC_MODE_CONTIN|ADC_MODE_SCAN|ADC_MODE_DISCON);   //单次模式
		
//    while(1)                                         
	{
		while((read_ahb32(ADC_STAT)&ADC_STAT_BUSY));	//等待转换完成
		while((read_ahb32(ADC_STAT)&0xf8)!=0x0)         //判断FIFO计数值是否为0
		{
			i=read_ahb32(ADC_DATA);
			j=read_ahb32(ADC_STAT);
			printf("data:0x%x stat:0x%x\r\n",i,j);      //每打印一次数据FIFO计数值就会递减
		}
		printf("\r\n");
	
	//    if(!read_ahb32(ADC_MODE))
	//	{
	//		write_ahb32(ADC_MODE,ADC_MODE_SINGLE);         //单次模式转换完通道会自动关闭
	//	}	  
	}
	}

	printf("ctrl:0x%x mode:0x%x stat:0x%x ch0:0x%x ch1:0x%x ch2:0x%x\r\n",read_ahb32(ADC_CTRL),read_ahb32(ADC_MODE),\
	read_ahb32(ADC_STAT),read_ahb32(ADC_CH0),read_ahb32(ADC_CH1),read_ahb32(ADC_CH2));	
	printf("\r\n");
}	
	

	