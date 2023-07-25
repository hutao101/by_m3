#include "sample_wdt.h"


//test for Interrupt

u8 wdt_irq_count_t4;

void sample_wdt_t4(void)
{
	struct wdt_cfg_param param ={0};
	
	printf("WWDT T4 begin \r\n");
	
	param.timeout_us = 600000;  //60ms timeout period	
	param.rst_time_ns = 500;  //500ns reset pulse width
	param.intr_en = 1;
	
	snps_wdt_init(&param);
		
	printf("WDT will ENABLE\r\n");
	snps_wdt_restart();
	//enable the WDT
	snps_wdt_enable();

	while(1)
	{
		snps_wdt_restart();
		delay_ms(40);
		printf("WDT_BASE_ADDR+WDT_CCVR = 0x%x\r\n\r\n",read_ahb32(WDT_BASE_ADDR+SNPS_WDT_CCVR));
		
	}
}


/**********************************************************************
system WDT interrupt handler
使用t4时需要使用中断，在不使用t4时为避免重定义应该将ISR屏蔽
*********************************************************************/
//void WWDG_IRQHandler(void) __irq 
//{
//	if (wdt_irq_count_t4 == 0)
//	{
//		//first timeout interrupt, clear the interrupt
//		snps_wdt_intr_clear();
//		printf("wdt_irq_count_t1 = 0x%x\r\n",wdt_irq_count_t4);
//	}
//	else						
//	{
//		//second timeout interrupt, clear the interrupt and restart the WDT
//		snps_wdt_intr_clear();
//		snps_wdt_restart();
//		printf("wdt_irq_count_t1 = 0x%x\r\n",wdt_irq_count_t4);
//		wdt_irq_count_t4 = 0 ;
//	}
//	//increase the WDT interrupt flag
//	printf("wdt_irq_count_t1 = 0x%x\r\n",wdt_irq_count_t4);
//	printf("the wdt has come in interrupt\r\n\r\n\r\n");
//	wdt_irq_count_t4++;
//}
