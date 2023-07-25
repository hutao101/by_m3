#ifndef __SAMPLE_USART_H
#define __SAMPLE_USART_H
#include "stdio.h"	
#include "sys.h" 
//#include "sample_can_conf.h" 

typedef struct
{
	u8 FLT1_IDH;
	u8 FLT1_DATAH:4;
	u8 FLT1_RTR:1;
	u8 FLT1_IDL3:3;
	u8 FLT2_IDH;
	u8 FLT1_DATAL:4;
	u8 FLT2_RTR:1;
	u8 FLT2_IDL3:3;
} Filter_StdTypeDef;




void sample_can_init(CAN_TypeDef* CANx);
void sample_can_nvic_cfg(CAN_TypeDef* can, FunctionalState state);
void sample_can_cfg_filter(CAN_TypeDef* CANx,u32 ACR,u32 AMR,FunctionalState single);


//FD模式
void sample_can_fd_init(CAN_TypeDef* CANx);

//TEST CASE 
void sample_can_t1(CAN_TypeDef* CANx);
void sample_can_t2(CAN_TypeDef* CANx);
void sample_can_t3(CAN_TypeDef* CANx);
void sample_can_t4(CAN_TypeDef* CANx);
void sample_can_t5(CAN_TypeDef* CANx, FunctionalState send);
void sample_can_t6(CAN_TypeDef* CANx, FunctionalState send);			
void sample_can_t7(CAN_TypeDef* CANx, FunctionalState send);
void sample_can_t8(CAN_TypeDef* CANx, FunctionalState send);
void sample_can_t9(CAN_TypeDef* CANx, FunctionalState send);
void sample_can_t10(CAN_TypeDef* CANx, FunctionalState send);
void sample_can_t11(CAN_TypeDef* CANx);
void sample_can_t12(CAN_TypeDef* CANx);
void sample_can_t13(CAN_TypeDef* CANx);
void sample_can_t14(CAN_TypeDef* CANx);
void sample_can_t15(CAN_TypeDef* CANx);
void sample_can_t16(CAN_TypeDef* CANx);

void sample_can_t17(CAN_TypeDef* CANx);
void sample_can_t18(CAN_TypeDef* CANx);
void sample_can_t19(CAN_TypeDef* CANx);
void sample_can_t20(CAN_TypeDef* CANx);

void sample_can_t26(CAN_TypeDef* CANx);
void sample_can_t29(CAN_TypeDef* CANx);


#endif

