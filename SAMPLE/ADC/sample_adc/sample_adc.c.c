#include "sys.h"
#include "delay.h"
#include "sample_adc.h"
#include "stm32f10x.h"

u32 single_mode = 0;

void adc1_nvic_init(u32 channel)
{	
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = channel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 		//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure); 	
}


void adc_tim_nvic_init(u32 channel)
{
	NVIC_InitTypeDef NVIC_InitStruct;
	
	NVIC_InitStruct.NVIC_IRQChannel = channel; 
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0; 
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 3; 
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE; 	
	NVIC_Init(&NVIC_InitStruct);
}


void ADC1_IRQHandler(void)
{
	u32 reg;
	printf("ADC IRQ:  ");
	
	reg = read_ahb32(ADC_CTRL);	
	if(read_ahb32(ADC_STAT)&ADC_STAT_overflow)
	{
		printf("overflow irq\r\n");
		write_ahb32(ADC_CTRL,reg&(~(ADC_CTRL_overint)));//|ADC_CTRL_datareadyint))); //连续模式上溢标志与数据准备标志会一起起来
	}
	
	reg = read_ahb32(ADC_CTRL);
	if(read_ahb32(ADC_STAT)&ADC_STAT_underflow)
	{
		printf("underflow irq\r\n");
		write_ahb32(ADC_CTRL,reg&(~ADC_CTRL_underint));
	}
	
	reg = read_ahb32(ADC_CTRL);
  	if(read_ahb32(ADC_STAT)&ADC_STAT_ready)
	{
		printf("data ready irq\r\n");
		write_ahb32(ADC_CTRL,reg&(~(ADC_CTRL_datareadyint)));  //关闭中断使能位
	}
}


u32 k,irq=1;
void SysTick_Handler(void)
{
	if(single_mode)
	{
		write_ahb32(ADC_MODE,ADC_MODE_SINGLE);  //单次模式测试频率需打开这条语句
	}
		
	k=read_ahb32(ADC_DATA);
	printf("%d:0x%x ",irq,k);
	irq=irq+1;
}


void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_CC2) != RESET) 
		{
			printf("TIM2 CC2 IRQ!!\r\n");
			TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);  
		}
}


void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) 
		{
			printf("TIM3 UP IRQ!!\r\n");
			TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  
		}
}


void TIM4_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM4, TIM_IT_CC4) != RESET) 
		{
			printf("TIM4 CC4 IRQ!!\r\n");
			TIM_ClearITPendingBit(TIM4, TIM_IT_CC4);  
		}
}


