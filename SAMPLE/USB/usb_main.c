//********************************************************************
//(Copyright 2012) P Inc. All Rights Reserved
//Company Confidential and Proprietary information
//This information may not be disclosed to unauthorized individual
//********************************************************************
//
//P Microelectronics Co.,Ltd
//
//File name 	: usb_main.c
//Author		: 
//Verision		: V0.1
//Created		: 
//Modified		:
//Description	:
//				  
//********************************************************************
#include "stm32f10x.h"
#include "usb.h"
#include "snps_dmac.h"   // DMA Controller public header

volatile unsigned char usb_ep_stall[5];
volatile unsigned char usb_ep_in_stall[5];
volatile unsigned char usb_ep_out_stall[5];
volatile unsigned char usb_setup_flag;
volatile unsigned char usb_out_flag;
volatile unsigned char usb_in_flag;
volatile unsigned char usb_finish_flag[5];
volatile unsigned char usb_reset_flag;
volatile unsigned char usb_suspend_flag;
volatile unsigned char usb_resume_flag;
volatile unsigned char usb_state_suspend = 0;
volatile unsigned char usb_state_resume = 0;
volatile USB_STATE_t usb_state;	
//volatile unsigned char usb_ep1_finish;
volatile unsigned char usb_ep_flag;
//
volatile unsigned char usb_send_epn_stall[5];
extern unsigned long debug_flag;
 
unsigned int temp;
unsigned int temp_ep;
unsigned int temp_epn;

extern unsigned char buffer[31];

#ifdef USB_INTR_MODE
//SECTION_ISR 
#endif	

void usb_nvic_cfg(FunctionalState state)
{

	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = 2; //USB 通道号
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = state; 		//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure); //根据指定的参数初始化NVIC寄存器
}

int USB_IRQHandler(void) 
{
	temp=usb_read_mreg16(USB_INT_STATE);
	write_ahb32(USB_INT_STATE,(unsigned char)temp);
	
	//printf("USB_INT_STATE:0x%x\r\n",temp);
	//if(temp > 0x20)
		//while(1);
	if(((unsigned char)temp)&USB_INTEP)  //
	{
		temp_ep=usb_read_mreg16(EP_INT_STATE);
		//write_ahb32(EP_INT_STATE, temp_ep);//clear interrupt
		if(((unsigned char)temp_ep)&EP0_INT)
		{
			//got ep0 interrupt, ep0 has dual-direction
			//write_ahb32(EP_INT_STATE, EP0_INT);			
			usb_ep_flag=USB_EP0;
			temp_epn=usb_read_mreg16(EP0_INT_STATE);			
			write_ahb32(EP0_INT_STATE,temp_epn);//clear interrupt

			printf("EP0_INT_STATE:0x%x",temp_epn);
			if(((unsigned char)temp_epn)&EP_SETUP)
			{
				//printf("EP0_INT_STATE:0x%x",temp_epn);
				//usb_setup_flag = TRUE;
				usb_setup_handle();
			}
			if(((unsigned char)temp_epn)& (EP_IN_NACK | EP_IN_ACK))
			{
				//printf("in:0x%x\r\n",temp_epn);
				
				usb_in_handle();
				
				//disable in_nack interrupt
				write_ahb32(EP0_INT_EN,(EP_SETUP | EP_IN_NACK | EP_IN_ACK | EP_OUT_ACK | EP_STALL));
				//usb_in_flag = TRUE;
			}
			if(temp_epn&EP_OUT_ACK)
			{
				//printf("EP0_INT_STATE:0x%x",temp_epn);
				usb_out_flag = TRUE;
			}
			if(temp_epn&EP_IN_STALL)
			{			
				usb_ep_in_stall[0] = TRUE;			
			}
			if(temp_epn&EP_OUT_STALL)
			{			
				usb_ep_out_stall[0] = TRUE;	
			}
		}		
#if ((EPOUT^EPIN))	 //if epin != epout,
		if(temp_ep&EPn_INT(EPOUT))
		{//got ep2 interrupt
			usb_ep_flag=USB_EPn(EPOUT);
			//EP2 only use for bulk OUT
			temp_epn=usb_read_mreg16(EPn_INT_STATE(EPOUT));
			write_ahb32(EPn_INT_STATE(EPOUT), temp_epn);//clear interrupt
			
			printf("EP1_INT_STATE:0x%x",temp_epn);
			
			if(temp_epn&EP_OUT_ACK)
			{
				usb_out_handle();
				//usb_out_flag = TRUE;
			}
			if(temp_epn&EP_IN_NACK)
			{
				usb_in_flag = TRUE;
			}
			if(temp_epn&EP_IN_STALL)
			{
				usb_ep_in_stall[EPOUT] = TRUE;
			}
			if(temp_epn&EP_OUT_STALL)
			{
				usb_ep_out_stall[EPOUT] = TRUE;
			}
		}
#endif
		if(temp_ep&EPn_INT(EPIN))
		{//got ep1 interrupt, ep1 is input
			usb_ep_flag=USB_EPn(EPIN);
			temp_epn=usb_read_mreg16(EPn_INT_STATE(EPIN));
			write_ahb32(EPn_INT_STATE(EPIN),temp_epn);//clear interrupt
			//printf("EPIN:0x%x",temp_epn);
			
			if(temp_epn & (EP_IN_ACK | EP_IN_NACK))
			{

				printf("buffer[0]:0x%x\r\n",buffer[0]);
				printf("DMAC_CFG_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(0x07)));
				//printf("DMA2 NUM:%x\r\n",read_ahb32(USB_DMA_BYTE_NUML(2)));
				printf("ep1 avl:%x\r\n",read_ahb32(USB_EPn_AVL(1)));
				
				//usb_in_flag = TRUE;
				usb_in_handle();
				//enable in_nack interrupt
				usb_in_flag = FALSE;//2012.04.24 by P//2012.05.03 by P
		   		write_ahb32(EP0_INT_EN,EP_SETUP|EP_IN_NACK|EP_OUT_ACK|EP_STALL);//2012.04.24 reused by P//2012.05.03 by P
				write_ahb32(EPn_INT_EN(EPIN),(EP_IN_ACK | EP_IN_NACK|EP_STALL));//2012.04.24 by P
				//printf("temp_ep_en:0x%x\r\n",usb_read_mreg16(EPn_INT_EN(EPIN)));
			}
			if(temp_epn&EP_OUT_ACK)
			{
				usb_out_flag = TRUE;			
			}
			if(temp_epn&EP_IN_STALL)
			{
				usb_ep_in_stall[EPIN] = TRUE;
			}
			if(temp_epn&EP_OUT_STALL)
			{
				usb_ep_out_stall[EPIN] = TRUE;
			}
		}
	}
	else if(temp & USB_INTHOSTRESET)
	{//got host reset interrupt
		usb_reset_flag = TRUE;
	}////else if(temp & USB_INTSUSPEND)////2012.05.04  by P for suspend and resume test
	if(temp & USB_INTSUSPEND)
	{
		usb_suspend_flag = TRUE;
	}////else if(temp & USB_INTRESUME)////2012.05.04  by P for suspend and resume test
	if(temp & USB_INTRESUME)
	{
		usb_resume_flag = TRUE;
	}
	return 0;
}

#ifdef USB_TEST_EN
void usb_test(void)
{
	u16 i = 0;
	u8 channel = 0x07;
	
	usb_init();
		
#ifdef USB_DMA_MODE
	//配置通道10为DMA模式，USB_DMA0:CH10；USB_DMA1:CH11
	temp = read_ahb32(0x4001b068);
	temp |= 1;//打开DMA模式共享
	//printf("temp:0x%x\r\n",temp);
	write_ahb32(0x4001b068, temp);
	printf("0x%x\r\n",read_ahb32(0x4001b068));
	
    //DMA初始化
	//clear all the status register of select channel
	dw_dmac_clearIrq(channel, IRQ_TYPE_ALL);
	dmac_per2mem_channel_cfg(channel); //DMA配置
	//dmac_mem2per_channel_cfg(channel); //DMA配置
	//enable the channel to start transfer
	dw_dmac_enableChannel(channel);
	
	printf("DAMC_ENABLE\r\n");

	printf("DMAC_CTL_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_L(channel)));
	printf("DMAC_CTL_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_H(channel)));
	
	printf("DMAC_CFG_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(channel)));
	printf("DMAC_CFG_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_H(channel)));
#endif

	usb_nvic_cfg(ENABLE);

	//打印USB初始化参数
#if 0
	for(i = 0; i < 99*4; i+=4)
	{
		printf("0x%x:0x%x\r\n",i,read_ahb32(USB_BASE+i));
	}
#endif

  	while(1)
    {
	//#ifndef USB_INTR_MODE
	//USB_IRQHandler();

	//CRC校验
	//write_ahb32(0x40013400, 0x06070607);
	//write_ahb32(0x40013400, usb_out_flag);
	//#endif
	//printf("pd:0x%x\r\n",read_ahb32(0xE000E200));
		if(usb_setup_flag==TRUE)       // Setup packet received    0x0002
        {
			usb_setup_flag = FALSE;
			usb_setup_handle();
        }
		else if(usb_out_flag==TRUE)         // OUT packet received      0x0008
        {
			//write_ahb32(0x40013400, 0x06060606);
			usb_out_flag = FALSE;
			usb_out_handle();
        }
		else if(usb_in_flag==TRUE)          // IN request received      0x0004
        {
			usb_in_handle();
			//enable in_nack interrupt
			usb_in_flag = FALSE;//2012.04.24 by P//2012.05.03 by P
			
		    //write_ahb32(EP0_INT_EN,EP_SETUP|EP_IN_NACK|EP_OUT_ACK|EP_STALL);//2012.04.24 reused by P//2012.05.03 by P
		    write_ahb32(EPn_INT_STATE(EPIN),EP_IN_NACK);
			write_ahb32(EPn_INT_EN(EPIN),(EP_IN_NACK|EP_OUT_ACK|EP_STALL));//2012.04.24 by P
        }
		else if(usb_reset_flag==TRUE)       // Usb host reset
        {		
			usb_reset_flag = FALSE;
			usb_reset_handle();
        }
		else if(usb_suspend_flag==TRUE)       // Usb suspend
        {
			usb_suspend_flag = FALSE;
			usb_suspend_handle();
        }
		else if(usb_resume_flag==TRUE)       // Usb resume
        {
			usb_resume_flag = FALSE;
			usb_resume_handle();
        }

		// 调试打印
		//printf("buffer[%d]:0x%x\r\n",i,buffer[i]);
		//printf("DMAC_CFG_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(channel)));
		//printf("DMA2 NUM:%x\r\n",read_ahb32(USB_DMA_BYTE_NUML(2)));
		//printf("ep1 avl:%x\r\n",read_ahb32(USB_EPn_AVL(1)));
		//printf("ep2 ctrl:%x\r\n",read_ahb32(USB_EPn_CTRL(2)));
    }
}

void delayms(unsigned long dly)
{
	unsigned long cnt,i;
	for(cnt=0;cnt<dly;cnt++)
	{
  	for(i=0;i<10;i++)
	  {}	
	}
}

void USB_MC_IRQHandler(void)
{
	//printf("a");
	USB_IRQHandler();
}


#endif //USB_TEST_EN



