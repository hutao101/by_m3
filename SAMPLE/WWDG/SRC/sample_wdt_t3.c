#include "sample_wdt.h"


//test for Interrupt

u8 wdt_irq_count_t3;

void sample_wdt_t3(void)
{
	struct wdt_cfg_param param ={0};
	
	printf("WWDT T3 begin\r\n");
	
	param.timeout_us = 5000000;  //60ms timeout period	
	param.rst_time_ns = 500;  //500ns reset pulse width
	param.intr_en = 1;
	
	snps_wdt_init(&param);
		
	printf("wdt will enable\r\n\r\n");
	
	snps_wdt_restart();	
	//enable the WDT
	snps_wdt_enable();

	while(1)
	{
		printf("go\r\n");
		delay_ms(2000);
	}
}


/**********************************************************************
system WDT interrupt handler
仅限T3使用，不用时关闭
*********************************************************************/
//void WWDG_IRQHandler(void) __irq 
//{
//	printf("wdt_irq_count_t3 = 0x%x\r\n",wdt_irq_count_t3);
//	printf("the wdt has come in interrupt\r\n\r\n\r\n");
//	wdt_irq_count_t3++;
//	snps_wdt_intr_clear();
//}
