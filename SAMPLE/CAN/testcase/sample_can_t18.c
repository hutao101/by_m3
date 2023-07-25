#include "sys.h"
#include "delay.h"
#include "sample_can.h"
#include "by_can.h"
#include "stm32f10x.h"

extern CAN_InitTypeDef CAN_InitStruct;
extern u8 can_data[8];

/*******************************************************************************
 * @brief   sample_uart_test_t1
 * Function 打开CTS流控模式，连续发送数据
 * @author  taohu
 * @date    2022-03-15
 * @param   
		USARTx:USART基地址
 * @return void
*******************************************************************************/    
void sample_can_t18(CAN_TypeDef* CANx)
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
	
    TxMessage.IDE = 0;     //标准帧
    TxMessage.StdId = 0x1FF; 
    TxMessage.DLC   = 8;
    for(i = 0;i < 8;i++)
    {
        TxMessage.Data[i] = can_data[i];
    }
	//write_ahb32(CAN_TX_BUF,0xAA55AA01);//TX_BUF
    while(1)
    {
    CAN_Transmit(CANx, &TxMessage);
    write_ahb32(CAN_MR,0x400);//start transmit
	
	while(((read_ahb32(CAN_MR) >> 24) & CAN_TMI) == 0);
	printf("MR:%x\r\n",read_ahb32(CAN_MR));
	
	printf("Clear Flag:\r\n");	
	CAN_ClearFlag(CANx, CAN_TMI);
	printf("MR:%x\r\n",read_ahb32(CAN_MR));	
    }
	
    //测试流程，释放资源，测试完成打印
    //printf("file:%s,line:%d\r\ntest is finished\r\n",__FILE__,__LINE__);
}




