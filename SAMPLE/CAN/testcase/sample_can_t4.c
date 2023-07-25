#include "sys.h"
#include "delay.h"
#include "sample_can.h"
#include "by_can.h"
#include "stm32f10x.h"

extern CAN_InitTypeDef CAN_InitStruct;
extern u8 can_data[8];

/*******************************************************************************
 * @brief   sample_uart_test_t1
 * Function ��CTS����ģʽ��������������
 * @author  taohu
 * @date    2022-03-15
 * @param   
		USARTx:USART����ַ
 * @return void
*******************************************************************************/    
void sample_can_t4(CAN_TypeDef* CANx)
{   
    u8 count = 0,i = 0;
    CanRxMsg RxMessage = {0};
    
    //���Կ�ʼ��ӡ
    printf("test is started\r\nfile:%s,line:%d\r\n",__FILE__,__LINE__);
	
    write_ahb32(0x4001b018, 0x208);
    read_ahb32(0xE000E280) = 0xFFFFFFFF;//���PENDINGλ
	printf("IP:%x\r\n",read_ahb32(0xE000E280));

    //���ò�������ʼ��
    sample_can_init(CANx);	//��Ĭ�ϲ�����ʼ����ӡ����

	CANx->IMR |= CAN_RMI;   
	//sample_can_nvic_cfg(CANx, ENABLE);
	
	//����FILTER
	CANx->MR |=0x04;
	CANx->MR |=0x01;
	CANx->AMR = 0xFFFFFFFF;
	printf("AmR1:%x\r\n",CANx->AMR);

	//write_ahb32(CAN_ACR0_3,0x00AAE03F);
	printf("MR:%x\r\n",CANx->MR);
	CANx->MR &= 0xFB;
	printf("MR:%x\r\n",CANx->MR);


	//ѭ����������
    while(1)
    {	
		while(((read_ahb32(CAN_MR) >> 24) & CAN_RMI) == 0);
		printf("MR:%x\r\n",read_ahb32(CAN_MR));
		
		while(((read_ahb32(CAN_MR) >> 16) & 0x80) != 0)
		{
			CAN_Receive(CANx,&RxMessage);
			
			printf("Clear Flag:\r\n");	
			CAN_ClearFlag(CANx, CAN_RMI);
			printf("MR:%x\r\n",read_ahb32(CAN_MR));	
			
			printf("IDE:%x\r\n",RxMessage.IDE);
			printf("RTR:%x\r\n",RxMessage.RTR);
			printf("DLC:%x\r\n",RxMessage.DLC);
			printf("StdId:%x\r\n",RxMessage.StdId);
			printf("ExtId:%x\r\n",RxMessage.ExtId);

			for( i= 0;i < RxMessage.DLC; i++)
			{
				printf("DATA[%d]:%x\r\n",i,RxMessage.Data[i]);
			}
		}
		if(count++ > 100)
			break;
	}

    //�������̣��ͷ���Դ��������ɴ�ӡ
    printf("file:%s,line:%d\r\ntest is finished\r\n",__FILE__,__LINE__);
}




