//********************************************************************
//(Copyright 2012) //P Inc. All Rights Reserved
//Company Confidential and Proprietary information
//This information may not be disclosed to unauthorized individual
//********************************************************************
//
////P Microelectronics Co.,Ltd
//
//File name 	: Bulkusb.c
//Author		:
//Verision		: 
//Created		: 
//Modified		: 
//Description	:
//				  
//********************************************************************
//#include "platform.h"
#include "usb.h"

#ifdef USB_TEST_EN
volatile unsigned char   dCBWSig[4] ;
volatile unsigned char   dCBWTag[4] ;
volatile unsigned char  bmCBWDataLength[4] ;
volatile unsigned char   bmCBWFlag ;
volatile unsigned char  bCBWLUN ;
volatile unsigned char   bCBWCBLength ;
volatile unsigned char   CBWCB[16];
volatile char usb_cbw_buf[CBW_BUF_SIZE];


//#define DATA_TOGGLE_TEST
void Deal_Trans_Items( void)
{
	//while(usb_read_mreg16(USB_EPn_CTRL(EPIN))&0x80);//wait the last transfer finish ????
	//printf("CBWCB[0]:0x%x",CBWCB[0]);
    switch( CBWCB[0] )
    {
        case    INQUIRY_OPCODE :    // 0x12
        {
            ScsiCmdInquiry( ) ;
            break ;
        }
		case    REQUEST_SENSE_OPCODE:    // 0x03
        {
            ScsiCmdRequestSense( ) ;
            break ;
        }
        case    _23_OPCODE :        // 0x23
        {
            _op_code_23_handle( ) ;
            break ;
        }
        case    READ_CAPACITY10_OPCODE :    // 0x25
        {
            _read_capacity_handle( ) ;
            break ;
        }
        case    READ10_OPCODE :             // 0x28
        {
            _read10_handle(TRUE) ;
            break ;
        }
        case    MODE_SENSE6_OPCODE :        // 0x1A
        {
            _sense6_handle( ) ;
            break ;
        }
        case    WRITE10_OPCODE :            // 0x2A
        {
            _write10_handle(TRUE);
            break ;
        }
        case    READTEST_OPCODE :             // 0xA8
        {
            _read10_handle(FALSE) ;
            break ;
        }
		case    WRITETEST_OPCODE :            // 0xAA
        {		
            _write10_handle(FALSE);
            break ;
        }
		case    STALLOUT_OPCODE :            // 0xB8
        {		
			write_ahb32(USB_EP_HALT,(1<<EPOUT));  //all epn except ep0 send stall
			usb_send_epn_stall[EPOUT]=TRUE;
            //USB_PRINTF_L0( "EP enable out stall Cmd, 0x%x\r\n", CBWCB[0] ) ;
            usb_running_state = USB_IDLE ;
			p_recv_buf = (unsigned long)usb_cbw_buf;
			recv_residue = sizeof(usb_cbw_buf);
            break ;
        }
		
		case    STALLIN_OPCODE :            // 0xBA
        {		
			usb_send_epn_stall[EPIN]=TRUE;
            //USB_PRINTF_L0( "EP enable In stall Cmd, 0x%x\r\n", CBWCB[0] ) ;
            usb_running_state = USB_IDLE ;
			p_recv_buf = (volatile unsigned long)usb_cbw_buf;
			recv_residue = sizeof(usb_cbw_buf);

            break ;
        }
		
        case    TEST_UNIT_READY_OPCODE :    // 0x00
        case    PREVENT_REMOVAL_OPCODE :    // 0x1E
        case    VERIFY10_OPCODE :           // 0x2F    
        {
            _cmd_common_handle( ) ;
            break ;
        }                
        default :
        {
            //USB_PRINTF_L0( "Unknown Cmd, 0x%x\r\n", CBWCB[0] ) ;
//#ifdef USB_DEVICE_MASS_STORAGE
#ifdef USB_EP_STALL_TEST
			write_ahb32(USB_EP_HALT,(1<<EPOUT));
			usb_send_epn_stall[EPOUT] = TRUE;
#endif
//#endif
#if 0
            usb_running_state = USB_IDLE ;			
			p_recv_buf = (volatile unsigned long)usb_cbw_buf;
			recv_residue = sizeof(usb_cbw_buf);
#endif
			_cmd_common_handle( ) ;
			break;
        }
    }    
}
// ==================================== Low Sub Feature ===================================
void usb_cbw_handle( void)
{
	unsigned long cnt = MIN(recv_residue,EPn_MAX_PACKAGE_SIZE);
	//USB_PRINTF_L0("p_recv_buf:%x %d %d\r\n",p_recv_buf,recv_residue,cnt);
#ifdef USB_CBWCSW_DMA_MODE	
	p_recv_buf = device_rx_fifo(p_recv_buf,cnt,TRUE);
#else
	p_recv_buf = device_rx_fifo(p_recv_buf,cnt,FALSE);
#endif
	//USB_PRINTF_L0("%x %d %d\r\n",p_recv_buf,recv_residue,cnt);
	recv_residue -= cnt;
	if(recv_residue == 0)
	{
		usb_deal_cbw();
    }
}

void usb_deal_cbw(void)
{
    unsigned char   cbw_ix;
    unsigned char   dCBWSigCont[4] = { 0x55, 0x53, 0x42, 0x43 } ;   // USBC dCBWSignature
    unsigned char   error_flag = 0 ;
	//***********************************************//
	//Table 5.1 Command Block Wrapper
	//***********************************************//	
 	//read dCBWSignature byte[3]--byte[0]
    for( cbw_ix = 0; cbw_ix <= 3; cbw_ix++ )  
   	{
		dCBWSig[cbw_ix-0] = usb_cbw_buf[cbw_ix];
    }
	//read dCBWTag byte[7]--byte[4]
    for( cbw_ix = 4; cbw_ix <= 7; cbw_ix++ )  
   	{
		dCBWTag[cbw_ix-4] = usb_cbw_buf[cbw_ix];
   	}
	//read dCBWDataTRansferLength byte[11]--byte[8]
    for( cbw_ix = 8; cbw_ix <= 11; cbw_ix++ )  
   	{
		bmCBWDataLength[cbw_ix-8] = usb_cbw_buf[cbw_ix];
   	}
	//read bmCBWFlags byte[12]
    bmCBWFlag = usb_cbw_buf[12];
	//read bCBWLUN byte[13]
    bCBWLUN = usb_cbw_buf[13];
	//read bCBWCBLength byte[14]
    bCBWCBLength = usb_cbw_buf[14];//valid data is 1--16
    //read CBWCB byte[30]--byte[15]
    for( cbw_ix = 15; cbw_ix <= 30; cbw_ix++ ) 
   	{
		CBWCB[cbw_ix-15] = usb_cbw_buf[cbw_ix];
   	}
    //judge correct or wrong
    for( cbw_ix = 0; cbw_ix < 4; cbw_ix++ )  
    {
        if( dCBWSigCont[cbw_ix] != dCBWSig[cbw_ix] )    
			error_flag = 1 ;
    }
    if( bCBWLUN != 0x00 )   
	{
		error_flag = 1 ;
	}
    if( error_flag )    
    {
		//USB_PRINTF_L0("CBW error!\r\n");
        usb_running_state = USB_IDLE ;
		p_recv_buf = (volatile unsigned long)usb_cbw_buf;
		recv_residue = sizeof(usb_cbw_buf);
        return ;
    }	
    usb_running_state = USB_BULK_CBW ;
  	Deal_Trans_Items( ) ;
}

void usb_bulk_in_token_handle( void)
{
    Deal_Trans_Items( ) ;
}
void usb_bulk_out_handle( void)
{
    Deal_Trans_Items( ) ;
}
void usb_bulk_trans_finish_handle( void)
{
    Deal_Trans_Items( ) ;
}
// =============================== usb IN OUT main fature ==========================
void usb_in_handle( void)       // IN Request Received
{
	if( usb_ep_flag== USB_EP0 )
	{
		ctrl_in_token_handle( ) ;
//		p_recv_buf = (unsigned long)usb_cbw_buf;
//		recv_residue = sizeof(usb_cbw_buf);
	}
	else 
	{
#ifdef DEVICE_HID
		usb_hid_report_handle();
#else //!DEVICE_HID
#ifdef DEVICE_HID_TEST
	//while(1){  //2012.05.29 by P, for the max hid interrupt performance test,if only run the send and receive test, then the while loop need annotated
	device_tx_fifo(USB_DISK_OFFSET,16,0);//2012.05.17 by P , the length is temparary constant, maybe modified for setting this length according to the receive data from host
		//}
#else //DEVICE_HID_TEST
#ifndef USB_DEVICE_MASS_STORAGE
		if( usb_running_state == USB_IDLE)
		{
			if(usb_send_epn_stall[EPIN])
			{
				write_ahb32(USB_EP_HALT,(1<<EPIN));  //all epn except ep0 send stall
	            //USB_PRINTF_L0( "EP Send in stall Cmd\r\n") ;
			}
			else
			{	
				printf("mass storage");
				write_ahb32(USB_EPn_FIFO(EPIN),'U');
				write_ahb32(USB_EPn_CTRL(EPIN),0x81);
	            //USB_PRINTF_L0( "in@IDLE\r\n") ;
			}
		}
		else
#endif //USB_DEVICE_MASS_STORAGE		
		{
			usb_bulk_in_token_handle( ) ;
		}		
//		p_recv_buf = (unsigned long)usb_cbw_buf;
//		recv_residue = sizeof(usb_cbw_buf);
#endif //DEVICE_HID_TEST
#endif //DEVICE_HID
	}

}

void usb_out_handle( void)      // OUT Packet Received
{
	if( usb_ep_flag== USB_EP0 )
	{
		write_ahb32(0x40013400, 0x01010101);
		ctrl_out_handle();
		p_recv_buf = (volatile unsigned long)usb_cbw_buf;
		recv_residue = sizeof(usb_cbw_buf);		
	}
	else
	{
		write_ahb32(0x40013400, 0x02020202);
		#ifdef DEVICE_HID_TEST
		device_rx_fifo(USB_DISK_OFFSET,16,0);//2012.05.17 by P , the length is temparary constant, maybe modified for setting this length according to the receive data from host
		#else
		if( usb_running_state == USB_IDLE)
		{
			write_ahb32(0x40013400, 0x03030303);
			//usb_cbw_handle();  //2022/06/22 DMA debug,close cpu process
		}
		else                       // Out for bulk trans 
		{
			write_ahb32(0x40013400, 0x04040404);
			//usb_bulk_out_handle();   	 //2022/06/22 DMA debug,close cpu process
				//p_recv_buf = (unsigned long)usb_cbw_buf; 
				//recv_residue = sizeof(usb_cbw_buf); 
		}
		#endif
	}

}

unsigned long device_rx_fifo(unsigned long dstAddr, unsigned long byteCnt,unsigned long bDMAmode)
{
//{
	unsigned long count;
	for( count = 0; count < byteCnt; count++ )
	{
		unsigned char val=(unsigned long)usb_read_mreg16(USB_EPn_FIFO(EPOUT));
		write_ahb32(dstAddr,val);
		dstAddr++;  //²ÈÄÚ´æ
    }
//}
	return dstAddr;
}

unsigned long device_tx_fifo(unsigned long srcAddr, unsigned long byteCnt, unsigned long bDMAmode)
{
//{
	unsigned long count;
	while(usb_read_mreg16(USB_EPn_CTRL(EPIN))&0x80);//wait last send data finish, maybe not need it
	for( count = 0; count < byteCnt; count++)
	{
		unsigned char val = usb_read_mreg16(srcAddr);
		write_ahb32(USB_EPn_FIFO(EPIN),(unsigned long)val);
		srcAddr++;
	}
	write_ahb32(USB_EPn_CTRL(EPIN),0x80|byteCnt);
//}
	return srcAddr;
}

#ifdef DEVICE_HID
void usb_hid_report_handle()
{
#ifdef EPn_IN_FUNCTION_STALL_TEST   //2012.04.19 BY P 
	if(stall_clear_flag[EPIN]==0)
		{
	        write_ahb32(USB_EP_HALT,(1<<(EPIN)));
	        //uart_putc('B');//2012.04.19 BY P	
	        //uart_putc((const char)stall_clear_flag[EPIN]);//2012.04.19 BY P
	        uart_putc((const char)7);//2012.04.19 BY P
		}
	////stall_clear_flag[EPIN]=0;//2012.05.07 by P, for see the mouse move state//2012.05.07 by P, not successful
	//uart_putc('C');//2012.04.19 BY P
#endif
#ifdef USB_EP_STALL_TEST
	static unsigned long hid_cnt = 1;
	if(((hid_cnt++)%0x80==0x00))	
	{
		write_ahb32(USB_EP_HALT,(1<<(EPIN)));	
		usb_send_epn_stall[EPIN] = TRUE;
	}
	else
#endif //USB_EP_STALL_TEST
	{
		//while(usb_read_mreg16(USB_EPn_CTRL(EPIN))&0x80);//wait last send data finish////2012.05.04 by P ,temp modified
		//	write_ahb32(USB_EPn_FIFO(EPIN),0x01);//left key pressed
		#ifdef DATA_TOGGLE_TEST   //2012.04.19 BY P 
		//write_ahb32(USB_TOG_CTRL1_4,(0x3<<((EPIN-1)*2)));//2012.04.19 test the data1
		write_ahb32(USB_TOG_CTRL1_4,(0x2<<((EPIN-1)*2)));//2012.04.19 test the data0
		#endif
		
#ifndef USB_DMA_MODE
		while(usb_read_mreg16(USB_EPn_CTRL(EPIN))&0x80);//wait last send data finish////2012.05.04 by P ,temp modified
		write_ahb32(USB_EPn_FIFO(EPIN),0x00);//left key not pressed
		write_ahb32(USB_EPn_FIFO(EPIN),1); //x offset two's complement
		write_ahb32(USB_EPn_FIFO(EPIN),1); //x offset two's complement
		write_ahb32(USB_EPn_FIFO(EPIN),0x00);
		write_ahb32(USB_EPn_CTRL(EPIN),0x84);//send 4 bytes packet
#else
    //DMA³õÊ¼»¯
    dw_dmac_disableChannel(0x07);
	dw_dmac_disable();

	//clear all the status register of select channel
	dw_dmac_clearIrq(0x07, 0x1f);
	//dmac_per2mem_channel_cfg(channel); //DMAÅäÖÃ
	dmac_mem2per_channel_cfg(0x07); //DMAÅäÖÃ
	//printf("DMAC_CTL_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_H(0x07)));
	//enable the channel to start transfer
	dw_dmac_enableChannel(0x07);
	//printf("DAMC_ENABLE\r\n");
	//printf("DMAC_CTL_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_H(0x07)));

	//printf("DMAC SAR:0x%x\r\n",read_ahb32(DMA1_BASE +(0x58*7)));

	//printf("DMAC_CTL_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_L(0x07)));
	//printf("DMAC_CTL_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_H(0x07)));
	
	//printf("DMAC_CFG_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(0x07)));
    //printf("DMAC_CFG_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_H(0x07)));
	//printf("DMAC_CTL_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_H(0x07)));
	//printf("DMAC_CTL_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_H(0x07)));
	//printf("DMAC_CTL_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_H(0x07)));
	//printf("DMAC_CTL_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_H(0x07)));
	//printf("DMAC_CTL_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_H(0x07)));
	//printf("DMAC_CTL_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_H(0x07)));
	//delay_us(20);
	
	write_ahb32(USB_EP_DMA,0x00);//disable dma mode
	usb_dma_init();
	write_ahb32(USB_EP_DMA,0x06);//enable dma mode
	
	write_ahb32(USB_EPn_CTRL(EPIN),0x80);//send 4 bytes packet

#endif
	}	
}

#endif //DEVICE_HID

#endif //USB_TEST_EN
