#include "sys.h"
#include "delay.h"
#include "sample_can.h"
#include "by_can.h"
#include "stm32f10x.h"

extern CAN_InitTypeDef CAN_InitStruct;
extern u8 can_data[8];

/*******************************************************************************
 * @brief   sample_uart_test_t29
 * Function can�жϲ���
 * @author  taohu
 * @date    2022-04-19
 * @param   
		USARTx:USART����ַ
 * @return void
*******************************************************************************/    
void sample_can_t29(CAN_TypeDef* CANx)
{   
	CanTxMsg TxMessage = {0};
    //CanRxMsg RxMessage = {0};
    
    //���Կ�ʼ��ӡ
    printf("test is started\r\nfile:%s,line:%d\r\n",__FILE__,__LINE__);
	
    write_ahb32(0x4001b018, 0x208);
    read_ahb32(0xE000E280) = 0xFFFFFFFF;//���PENDINGλ
	printf("IP:%x\r\n",read_ahb32(0xE000E280));

    //���ò�������ʼ��
    sample_can_init(CANx);	//��Ĭ�ϲ�����ʼ����ӡ����

#if 0   //DOI
	
	//����FILTER
	sample_can_cfg_filter(CANx,0,0xFFFFFFFF, ENABLE);
	printf("AmR:%x\r\n",CANx->AMR);
	
	CANx->IMR |= CAN_DOIM;   
	sample_can_nvic_cfg(CANx, ENABLE);
	while(1);
#endif

#if 0   //RI
	
	//����FILTER
	sample_can_cfg_filter(CANx,0,0xFFFFFFFF, ENABLE);
	printf("AmR:%x\r\n",CANx->AMR);
	
	CANx->IMR |= CAN_RMI;   
	sample_can_nvic_cfg(CANx, ENABLE);
	while(1);
#endif


#if 1   //TI
	
	//����FILTER
	sample_can_cfg_filter(CANx,0,0xFFFFFFFF, ENABLE);
	printf("AmR:%x\r\n",CANx->AMR);
	
	CANx->IMR |= CAN_TMI;   
	sample_can_nvic_cfg(CANx, ENABLE);

	TxMessage.IDE = 0;     //��׼֡
    TxMessage.StdId = 0x1FF; 
    TxMessage.DLC   = 1;
	
    TxMessage.Data[0] = can_data[0];
 
    CAN_Transmit(CANx, &TxMessage);
    write_ahb32(CAN_MR,0x400);//start transmit
    
	while(1);
#endif

#if 0  //BEI
	
	//����FILTER
	sample_can_cfg_filter(CANx,0,0xFFFFFFFF, ENABLE);
	printf("AmR:%x\r\n",CANx->AMR);
	
	CANx->IMR |= CAN_BEMI;   
	sample_can_nvic_cfg(CANx, ENABLE);

	TxMessage.IDE = 0;     //��׼֡
    TxMessage.StdId = 0x1FF; 
    TxMessage.DLC   = 1;
	
    TxMessage.Data[0] = can_data[0];
 
    CAN_Transmit(CANx, &TxMessage);
    write_ahb32(CAN_MR,0x400);//start transmit
    
	while(1);
#endif

#if 0   //CAN_EPMI
	
	//����FILTER
	sample_can_cfg_filter(CANx,0,0xFFFFFFFF, ENABLE);
	printf("AmR:%x\r\n",CANx->AMR);
	
	CANx->IMR |= CAN_EPMI;   
	sample_can_nvic_cfg(CANx, ENABLE);

	TxMessage.IDE = 0;     //��׼֡
    TxMessage.StdId = 0x1FF; 
    TxMessage.DLC   = 1;
	
    TxMessage.Data[0] = can_data[0];
 
    CAN_Transmit(CANx, &TxMessage);
    write_ahb32(CAN_MR,0x400);//start transmit
    
	while(1);
#endif

#if 0   //CAN_EWMI
	
	//����FILTER
	sample_can_cfg_filter(CANx,0,0xFFFFFFFF, ENABLE);
	printf("AmR:%x\r\n",CANx->AMR);
	
	CANx->IMR |= CAN_EWMI;   
	sample_can_nvic_cfg(CANx, ENABLE);

	TxMessage.IDE = 0;     //��׼֡
    TxMessage.StdId = 0x1FF; 
    TxMessage.DLC   = 1;
	
    TxMessage.Data[0] = can_data[0];
 
    CAN_Transmit(CANx, &TxMessage);
    write_ahb32(CAN_MR,0x400);//start transmit
    
	while(1);
#endif

#if 0   //CAN_ALMI
		
		//����FILTER
		sample_can_cfg_filter(CANx,0,0xFFFFFFFF, ENABLE);
		printf("AmR:%x\r\n",CANx->AMR);
		
		CANx->IMR |= CAN_ALMI;	 
		sample_can_nvic_cfg(CANx, ENABLE);
	
		TxMessage.IDE = 1;	   //��׼֡
		TxMessage.ExtId = 0xFFFFFF; 
		TxMessage.DLC	= 0;
		TxMessage.RTR	= 0;
	 
		CAN_Transmit(CANx, &TxMessage);
		write_ahb32(CAN_MR,0x400);//start transmit
		
		while(1);
#endif

    //�������̣��ͷ���Դ��������ɴ�ӡ
    //printf("file:%s,line:%d\r\ntest is finished\r\n",__FILE__,__LINE__);
}




