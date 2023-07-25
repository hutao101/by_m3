#include "sample_lowpower.h"

// sleep mode


void wkup_pin_init(void)
{
	u32 reg = 0;
	write_ahb32(GPIO_BASE_A+GPIO_SWPORTA_CTL,0);  //GPIO Software Mode
	write_ahb32(GPIO_BASE_A+GPIO_SWPORTA_DDR,0);  //GPIO Input Direction 
//	write_ahb32(GPIO_BASE_A+GPIO_SWPORTA_DR,0);  //GPIO data = 0

	printf("PA0 = 0x%x\r\n",read_ahb32(GPIO_BASE_A+GPIO_EXT_PORTA) & 0x1);
	
	reg = read_ahb32(PwrCtrl_base+0x08);
	reg |= (1<<8);  //Enable WKUP
	write_ahb32(PwrCtrl_base+0x08,reg);
	printf("WKUP ENABLE= 0x%x\r\n",(read_ahb32(PwrCtrl_base+0x08)>>8) & 0x1);
}

void sample_lowpower_t3(void)
{
	u32 reg = 0;
	printf("begin CFG!!\r\n");
	
	printf("Standby flag = 0x%x\r\n",(read_ahb32(PwrCtrl_base+0x08)>>1) & 0x1);
	printf("Wakeup flag = 0x%x\r\n",read_ahb32(PwrCtrl_base+0x08) & 0x1);
	
	reg =read_ahb32(PwrCtrl_base+0x04);
	reg |= 3<<3;     //clear standby flag¡¢clear wakeup flag
	write_ahb32(PwrCtrl_base+0x04,reg);
	
	SCB->SCR |= (1<<2); //SCR.SLEEPDEEP = deepsleep
	
	reg = read_ahb32(PwrCtrl_base+0x04);
	reg |= (1<<1); //Enter standby
	write_ahb32(PwrCtrl_base+0x04,reg);
	
	printf("PWR_CTRL = 0x%x\r\n",read_ahb32(PwrCtrl_base+0x04));
	printf("SCB->SCR = 0x%x\r\n",SCB->SCR);
		
	printf("LOW POWER CFG over!!\r\n");
	
	
	#if 1    
		#if 1  //WKUPÒý½Å»½ÐÑ
			wkup_pin_init();
		#else  //IWDT»½ÐÑ
			sample_iwdg_t4();
		#endif
	#else
		#if 0
			sample_rtc_t7();  //rtc»½ÐÑ
		#else
			sample_timer2_t(); //¶¨Ê±Æ÷»½ÐÑ
		#endif
	#endif
	

	printf("RST_STAT = 0x%x\r\n",read_ahb32(PwrCtrl_base+0x0C));
	printf("will into standby\r\n");
	
	#if 0
	__WFE();
	printf("WFE has over\r\n");
	
	#else  
	__WFI();
	printf("WFI has over\r\n");
	
	#endif
	
	printf("Standby flag = 0x%x\r\n",(read_ahb32(PwrCtrl_base+0x08)>>1) & 0x1);
	printf("Wakeup flag = 0x%x\r\n",read_ahb32(PwrCtrl_base+0x08) & 0x1);

	printf("RST_STAT = 0x%x\r\n",read_ahb32(PwrCtrl_base+0x0C));
	printf("PA0 = 0x%x\r\n",read_ahb32(GPIO_BASE_A+GPIO_EXT_PORTA) & 0x1);
	while(1)
	{
		printf("go\r\n");
		delay_ms(5000);
	}
}
