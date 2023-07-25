#include "sys.h"
#include "delay.h"
#include "sample_can.h"
#include "by_can.h"
#include "stm32f10x.h"

extern CAN_InitTypeDef CAN_InitStruct;
extern u8 can_data[8];

/*******************************************************************************
 * @brief   sample_can_t6
 * Function filter���ܲ���:˫��������չ֡����
 * @author  taohu
 * @date    2022-04-14
 * @param   
		CANx:CAN����ַ
 * @return void
*******************************************************************************/    
void sample_can_t9(CAN_TypeDef* CANx, FunctionalState send)
{   
    u16 tx_count = 0,rx_count = 0;
    CanRxMsg RxMessage = {0};
	CanTxMsg TxMessage = {0};
    u32 ACR = 0xFBFFFFFF, AMR = 0xFFFFFFFE; //ACR��RTRλĬ��ƥ��

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
		
	    TxMessage.IDE = 1;     //��չ֡
		TxMessage.ExtId = 0X1FFFFFFF;

		//���ڷ���֡����
	    TxMessage.DLC   = 1;
		TxMessage.Data[0] = 0;

		while(1)
		{
			delay_ms(100);
			CAN_Transmit(CANx, &TxMessage);
			write_ahb32(CAN_MR,0x600);//start transmit
			
			//while(((read_ahb32(CAN_MR) >> 24) & CAN_TMI) == 0);
			printf("MR:%x\r\n",read_ahb32(CAN_MR));
			
			printf("Clear Flag:\r\n");	
			CAN_ClearFlag(CANx, CAN_TMI);
			//printf("MR:%x\r\n",read_ahb32(CAN_MR)); 
			if(++tx_count >= 80)
				break;

			TxMessage.Data[0] = tx_count;
		}
	    
	}
	else
	{
		//�����ж�
		//CAN_ITConfig(CANx, CAN_RMI, ENABLE);   
		//sample_can_nvic_cfg(CANx, ENABLE);

		//ѭ����������
	    while(1)
	    {	

			switch(rx_count/16)
			{
				case 0:      //MASKλΪ1����λ��0 ---- MASK:00
				{
					AMR = 0xFFFFFFFF;
					ACR = (~(1<<rx_count) & 0xFFFF);
					ACR <<= 16;
					ACR |= (~(1<<rx_count) & 0xFFFF);
				}
				break;
				
				case 1:      //MASKλ�ر�,����filterΪ1��unMask:01
				{
					AMR = 0;
					ACR = (~(1 << (rx_count%16)) & 0xFFFF); //��λΪ0
					ACR <<= 16;
					ACR |= 0xFFFF;    //��λΪ1
				}
				break;
				
				case 2:      //MASKλ�ر�,����filterΪ1��unMask:10
				{
					AMR = 0;
					ACR = 0xFFFF;    //��λΪ1
					ACR <<= 16;
					ACR |= (~(1 << (rx_count%16)) & 0xFFFF);  //��λΪ0
				}
				break;

				case 3:      //MASKλ�ر�,������filter��Ϊ1��unMask:11
				{
					AMR = 0;
					ACR = 0xFFFF;    //��λΪ1
					ACR <<= 16;
					ACR |= 0xFFFF;  //��λΪ1
				}
				break;

				case 4:      //MASKλ�ر�,������filter��Ϊ0��unMask:0
				{
					AMR = 0;
					ACR = (~(1<<(rx_count%16)) & 0xFFFF);
					ACR <<= 16;
					ACR |= (~(1<<(rx_count%16)) & 0xFFFF);
				}
				break;
				default:
				break;
			}
            
			//����filter
			sample_can_cfg_filter(CANx,ACR,AMR,DISABLE);

			
			while(((read_ahb32(CAN_MR) >> 24) & CAN_RMI) == 0);
			//printf("MR:%x\r\n",read_ahb32(CAN_MR));
			
			while(((read_ahb32(CAN_MR) >> 16) & 0x80) != 0)
			{
				RxMessage.ExtId = 0;
				CAN_Receive(CANx,&RxMessage);
				
				//printf("Clear Flag:\r\n");	
				CAN_ClearFlag(CANx, CAN_RMI);
				//printf("MR:%x\r\n",read_ahb32(CAN_MR));	
				
				//printf("IDE:%x\r\n",RxMessage.IDE);
				//printf("RTR:%x\r\n",RxMessage.RTR);
				//printf("DLC:%x\r\n",RxMessage.DLC);
				//printf("Ext:%x\r\n",RxMessage.ExtId);
				//printf("ExtId:%x\r\n",RxMessage.ExtId);
		
				printf("D[0]:%x\r\n",RxMessage.Data[0]);
			}
			
			if(++rx_count >= 80)
				break;
		}

	}

    //�������̣��ͷ���Դ��������ɴ�ӡ
    printf("file:%s,line:%d\r\ntest is finished\r\n",__FILE__,__LINE__);
}





