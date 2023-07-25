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
void sample_can_t4(CAN_TypeDef* CANx)
{   
    u8 count = 0,i = 0;
    CanRxMsg RxMessage = {0};
    
    //测试开始打印
    printf("test is started\r\nfile:%s,line:%d\r\n",__FILE__,__LINE__);
	
    write_ahb32(0x4001b018, 0x208);
    read_ahb32(0xE000E280) = 0xFFFFFFFF;//清除PENDING位
	printf("IP:%x\r\n",read_ahb32(0xE000E280));

    //配置参数并初始化
    sample_can_init(CANx);	//以默认参数初始化打印串口

	CANx->IMR |= CAN_RMI;   
	//sample_can_nvic_cfg(CANx, ENABLE);
	
	//设置FILTER
	CANx->MR |=0x04;
	CANx->MR |=0x01;
	CANx->AMR = 0xFFFFFFFF;
	printf("AmR1:%x\r\n",CANx->AMR);

	//write_ahb32(CAN_ACR0_3,0x00AAE03F);
	printf("MR:%x\r\n",CANx->MR);
	CANx->MR &= 0xFB;
	printf("MR:%x\r\n",CANx->MR);


	//循环接收数据
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

    //测试流程，释放资源，测试完成打印
    printf("file:%s,line:%d\r\ntest is finished\r\n",__FILE__,__LINE__);
}




