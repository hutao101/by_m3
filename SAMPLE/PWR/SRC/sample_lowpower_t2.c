#include "sample_lowpower.h"


void sample_lowpower_t2(void)
{
	u32 reg = 0;
	
	printf("LowPower begin CFG!!\r\n");
	
	/*******stop mode cfg**************/
	reg =read_ahb32(PwrCtrl_base+0x04);
	reg |= 3<<3;     //clear standby flag¡¢clear wakeup flag
	write_ahb32(PwrCtrl_base+0x04,reg);
	
	SCB->SCR |= (1<<2); //SCR.SLEEPDEEP = deepsleep
	
	reg = read_ahb32(PwrCtrl_base+0x04);
	reg &= ~(1<<1); //Enter Stop
	reg &= ~(1<<0); //Voltage regulator on when stop
	write_ahb32(PwrCtrl_base+0x04,reg);
	
	printf("LOW POWER CFG over!!\r\n");
	
	
	#if 1
	/*******timer cfg**************/
	sample_timer2_t();
	#else
	/********inturrupt cfg*************/
	sample_exit_t1();
	#endif
	
	
	printf("\r\nrun WFI/WFE\r\n");
	
	printf("Standby flag = 0x%x\r\n",(read_ahb32(PwrCtrl_base+0x08)>>1) & 0x1);
	printf("Wakeup flag = 0x%x\r\n",read_ahb32(PwrCtrl_base+0x08) & 0x1);
	
	#if 0
	__WFE();
	printf("WFE has over\r\n");
	
	#else  
	__WFI();
	printf("WFI has over\r\n");
	
	#endif
	
	printf("Standby flag = 0x%x\r\n",(read_ahb32(PwrCtrl_base+0x08)>>1) & 0x1);
	printf("Wakeup flag = 0x%x\r\n",read_ahb32(PwrCtrl_base+0x08) & 0x1);
	
	while(1)
	{
		printf("go\r\n");
//		printf("GPIO_BASE_A+GPIO_EXT_PORTA = 0x%x\r\n",read_ahb32(GPIO_BASE_A+GPIO_EXT_PORTA));
//		printf("GPIO_BASE_A+GPIO_INTSTATUS = 0x%x\r\n",read_ahb32(GPIO_BASE_A+GPIO_INTSTATUS));
//		printf("GPIO_BASE_A+GPIO_RAW_INTSTATUS = 0x%x\r\n",read_ahb32(GPIO_BASE_A+GPIO_RAW_INTSTATUS));
		delay_ms(5000);
	}
}



