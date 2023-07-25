#include "sys.h"
#include "delay.h"
#include "sample_can.h"
#include "by_can.h"
#include "stm32f10x.h"

extern CAN_InitTypeDef CAN_InitStruct;
extern u8 can_data[8];

/*******************************************************************************
 * @brief   sample_uart_test_t29
 * Function can中断测试
 * @author  taohu
 * @date    2022-04-19
 * @param   
		USARTx:USART基地址
 * @return void
*******************************************************************************/    
void sample_can_t29(CAN_TypeDef* CANx)
{   
	CanTxMsg TxMessage = {0};
    //CanRxMsg RxMessage = {0};
    
    //测试开始打印
    printf("test is started\r\nfile:%s,line:%d\r\n",__FILE__,__LINE__);
	
    write_ahb32(0x4001b018, 0x208);
    read_ahb32(0xE000E280) = 0xFFFFFFFF;//清除PENDING位
	printf("IP:%x\r\n",read_ahb32(0xE000E280));

    //配置参数并初始化
    sample_can_init(CANx);	//以默认参数初始化打印串口

#if 0   //DOI
	
	//设置FILTER
	sample_can_cfg_filter(CANx,0,0xFFFFFFFF, ENABLE);
	printf("AmR:%x\r\n",CANx->AMR);
	
	CANx->IMR |= CAN_DOIM;   
	sample_can_nvic_cfg(CANx, ENABLE);
	while(1);
#endif

#if 0   //RI
	
	//设置FILTER
	sample_can_cfg_filter(CANx,0,0xFFFFFFFF, ENABLE);
	printf("AmR:%x\r\n",CANx->AMR);
	
	CANx->IMR |= CAN_RMI;   
	sample_can_nvic_cfg(CANx, ENABLE);
	while(1);
#endif


#if 1   //TI
	
	//设置FILTER
	sample_can_cfg_filter(CANx,0,0xFFFFFFFF, ENABLE);
	printf("AmR:%x\r\n",CANx->AMR);
	
	CANx->IMR |= CAN_TMI;   
	sample_can_nvic_cfg(CANx, ENABLE);

	TxMessage.IDE = 0;     //标准帧
    TxMessage.StdId = 0x1FF; 
    TxMessage.DLC   = 1;
	
    TxMessage.Data[0] = can_data[0];
 
    CAN_Transmit(CANx, &TxMessage);
    write_ahb32(CAN_MR,0x400);//start transmit
    
	while(1);
#endif

#if 0  //BEI
	
	//设置FILTER
	sample_can_cfg_filter(CANx,0,0xFFFFFFFF, ENABLE);
	printf("AmR:%x\r\n",CANx->AMR);
	
	CANx->IMR |= CAN_BEMI;   
	sample_can_nvic_cfg(CANx, ENABLE);

	TxMessage.IDE = 0;     //标准帧
    TxMessage.StdId = 0x1FF; 
    TxMessage.DLC   = 1;
	
    TxMessage.Data[0] = can_data[0];
 
    CAN_Transmit(CANx, &TxMessage);
    write_ahb32(CAN_MR,0x400);//start transmit
    
	while(1);
#endif

#if 0   //CAN_EPMI
	
	//设置FILTER
	sample_can_cfg_filter(CANx,0,0xFFFFFFFF, ENABLE);
	printf("AmR:%x\r\n",CANx->AMR);
	
	CANx->IMR |= CAN_EPMI;   
	sample_can_nvic_cfg(CANx, ENABLE);

	TxMessage.IDE = 0;     //标准帧
    TxMessage.StdId = 0x1FF; 
    TxMessage.DLC   = 1;
	
    TxMessage.Data[0] = can_data[0];
 
    CAN_Transmit(CANx, &TxMessage);
    write_ahb32(CAN_MR,0x400);//start transmit
    
	while(1);
#endif

#if 0   //CAN_EWMI
	
	//设置FILTER
	sample_can_cfg_filter(CANx,0,0xFFFFFFFF, ENABLE);
	printf("AmR:%x\r\n",CANx->AMR);
	
	CANx->IMR |= CAN_EWMI;   
	sample_can_nvic_cfg(CANx, ENABLE);

	TxMessage.IDE = 0;     //标准帧
    TxMessage.StdId = 0x1FF; 
    TxMessage.DLC   = 1;
	
    TxMessage.Data[0] = can_data[0];
 
    CAN_Transmit(CANx, &TxMessage);
    write_ahb32(CAN_MR,0x400);//start transmit
    
	while(1);
#endif

#if 0   //CAN_ALMI
		
		//设置FILTER
		sample_can_cfg_filter(CANx,0,0xFFFFFFFF, ENABLE);
		printf("AmR:%x\r\n",CANx->AMR);
		
		CANx->IMR |= CAN_ALMI;	 
		sample_can_nvic_cfg(CANx, ENABLE);
	
		TxMessage.IDE = 1;	   //标准帧
		TxMessage.ExtId = 0xFFFFFF; 
		TxMessage.DLC	= 0;
		TxMessage.RTR	= 0;
	 
		CAN_Transmit(CANx, &TxMessage);
		write_ahb32(CAN_MR,0x400);//start transmit
		
		while(1);
#endif

    //测试流程，释放资源，测试完成打印
    //printf("file:%s,line:%d\r\ntest is finished\r\n",__FILE__,__LINE__);
}




