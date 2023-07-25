#include "sys.h"
#include "delay.h"
#include "stm32f10x.h"
#include "sample_iwdg.h"

/*******************************************************************************
 * @brief   sample_iwdg_t5
 * Function IWDGͣ��������
 * @author  licuihuang
 * @date    2022-6-15
 * @param   void
 * @return  void
*******************************************************************************/
void sample_iwdg_t5(void)
{
	u16 reg;
	printf("iwdg reset!!\r\n");
	printf("RST_STAT:0x%x\r\n",read_ahb32(RST_STAT));
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
	printf("clear:0x%x\r\n",read_ahb32(PWR_STAT));
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	sample_iwdg_init(IWDG_Prescaler_256,0xFFF);  //(0x256,0xfff)=26s
//	IWDG_Enable(); //�������Ź�
	IWDG_ReloadCounter();

	while(IWDG_GetFlagStatus(IWDG_FLAG_PVU)||IWDG_GetFlagStatus(IWDG_FLAG_RVU));
	printf("SR:0x%x PR:0x%x RLR:0x%x\r\n",read_ahb32(0x4001ac0c),read_ahb32(0x4001ac04),read_ahb32(0x4001ac08));

//	iwdg_exit_init();          //�ⲿ�ж�PA1����(���ŵ�ƽ��1��0�����ж�)	
	iwdg_rtc_maskirq_init();   //RTCÿ2s���������ж�
	
    stop_iwdg_init();
	printf("exit stop mode\r\n");
    printf("PWR_CTRL:0x%x PWR_STAT:0x%x RST_STAT:0x%x\r\n",read_ahb32(PWR_CTRL),read_ahb32(PWR_STAT),read_ahb32(RST_STAT));	
	printf("\r\n");
}

		