#include "sys.h"
#include "delay.h"
#include "sample_iwdg.h"
#include "stm32f10x.h"

void sample_iwdg_init(u16 pre,u16 reload) 
{		
	IWDG_SetPrescaler(pre);  //设置IWDG预分频值	
	IWDG_SetReload(reload);  //设置IWDG重装载值	
}


void sample_iwdg_feed(void)
{
	IWDG_ReloadCounter();
	printf("feed the dog\r\n");
}


void standby_iwdg_init(void)
{
	write_ahb32(PWR_CTRL,PWR_CTRL_clesta);
	write_ahb32(PWR_CTRL,PWR_CTRL_standby);
	SCB->SCR |= SCB_SCR_SLEEPDEEP;
	__WFI();
}


void stop_iwdg_init(void)
{
	u16 reg;
	reg = read_ahb32(PWR_CTRL);
	reg &= 0xffc;              //设低2位为0：[1]_0:进入停机模式，[0]_0:停机模式下电压调节器开启
//	printf("stop 1:0x%x\r\n",read_ahb32(PWR_CTRL));
	write_ahb32(PWR_CTRL,reg);
//	printf("stop 2:0x%x\r\n",read_ahb32(PWR_CTRL));
	SCB->SCR |= SCB_SCR_SLEEPDEEP;
//	printf("stop 3:0x%x 0x%x\r\n",read_ahb32(PWR_CTRL),SCB->SCR);
	__WFI();     
//	printf("stop 4:0x%x\r\n",read_ahb32(PWR_CTRL));
}


void iwdg_rtc_maskirq_init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = 19; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
	
	write_ahb32(0x4001ae04,0x800);     //后备域写访问	
	write_ahb32(RTC_CMR,0x1ffff);       //计数到0xffff进入匹配中断(2s)
	write_ahb32(RTC_CLR,0x0);          //从0开始向上计数
	write_ahb32(RTC_CCR,RTC_WRAP_ENABLE|RTC_INT_UNMASK|RTC_INT_ENABLE|RTC_ENABLE); //RTC_ENABLE
}


void iwdg_exit_init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = 7;			//PA1
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure); 
	
	write_ahb32(GPIO_BASE_A+GPIO_PORTA_EOI,1<<1);  //PA1 clear int
	write_ahb32(GPIO_BASE_A+GPIO_INTTYPE_LEVEL,0x0); // Interrupt is level sensitive
	write_ahb32(GPIO_BASE_A+GPIO_INT_POLARITY,0x0);  //active_low 低电平触发
	write_ahb32(GPIO_BASE_A+GPIO_INTMASK,0x0);  //  interrupu  are unmasked
	write_ahb32(GPIO_BASE_A+GPIO_INTEN,1<<1);   //interrupt  enable
}


void EXTI1_IRQHandler(void)
{
	delay_ms(2000);//消抖
	printf("has come in exit interrupt!!!!!!!!!!!!!\r\n");
	write_ahb32(0xE000E280,1<<7);  //清除pending位	
}


void RTC_IRQHandler(void)
{		
	if(read_ahb32(RTC_STAT))
	{
		printf("rtc match irq\r\n");
		read_ahb32(RTC_EOI);   //清匹配标志位,隔一段时间进中断打印一次
	}
}

