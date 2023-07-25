//********************************************************************
//(Copyright 2012) i2c Inc. All Rights Reserved
//Company confidential and Properietary information.
//This information may not be disclosed to unauthorized individual.
//********************************************************************
//
// i2c Microelectronics Co.,Ltd.
//
// File Name: can.h
// Author:     
// Version:   
// Created:   
// Modified:
// Description:
//********************************************************************
#ifndef _by_can_h
#define _by_can_h
#include "stm32f10x.h"
//********************************************************************

#define CAN_MR CAN_BASE
#define CAN_IMR CAN_BASE+0x4
#define CAN_TX_BUF CAN_BASE+0x08
#define CAN_RX_BUF CAN_BASE+0x0C
#define CAN_ACR0_3 CAN_BASE+0x10
#define CAN_AMR0_3 CAN_BASE+0x14
#define CAN_ERR_LOST CAN_BASE+0x18
#define CAN_NBT CAN_BASE+0x1C
#define CAN_DBT CAN_BASE+0x20
#define CAN_FD CAN_BASE+0x24


//MR mode register
#define CAN_RST_MODE 0x04
#define CAN_RLEASE_MODE (0xfffffffb)
#define CAN_LISTEN_MODE 0x01
#define CAN_AFM_DUAL 0x00
#define CAN_AFM_SINGLE 0x01

//CMR command register
#define CAN_TranReq_CONTINUE (0x4)<<8
#define CAN_TranReq_SINGLE  (0x6)<<8
#define CAN_TranAbort  (0x2)<<8

//SR status register
#define CAN_Bus_Off (0x1)<<16
#define CAN_Bus_Err (0x2)<<16
#define CAN_MsgTraning (0x4)<<16
#define CAN_MsgRecving (0x8)<<16
#define CAN_TranBuffRelease (0x20)<<16
#define CAN_TranBuffBusy (0xdf)<<16
#define CAN_RecvOverFlow (0x40)<<16
#define CAN_RecvValidData (0x80)<<16

// can interrupt register
#define CAN_DOI (0x1)<<24 // RECV FIFO OVERRUN
#define CAN_BEI (0x2)<<24 // BUS ERROR INTERRUPT
#define CAN_TI (0x4)<<24 // SUCCESSFUL TRANSMISSION
#define CAN_RI (0x8)<<24 // AT LEAST ONE MESSAGE IN RECEIVE FIFO
#define CAN_EPI (0x10)<<24 // ERROR PASSIVE LEVEL
#define CAN_EWI (0x20)<<24 // CHANGE WHEN ES/BS HIGH IN SR
#define CAN_ALI (0x40)<<24 // LOST ARBITRATION BECAME A RECEIVER

// CAN INTERRUPT MASK
#define CAN_DOIM (0x1) // RECV FIFO OVERRUN
#define CAN_BEMI (0x2) // BUS ERROR INTERRUPT
#define CAN_TMI (0x4) // SUCCESSFUL TRANSMISSION
#define CAN_RMI (0x8) // AT LEAST ONE MESSAGE IN RECEIVE FIFO
#define CAN_EPMI (0x10) // ERROR PASSIVE LEVEL
#define CAN_EWMI (0x20) // CHANGE WHEN ES/BS HIGH IN SR
#define CAN_ALMI (0x40) // LOST ARBITRATION BECAME A RECEIVER

//can timing register 0
#define CAN_BRP_1 ((0x0)&0x3F)<<16 // 6bits divider tclk of can control to tsclk
#define CAN_BRP_(x) ((x)&0x3F)<<16 // 6bits divider tclk of can control to tsclk
//Tsclk = 2 tclk * (32*BRP.5+16*BRP.4+8*BRP.3+4*BRP.2+2*BRP.1+BRP.0+1)
#define CAN_SJW_1 (((0x0)&0x3)<<6)<<16 // 2bits for SJW
#define CAN_SJW_(x) (((x)&0x3)<<6)<<16 // 2bits for SJW
//Tsjw=Tsclk*ги2xSJW.1+SJW0+1)
//can timing register 1
#define CAN_TSEG1_1 ((0x0)&0xf)<<24 // 
#define CAN_TSEG2_1 (((0x0)&0x7)<<4)<<24 // 
#define CAN_TSEG1_(x) ((x)&0xf)<<24 // 
#define CAN_TSEG2_(x) (((x)&0x7)<<4)<<24 //
//Tsyncseg = Tsclk
//Ttseg1 = Tsclk*(8*Tseg1.3+4*Tseg1.2+2*Tseg1.1+tseg1.0+1)
//Ttseg2 = Tsclk*(4*Tseg2.2+2*Tseg2.1+tseg2.0+1)
#define CAN_SAM_ONCE (((0x0)&0x1)<<8)<<24 // 
#define CAN_SAM_TRI (((0x1)&0x1)<<8)<<24 // 

//can error code //0x18

//CAN FD cfg 

#define CAN_NBRP_(x) ((x)&0x3ff)
#define CAN_NSEG1_(x) (((x)&0x3f)<<10)
#define CAN_NSEG2_(x) (((x)&0xf)<<16)
#define CAN_NSJM_(x) (((x)&0xf)<<20)

//data
#define CAN_DBRP_(x) ((x)&0x1f)
#define CAN_DSJM_(x) (((x)&0x7)<<5)
#define CAN_DSEG1_(x) (((x)&0x1f)<<8)
#define CAN_DSEG2_(x) (((x)&0x7)<<13)
#define CAN_TDCR_(x) (((x)&0x7f)<<16)
#define CAN_TDCEN_(x) (((x)&0x1)<<23)
#define CAN_SSPP_(x) (((x)&0x7F)<<24)

//FD CONTROL
#define CAN_FD_REOM ((0x1)<<5)// RESTRICTED OR CLASS MODE
#define CAN_CLASS_MD ((0x0)<<5)
#define CAN_FD_DAR ((0x1)<<4)// DISABLE AUTO RETRANSMISSION
#define CAN_FD_EAR ((0x0)<<4)
#define CAN_FD_11898 ((0x1)<<3)
#define CAN_FD_Bosch ((0x0)<<3)
#define CAN_FD_EXTBT_NBP ((0x1)<<2)
#define CAN_FD_EXTBT_BTR ((0x0)<<2)
#define CAN_FD_BATERATE_SEN ((0x1)<<1)
#define CAN_FD_BATERATE_KEEP ((0x1)<<1)
#define CAN_FD_EN ((0x1)<<0)
#define CAN_FD_NEN ((0x0)<<0)

//fd error
#define CAN_ST_INTEGRATING ((0x0)<<6)
#define CAN_ST_IDLE ((0x1)<<6)
#define CAN_ST_RCV ((0x2)<<6)
#define CAN_ST_TRAS ((0x3)<<6)

#define CAN_ST_STUERR ((0x1)<<3)
#define CAN_ST_FRMERR ((0x1)<<2)
#define CAN_ST_CRCERR ((0x1)<<1)
#define CAN_ST_BITERR ((0x1)<<0)

typedef struct
{
  uint16_t CAN_Prescaler;   /*!< Specifies the length of a time quantum. 
                                 It ranges from 1 to 1024. */
  
  uint8_t CAN_Mode;         /*!< Specifies the CAN operating mode.
                                 This parameter can be a value of 
                                @ref CAN_operating_mode */						 
  
  uint8_t CAN_SJW;          /*!< Specifies the maximum number of time quanta 
                                 the CAN hardware is allowed to lengthen or 
                                 shorten a bit to perform resynchronization.
                                 This parameter can be a value of 
                                 @ref CAN_synchronisation_jump_width */

  uint8_t CAN_BS1;          /*!< Specifies the number of time quanta in Bit 
                                 Segment 1. This parameter can be a value of 
                                 @ref CAN_time_quantum_in_bit_segment_1 */

  uint8_t CAN_BS2;          /*!< Specifies the number of time quanta in Bit 
                                 Segment 2.
                                 This parameter can be a value of 
                                 @ref CAN_time_quantum_in_bit_segment_2 */
  uint8_t CAN_SAMP;          /*!< Specifies the CAN SAMPLE TIME*/

} CAN_InitTypeDef;


typedef struct
{
//NB
uint16_t CANFD_NBRP;
uint8_t CANFD_NSEG1;
uint8_t CANFD_NSEG2;
uint8_t CANFD_NSJM;
	
//data
uint8_t CANFD_DBRP;
uint8_t CANFD_DSJM;
uint8_t CANFD_DSEG1;
uint8_t CANFD_DSEG2;
uint8_t CANFD_TDCR;
uint8_t CANFD_TDCEN;
uint8_t CANFD_SSPP;
	
//FD CONTROL
uint8_t CANFD_REOM:1;
uint8_t CANFD_DAR:1;    
uint8_t CANFD_ISO:1;
uint8_t CANFD_EXTBT:1;
uint8_t CANFD_BRSEN:1;
uint8_t CANFD_EN:1;
uint8_t rev:2;
uint8_t rev1;
uint16_t rev2;
} CANFD_InitTypeDef;



typedef struct
{
  uint32_t StdId;  /*!< Specifies the standard identifier.
                        This parameter can be a value between 0 to 0x7FF. */

  uint32_t ExtId;  /*!< Specifies the extended identifier.
                        This parameter can be a value between 0 to 0x1FFFFFFF. */

  uint8_t IDE;     /*!< Specifies the type of identifier for the message that 
                        will be transmitted. This parameter can be a value 
                        of @ref CAN_identifier_type */

  uint8_t RTR;     /*!< Specifies the type of frame for the message that will 
                        be transmitted. This parameter can be a value of 
                        @ref CAN_remote_transmission_request */

  uint8_t DLC;     /*!< Specifies the length of the frame that will be 
                        transmitted. This parameter can be a value between 
                        0 to 8 */
  uint8_t FDF;
						
  uint8_t Data[8]; /*!< Contains the data to be transmitted. It ranges from 0 
                        to 0xFF. */
} CanTxMsg;

/** 
  * @brief  CAN Rx message structure definition  
  */

typedef struct
{
  uint32_t StdId;  /*!< Specifies the standard identifier.
                        This parameter can be a value between 0 to 0x7FF. */

  uint32_t ExtId;  /*!< Specifies the extended identifier.
                        This parameter can be a value between 0 to 0x1FFFFFFF. */

  uint8_t IDE;     /*!< Specifies the type of identifier for the message that 
                        will be received. This parameter can be a value of 
                        @ref CAN_identifier_type */

  uint8_t RTR;     /*!< Specifies the type of frame for the received message.
                        This parameter can be a value of 
                        @ref CAN_remote_transmission_request */

  uint8_t DLC;     /*!< Specifies the length of the frame that will be received.
                        This parameter can be a value between 0 to 8 */

  uint8_t Data[8]; /*!< Contains the data to be received. It ranges from 0 to 
                        0xFF. */

  uint8_t FMI;     /*!< Specifies the index of the filter the message stored in 
                        the mailbox passes through. This parameter can be a 
                        value between 0 to 0xFF */
} CanRxMsg;

typedef struct
{
	u8 DLC:4;
	u8 BRS:1;
    u8 FDF:1;
	u8 RTR:1;	
	u8 FF:1;
	u8 IDH;
	u8 rev:5;
	u8 IDL3:3;
	u8 Data[8];
} CAN_StdTypeDef;

typedef struct
{
	u8 DLC:4;
	u8 BRS:1;
	u8 FDF:1;
	u8 RTR:1;
	u8 FF:1;
	u8 IDH[3];
	u8 rev:3;
	u8 IDL5:5;
	u8  Data[8];
} CAN_ExtTypeDef;


void CAN_Init(CAN_TypeDef* CANx, CAN_InitTypeDef* CAN_InitStruct);
void CAN_StructInit(CAN_InitTypeDef* CAN_InitStruct);
void CAN_Transmit(CAN_TypeDef* CANx, CanTxMsg* TxMessage);
void CAN_TransmitCmd(CAN_TypeDef* CANx, FunctionalState Start, FunctionalState Abort);

void CAN_Receive(CAN_TypeDef* CANx,  CanRxMsg* RxMessage);

void CANFD_Init(CAN_TypeDef* CANx, CANFD_InitTypeDef* CANFD_InitStruct);

void CAN_ITConfig(CAN_TypeDef* CANx, uint8_t CAN_IT, FunctionalState NewState);
FlagStatus CAN_GetFlagStatus(CAN_TypeDef* CANx, uint8_t CAN_FLAG);
void CAN_ClearFlag(CAN_TypeDef* CANx, uint8_t CAN_FLAG);

#endif //VERIFY_CAN_CASE_H

