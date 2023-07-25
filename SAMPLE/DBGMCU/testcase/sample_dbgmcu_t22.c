#include "sys.h"
#include "delay.h"
#include "sample_dbgmcu.h"
#include "stm32f10x.h"
#include "core_cm3.h"

/*******************************************************************************
 * @brief   sample_uart_test_t1
 * Function DWT compare
 * @author  taohu
 * @date    2022-06-11
 * @param   
		
 * @return void
*******************************************************************************/   
//DWT_COMP0-3   0x20/30/40/50
//DWT_MASK0-3   0x24/34/44/54
//DWT_FUNC0-3   0x28/38/48/58


void sample_dbgmcu_t22(void)
{	
	//pin setting�����ô�ӡ��PIN_MUX
	write_ahb32(0x4001b018, 0X0);
	
	//����DEMCR TRCENA
	write_ahb32(0xE000EDFC,1<<24 | read_ahb32(0xE000EDFC)); 
	
	//TPIU����
	write_ahb32(0xE0040004,1<<3); //CPS�ַ�    b0:1��b1:2;b3:4
	write_ahb32(0xE00400F0,0);    //SPP�ַ�����  λ10��00:ͬ�� 01�����У����� 10��NRZ
	write_ahb32(0xE0040304,0x102);    //FFC
	
	write_ahb32(0x4001b004,2<<23); //MCU_CFG��ͬ��ģʽ
	
	//ITM����
	//DBGMCU->CR = 0xFF67;//ʹ��TRACE������
	write_ahb32(0xE0000FB0,0xC5ACCE55);//����д����ITM�Ĵ���
	write_ahb32(0xE0000E80,0x00010005);//ITM->TCR = 0x00010005; //ʹ��TPIU��ͬ������ʹ������ITM����
	write_ahb32(0xE0000E00,0x01);//ITM->TER = 0x01;//��ʹ�ܴ����˿�0
	write_ahb32(0xE0000E40,0x01);//ITM->TPR = 0x01;//�رնԴ����˿�7:0������

	ITM_SendChar(0x55);
	ITM_SendChar(0x55);
	ITM_SendChar(0x55);
	ITM_SendChar(0x55);
}
