#include "sys.h"
#include "delay.h"
#include "stm32f10x.h"
#include "sample_iwdg.h"

/*******************************************************************************
 * @brief   sample_iwdg_t4
 * Function IWDG待机下运行
 * @author  licuihuang
 * @date    2022-6-15
 * @param   void
 * @return  void
*******************************************************************************/
void sample_iwdg_t4(void)
{
	printf("reset iwdg!!\r\n");
//	printf("RST_STAT:0x%x\r\n",read_ahb32(RST_STAT));
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
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	sample_iwdg_init(IWDG_Prescaler_32,0xFFF);  //(0x3,0xfff)=3.2s
	IWDG_Enable(); //启动看门狗
	IWDG_ReloadCounter();

	while(IWDG_GetFlagStatus(IWDG_FLAG_PVU)||IWDG_GetFlagStatus(IWDG_FLAG_RVU));   //等待PR/RLR更新完成
	printf("SR:0x%x PR:0x%x RLR:0x%x\r\n",read_ahb32(0x4001ac0c),read_ahb32(0x4001ac04),read_ahb32(0x4001ac08));
	
    standby_iwdg_init();    //进入待机模式
	printf("exit standby mode\r\n");
    printf("PWR_CTRL:0x%x PWR_STAT:0x%x RST_STAT:0x%x\r\n",read_ahb32(PWR_CTRL),read_ahb32(PWR_STAT),read_ahb32(RST_STAT));	
	printf("\r\n");
}

		
