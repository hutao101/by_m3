#include "sys.h"
#include "delay.h"
#include "sample_iwdg.h"
#include "stm32f10x.h"

void sample_iwdg_init(u16 pre,u16 reload) 
{		
	IWDG_SetPrescaler(pre);  //����IWDGԤ��Ƶֵ	
	IWDG_SetReload(reload);  //����IWDG��װ��ֵ	
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
	reg &= 0xffc;              //���2λΪ0��[1]_0:����ͣ��ģʽ��[0]_0:ͣ��ģʽ�µ�ѹ����������
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
	
	write_ahb32(0x4001ae04,0x800);     //����д����	
	write_ahb32(RTC_CMR,0x1ffff);       //������0xffff����ƥ���ж�(2s)
	write_ahb32(RTC_CLR,0x0);          //��0��ʼ���ϼ���
	write_ahb32(RTC_CCR,RTC_WRAP_ENABLE|RTC_INT_UNMASK|RTC_INT_ENABLE|RTC_ENABLE); //RTC_ENABLE
}


void iwdg_exit_init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = 7;			//PA1
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2�� 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure); 
	
	write_ahb32(GPIO_BASE_A+GPIO_PORTA_EOI,1<<1);  //PA1 clear int
	write_ahb32(GPIO_BASE_A+GPIO_INTTYPE_LEVEL,0x0); // Interrupt is level sensitive
	write_ahb32(GPIO_BASE_A+GPIO_INT_POLARITY,0x0);  //active_low �͵�ƽ����
	write_ahb32(GPIO_BASE_A+GPIO_INTMASK,0x0);  //  interrupu  are unmasked
	write_ahb32(GPIO_BASE_A+GPIO_INTEN,1<<1);   //interrupt  enable
}


void EXTI1_IRQHandler(void)
{
	delay_ms(2000);//����
	printf("has come in exit interrupt!!!!!!!!!!!!!\r\n");
	write_ahb32(0xE000E280,1<<7);  //���pendingλ	
}


void RTC_IRQHandler(void)
{		
	if(read_ahb32(RTC_STAT))
	{
		printf("rtc match irq\r\n");
		read_ahb32(RTC_EOI);   //��ƥ���־λ,��һ��ʱ����жϴ�ӡһ��
	}
}

