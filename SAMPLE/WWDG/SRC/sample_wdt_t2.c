#include "sample_wdt.h"

//test for Interrupt

u8 wdt_irq_count_t2;

void sample_wdt_t2(void)
{
	
	struct wdt_cfg_param param ={0};
	u32 reg = 0;
	
	printf("WWDT T2 begin\r\n");
	
	param.timeout_us = 6000000;  //60ms timeout period	
	param.rst_time_ns = 500;  //500ns reset pulse width
	param.intr_en = 0;
	
	snps_wdt_init(&param);
	
	reg = read_ahb32(WDT_BASE_ADDR+SNPS_WDT_TORR);
	reg |= (0xC << 4);
	printf("reg = 0x%x\r\n",reg);
	write_ahb32(WDT_BASE_ADDR+SNPS_WDT_TORR, reg);
	printf("TORR = 0x%x\r\n",read_ahb32(WDT_BASE_ADDR+SNPS_WDT_TORR));
	
	//enable the WDT
	snps_wdt_restart();	
	snps_wdt_enable();
	
	printf("go\r\n");
	while(1)
	{
		delay_ms(2000);
		
		if( (read_ahb32(0x4001AE00+0x0C) != 0x1) || (read_ahb32(0xE0000000+0x14) != 0x1) )
		{
			printf("RST_STAT = 0x%x\r\n",read_ahb32(0x4001AE00+0x0C));  
			printf("SW_RST = 0x%x\r\n\r\n",read_ahb32(0xE0000000+0x14)); 
			
		}
		
	}
}
