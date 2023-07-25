#include "sys.h"
#include "delay.h"
#include "sample_can.h"
#include "by_can.h"
#include "stm32f10x.h"

extern CAN_InitTypeDef CAN_InitStruct;
extern u8 can_data[8];

/*******************************************************************************
 * @brief   sample_can_t5
 * Function filter功能测试
 * @author  taohu
 * @date    2022-04-14
 * @param   
		CANx:CAN基地址
 * @return void
*******************************************************************************/    
void sample_can_t5(CAN_TypeDef* CANx, FunctionalState send)
{   
    u8 tx_count = 0,rx_count = 0,i = 0;
    CanRxMsg RxMessage = {0};
	CanTxMsg TxMessage = {0};
    u32 ACR = 0xFFFFEFFF, AMR = 0xFFFFFFFE; //ACR的RTR位默认匹配
	u32 temp = 0;

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
	    TxMessage.StdId = ((temp & 0xFF) << 3) | (temp >> 13);
	    TxMessage.DLC   = 3;
	    for(i = 0;i < 2;i++)
	    {
	        TxMessage.Data[i] = temp >> (16+8*i);
	    }
		TxMessage.Data[3] = 0;

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
			if(++tx_count >= 64)
				break;
			
			if((tx_count%2) == 0)
				temp = 0 << (tx_count/2);
            else
				temp = 1 << (tx_count/2);
				
			TxMessage.StdId = ((temp & 0xFF) << 3) | ((temp & 0xFF00) >> 13);
			
			for(i = 0;i < 2;i++)
	    	{
	        	TxMessage.Data[i] = temp >> (16+8*i);
	    	}
			TxMessage.Data[2] = tx_count;
		}
	    
	}
	else
	{
		//配置中断
		//CAN_ITConfig(CANx, CAN_RMI, ENABLE);   
		//sample_can_nvic_cfg(CANx, ENABLE);

		//配置filter
		sample_can_cfg_filter(CANx,ACR,AMR,ENABLE);
		//循环接收数据
	    while(1)
	    {	
			while(((read_ahb32(CAN_MR) >> 24) & CAN_RMI) == 0);
			//printf("MR:%x\r\n",read_ahb32(CAN_MR));
			
			while(((read_ahb32(CAN_MR) >> 16) & 0x80) != 0)
			{
				CAN_Receive(CANx,&RxMessage);
				
				//printf("Clear Flag:\r\n");	
				CAN_ClearFlag(CANx, CAN_RMI);
				//printf("MR:%x\r\n",read_ahb32(CAN_MR));	
				
				//printf("IDE:%x\r\n",RxMessage.IDE);
				//printf("RTR:%x\r\n",RxMessage.RTR);
				//printf("DLC:%x\r\n",RxMessage.DLC);
				printf("Std:%x\r\n",RxMessage.StdId);
				//printf("ExtId:%x\r\n",RxMessage.ExtId);
				for(i = 0; i <  3; i++)
			    {
					printf("D[%d]:%x\r\n",i,RxMessage.Data[i]);
				}
				printf("\r\n");
			}
			
			rx_count++;
			AMR = ~(1 << rx_count);
            
			//配置filter
			sample_can_cfg_filter(CANx,ACR,AMR,ENABLE);
			
			if(rx_count > 100)
				break;
		}

	}

    //测试流程，释放资源，测试完成打印
    printf("file:%s,line:%d\r\ntest is finished\r\n",__FILE__,__LINE__);
}




