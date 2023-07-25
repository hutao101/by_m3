//********************************************************************
//(Copyright 2012) P Inc. All Rights Reserved
//Company Confidential and Proprietary information
//This information may not be disclosed to unauthorized individual
//********************************************************************
//
//////P Microelectronics Co.,Ltd
//
//File name 	: ctrlusb.c
//Author		:
//Verision		: 
//Created		: 
//Modified		: 
//Description	:
//				  
//********************************************************************
//#include "platform.h"
#include "usb.h"
//#include "common.h"
//#include "uart.h"

#ifdef USB_TEST_EN
volatile unsigned char   usb_running_state;  // running stage
volatile unsigned char   usb_running_ctrl_state;  // running stage
volatile unsigned char   bmRequestType;
volatile unsigned char   bRequest;
volatile unsigned short int  wValue;
volatile unsigned short int  wIndex;
volatile unsigned char  wLength;
volatile unsigned char   req_dir;//bmRequestType[7];
volatile unsigned char   req_type;//bmRequestType[6:5];
volatile unsigned char   req_recip;//bmRequestType[4:0];
volatile unsigned char   stall_clear_flag[5];  //2012.04.19 BY P 
#define SYS_WAKE_UP
#define USB_SUSP_SYS_TEST
//#define AUTO_SUSPEND_TEST
//=========================== Each Request =============================

//=================================== Handler================================
#if 1
void (*StandardDeviceRequest[MAX_STD_REQUEST])(void) =
{
	Chap9_GetStatus,	//0x00
	Chap9_ClearFeature,	//0x01
	MLsup_StallEP0,		//reserve
	Chap9_SetFeature,	//0x03
	MLsup_StallEP1,		//reserve
	Chap9_SetAddress,	//0x05
	Chap9_GetDescriptor,//0x06
	Chap9_SetDescriptor,//0x07
	Chap9_GetConfiguration,	//0x08
	Chap9_SetConfiguration,	//0x09
	Chap9_GetInterface,		//0x0a
	Chap9_SetInterface,		//0x0b
};
#endif

void usb_setup_handle( void)
{
    unsigned char    temp ;
	if(usb_running_ctrl_state==USB_IDLE)
	{
	 	//bmrequest type one byte
	    bmRequestType = usb_read_mreg16(USB_EP_SETUPn(0));
		//brequest one byte
		bRequest = usb_read_mreg16(USB_EP_SETUPn(1));
		//wvalue two bytes
	    temp = usb_read_mreg16(USB_EP_SETUPn(2));
		wValue = usb_read_mreg16(USB_EP_SETUPn(3));
		//printf("wValue1:0x%x",temp);
		//printf("wValue2:0x%x",wValue);
		wValue = (wValue<<8) + temp;
		//windex two bytes
	    temp = usb_read_mreg16(USB_EP_SETUPn(4));
		wIndex = usb_read_mreg16(USB_EP_SETUPn(5));
		wIndex = (wIndex<<8) + temp;
		//wlength two bytes
	    temp = usb_read_mreg16(USB_EP_SETUPn(6));
		wLength = usb_read_mreg16(USB_EP_SETUPn(7));
		wLength = (wLength<<8) + temp;
		//direction
		req_dir = bmRequestType & USB_REQ_DIR_MASK ;
		//recipient
		req_recip = bmRequestType & USB_REQ_RECIP_MASK ;
		//type
	    req_type = bmRequestType & USB_REQ_TYPE_MASK ;

	    usb_running_ctrl_state = USB_CTRL_SETUP ;

#if 0
	printf("bmRequestType:0x%x \r\n",bmRequestType);
	printf("wValue:0x%x \r\n",wValue);
	printf("bRequest:0x%x \r\n",bRequest);
	printf("wIndex:0x%x \r\n",wIndex);
	printf("wLength:0x%x \r\n\r\n",wLength);
#endif
	
	}
	ctrl_deal_handle();

}

void ctrl_deal_handle(void)
{

	//printf("c:%x\r\n",bRequest);
	//printf("d:%x\r\n",req_type);
	if( req_type == USB_REQ_TYPE_STANDARD )     // standard request type
	{
		if( bRequest < MAX_STD_REQUEST )    // handle 0--12
		{
			(*StandardDeviceRequest[bRequest])( ) ;
		}
	}
	else if( req_type == USB_REQ_TYPE_CLASS )  // class request type
	{
		switch(bRequest)
		{
			case GET_MAX_LUN:
			{
				Class_Send_Max_Lun( ) ;
				break;
			}
			default:
			{
				if(req_dir==USB_REQ_DIR_IN)
				{
					EP0_Send_Empty_Packet();
				}
				break;
			}
		}		
	}
	else //vendor
	{
		if(req_dir==USB_REQ_DIR_IN)
		{
			EP0_Send_Empty_Packet();
		}
	}
}

void ctrl_in_token_handle( void)
{
	if(req_dir==USB_REQ_DIR_IN)// data stage when control read
	{
		ctrl_deal_handle();
	}else //status stage when control write (like setAddress) or no-data control
	{
		EP0_Send_Empty_Packet();
	}
}

void ctrl_out_handle( void) 
{
	if(req_dir==USB_REQ_DIR_OUT)//data stage when control write
	{
		ctrl_deal_handle();
	}else // status stage when control read, IP will send ack auto
	{	
		//write_ahb32(USB_EP0_CTRL,0x00);// clear FIFO
		#ifdef EPn_OUT_S_PROTOCOL_STALL_TEST_ERROR   //2012.05.07 BY P , to test the functional stall when set the halt feature of EPn in control transfer
		if(stall_clear_flag[EPIN]==0)
		{
			write_ahb32(USB_EP_HALT,(1<<(EPIN)));
		}
		#endif
		
		#ifdef EPn_OUT_S_PROTOCOL_STALL_TEST   //2012.05.07 BY P , to test real out protocol stall in stage state, can test the stall bit auto cleared too
		if(stall_clear_flag[0]==0)
		{
			write_ahb32(USB_EP_HALT,(1<<0));       	
	       	stall_clear_flag[0]=1;//2012.05.08 for the code can run normally after the stall, by P		
		}
		#endif
	}
}

void usb_dma_init(void)
{
	write_ahb32(USB_EP_DMA_DIR,EPn_DIR_IN(EPIN)|EPn_DIR_OUT(EPOUT));//ep2 is D2H(IN), ep1 is H2D(out)
    write_ahb32(USB_EP_DMA_SIZE,0x00);	//EP1 EP2 DMA size
	write_ahb32(USB_AHB_DMA,USB_DMA_BURST_1BYTE(0)|USB_DMA_BURST_1BYTE(1)|USB_DMA_TIMEOUT);
	
	write_ahb32(USB_DMA_BYTE_NUML(1),0x04);
	write_ahb32(USB_DMA_BYTE_NUML(2),0x04);

}




void usb_init( void)
{
//  unsigned char	temp ;
	unsigned char ix;
	volatile unsigned int val;

	usb_setup_flag = 0;
	usb_out_flag = 0;
	usb_in_flag = 0;
	usb_reset_flag = 0;
	usb_suspend_flag = 0;
	usb_resume_flag = 0;
	//usb_ep1_finish=0;
	usb_ep_flag=USB_EP0;
	write_finish = 1;
	dma_finished_flag[0] = 0;
	dma_finished_flag[1] = 0;
	usb_state = USB_STATE_POWERED;
	usb_running_state = USB_IDLE;
	usb_running_ctrl_state = USB_IDLE;
	/*************Enable clk, reset, suspend******************/
	//zsp_bypass_pll(FALSE);
	//zspclk_setdiv(2);// 24M
	//val=usb_read_mreg16(DIS_CLK_SYS);
	//write_ahb32(DIS_CLK_SYS, val&0xFEFF);
	//val=usb_read_mreg16(USB_SOFT_RST);
	//////write_ahb32(USB_AHB_RST,0xf);
	//////write_ahb32(USB_AHB_RST,0x0);
	//write_ahb32(USB_AHB_POWER,0x0000000d);//register will be all 0
	//write_ahb32(USB_AHB_POWER,0x0000000f);
	val = read_ahb32(USB_TOP);
	val = val | 0x3;
	/////write_ahb32(USB_TOP,val);
#ifdef USB_LS_TEST
	write_ahb32(USB_TOP,0x00000003);
#else
	write_ahb32(USB_TOP,0x00000002);
#endif

    write_ahb32(USB_INT_EN,0x00000037);    // enable interrupt and clear interrupt	
    //enable all EndPoint
    write_ahb32(EP_INT_EN,0x000000FF);//1F
    write_ahb32(EP0_INT_EN,0x0000000b);
	write_ahb32(EPn_INT_EN(1),0x0000000c);	
//while(1);




	//write_ahb32(USB_TOP,0x00000001);
	//write_ahb32(USB_TOP,0x00000083);
	//write_ahb32(USB_SOFT_RST, val&0xFEFF );
	//write_ahb32(USB_SOFT_RST, val|0x100 );
	//write_ahb32(USB_SUSP_SYS,  0x0);
	/*************Enable clk, reset, suspend******************/
	for(ix = 0; ix < 5; ix++)
	{
		usb_ep_stall[ix] = FALSE;
		usb_ep_in_stall[ix] = FALSE;
		usb_ep_out_stall[ix] = FALSE;		
		usb_finish_flag[ix] = FALSE;
		usb_send_epn_stall[ix] = FALSE;
		stall_clear_flag[ix]=0;  //2012.04.19 BY P 
		write_ahb32(USB_EPn_CTRL(ix),0x00);//clear EP fifo data
	}
#ifdef USB_LS_TEST
	write_ahb32(USB_TOP, USB_USBRST | USB_CONNECT | USB_LS);
#else
    write_ahb32(USB_TOP, USB_USBRST | USB_CONNECT); // reset usb block + usb disconnect
#endif
	printf("TOP:0x%x",read_ahb32(USB_TOP));
	delay_ms(100);
	write_ahb32(USB_TOP, USB_DISCONNECT);
	
	//clear interrupt
    write_ahb32(USB_INT_STATE,usb_read_mreg16(USB_INT_STATE));
    write_ahb32(EP_INT_STATE,usb_read_mreg16(EP_INT_STATE));
    write_ahb32(EP0_INT_STATE,usb_read_mreg16(EP0_INT_STATE));
    write_ahb32(EPn_INT_STATE(1),usb_read_mreg16(EPn_INT_STATE(1)));
    write_ahb32(EPn_INT_STATE(2),usb_read_mreg16(EPn_INT_STATE(2)));
    write_ahb32(EPn_INT_STATE(3),usb_read_mreg16(EPn_INT_STATE(3)));
    write_ahb32(EPn_INT_STATE(4),usb_read_mreg16(EPn_INT_STATE(4)));
	//
	write_ahb32(USB_EP0_CTRL,0x00000000);
	write_ahb32(USB_EPn_CTRL(1),0x00000000);
	write_ahb32(USB_EPn_CTRL(2),0x00000000);
	write_ahb32(USB_EPn_CTRL(3),0x00000000);
	write_ahb32(USB_EPn_CTRL(4),0x00000000);
	//
    write_ahb32(USB_INT_EN,USB_INTHOSTRESET |USB_INTSUSPEND|USB_INTRESUME|USB_INTEP| 0x60);    //USB_INT_DMA|USB_INT_DMA_CH0 enable interrupt and clear interrupt	
    //enable all EndPoint
    write_ahb32(EP_INT_EN,0xFF);//1F
    write_ahb32(EP0_INT_EN,EP_SETUP|EP_FINISH|EP_IN_NACK|EP_OUT_ACK|EP_STALL|EP_IN_ACK);
    //configure EPIN EPOUT interrupt
#if ((EPOUT^EPIN))	 //if epin != epout,    
    write_ahb32(EPn_INT_EN(EPIN),EP_FINISH|EP_IN_NACK|EP_STALL);
    write_ahb32(EPn_INT_EN(EPOUT),EP_FINISH|EP_OUT_ACK|EP_STALL);
#else
    write_ahb32(EPn_INT_EN(EPIN),EP_FINISH|EP_IN_NACK|EP_OUT_ACK|EP_STALL);
    write_ahb32(EPn_INT_EN(EPOUT),EP_FINISH|EP_IN_NACK|EP_OUT_ACK|EP_STALL);
#endif
	//enable EP1, EP2, EP3, EP4
	write_ahb32(EPn_INT_EN(EPIN),0xc);
    write_ahb32(USB_EP_EN,0xF);
		
    write_ahb32(USB_ADDR, 0x0000);// initial address is 0	
    
    //
    write_ahb32(USB_PACKAGE_SIZE0,EPn_MAX_PACKAGE_SIZE);//for bullk
    write_ahb32(USB_PACKAGE_SIZE1,EPn_MAX_PACKAGE_SIZE);
	
	
#ifdef USB_DMA_MODE
  	write_ahb32(USB_EP_DMA_DIR,0x2);//ep1 is H2D(out), ep2 is D2H(IN)
	write_ahb32(USB_EP_DMA,0x00);//disable dma mode
	usb_dma_init();
	write_ahb32(USB_EP_DMA,0x06);//enable dma mode
#endif

	//write_ahb32(USB_EP_HALT,0x00);
	
	write_ahb32(USB_EPn_CTRL(1),0x00000080);
	write_ahb32(USB_EPn_CTRL(2),0x00000080);
	printf("EP1 Ctr:0x%x\r\n",read_ahb32(USB_BASE+0x144));
	printf("EP2 Ctr:0x%x\r\n",read_ahb32(USB_BASE+0x148));
	
#ifdef USB_LS_TEST
    write_ahb32(USB_TOP,USB_CONNECT | USB_LS);
#else
	write_ahb32(USB_TOP,USB_CONNECT);
	//write_ahb32(USB_TOP,USB_LS);
	//write_ahb32(USB_AHB_POWER,0x0c);//03 //enter work state
#endif

	write_ahb32(USB_EPn_CTRL(1),0x00000080);
	write_ahb32(USB_EPn_CTRL(2),0x00000080);
	printf("EP1 Ctr:0x%x\r\n",read_ahb32(USB_BASE+0x144));
	printf("EP2 Ctr:0x%x\r\n",read_ahb32(USB_BASE+0x148));
}
//2012.04.13 modified by P
void reset_process( void)
{
//    unsigned char	temp ;
	unsigned char ix;
	usb_setup_flag = 0;
	usb_out_flag = 0;
	usb_in_flag = 0;
	usb_reset_flag = 0;
	usb_suspend_flag = 0;
	usb_resume_flag = 0;
	//usb_ep1_finish=0;
	usb_ep_flag=USB_EP0;
	write_finish = 1;
	dma_finished_flag[0] = 0;
	dma_finished_flag[1] = 0;
	usb_state = USB_STATE_POWERED;
	usb_running_state = USB_IDLE;
	usb_running_ctrl_state = USB_IDLE;
	
	for(ix = 0; ix < 5; ix++)
	{
		usb_ep_stall[ix] = FALSE;
		usb_ep_in_stall[ix] = FALSE;
		usb_ep_out_stall[ix] = FALSE;		
		usb_finish_flag[ix] = FALSE;
		usb_send_epn_stall[ix] = FALSE;
		stall_clear_flag[ix]=0;  //2012.04.19 BY P 
		write_ahb32(USB_EPn_CTRL(ix),0x00);//clear EP fifo data
	}
	write_ahb32(USB_INT_STATE,usb_read_mreg16(USB_INT_STATE));	
    write_ahb32(EP_INT_STATE,usb_read_mreg16(EP_INT_STATE));
    write_ahb32(EP0_INT_STATE,usb_read_mreg16(EP0_INT_STATE));
    write_ahb32(EPn_INT_STATE(1),usb_read_mreg16(EPn_INT_STATE(1)));
    write_ahb32(EPn_INT_STATE(2),usb_read_mreg16(EPn_INT_STATE(2)));
    write_ahb32(EPn_INT_STATE(3),usb_read_mreg16(EPn_INT_STATE(3)));
    write_ahb32(EPn_INT_STATE(4),usb_read_mreg16(EPn_INT_STATE(4)));
	//
	write_ahb32(USB_EP0_CTRL,0x00000000);
	write_ahb32(USB_EPn_CTRL(1),0x00000080);
	write_ahb32(USB_EPn_CTRL(2),0x00000080);
	write_ahb32(USB_EPn_CTRL(3),0x00000000);
	write_ahb32(USB_EPn_CTRL(4),0x00000000);
}
void usb_reset_handle(void)
{
	unsigned long val;
	//2012.04.13 modified by P
//	usb_init();
	reset_process();
	val = usb_read_mreg16(USB_TOP);
	usb_state = USB_STATE_DEFAULT;
	//USB_PRINTF_L0("USB Reset 0x%lx\r\n",val);
}
void usb_suspend_handle(void)
{	
	unsigned long val;
	if(usb_state > USB_STATE_ADDRESS)//note it
	{
		printf("suspend handle\r\n");
		#ifdef USB_SUSP_SYS_TEST
		//write_ahb32(USB_SUSP_SYS, 0x1);
		#else
		#ifdef AUTO_SUSPEND_TEST
		
		write_ahb32(USB_AHB_POWER,0x02); //enter suspend state, will shutdown phy clock
		#else

		write_ahb32(USB_AHB_POWER,0x01); //enter suspend state, will shutdown phy clock
		#endif
		#endif
		val = usb_read_mreg16(USB_TOP);
		//USB_PRINTF_L0("USB enter suspend 0x%lx\r\n",val);

		/***************JUST FOR RESUME TEST START*******************/
		delayms(10000);//10s
		//USB_PRINTF_L0("USB timeout, wakeup now!\r\n");
		#ifdef SYS_WAKE_UP
		//sys_wakeup_handle();
		#else
		usb_wakeup_handle();
        #endif
		//USB_PRINTF_L0("USB exit suspend\r\n");
		/***************JUST FOR RESUME TEST END*********************/
		usb_state = USB_STATE_SUSPENDED;
	}
}

void usb_resume_handle(void)
{
	unsigned long val;
	val = usb_read_mreg16(USB_TOP);
	//USB_PRINTF_L0("USB got resume 0x%lx\r\n",val);
}

void usb_wakeup_handle(void)
{ 
	unsigned long val;
	val = usb_read_mreg16(USB_TOP);
	//USB_PRINTF_L0("USB before wakeup 0x%lx\r\n",val);
#ifdef AUTO_SUSPEND_TEST
	write_ahb32(USB_AHB_POWER,0x0A); //1011 wakeup from suspend stete
	//
	delay1ms(3);// 3ms//wait bus wakeup
	//while(usb_read_mreg16(USB_TOP)&0x10);//wait controller enter work state
	write_ahb32(USB_AHB_POWER,0x02); //0011 wakeup from suspend stete
#else
	write_ahb32(USB_AHB_POWER,0x03); //0011 enter work state
	write_ahb32(USB_AHB_POWER,0x0B); //1011 wakeup from suspend stete
	//
	delayms(3);// 3ms//wait bus wakeup
	//while(usb_read_mreg16(USB_TOP)&0x10);//wait controller enter work state
	write_ahb32(USB_AHB_POWER,0x03); //0011 wakeup from suspend stete
#endif
	val = usb_read_mreg16(USB_TOP);
	usb_state = USB_STATE_CONFIGURED;
	//USB_PRINTF_L0("USB after wakeup 0x%lx\r\n",val);
}

#ifdef SYS_WAKE_UP
void sys_wakeup_handle(void)
{ 
	unsigned char val;
#ifdef USB_SUSP_SYS_TEST
	write_ahb32(USB_SUSP_SYS, 0x0);
#endif
	write_ahb32(USB_WAKE_SYS,0x1);
	delayms(3);// 3ms//wait bus wakeup
	write_ahb32(USB_WAKE_SYS,0x0);
	val = usb_read_mreg16(USB_TOP);
	//USB_PRINTF_L0("USB after wakeup 0x%lx\r\n",val);
}
#endif

#endif //USB_TEST_EN

#ifdef DEFAULT_TEST_EN

const default_check_matrix usb_default_val[]= {
{USB_TOP   , 0x000000a0, 0x00000000},
{USB_INT_STATE	, 0x00000000, 0x00000000},
{EP_INT_STATE	, 0x00000000, 0x00000000},
{EP0_INT_STATE	, 0x00000000, 0x00000000},
{USB_INT_EN	, 0x00000000, 0x00000000},
{EP_INT_EN	, 0x00000000, 0x00000000},
{EP0_INT_EN	, 0x00000000, 0x00000000},
{EPn_INT_STATE(0)	, 0x00000000, 0x00000000},
{EPn_INT_STATE(1)	, 0x00000000, 0x00000000},
{EPn_INT_STATE(2)	, 0x00000000, 0x00000000},
{EPn_INT_STATE(3)	, 0x00000000, 0x00000000},
{EPn_INT_STATE(4)	, 0x00000000, 0x00000000},
{EPn_INT_EN(1)	, 0x00000000, 0x00000000},
{EPn_INT_EN(2)	, 0x00000000, 0x00000000},
{EPn_INT_EN(3)	, 0x00000000, 0x00000000},
{EPn_INT_EN(4)	, 0x00000000, 0x00000000},
{USB_ADDR	, 0x00000000, 0x00000000},
{USB_EP_EN	, 0x00000000, 0x00000000},
{USB_EP_INDEX1_2	, 0x00000021, 0x00000000},
{USB_EP_INDEX3_4	, 0x00000043, 0x00000000},
{USB_TOG_CTRL1_4	, 0x00000000, 0x00000000},
{USB_EP_SETUPn(0)	, 0x00000000, 0x00000000},
{USB_EP_SETUPn(1)	, 0x00000000, 0x00000000},
{USB_EP_SETUPn(2)	, 0x00000000, 0x00000000},
{USB_EP_SETUPn(3)	, 0x00000000, 0x00000000},
{USB_EP_SETUPn(4)	, 0x00000000, 0x00000000},
{USB_EP_SETUPn(5)	, 0x00000000, 0x00000000},
{USB_EP_SETUPn(6)	, 0x00000000, 0x00000000},
{USB_EP_SETUPn(7)	, 0x00000000, 0x00000000},
{USB_PACKAGE_SIZE0	, 0x00000040, 0x00000000},
{USB_PACKAGE_SIZE1	, 0x00000000, 0x00000000},
{USB_EP0_AVL	, 0x00000000, 0x00000000},
{USB_EPn_AVL(1)	, 0x00000000, 0x00000000},
{USB_EPn_AVL(2)	, 0x00000000, 0x00000000},
{USB_EPn_AVL(3)	, 0x00000000, 0x00000000},
{USB_EPn_AVL(4)	, 0x00000000, 0x00000000},
{USB_EP0_CTRL	, 0x00000000, 0x00000000},
{USB_EPn_CTRL(1)	, 0x00000000, 0x00000000},
{USB_EPn_CTRL(2)	, 0x00000000, 0x00000000},
{USB_EPn_CTRL(3)	, 0x00000000, 0x00000000},
{USB_EPn_CTRL(4)	, 0x00000000, 0x00000000},
{USB_EP0_FIFO	, 0x00000000, 0x00000000},
{USB_EPn_FIFO(1)	, 0x00000000, 0x00000000},
{USB_EPn_FIFO(2)	, 0x00000000, 0x00000000},
{USB_EPn_FIFO(3)	, 0x00000000, 0x00000000},
{USB_EPn_FIFO(4)	, 0x00000000, 0x00000000},
{USB_EP_MEM	, 0x00000000, 0x00000000},
{USB_EP_HALT	, 0x00000000, 0x00000000},
{USB_AHB_POWER	, 0x00000003, 0x00000000},
{USB_AHB_RST	, 0x00000000, 0x00000000},
//{DIS_CLK_SYS	, 0x0000016c, 0x00000000},
//{USB_SOFT_RST	, 0x00000505, 0x00000000},
//{USB_SUSP_SYS	, 0x00000001, 0x00000000},
//{USB_WAKE_SYS	, 0x00000000, 0x00000000},

};

void usb_reset()
{
	sys_release_ip(IP_USB, FALSE);
	sys_release_ip(IP_USB, TRUE);
}

void usb_check_default()
{
	DPRINTF_L1("usb_check_default\n");
	check_default_value(usb_default_val,DEFAULT_ARRAY_SIZE(usb_default_val), usb_reset);
}
#endif //DEFAULT_TEST_EN
