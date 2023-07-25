#include "sample_wdt.h"

u8 wdt_irq_count_t5;


// timeout = 0-15
void sample_wdt_t5(u8 timeout)
{
	u32 reg = 0;
	
	printf("top = %d",timeout);
	
	NVIC_EnableIRQ(WWDG_IRQn);
	
	reg = 0;
	reg |= timeout;  //count timeout = 1M clocks  = 1000 000/36000 000 = 28ms
	write_ahb32(WDT_BASE_ADDR+SNPS_WDT_TORR,reg);
	
	reg = read_ahb32(WDT_BASE_ADDR+SNPS_WDT_CR);
	reg |= 1<<1;  //Response mode = interrupu then reset
	reg |= 7<<2;     //reset pulse length = 256 clock				
	write_ahb32(WDT_BASE_ADDR+SNPS_WDT_CR,reg);
	
	printf("WDT_TORR = 0x%x\r\n",read_ahb32(WDT_BASE_ADDR+SNPS_WDT_TORR));
	
	snps_wdt_restart();
	
	reg = read_ahb32(WDT_BASE_ADDR+SNPS_WDT_CR);
	reg |= 1;   //wdt is enable
	write_ahb32(WDT_BASE_ADDR+SNPS_WDT_CR,reg);

	printf("SNPS_WDT_CR = 0x%x\r\n",read_ahb32(WDT_BASE_ADDR+SNPS_WDT_CR) );

	while(1);
}


///**********************************************************************
//system WDT interrupt handler
//仅限T5使用
//*********************************************************************/
void WWDG_IRQHandler(void) __irq 
{
	
	if (wdt_irq_count_t5 == 0)
	{
		//first timeout interrupt, clear the interrupt
		snps_wdt_intr_clear();
		printf("wdt_irq_count_t1 = 0x%x\r\n",wdt_irq_count_t5);
	}
	else						
	{
		//second timeout interrupt, clear the interrupt and restart the WDT
		snps_wdt_intr_clear();
		snps_wdt_restart();
		printf("wdt_irq_count_t1 = 0x%x\r\n",wdt_irq_count_t5);
		wdt_irq_count_t5 = 0 ;
	}
	//increase the WDT interrupt flag
	wdt_irq_count_t5++;
	printf("T\r\n\r\n");
}
