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
void sample_can_t9(CAN_TypeDef* CANx, FunctionalState send)
{   
    u16 tx_count = 0,rx_count = 0;
    CanRxMsg RxMessage = {0};
	CanTxMsg TxMessage = {0};
    u32 ACR = 0xFBFFFFFF, AMR = 0xFFFFFFFE; //ACR的RTR位默认匹配

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
		
	    TxMessage.IDE = 1;     //扩展帧
		TxMessage.ExtId = 0X1FFFFFFF;

		//用于发送帧计数
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
		//配置中断
		//CAN_ITConfig(CANx, CAN_RMI, ENABLE);   
		//sample_can_nvic_cfg(CANx, ENABLE);

		//循环接收数据
	    while(1)
	    {	

			switch(rx_count/16)
			{
				case 0:      //MASK位为1，逐位置0 ---- MASK:00
				{
					AMR = 0xFFFFFFFF;
					ACR = (~(1<<rx_count) & 0xFFFF);
					ACR <<= 16;
					ACR |= (~(1<<rx_count) & 0xFFFF);
				}
				break;
				
				case 1:      //MASK位关闭,单个filter为1；unMask:01
				{
					AMR = 0;
					ACR = (~(1 << (rx_count%16)) & 0xFFFF); //高位为0
					ACR <<= 16;
					ACR |= 0xFFFF;    //低位为1
				}
				break;
				
				case 2:      //MASK位关闭,单个filter为1；unMask:10
				{
					AMR = 0;
					ACR = 0xFFFF;    //高位为1
					ACR <<= 16;
					ACR |= (~(1 << (rx_count%16)) & 0xFFFF);  //低位为0
				}
				break;

				case 3:      //MASK位关闭,两个个filter都为1；unMask:11
				{
					AMR = 0;
					ACR = 0xFFFF;    //高位为1
					ACR <<= 16;
					ACR |= 0xFFFF;  //低位为1
				}
				break;

				case 4:      //MASK位关闭,两个个filter都为0；unMask:0
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
		
				printf("D[0]:%x\r\n",RxMessage.Data[0]);
			}
			
			if(++rx_count >= 80)
				break;
		}

	}

    //测试流程，释放资源，测试完成打印
    printf("file:%s,line:%d\r\ntest is finished\r\n",__FILE__,__LINE__);
}





