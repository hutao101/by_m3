#include "sys.h"
#include "delay.h"
#include "sample_can.h"
#include "by_can.h"
#include "stm32f10x.h"

extern CAN_InitTypeDef CAN_InitStruct;
extern u8 can_data[8];

/*******************************************************************************
 * @brief   sample_can_test_t14
 * Function ����֡
 * @author  taohu
 * @date    2022-04-18
 * @param   
		USARTx:USART����ַ
 * @return void
*******************************************************************************/    
void sample_can_t14(CAN_TypeDef* CANx)
{   
    u8 i = 0;
    CanTxMsg TxMessage;

    //���Կ�ʼ��ӡ
    printf("test is started\r\nfile:%s,line:%d\r\n",__FILE__,__LINE__);
	
    write_ahb32(0x4001b018, 0x208);
    read_ahb32(0xE000E280) = 0xFFFFFFFF;//���PENDINGλ
	printf("IP:%x\r\n",read_ahb32(0xE000E280));

    //���ò�������ʼ��
    //sample_can_init(CANx);	//��Ĭ�ϲ�����ʼ����ӡ����

	//CAN_ITConfig(CANx, CAN_TMI, ENABLE);
	//sample_can_nvic_cfg(CANx, ENABLE);
	
	TxMessage.IDE = 0;     //��׼֡
    TxMessage.StdId = 0x1FF; 
    TxMessage.DLC   = 8;
    for(i = 0;i < 8;i++)
    {
        TxMessage.Data[i] = can_data[i];
    }
	
    //CAN_Transmit(CANx, &TxMessage);
    //write_ahb32(CAN_MR,0x400);//start transmit


	  write_ahb32(CAN_MR,0x04);//SET_RESET
	  write_ahb32(CAN_IMR,0x11030000);//disable interrupt
	  write_ahb32(CAN_MR,0x00);//release bus
	  write_ahb32(CAN_TX_BUF,0x5A5A5A5A);//TX_BUF
	  write_ahb32(CAN_MR,0x400);//release bus
	
	while(1)
	{
		//if(read_ahb32(CAN_ERR_LOST) & 0x0c)  //FRMER
		{
			printf("SR:0x%x\r\n",((read_ahb32(CAN_MR) >> 16) & 0xFF)); //��ȡ״̬��Ϣ
			printf("ISR:0x%x\r\n",((read_ahb32(CAN_MR) >> 24) & 0xFF)); //��ȡ״̬��Ϣ
			printf("ECC:0x%x\r\n",read_ahb32(CAN_ERR_LOST) & 0xFF); //��ȡ״̬��Ϣ
			printf("RX_ERR:0x%x\r\n",(read_ahb32(CAN_ERR_LOST) >> 8) & 0xFF); //��ȡ״̬��Ϣ
			printf("TX_ERR:0x%x\r\n",(read_ahb32(CAN_ERR_LOST) >> 16) & 0xFF); //��ȡ״̬��Ϣ
			printf("\r\n"); 
		}
	}
	
	
    //�������̣��ͷ���Դ��������ɴ�ӡ
    //printf("file:%s,line:%d\r\ntest is finished\r\n",__FILE__,__LINE__);
}



