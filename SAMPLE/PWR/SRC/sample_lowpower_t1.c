#include "sample_lowpower.h"

// sleep mode


void sample_lowpower_t1(void)
{
	u32 reg = 0;
	printf("begin CFG!!\r\n");
	
	reg =read_ahb32(PwrCtrl_base+0x04);
	reg |= 1<<3;     //clear standby flag
	write_ahb32(PwrCtrl_base+0x04,reg);
	
	SCB->SCR &= ~(1<<2); //SCR.SLEEPDEEP = sleep
	
	printf("LOW POWER CFG over!!\r\n");
	printf("\r\nbegin CFG WDT and run WFI/WFE\r\n");
	
//	/************WFE***************/
	SCB->SCR |= 1<<4;  //enable SEVONPEND
	
	#if 0
	sample_timer2_t();  //定时器唤醒
	#else
	sample_exit_t1();   //外部中断唤醒
	#endif
	
		
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
//		printf("GPIO_BASE_B+GPIO_EXT_PORTA = 0x%x\r\n",read_ahb32(GPIO_BASE_B+GPIO_EXT_PORTA));
//		printf("GPIO_BASE_B+GPIO_INTSTATUS = 0x%x\r\n",read_ahb32(GPIO_BASE_B+GPIO_INTSTATUS));
		delay_ms(5000);
	}
}



 
