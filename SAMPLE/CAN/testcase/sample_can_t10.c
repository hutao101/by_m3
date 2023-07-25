#include "sys.h"
#include "delay.h"
#include "sample_can.h"
#include "by_can.h"
#include "stm32f10x.h"

extern CAN_InitTypeDef CAN_InitStruct;
extern u8 can_data[8];

/*******************************************************************************
 * @brief   sample_can_t10
 * Function filter���ܲ���:˫��������׼Զ��֡����
 * @author  taohu
 * @date    2022-04-14
 * @param   
		CANx:CAN����ַ
 * @return void
*******************************************************************************/    
void sample_can_t10(CAN_TypeDef* CANx, FunctionalState send)
{   
    u16 tx_count = 0,rx_count = 0;
    CanRxMsg RxMessage = {0};
	CanTxMsg TxMessage = {0};
    u32 ACR = 0xFBFFFFFF, AMR = 0xFFFFFFFE; //ACR��RTRλĬ��ƥ��
    Filter_StdTypeDef tmp_f = {0};

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
		
	    TxMessage.IDE = 0;     //��׼
		TxMessage.StdId = 0;
		TxMessage.RTR = 1;

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
			if(++tx_count >= 5)
				break;

			TxMessage.StdId = tx_count;
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

			switch(rx_count)
			{
				case 0:      //MASKλΪ1����λ��0 ---- MASK:00;��ѯID��DATA��=0
				{
					AMR = 0XFFFFFFFF;
					tmp_f.FLT1_DATAH = 0xF;
					tmp_f.FLT1_DATAL = 0xF;
					tmp_f.FLT1_RTR = 0;
					tmp_f.FLT2_RTR = 0;
					
					tmp_f.FLT1_IDL3 = 0x07;
					tmp_f.FLT1_IDH =  0xFF;

					tmp_f.FLT2_IDL3 = 0x07;
					tmp_f.FLT2_IDH = 0xFF;

					ACR = *(u32*)(&tmp_f);
				}
				break;
				
				case 1:      //MASKλ�ر�,��һfilter RTRΪ1��unMask:01
				{
					AMR = 0xE0FFE0FF;
					tmp_f.FLT1_DATAH = 0xF;
					tmp_f.FLT1_DATAL = 0xF;
					tmp_f.FLT1_RTR = 1;
					tmp_f.FLT2_RTR = 0;
					tmp_f.FLT1_IDL3 = 0x07;
					tmp_f.FLT1_IDH = 0xFF;

					tmp_f.FLT2_IDL3 = 0x07;
					tmp_f.FLT2_IDH = 0xFF;
				
					ACR = *(u32*)(&tmp_f);
				}
				break;
				
				case 2:      //MASKλ�ر�,�ڶ�filterȫΪ1��unMask:10��data=0XFF
				{
					AMR = 0xE0FFE0FF;
					tmp_f.FLT1_DATAH = 0xF;
					tmp_f.FLT1_DATAL = 0xF;
					tmp_f.FLT1_RTR = 0;
					tmp_f.FLT2_RTR = 1;
					tmp_f.FLT1_IDL3 = 0x07;
					tmp_f.FLT1_IDH = 0xFF;

					tmp_f.FLT2_IDL3 = 0x07;
					tmp_f.FLT2_IDH = 0xFF;
				
					ACR = *(u32*)(&tmp_f);

				}
				break;

				case 3:      //MASKλ�ر�,�ڶ�filterȫΪ1��unMask:10��data����
				{
					AMR = 0xE0FFE0FF;
					tmp_f.FLT1_DATAH = 0xF;
					tmp_f.FLT1_DATAL = 0xF;
					tmp_f.FLT1_RTR = 1;
					tmp_f.FLT2_RTR = 1;
					tmp_f.FLT1_IDL3 = 0x07;
					tmp_f.FLT1_IDH = 0xFF;
					
					tmp_f.FLT2_IDL3 = 0x07;
					tmp_f.FLT2_IDH = 0xFF;
				
					ACR = *(u32*)(&tmp_f);

				}
				break;

				case 4:      //MASKλ�ر�,������filter��Ϊ1��unMask:11
				{
					AMR = 0xE0FFE0FF;
					tmp_f.FLT1_DATAH = 0xF;
					tmp_f.FLT1_DATAL = 0xF;
					tmp_f.FLT1_RTR = 0;
					tmp_f.FLT2_RTR = 0;
					tmp_f.FLT1_IDL3 = 0x07;
					tmp_f.FLT1_IDH = 0xFF;
					
					tmp_f.FLT2_IDL3 = 0x07;
					tmp_f.FLT2_IDH = 0xFF;
					
					ACR = *(u32*)(&tmp_f);

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
				printf("RTR:%x\r\n",RxMessage.RTR);
				//printf("DLC:%x\r\n",RxMessage.DLC);
				printf("Std:%x\r\n",RxMessage.StdId);
				//printf("ExtId:%x\r\n",RxMessage.ExtId);
		
				//printf("D[1]:%x\r\n",RxMessage.Data[1]);
			}
			
			if(++rx_count >= 5)
				break;
		}

	}

    //�������̣��ͷ���Դ��������ɴ�ӡ
    printf("file:%s,line:%d\r\ntest is finished\r\n",__FILE__,__LINE__);
}





