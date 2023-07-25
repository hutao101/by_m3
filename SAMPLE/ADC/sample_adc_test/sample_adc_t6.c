#include "sys.h"
#include "delay.h"
#include "sample_adc.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_adc_t6
 * Function 扫描模式
 * @author  licuihuang
 * @date    2022-6-21
 * @param   void
 * @return  void
*******************************************************************************/
void sample_adc_t6(void)
{	
	u32 i=0,j=0,u[5];
	
	write_ahb32(ADC_CTRL,ADC_CTRL_FIFO_10);   //FIFO阈值 ADC_CTRL_FIFO_6
	write_ahb32(ADC_CH2,ADC_CH2_CHALEN_5);    //转换通道个数
	write_ahb32(ADC_CH0,0x520c41);            //需转换的通道(12345,1先转)
	write_ahb32(ADC_MODE,ADC_MODE_SCAN);      //ADC_MODE_SINGLE|ADC_MODE_CONTIN|ADC_MODE_SCAN|ADC_MODE_DISCON);   //扫描模式		
	
	printf("ctrl:0x%x mode:0x%x stat:0x%x ch0:0x%x ch1:0x%x ch2:0x%x\r\n",read_ahb32(ADC_CTRL),read_ahb32(ADC_MODE),\
	read_ahb32(ADC_STAT),read_ahb32(ADC_CH0),read_ahb32(ADC_CH1),read_ahb32(ADC_CH2));	
	while(1)	
	{	
		while(read_ahb32(ADC_STAT)&ADC_STAT_BUSY);	
		printf("0x%x 0x%x\r\n",read_ahb32(ADC_STAT),read_ahb32(ADC_MODE));	
		
		while((read_ahb32(ADC_STAT)&0xf8)!=0x0)    //判断FIFO计数值是否为0
		{
			i=read_ahb32(ADC_DATA);
			delay_ms(2);
			j=read_ahb32(ADC_STAT);
			printf("data:0x%x stat:0x%x\r\n",i,j);  //每打印一次数据FIFO计数值就会递减
		}
		printf("\r\n");
		
		printf("ctrl:0x%x mode:0x%x stat:0x%x ch0:0x%x ch1:0x%x ch2:0x%x\r\n",read_ahb32(ADC_CTRL),read_ahb32(ADC_MODE),\
		read_ahb32(ADC_STAT),read_ahb32(ADC_CH0),read_ahb32(ADC_CH1),read_ahb32(ADC_CH2));
		
		if(!read_ahb32(ADC_MODE))
		{			
			write_ahb32(ADC_MODE,ADC_MODE_SCAN);    //扫描模式转换完所有通道会自动关闭
			printf("ctrl:0x%x mode:0x%x stat:0x%x ch0:0x%x ch1:0x%x ch2:0x%x\r\n",read_ahb32(ADC_CTRL),read_ahb32(ADC_MODE),\
			read_ahb32(ADC_STAT),read_ahb32(ADC_CH0),read_ahb32(ADC_CH1),read_ahb32(ADC_CH2));
		}
	}	
	
	printf("\r\n");
}	
	

	