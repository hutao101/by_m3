#include "sys.h"
#include "delay.h"
#include "sample_can.h"
#include "by_can.h"
#include "stm32f10x.h"

extern CAN_InitTypeDef CAN_InitStruct;
extern u8 can_data[8];

/*******************************************************************************
 * @brief   sample_can_t6
 * Function filter功能测试:双过滤器扩展帧接收
 * @author  taohu
 * @date    2022-04-14
 * @param   
		CANx:CAN基地址
 * @return void
*******************************************************************************/    
void sample_can_t8(CAN_TypeDef* CANx, FunctionalState send)
{   
    u16 temp = 0,tx_count = 0,rx_count = 0;
    CanRxMsg RxMessage = {0};
	CanTxMsg TxMessage = {0};
    u32 ACR = 0xFBFFFFFF, AMR = 0xFFFFFFFE; //ACR的RTR位默认匹配
    Filter_StdTypeDef tmp_f = {0};

    //测试开始打印
    printf("test is started\r\nfile:%s,line:%d\r\n",__FILE__,__LINE__);
	write_ahb32(0x4001b018, 0x208);
    read_ahb32(0xE000E280) = 0xFFFFFFFF;//清除PENDING位
	printf("PENDING:%x\r\n",read_ahb32(0xE000E280));

	//配置参数并初始化
    sample_can_init(CANx);	//以默认参数初始化打印串口
	
	if(send == ENABLE)
	{
		//CAN_ITConfig(CANx, CAN_TMI, ENABLE);
		//sample_can_nvic_cfg(CANx, ENABLE);
		
	    TxMessage.IDE = 0;     //标准帧
		TxMessage.StdId = 0x7FF;

		//用于发送帧计数
	    TxMessage.DLC   = 2;
		TxMessage.Data[0] = 0xFF;
		TxMessage.Data[1] = 0;

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

			TxMessage.Data[1] = tx_count;
		}
	    
	}
	else
	{
		//配置中断
		//CAN_ITConfig(CANx, CAN_RMI, ENABLE);   
		//sample_can_nvic_cfg(CANx, ENABLE);

		//循环接收数据
	    while(1)
	    {	

			switch(rx_count/11)
			{
				case 0:      //MASK位为1，逐位置0 ---- MASK:00;轮询ID，DATA段=0
				{
					AMR = 0XFFFFFFFF;
					tmp_f.FLT1_DATAH = 0;
					tmp_f.FLT1_DATAL = 0;
					tmp_f.FLT1_RTR = 1;
					tmp_f.FLT2_RTR = 1;
			
					temp = ~(1 << (rx_count%11)) & 0x7FF;
					
					tmp_f.FLT1_IDL3 = temp & 0x07;
					tmp_f.FLT1_IDH = (temp >> 3) & 0xFF;


					tmp_f.FLT2_IDL3 = temp & 0x07;
					tmp_f.FLT2_IDH = (temp >> 3) & 0xFF;

					ACR = *(u32*)(&tmp_f);

				}
				break;
				
				case 1:      //MASK位关闭,第一filter全为1；unMask:01
				{
					AMR = 0;
					tmp_f.FLT1_DATAH = 0xF;
					tmp_f.FLT1_DATAL = 0xF;
					tmp_f.FLT1_RTR = 0;
					tmp_f.FLT2_RTR = 0;
					tmp_f.FLT1_IDL3 = 0x07;
					tmp_f.FLT1_IDH = 0xFF;


					temp = ~(1 << (rx_count%11)) & 0x7FF;
					tmp_f.FLT2_IDL3 = temp & 0x07;
					tmp_f.FLT2_IDH = (temp >> 3) & 0xFF;
				
					ACR = *(u32*)(&tmp_f);
				}
				break;
				
				case 2:      //MASK位关闭,第二filter全为1；unMask:10；data=0XFF
				{
					AMR = 0;
					tmp_f.FLT1_RTR = 0;
					tmp_f.FLT2_RTR = 0;
					tmp_f.FLT2_IDL3 =0x07;
					tmp_f.FLT2_IDH = 0xFF;
					tmp_f.FLT1_DATAH = 0xF;
					tmp_f.FLT1_DATAL = 0xF;
					
					temp = ~(1 << (rx_count%11)) & 0x7FF;

					tmp_f.FLT1_IDL3 = temp & 0x07;
					tmp_f.FLT1_IDH = (temp >> 3) & 0xFF;
				
					ACR = *(u32*)(&tmp_f);

				}
				break;

				case 3:      //MASK位关闭,第二filter全为1；unMask:10；data遍历
				{
					AMR = 0;
					tmp_f.FLT1_RTR = 0;
					tmp_f.FLT2_RTR = 0;
					tmp_f.FLT2_IDL3 =0x07;
					tmp_f.FLT2_IDH = 0xFF;
					tmp_f.FLT1_IDL3 = 0x07;
					tmp_f.FLT1_IDH = 0xFF;
					
					temp = ~(1 << (rx_count%11)) & 0x7FF;

					tmp_f.FLT1_DATAL = temp & 0xF;
					tmp_f.FLT1_DATAH = (temp>>4)& 0xF;
	
					ACR = *(u32*)(&tmp_f);
				}
				break;

				case 4:      //MASK位关闭,两个个filter都为1；unMask:11
				{
				
					AMR = 0;
					tmp_f.FLT1_RTR = 0;
					tmp_f.FLT2_RTR = 0;
					tmp_f.FLT2_IDL3 =0x07;
					tmp_f.FLT2_IDH = 0xFF;
					tmp_f.FLT1_IDL3 = 0x07;
					tmp_f.FLT1_IDH = 0xFF;

					tmp_f.FLT1_DATAL = 0xF;
					tmp_f.FLT1_DATAH = 0xF;
	
					ACR = *(u32*)(&tmp_f);
				}
				break;
				
				case 5:      //MASK位关闭,两个个filter都为0；unMask:00
				{
					AMR = 0;
					tmp_f.FLT1_DATAH = 0xF;
					tmp_f.FLT1_DATAL = 0xF;
					tmp_f.FLT1_RTR = 0;
					tmp_f.FLT2_RTR = 0;
			
					temp = ~(1 << (rx_count%11)) & 0x7FF;
					
					tmp_f.FLT1_IDL3 = temp & 0x07;
					tmp_f.FLT1_IDH = (temp >> 3) & 0xFF;


					tmp_f.FLT2_IDL3 = temp & 0x07;
					tmp_f.FLT2_IDH = (temp >> 3) & 0xFF;

					ACR = *(u32*)(&tmp_f);
				}
				break;
				default:
				break;
			}
            
			//配置filter
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
		
				printf("D[1]:%x\r\n",RxMessage.Data[1]);
			}
			
			if(++rx_count >= 80)
				break;
		}

	}

    //测试流程，释放资源，测试完成打印
    printf("file:%s,line:%d\r\ntest is finished\r\n",__FILE__,__LINE__);
}





