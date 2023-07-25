#include "sys.h"
#include "delay.h"
#include "sample_can.h"
#include "by_can.h"
#include "stm32f10x.h"

extern CAN_InitTypeDef CAN_InitStruct;
extern u8 can_data[8];

/*******************************************************************************
 * @brief   sample_uart_test_t26
 * Function 仲裁功能
 * @author  taohu
 * @date    2022-04-15
 * @param   

 * @return void
*******************************************************************************/    
void sample_can_t26(CAN_TypeDef* CANx)
{   
    u8 i = 0;
    CanTxMsg TxMessage;
    
    //测试开始打印
    printf("test is started\r\nfile:%s,line:%d\r\n",__FILE__,__LINE__);
	
    write_ahb32(0x4001b018, 0x208);
    read_ahb32(0xE000E280) = 0xFFFFFFFF;//清除PENDING位
	printf("IP:%x\r\n",read_ahb32(0xE000E280));

    //配置参数并初始化
    sample_can_init(CANx);	//以默认参数初始化打印串口

	//CAN_ITConfig(CANx, CAN_TMI, ENABLE);
	//sample_can_nvic_cfg(CANx, ENABLE);
	
    TxMessage.IDE = 1;     //标准帧
    TxMessage.ExtId = 0xFFFFFF;
	TxMessage.RTR = 1;
    TxMessage.DLC   = 0;
    for(i = 0;i < 8;i++)
    {
        TxMessage.Data[i] = can_data[i];
    }
    while(1)
    {
    	CAN_Transmit(CANx, &TxMessage);
    	write_ahb32(CAN_MR,0x400);//start transmit
    	//while(((read_ahb32(CAN_MR) >> 24) & CAN_TMI) == 0);

		if((read_ahb32(CAN_MR) >> 24) & 0x40)
		{
			printf("ISR:%x\r\n",(read_ahb32(CAN_MR) >> 24) & 0xFF);
			printf("ALC:%x\r\n",(read_ahb32(CAN_ERR_LOST) >> 24) & 0xFF);
			while(1);
		}
	
		//printf("Clear Flag:\r\n");	
		CAN_ClearFlag(CANx, CAN_TMI);
		//printf("MR:%x\r\n",read_ahb32(CAN_MR));	
    }
	
    //测试流程，释放资源，测试完成打印
    //printf("file:%s,line:%d\r\ntest is finished\r\n",__FILE__,__LINE__);
}




