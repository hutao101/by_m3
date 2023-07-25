#include "sys.h"
#include "delay.h"
#include "sample_iwdg.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_iwdg_t6
 * Function ϵͳ��λ�ɹر�IWDG
 * @author  licuihuang
 * @date    2022-6-15
 * @param   void
 * @return  void
*******************************************************************************/
void sample_iwdg_t6(void)
{
	delay_ms(5000);
	printf("delay 5s\r\n");
	
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); //��0�������Ź�
	sample_iwdg_init(IWDG_Prescaler_16,0xFFF); //(0x2,0xfff)=1.6s
	printf("iwdg init\r\n");
	
	IWDG_Enable();
	printf("iwdg enable\r\n");
}

