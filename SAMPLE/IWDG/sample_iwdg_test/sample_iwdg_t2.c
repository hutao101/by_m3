#include "sys.h"
#include "delay.h"
#include "sample_iwdg.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_iwdg_test_t2
 * Function KR=0x5555 对PR RLR写访问
 * @author  licuihuang
 * @date    2022-6-15
 * @param   void
 * @return  void
*******************************************************************************/
void sample_iwdg_t2(void)
{	
	printf("reset\r\n");
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Disable); //用0启动看门狗
	sample_iwdg_init(IWDG_Prescaler_16,0x7FF);  //初始化，对PR,RLR赋初值，设置为1.64s发生复位	
	delay_ms(1);
	printf("WRITE_DIS  SR:0x%x PR:0x%x RLR:0x%x\r\n",read_ahb32(0x4001ac0c),read_ahb32(0x4001ac04),read_ahb32(0x4001ac08));
	
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); //开启写访问
	sample_iwdg_init(IWDG_Prescaler_32,0xEFF);  //初始化，对PR,RLR赋初值，设置为1.64s发生复位	
	delay_ms(1);
	printf("WRITE_EN   SR:0x%x PR:0x%x RLR:0x%x\r\n",read_ahb32(0x4001ac0c),read_ahb32(0x4001ac04),read_ahb32(0x4001ac08));
	
    IWDG_Enable(); //启动看门狗
	IWDG_ReloadCounter();
	printf("ENABLE     SR:0x%x PR:0x%x RLR:0x%x\r\n",read_ahb32(0x4001ac0c),read_ahb32(0x4001ac04),read_ahb32(0x4001ac08));
	
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Disable); //用0启动看门狗
	sample_iwdg_init(IWDG_Prescaler_64,0x9FF);  //初始化，对PR,RLR赋初值，设置为1.64s发生复位	
	delay_ms(1);
	printf("WRITE_DIS  SR:0x%x PR:0x%x RLR:0x%x\r\n",read_ahb32(0x4001ac0c),read_ahb32(0x4001ac04),read_ahb32(0x4001ac08));
	
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); //开启写访问
	sample_iwdg_init(IWDG_Prescaler_128,0x8FF);  //初始化，对PR,RLR赋初值，设置为1.64s发生复位	
	delay_ms(1);
	printf("WRITE_EN   SR:0x%x PR:0x%x RLR:0x%x\r\n",read_ahb32(0x4001ac0c),read_ahb32(0x4001ac04),read_ahb32(0x4001ac08));

	
  printf("\r\n");
}
		
