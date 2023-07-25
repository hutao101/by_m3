#include "sys.h"
#include "delay.h"
#include "sample_iwdg.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_iwdg_test_t2
 * Function KR=0x5555 ��PR RLRд����
 * @author  licuihuang
 * @date    2022-6-15
 * @param   void
 * @return  void
*******************************************************************************/
void sample_iwdg_t2(void)
{	
	printf("reset\r\n");
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Disable); //��0�������Ź�
	sample_iwdg_init(IWDG_Prescaler_16,0x7FF);  //��ʼ������PR,RLR����ֵ������Ϊ1.64s������λ	
	delay_ms(1);
	printf("WRITE_DIS  SR:0x%x PR:0x%x RLR:0x%x\r\n",read_ahb32(0x4001ac0c),read_ahb32(0x4001ac04),read_ahb32(0x4001ac08));
	
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); //����д����
	sample_iwdg_init(IWDG_Prescaler_32,0xEFF);  //��ʼ������PR,RLR����ֵ������Ϊ1.64s������λ	
	delay_ms(1);
	printf("WRITE_EN   SR:0x%x PR:0x%x RLR:0x%x\r\n",read_ahb32(0x4001ac0c),read_ahb32(0x4001ac04),read_ahb32(0x4001ac08));
	
    IWDG_Enable(); //�������Ź�
	IWDG_ReloadCounter();
	printf("ENABLE     SR:0x%x PR:0x%x RLR:0x%x\r\n",read_ahb32(0x4001ac0c),read_ahb32(0x4001ac04),read_ahb32(0x4001ac08));
	
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Disable); //��0�������Ź�
	sample_iwdg_init(IWDG_Prescaler_64,0x9FF);  //��ʼ������PR,RLR����ֵ������Ϊ1.64s������λ	
	delay_ms(1);
	printf("WRITE_DIS  SR:0x%x PR:0x%x RLR:0x%x\r\n",read_ahb32(0x4001ac0c),read_ahb32(0x4001ac04),read_ahb32(0x4001ac08));
	
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); //����д����
	sample_iwdg_init(IWDG_Prescaler_128,0x8FF);  //��ʼ������PR,RLR����ֵ������Ϊ1.64s������λ	
	delay_ms(1);
	printf("WRITE_EN   SR:0x%x PR:0x%x RLR:0x%x\r\n",read_ahb32(0x4001ac0c),read_ahb32(0x4001ac04),read_ahb32(0x4001ac08));

	
  printf("\r\n");
}
		
