#include "sample_wdt.h"


//test for Interrupt

u8 wdt_irq_count_t1;

void sample_wdt_t1(void)
{
	struct wdt_cfg_param param ={0};
	u32 reg = 0;
	
	printf("WWDT T1 begin \r\n");
	
	param.timeout_us = 6000000;  //60ms timeout period	
	param.rst_time_ns = 500;  //500ns reset pulse width
	param.intr_en = 1;
	
	snps_wdt_init(&param);
	
	reg = read_ahb32(WDT_BASE_ADDR+SNPS_WDT_TORR);
	reg |= (0xC << 4);
	printf("reg = 0x%x\r\n",reg);
	write_ahb32(WDT_BASE_ADDR+SNPS_WDT_TORR, reg);
	printf("TORR = 0x%x\r\n",read_ahb32(WDT_BASE_ADDR+SNPS_WDT_TORR));
	
	//enable the WDT
	snps_wdt_restart();	
	snps_wdt_enable();

	while(1);
}


/**********************************************************************
system WDT interrupt handler
仅限T1使用，不用时屏蔽
*********************************************************************/
//void WWDG_IRQHandler(void) __irq 
//{
//	snps_wdt_restart();	
//	printf("0000\r\n");
//	if (wdt_irq_count_t1 == 0)
//	{
//		//first timeout interrupt, clear the interrupt
//		snps_wdt_intr_clear();
//		printf("wdt_irq_count_t1 = 0x%x\r\n",wdt_irq_count_t1);
//	}
//	else						
//	{
//		//second timeout interrupt, clear the interrupt and restart the WDT
//		snps_wdt_intr_clear();
//		printf("wdt_irq_count_t1 = 0x%x\r\n",wdt_irq_count_t1);
//		wdt_irq_count_t1 = 0 ;
//		snps_wdt_restart();
//	}
//	//increase the WDT interrupt flag
//	printf("wdt_irq_count_t1 = 0x%x\r\n",wdt_irq_count_t1);
//	printf("the wdt has come in interrupt\r\n\r\n\r\n");
//	wdt_irq_count_t1++;
//}
