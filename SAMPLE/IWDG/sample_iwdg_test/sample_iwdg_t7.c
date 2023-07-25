#include "sys.h"
#include "delay.h"
#include "stm32f10x.h"
#include "sample_iwdg.h"

/*******************************************************************************
 * @brief   sample_iwdg_t7
 * Function RTC唤醒停机模式
 * @author  licuihuang
 * @date    2022-6-15
 * @param   void
 * @return  void
*******************************************************************************/
void sample_iwdg_t7(void)
{
	u16 reg;
	printf("PWR_STAT:0x%x\r\n",read_ahb32(PWR_STAT));
	while(read_ahb32(PWR_STAT)&0x2)
	{
		printf("PWR_STAT:0x%x\r\n",read_ahb32(PWR_STAT));
		write_ahb32(PWR_CTRL,PWR_CTRL_clesta);
		printf("exit standby mode and clear stady flag\r\n");		
	}
	
	while(read_ahb32(PWR_STAT)&0x1)
	{
		printf("PWR_STAT:0x%x\r\n",read_ahb32(PWR_STAT));
		write_ahb32(PWR_CTRL,PWR_CTRL_clewake);
		printf("clear wakeup flag\r\n");		
	}
	
	write_ahb32(PWR_CTRL,PWR_CTRL_clesta|PWR_CTRL_clewake);
	printf("clear PWR_STAT:0x%x\r\n",read_ahb32(PWR_STAT));

	iwdg_rtc_maskirq_init();   //RTC每4s进入闹钟中断
	
    stop_iwdg_init();
	printf("exit stop mode\r\n");
    printf("PWR_CTRL:0x%x PWR_STAT:0x%x\r\n",read_ahb32(PWR_CTRL),read_ahb32(PWR_STAT));	
	printf("\r\n");
}

		