#include "sys.h"
#include "delay.h"
#include "sample_adc.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_adc_t5
 * Function 连续模式
 * @author  licuihuang
 * @date    2022-6-15
 * @param   void
 * @return  void
*******************************************************************************/
void sample_adc_t5(void)
{	
	u32 i=0,j=0,u;
	write_ahb32(ADC_CTRL,0);
    write_ahb32(ADC_CTRL,ADC_CTRL_FIFO_10);   //FIFO阈值 ADC_CTRL_FIFO_6
	write_ahb32(ADC_CH2,ADC_CH2_CHALEN_1);   //转换通道个数
	write_ahb32(ADC_CH0,0x0);                //需转换的通道
	write_ahb32(ADC_MODE,ADC_MODE_CONTIN);//ADC_MODE_SINGLE|ADC_MODE_CONTIN|ADC_MODE_SCAN|ADC_MODE_DISCON);   //模式
	
	while(!(read_ahb32(ADC_STAT)&ADC_STAT_ready)); //不能用ADC_STAT_BUSY标志来判断，转完第一个通道此标志就为0了
	write_ahb32(ADC_MODE,0);				
	while((read_ahb32(ADC_STAT)&0xf8)!=0x0)  //判断FIFO计数值是否为0
	{
		u=read_ahb32(ADC_STAT);
		printf("stat:0x%x ",u);  //每打印一次数据FIFO计数值就会递减
		i=read_ahb32(ADC_DATA);
		printf("data:0x%x ",i);
		j=read_ahb32(ADC_STAT);
		printf("stat:0x%x\r\n",j);  //每打印一次数据FIFO计数值就会递减
	}

	read_ahb32(ADC_DATA);
	printf("stat:0x%x\r\n",read_ahb32(ADC_STAT));
	read_ahb32(ADC_STAT);
	read_ahb32(ADC_DATA);
	printf("stat:0x%x\r\n",read_ahb32(ADC_STAT));
	printf("\r\n");
}	
	

