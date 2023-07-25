/*******************************************************************************
 * SNPS WDT IP driver.
 *******************************************************************************/

#include "snps_wdt.h"

u8 snps_wdt_irq_count;

/**********************************************************************
//set the timeout period in us
*********************************************************************/
void snps_wdt_set_timeout(u32 timeout_us)
{   
	u32 timeout_cycles,ref_cycles;
	u32 time_out;

	timeout_cycles = timeout_us * SNPS_WDT_CLK_FREQ;
	for(time_out = 0 ; time_out < 16 ; time_out++) {
		ref_cycles = ( (u32)1 ) << ( time_out + 16);
	 	if(ref_cycles >= timeout_cycles)
			break;
	}
	write_ahb32(WDT_BASE_ADDR+SNPS_WDT_TORR, time_out);
}

/**********************************************************************
//get the timeout period in us
*********************************************************************/
u32 snps_wdt_get_timeout(void)
{
	u32 time_out_us;
	u32 reg;

	reg = read_ahb32(WDT_BASE_ADDR+SNPS_WDT_TORR);
	time_out_us = (((u32)1)<<(reg+16)) * 1000 / SNPS_WDT_CLK_FREQ;
	return  time_out_us; 
}

/**********************************************************************
//Get the current WDT counter value
*********************************************************************/
u32 snps_get_counter(void)
{
	return read_ahb32(WDT_BASE_ADDR+SNPS_WDT_CCVR);
}

/**********************************************************************
//Set the WDT conuter restart
*********************************************************************/
void snps_wdt_restart(void)
{
	write_ahb32(WDT_BASE_ADDR+SNPS_WDT_CRR, COUNTER_RESTART_VALUE);
}

/**********************************************************************
//WDT interrupt status clear
*********************************************************************/
void snps_wdt_intr_clear(void)
{
	read_ahb32(WDT_BASE_ADDR+SNPS_WDT_EIO);
}

/**********************************************************************
//Check the WDT is enable or disable, return true = enable; false = disable
*********************************************************************/
bool snps_wdt_isenabled(void)
{
	u32 reg;
	
	reg = read_ahb32(WDT_BASE_ADDR+SNPS_WDT_CR);
	if((reg & SNPS_WDT_ENABLE) == SNPS_WDT_ENABLE)
		return true;
	else 
		return false;	
}

/**********************************************************************
//disable the WDT
*********************************************************************/
void snps_wdt_disable(void)
{
	u32 reg = 0;

	NVIC_DisableIRQ(WWDG_IRQn);
	reg = read_ahb32(WDT_BASE_ADDR+SNPS_WDT_CR);
	write_ahb32(WDT_BASE_ADDR+SNPS_WDT_CR, reg & (~SNPS_WDT_ENABLE));
}

/**********************************************************************
//enable the WDT
*********************************************************************/
void snps_wdt_enable(void)
{
	u32 reg = 0;
	reg = read_ahb32(WDT_BASE_ADDR+SNPS_WDT_CR);
	write_ahb32(WDT_BASE_ADDR+SNPS_WDT_CR, reg | SNPS_WDT_ENABLE);
}

/**********************************************************************
//system WDT initial conifgure 
*********************************************************************/
u8 snps_wdt_init(struct wdt_cfg_param *param)
{
	u32 response_mode;
	u32 reg;
	u32 reset_pulse = 0, reset_ns = 0;
	
	if ( snps_wdt_isenabled())
		snps_wdt_disable();
	
	if (param->intr_en)
	{
		snps_wdt_intr_clear();
		response_mode = SNPS_WDT_INTR_MODE;
		NVIC_EnableIRQ(WWDG_IRQn);
	}
	else
	{
		response_mode = SNPS_WDT_RESET_MODE;
		NVIC_DisableIRQ(WWDG_IRQn);
	}
	//set the timeout cycles
	snps_wdt_set_timeout(param->timeout_us);
	//get the reset pulse cycle from the configure time
	for(reset_pulse = 0; reset_pulse < 8 ; reset_pulse++) {
		reset_ns = (1 << (1+reset_pulse)) * 1000 / SNPS_WDT_CLK_FREQ ;
		if(reset_ns >= param->rst_time_ns)
			break;
	}
		
	//clear the bit of RPL and RMOD
	reg = read_ahb32(WDT_BASE_ADDR+SNPS_WDT_CR);
	reg = reg & 0xffe1;
	//set the RPL and RMOD with setting value
	reg |= (reset_pulse << 2) | response_mode;
	write_ahb32(WDT_BASE_ADDR+SNPS_WDT_CR, reg);

	return 0;
}





