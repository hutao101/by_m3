#include "sys.h"
#include "delay.h"
#include "sample_can.h"
#include "by_can.h"
#include "stm32f10x.h"

extern CAN_InitTypeDef CAN_InitStruct;
extern u8 can_data[8];

/*******************************************************************************
 * @brief   sample_can_t6
 * Function filter���ܲ���:��������Զ��֡����
 * @author  taohu
 * @date    2022-04-14
 * @param   
		CANx:CAN����ַ
 * @return void
*******************************************************************************/    
void sample_can_t7(CAN_TypeDef* CANx, FunctionalState send)
{   
    u8 tx_count = 0,rx_count = 0;
    CanRxMsg RxMessage = {0};
	CanTxMsg TxMessage = {0};
    u32 ACR = 0xFFFFFFFF, AMR = 0xFF00EFFF; 

    //���Կ�ʼ��ӡ
    printf("test is started\r\nfile:%s,line:%d\r\n",__FILE__,__LINE__);
	write_ahb32(0x4001b018, 0x208);
    read_ahb32(0xE000E280) = 0xFFFFFFFF;//���PENDINGλ
	printf("PENDING:%x\r\n",read_ahb32(0xE000E280));

	//���ò�������ʼ��
    sample_can_init(CANx);	//��Ĭ�ϲ�����ʼ����ӡ����
	
	if(send == ENABLE)
	{
		//CAN_ITConfig(CANx, CAN_TMI, ENABLE);
		//sample_can_nvic_cfg(CANx, ENABLE);

		TxMessage.DLC = 2;

		while(1)
		{
			delay_ms(100);
			
			switch(tx_count)
			{
				case 0:
				{
					TxMessage.IDE = 0;     //��׼֡
					TxMessage.RTR = 0;
					TxMessage.StdId = 0;
				}
				break;
				case 1:
				{
				    TxMessage.IDE = 0;     //��׼Զ��֡
				    TxMessage.RTR = 1;
					TxMessage.StdId = 0x3FF;
				}
				break;
				case 2:
			    {
					TxMessage.IDE = 1;     //��չ֡
					TxMessage.RTR = 0;
					TxMessage.ExtId = 0;
				}
				break;
				case 3:
			    {
					TxMessage.IDE = 1;     //��չԶ��֡
					TxMessage.RTR = 1;
					TxMessage.ExtId = 0x3FF;
				}
				break;
			}

			//��������֡
			CAN_Transmit(CANx, &TxMessage);
			write_ahb32(CAN_MR,0x600);//start transmit
			
			//while(((read_ahb32(CAN_MR) >> 24) & CAN_TMI) == 0);
			printf("MR:%x\r\n",read_ahb32(CAN_MR));
			
			printf("Clear Flag:\r\n");	
			CAN_ClearFlag(CANx, CAN_TMI);
			//printf("MR:%x\r\n",read_ahb32(CAN_MR)); 


			if(++tx_count >= 4)
				break;
			
		}
	    
	}
	else
	{
		//�����ж�
		//CAN_ITConfig(CANx, CAN_RMI, ENABLE);   
		//sample_can_nvic_cfg(CANx, ENABLE);

		//����filter
		sample_can_cfg_filter(CANx,ACR,AMR,ENABLE);
		//ѭ����������
	    while(1)
	    {	
			while(((read_ahb32(CAN_MR) >> 24) & CAN_RMI) == 0);
			//printf("MR:%x\r\n",read_ahb32(CAN_MR));
			
			while(((read_ahb32(CAN_MR) >> 16) & 0x80) != 0)
			{
				RxMessage.ExtId = 0;
				RxMessage.StdId = 0;
				CAN_Receive(CANx,&RxMessage);
				
				//printf("Clear Flag:\r\n");	
				CAN_ClearFlag(CANx, CAN_RMI);
				//printf("MR:%x\r\n",read_ahb32(CAN_MR));	
				
				printf("IDE:%x\r\n",RxMessage.IDE);
				printf("RTR:%x\r\n",RxMessage.RTR);
				//printf("DLC:%x\r\n",RxMessage.DLC);
				printf("StdId:%x\r\n",RxMessage.StdId);
				printf("ExtId:%x\r\n",RxMessage.ExtId);
		
				printf("D[0]:%x\r\n",RxMessage.Data[0]);
			}
			
			rx_count++;
			AMR = 0xFBFFFFFF; //RTRλ������
            
			//����filter
			sample_can_cfg_filter(CANx,ACR,AMR,ENABLE);
			
			if(rx_count > 100)
				break;
		}

	}

    //�������̣��ͷ���Դ��������ɴ�ӡ
    printf("file:%s,line:%d\r\ntest is finished\r\n",__FILE__,__LINE__);
}





