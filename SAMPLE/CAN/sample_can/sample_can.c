#include "sys.h"
#include "delay.h"
#include "by_can.h"
#include "sample_can.h"
#include "stm32f10x.h"
  

/****************************************宏定义***************************************/
//C文件内使用宏定义



/****************************************枚举***************************************/
//C文件内使用枚举



/****************************************结构体***************************************/
//C文件内使用结构体


/**********************************************************************************/

CAN_InitTypeDef CAN_InitStruct = {
								     15,  //CAN_Prescaler	
  									 0,  //CAN_Mode 0:normal; 1:listen only					 
									 1,  //CAN_SJW		
									 4,  //CAN_BS1			
									 4,  //CAN_BS2		
									 0,  //CAN_SAMP	 
								 };
 CANFD_InitTypeDef CANFD_InitStruct ={
										 //NB
										 8,    //uint16_t CANFD_NBRP;
										 4,    //uint8_t CANFD_NSEG1;
										 4,     //uint8_t CANFD_NSEG2;
										 1,     //uint8_t CAN_NSJM;
											 
										 //data
										 8,     //uint8_t CANFD_DBRP;
										 1,     //uint8_t CANFD_DSJM;
										 4,     //uint8_t CANFD_DSEG1;
										 4,     //uint8_t CANFD_DSEG2;
										 0,		//uint8_t CANFD_TDCR;
										 0,     //uint8_t CANFD_TDCEN;
										 0,     //uint8_t CANFD_SSPP;
											 
										 //FD CONTROL
										 0,   //uint8_t CANFD_REOM:1;
										 0,   //uint8_t CANFD_DAR:1;	 
										 0,   //uint8_t CANFD_ISO:1;
										 1,   //uint8_t CANFD_EXTBT:1;
										 0,   //uint8_t CANFD_BRSEN:1;
										 1,   //uint8_t CANFD_EN:1;
										 0,	  //uint8_t rev:2;
										 0,	  //uint8_t rev1;
										 0    //uint16_t rev2;
									 };


/**********************************************************************************/

								 
u8 can_data[8] = {0xAA,0x55,0x00,0xFF,0xAA,0x55,0x00,0xFF};								 
								 
								 
	
 
 /*******************************************************************************
  * @brief	 uart_nvic_cfg
  * Function UART的NVIC配置
  * @author  taohu
  * @date	 2022-1-6
  * @param	 
		 USARTx:USART基地址；通过基地址选择中断通道号
  * @return void
 *******************************************************************************/
void sample_can_nvic_cfg(CAN_TypeDef* can, FunctionalState state)
{

	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = 20;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = state;		//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure); //根据指定的参数初始化NVIC寄存器
}

 
 /*******************************************************************************
  * @brief	 sample_uart_init
  * Function UART初始化接口
  * @author  taohu
  * @date	 2022-1-6
  * @param	 
		 USARTx:USART基地址
  * @return void
 *******************************************************************************/
 void sample_can_init(CAN_TypeDef* CANx)
 {
	 
	 //GPIO端口设置
	 //uart_gpio_clk_cfg((u32)USARTx);
	 
	 CAN_Init(CANx, &CAN_InitStruct); //初始化串口
	 //USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);
	 
	 //nvic设置
	 //sample_can_nvic_cfg((u32)CANx,ENABLE);
	 //使能串口

	 
	 printf("can address:0x%x\r\n",CANx);
 }
 
 /*******************************************************************************
  * @brief	 sample_uart_deinit
  * Function UART deinit接口
  * @author  taohu
  * @date	 2022-1-6
  * @param	 
		 USARTx:USART基地址
  * @return void
 *******************************************************************************/
 void sample_can_deinit(CAN_TypeDef* can)
 {

 }

 /*******************************************************************************
  * @brief	 sample_uart_init
  * Function UART初始化接口
  * @author  taohu
  * @date	 2022-1-6
  * @param	 
		 USARTx:USART基地址
  * @return void
 *******************************************************************************/
 void sample_can_cfg_filter(CAN_TypeDef* CANx,u32 ACR,u32 AMR, FunctionalState single)
 {
	//设置FILTER
	CANx->MR |=0x04;
	
	if(single == ENABLE)
		CANx->MR |=0x01;
	else
		CANx->MR &=0xFE;
	
	CANx->AMR = AMR;
	CANx->ACR = ACR;
	CANx->MR &= 0xFB;
	
	printf("AMR:%x\r\n",CANx->AMR);
	printf("ACR:%x\r\n",CANx->ACR);
 }

 #if 0
 /*******************************************************************************
  * @brief	 sample_uart_set_buad
  * Function 修改配置参数接口
  * @author  taohu
  * @date	 2022-1-6
  * @param	 
		 type:修改参数类型
		 param：参数值
  * @return void
 *******************************************************************************/
//void sample_can_set_param(T_eUART_PARAM type,u32 param)
// {

// }
 
 

 /*******************************************************************************
  * @brief	 sample_uart_read_buad
  * Function 读取配置参数接口
  * @author  taohu
  * @date	 2022-1-6
  * @param	 
		 type:读取参数类型
  * @return void
 *******************************************************************************/
 u32 sample_uart_read_param(T_eUART_PARAM type)
 {
	 switch(type)
	 {
		 case eUSART_BAUD:
			 return USART_InitStructure.USART_BaudRate;
	 
		 case eUSART_WORDLEN:
			 return USART_InitStructure.USART_WordLength;
			 
		 case eUSART_STPB:
			 return USART_InitStructure.USART_StopBits;
 
		 case eUSART_PRT:
			 return USART_InitStructure.USART_Parity;
 
		 case eUSART_MODE:
			 return USART_InitStructure.USART_Mode;
			 
		 case eUSART_FLOW:
			 return USART_InitStructure.USART_HardwareFlowControl;	 
 
		 default:
			 return 0;
	 }
 }
#endif

/*******************************************************************************
 * @brief	sample_can_fd_init
 * Function UART初始化接口
 * @author	taohu
 * @date	2022-1-6
 * @param	
		USARTx:USART基地址
 * @return void
*******************************************************************************/
void sample_can_fd_init(CAN_TypeDef* CANx)
{
	
	//GPIO端口设置
	//uart_gpio_clk_cfg((u32)USARTx);
	
	CANFD_Init(CANx, &CANFD_InitStruct);
	//USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);
	
	//nvic设置
	//sample_can_nvic_cfg((u32)CANx,ENABLE);
	//使能串口

	
	printf("can address:0x%x\r\n",CANx);
}



void CAN_IRQHandler(void)
{

	printf("CAN_IRQ\r\n");
	
	printf("ISR:%x\r\n",(read_ahb32(CAN_MR) >> 24) &0xFF);	
	printf("IMR:%x\r\n",read_ahb32(CAN_IMR));

#if 0
	 u8 i = 0;
    CanRxMsg RxMessage;
	
 	printf("MR:%x\r\n",read_ahb32(CAN_MR));	
	printf("IMR:%x\r\n",read_ahb32(CAN_IMR));
	
	CAN_ClearFlag(CAN1, CAN_TMI);
	CAN_ClearFlag(CAN1, CAN_RMI);
	
	CAN_Receive(CAN1,&RxMessage);

	printf("IDE:%x\r\n",RxMessage.IDE);
	printf("RTR:%x\r\n",RxMessage.RTR);
	printf("DLC:%x\r\n",RxMessage.DLC);
	printf("StdId:%x\r\n",RxMessage.StdId);

	for( i= 0;i < RxMessage.DLC; i++)
	{
		printf("DATA[%d]:%x\r\n",i,RxMessage.Data[i]);
	}
#endif
}

