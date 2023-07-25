#include "sys.h"
#include "delay.h"
#include "sample_adc.h"
#include "stm32f10x_tim.h"

/*******************************************************************************
 * @brief   sample_adc_t7_5
 * Function 间断模式:T3_TRGO触发
 * @author  licuihuang
 * @date    2022-9-21
 * @param   void
 * @return  void
*******************************************************************************/
void sample_adc_t7_5(void)
{	
	u32 i=0,j=0,temp;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure = {0};
//	adc_tim_nvic_init(17);
//	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	
	temp = ADC_CTRL_FIFO_10 | ADC_EXTTRIG_Set | ADC_TrigConv_T3_TRGO | (2<<13); 
	write_ahb32(ADC_CTRL,temp);               //FIFO阈值 
	write_ahb32(ADC_CH2,ADC_CH2_CHALEN_5);    //转换通道个数
	write_ahb32(ADC_CH0,0xc520c41);           //需转换的通道,0x520c41即54321通道(1先转) 	
	write_ahb32(ADC_MODE,ADC_MODE_DISCON);    //间断模式	
    printf("ADC_STAT:0x%x ADC_MODE:0x%x ADC_CTRL:0x%x\r\n",read_ahb32(ADC_STAT),read_ahb32(ADC_MODE),read_ahb32(ADC_CTRL));

	TIM_TimeBaseStructure.TIM_Period = 20000; 
	TIM_TimeBaseStructure.TIM_Prescaler = 3600;    //周期为2s
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 	
	TIM_SelectOutputTrigger(TIM3,TIM_TRGOSource_Update);
	TIM_Cmd(TIM3, ENABLE);
	
	while(1)
	{   
		while((read_ahb32(ADC_STAT)&0xf8)!=0x0)    //不用重新触发转换，每2秒进行一轮转换(3/2个通道)
		{
//			while((read_ahb32(ADC_STAT)&ADC_STAT_BUSY));
		    j=read_ahb32(ADC_STAT);
			i=read_ahb32(ADC_DATA);
			printf("data0:0x%x stat0:0x%x\r\n\r\n",i,j);  
		}
//		else
	    {
//			printf("START CONV:\r\n"); 
//			printf("0x%x 0x%x\r\n",read_ahb32(ADC_MODE),read_ahb32(ADC_CTRL));  //除了CTRL[26]软件使能位会自动清0，其他位均不清
//		    temp = read_ahb32(ADC_CTRL);
//			temp |= ADC_CTRL_FIFO_10 | ADC_EXTTRIG_Set | ADC_TrigConv_T3_TRGO | (2<<13); 
//			write_ahb32(ADC_CTRL,temp);
//			write_ahb32(ADC_MODE,ADC_MODE_DISCON);
			delay_ms(1);
		}	
	}
}

