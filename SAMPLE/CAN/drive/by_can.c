//********************************************************************
//(Copyright 2012) can Inc. All Rights Reserved
//Company confidential and Properietary information.
//This information may not be disclosed to unauthorized individual.
//********************************************************************
//
// P Microelectronics Co.,Ltd.
//
// File Name: can.c
// Author:    
// Version:   
// Created:   
// Modified:
// Description:
//********************************************************************
#include "by_can.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <usart.h>

/**
  * @brief  Deinitializes the CAN peripheral registers to their default reset values.
  * @param  CANx: where x can be 1 or 2 to select the CAN peripheral.
  * @retval None.
  */
void CAN_DeInit(CAN_TypeDef* CANx)
{

}

void CAN_Init(CAN_TypeDef* CANx, CAN_InitTypeDef* CAN_InitStruct)
{
	CAN_InitTypeDef* Param = CAN_InitStruct;

	CANx->MR |= CAN_RST_MODE; //reset mode
    CANx->BTR0 = Param->CAN_Prescaler | (Param->CAN_SJW << 6);
	CANx->BTR1 = Param->CAN_BS1 | (Param->CAN_BS2 << 4) | (Param->CAN_SAMP << 7);

	CANx->MR |= Param->CAN_Mode << 1; //can mode
	CANx->MR &= CAN_RLEASE_MODE;
} ;

/**
  * @brief  Initializes the CAN peripheral according to the specified
  *         parameters in the CAN_FilterInitStruct.
  * @param  CAN_FilterInitStruct: pointer to a CAN_FilterInitTypeDef
  *                               structure that contains the configuration 
  *                               information.
  * @retval None.
  */
#if 0
void CAN_FilterInit(CAN_FilterInitTypeDef* CAN_FilterInitStruct)
{
  
}
#endif

/**
  * @brief  Fills each CAN_InitStruct member with its default value.
  * @param  CAN_InitStruct: pointer to a CAN_InitTypeDef structure which
  *                         will be initialized.
  * @retval None.
  */
void CAN_StructInit(CAN_InitTypeDef* CAN_InitStruct)
{
  
}

/**
  * @brief  Initiates the transmission of a message.
  * @param  CANx:      where x can be 1 or 2 to to select the CAN peripheral.
  * @param  TxMessage: pointer to a structure which contains CAN Id, CAN
  *                    DLC and CAN data.
  * @retval The number of the mailbox that is used for transmission
  *                    or CAN_TxStatus_NoMailBox if there is no empty mailbox.
*/
void CAN_Transmit(CAN_TypeDef* CANx, CanTxMsg* TxMessage)
{
	CanTxMsg* Param = TxMessage;
	CAN_StdTypeDef Std = {0};
	CAN_ExtTypeDef Ext = {0};
	u32* temp;
    u8 i;
	
	if(Param->IDE == 0) //std frame
	{
		Std.FF = Param->IDE;
		Std.RTR = Param->RTR;
		Std.DLC = Param->DLC;
		Std.IDL3 = Param->StdId & 0x07;
		Std.IDH = (Param->StdId >> 3) & 0xff;
		//FD frame
		if(Param->FDF == 1)
		{
			Std.FDF = Param->FDF;
			Std.BRS = (CANx->FDCFG >> 1) & 0x01;
		}

		
		for(i = 0; i < Std.DLC;i++)
		{
			Std.Data[i] = Param->Data[i];
		}
		//printf("data:0x%x\r\n",Std.Data[0]);
		temp = (u32*)(&Std);
		
		//printf("temp:0x%x\r\n",*temp);
		//printf("STD:0x%x\r\n",*(u8*)(&Std));
		if(Std.DLC <= 1)
		{
			CANx->TXBUF = *temp;
		}
		else if(1< Std.DLC &&  Std.DLC<= 5)
		{
			CANx->TXBUF = *temp++;
			CANx->TXBUF = *temp++;
		}
		else
		{
			//printf("temp:0x%x\r\n",*temp);
			CANx->TXBUF = *temp++;
			//printf("temp:0x%x\r\n",*temp);
			CANx->TXBUF = *temp++;
			//printf("temp:0x%x\r\n",*temp);
			CANx->TXBUF = *temp++;
		}
	}
	else              
	{
		Ext.FF = Param->IDE;
		Ext.RTR = Param->RTR;
		Ext.DLC = Param->DLC;
			
		//printf("Param->ExtId:0x%x\r\n", Param->ExtId);
		Ext.IDL5 = Param->ExtId & 0x1F;
		for (i = 0; i < 3; i++)
		{
			Ext.IDH[i] = (Param->ExtId >> (21-8*i)) & 0xFF;
		}

		//FD frame
		if(Param->FDF == 1)
			
		{
			Std.FDF = Param->FDF;
			Std.BRS = (CANx->FDCFG >> 1) & 0x01;
		}
		
		for(i = 0; i < Ext.DLC;i++)
		{
			Ext.Data[i] = Param->Data[i];
		}
		temp = (u32*)(&Ext);
		if(Ext.DLC <= 3)
		{
			CANx->TXBUF = *temp++;
			CANx->TXBUF = *temp++;
		}
		else if(4 <= Ext.DLC &&  Ext.DLC <= 7)
		{
            CANx->TXBUF = *temp++;
			CANx->TXBUF = *temp++;
			CANx->TXBUF = *temp++;
		}
		else
		{
			CANx->TXBUF = *temp++;
			CANx->TXBUF = *temp++;
			CANx->TXBUF = *temp++;
			CANx->TXBUF = *temp++;
		}
	}
}

/**
  * @brief  Checks the transmission of a message.
  * @param  CANx:            where x can be 1 or 2 to to select the 
  *                          CAN peripheral.
  * @param  TransmitMailbox: the number of the mailbox that is used for 
  *                          transmission.
  * @retval CAN_TxStatus_Ok if the CAN driver transmits the message, CAN_TxStatus_Failed 
  *         in an other case.
  */
void CAN_TransmitCmd(CAN_TypeDef* CANx, FunctionalState Start, FunctionalState Abort)
{
    if(Start == ENABLE)
		CANx->CMR |= 1<<2;
	else
		CANx->CMR &= ~(1<<2);
	
	if(Abort == ENABLE)
		CANx->CMR |= 1<<1;
	else
		CANx->CMR &= ~(1<<1);
}


/**
  * @brief  Receives a message.
  * @param  CANx:       where x can be 1 or 2 to to select the CAN peripheral.
  * @param  FIFONumber: Receive FIFO number, CAN_FIFO0 or CAN_FIFO1.
  * @param  RxMessage:  pointer to a structure receive message which contains 
  *                     CAN Id, CAN DLC, CAN datas and FMI number.
  * @retval None.
  */
void CAN_Receive(CAN_TypeDef* CANx,  CanRxMsg* RxMessage)
{
	u32 temp = 0;
	u8 i;
	
	temp = CANx->RXBUF; 

	RxMessage->IDE = (temp & 0x80) >> 7;
	RxMessage->RTR = (temp & 0x40) >> 6;
	
	RxMessage->DLC = (temp & 0x0F);
	
	if(RxMessage->IDE == 0) //std frame
	{

		RxMessage->StdId = (((temp >> 21) & 0x07) | ((temp >> 8) & 0xFF) << 3);
		
		if(RxMessage->DLC >= 1)
		{
			RxMessage->Data[0] = temp >> 24;
		}
		
		for( i= 1;i < RxMessage->DLC;i++)
		{
			if((i==1) | (i==5))
			{
				temp = CANx->RXBUF;
				//printf("temp:0x%x\r\n",temp);
			}
						
			RxMessage->Data[i] = (temp >> (((i-1)%4) * 8)) & 0xff;		
		}	
	
	}
	else
    {
		for(i = 1; i <= 3; i++)
		{
			RxMessage->ExtId |= (temp >> (i*8)) & 0xFF;
			if(i != 3)
				RxMessage->ExtId <<= 8;
			else
				RxMessage->ExtId <<= 5;
		}
		//read word 2
		temp = CANx->RXBUF;
		RxMessage->ExtId |= (temp >> 3) &0x1F;

		for(i = 0; i < (RxMessage->DLC > 3 ? 3 : RxMessage->DLC); i++)
		{
			RxMessage->Data[i] = (temp >> ((i+1)*8)) & 0xFF;
		}

		//read data
		for( i= 3;i < RxMessage->DLC;i++)
		{
			if((i==3) | (i==7))
			{
				temp = CANx->RXBUF;
				printf("i:%d_temp:0x%x\r\n",i,temp);
			}
						
			RxMessage->Data[i] = (temp >> (((i-3)%4)*8)) & 0xFF;	
		}	


	}
}

/**
  * @brief  Returns the CANx's last error code (LEC).
  * @param  CANx:          where x can be 1 or 2 to to select the CAN peripheral.  
  * @retval CAN_ErrorCode: specifies the Error code : 
  *                        - CAN_ERRORCODE_NoErr            No Error  
  *                        - CAN_ERRORCODE_StuffErr         Stuff Error
  *                        - CAN_ERRORCODE_FormErr          Form Error
  *                        - CAN_ERRORCODE_ACKErr           Acknowledgment Error
  *                        - CAN_ERRORCODE_BitRecessiveErr  Bit Recessive Error
  *                        - CAN_ERRORCODE_BitDominantErr   Bit Dominant Error
  *                        - CAN_ERRORCODE_CRCErr           CRC Error
  *                        - CAN_ERRORCODE_SoftwareSetErr   Software Set Error  
  */
 
uint8_t CAN_GetLastErrorCode(CAN_TypeDef* CANx)
{
	return 0;
}

/**
  * @brief  Returns the CANx Receive Error Counter (REC).
  * @note   In case of an error during reception, this counter is incremented 
  *         by 1 or by 8 depending on the error condition as defined by the CAN 
  *         standard. After every successful reception, the counter is 
  *         decremented by 1 or reset to 120 if its value was higher than 128. 
  *         When the counter value exceeds 127, the CAN controller enters the 
  *         error passive state.  
  * @param  CANx: where x can be 1 or 2 to to select the CAN peripheral.  
  * @retval CAN Receive Error Counter. 
  */
uint8_t CAN_GetReceiveErrorCounter(CAN_TypeDef* CANx)
{
	return 0;
}


/**
  * @brief  Returns the LSB of the 9-bit CANx Transmit Error Counter(TEC).
  * @param  CANx:   where x can be 1 or 2 to to select the CAN peripheral.  
  * @retval LSB of the 9-bit CAN Transmit Error Counter. 
  */
uint8_t CAN_GetLSBTransmitErrorCounter(CAN_TypeDef* CANx)
{
	return 0;
}


/**
  * @brief  Enables or disables the specified CANx interrupts.
  * @param  CANx:   where x can be 1 or 2 to to select the CAN peripheral.
  * @param  CAN_IT: specifies the CAN interrupt sources to be enabled or disabled.
  CAN_DOI (0x1)<<24 // RECV FIFO OVERRUN
  CAN_BEI (0x2)<<24 // BUS ERROR INTERRUPT
  CAN_TI (0x4)<<24 // SUCCESSFUL TRANSMISSION
  CAN_RI (0x8)<<24 // AT LEAST ONE MESSAGE IN RECEIVE FIFO
  CAN_EPI (0x10)<<24 // ERROR PASSIVE LEVEL
  CAN_EWI (0x20)<<24 // CHANGE WHEN ES/BS HIGH IN SR
  CAN_ALI (0x40)<<24 // LOST ARBITRATION BECAME A RECEIVER

  * @param  NewState: new state of the CAN interrupts.
  *                   This parameter can be: ENABLE or DISABLE.
  * @retval None.
  */
void CAN_ITConfig(CAN_TypeDef* CANx, uint8_t CAN_IT, FunctionalState NewState)
{
	if(NewState == DISABLE)
		CANx->IMR &= ~CAN_IT;
	else
		CANx->IMR |= CAN_IT;
}
/**
  * @brief  Checks whether the specified CAN flag is set or not.
  * @param  CANx:     where x can be 1 or 2 to to select the CAN peripheral.
  * @param  CAN_FLAG: specifies the flag to check.
      
  * @retval The new state of CAN_FLAG (SET or RESET).
  */
FlagStatus CAN_GetFlagStatus(CAN_TypeDef* CANx, uint8_t CAN_FLAG)
{
    if(CANx->ISR & CAN_FLAG)
		return SET;
	else
		return RESET;
}

/**
  * @brief  Clears the CAN's pending flags.
  * @param  CANx:     where x can be 1 or 2 to to select the CAN peripheral.
  * @param  CAN_FLAG: write 1 to clear the flag.
  *                   This parameter can be one of the following flags: 
      
  * @retval None.
  */
void CAN_ClearFlag(CAN_TypeDef* CANx, uint8_t CAN_FLAG)
{
	CANx->ISR |=  CAN_FLAG;
}

/**
  * @brief  Checks whether the specified CANx interrupt has occurred or not.
  * @param  CANx:    where x can be 1 or 2 to to select the CAN peripheral.
  * @param  CAN_IT:  specifies the CAN interrupt source to check.
  *                  This parameter can be one of the following flags: 
  *                 -  CAN_IT_TME               
  *                 -  CAN_IT_FMP0              
  *                 -  CAN_IT_FF0               
  *                 -  CAN_IT_FOV0              
  *                 -  CAN_IT_FMP1              
  *                 -  CAN_IT_FF1               
  *                 -  CAN_IT_FOV1              
  *                 -  CAN_IT_WKU  
  *                 -  CAN_IT_SLK  
  *                 -  CAN_IT_EWG    
  *                 -  CAN_IT_EPV    
  *                 -  CAN_IT_BOF    
  *                 -  CAN_IT_LEC    
  *                 -  CAN_IT_ERR 
  * @retval The current state of CAN_IT (SET or RESET).
  */
ITStatus CAN_GetITStatus(CAN_TypeDef* CANx, uint32_t CAN_IT)
{
    return RESET;
}

/**
  * @brief  Clears the CANx's interrupt pending bits.
  * @param  CANx:    where x can be 1 or 2 to to select the CAN peripheral.
  * @param  CAN_IT: specifies the interrupt pending bit to clear.
  *                  -  CAN_IT_TME                     
  *                  -  CAN_IT_FF0               
  *                  -  CAN_IT_FOV0                     
  *                  -  CAN_IT_FF1               
  *                  -  CAN_IT_FOV1              
  *                  -  CAN_IT_WKU  
  *                  -  CAN_IT_SLK  
  *                  -  CAN_IT_EWG    
  *                  -  CAN_IT_EPV    
  *                  -  CAN_IT_BOF    
  *                  -  CAN_IT_LEC    
  *                  -  CAN_IT_ERR 
  * @retval None.
  */
void CAN_ClearITPendingBit(CAN_TypeDef* CANx, uint32_t CAN_IT)
{

}

void CANFD_Init(CAN_TypeDef* CANx, CANFD_InitTypeDef* CANFD_InitStruct)
{
	CANFD_InitTypeDef* Param = CANFD_InitStruct;

	CANx->MR |= CAN_RST_MODE; //reset mode

	printf("MR:0x%x\r\n",read_ahb32(CAN_MR));
	//NB
 	write_ahb32(CAN_NBT,CAN_NBRP_(Param->CANFD_NBRP) | CAN_NSEG1_(Param->CANFD_NSEG1) | CAN_NSEG2_(Param->CANFD_NSEG2) | CAN_NSJM_(Param->CANFD_NSJM));
	//write_ahb32(CAN_NBT,0x181008);

	//DB
	write_ahb32(CAN_DBT,CAN_DBRP_(Param->CANFD_DBRP) | CAN_DSEG1_(Param->CANFD_DSEG1) | CAN_DSEG2_(Param->CANFD_DSEG2) | CAN_DSJM_(Param->CANFD_DSJM) 
				| CAN_TDCR_(Param->CANFD_TDCR) | CAN_TDCEN_(Param->CANFD_TDCEN) | CAN_SSPP_(Param->CANFD_SSPP));

	//write_ahb32(CAN_DBT,0x8428);

#if 0
	CANx->DBTL = 0x28;
	CANx->DBTH = 0x84;
	CANx->TDCR = 0;
#endif


	printf("NBT:0x%x\r\n",read_ahb32(CAN_NBT));
	printf("DBT:0x%x\r\n",read_ahb32(CAN_DBT));

	//CFG
	//CANx->FDCFG = Param->CANFD_REOM | Param->CANFD_DAR | Param->CANFD_ISO | Param->CANFD_EXTBT | Param->CANFD_BRSEN | Param->CANFD_EN;
	CANx->FDCFG = 0x01;

	printf("NBT:0x%x\r\n",read_ahb32(CAN_NBT));
	printf("DBT:0x%x\r\n",read_ahb32(CAN_DBT));
	printf("CTL:0x%x\r\n",CANx->FDCFG);

	CANx->MR &= CAN_RLEASE_MODE;
	
} ;




/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/


