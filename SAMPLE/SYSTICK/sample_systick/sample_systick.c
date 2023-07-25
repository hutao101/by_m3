#include "sys.h"
#include "delay.h"
#include "sample_systick.h"
#include "stm32f10x.h"

u32 systick_t4=0,systick_t9=0;
void systick_close(void)
{
	SysTick->CTRL = 0x0;
}


void systick_nvic_init(u32 channel)   //²»ÓÃ
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = channel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
}


void SysTick_Handler(void)
{
	if(systick_t4 == 1)
	{
//		printf("CTRL:0x%x\r\n",SysTick->CTRL);	
		SysTick->VAL=0xFF;
		printf("VAL change to 0xFF,CTRL:0x%x\r\n",SysTick->CTRL);
	}
	if(systick_t9 == 1)
	{
		u32 i;	
		i=SysTick->VAL;
		printf("0x%x\r\n",i);
	}
	
	printf("systick irq!!\r\n");
}

