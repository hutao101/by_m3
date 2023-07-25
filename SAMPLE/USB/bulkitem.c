//********************************************************************
//(Copyright 2012) //P Inc. All Rights Reserved
//Company Confidential and Proprietary information
//This information may not be disclosed to unauthorized individual
//********************************************************************
//
////P Microelectronics Co.,Ltd
//
//File name 	: bulkiten.c
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
unsigned long debug_flag=0;

volatile unsigned char  InquriySendBack[36] = {   0x00, 0x80/*removable media*/, 0x00, 0x01/*Response Data Format*/, 0x1F/*additional length*/, 0x00, 0x00, 0x00,         // 0x12
                                   'G', 'e', 'n', 'e', 'r', 'i', 'c', ' ',
                                   'U', 'S', 'B', ' ', 'S', 't', 'o', 'r',
                                    'a', 'g', 'e', '-', 'S', 'C', 'C', '5',
                                    '0', '1', '1', '0' };
//volatile unsigned char  InquriySendBack[39] = {   0x00, 0x80/*removable media*/, 0x00, 0x01/*Response Data Format*/, 0x22/*additional length*/, 0x00, 0x00, 0x00,         // 0x12
//                                    'G', 'e', 'n', 'e', 'r', 'i', 'c', ' ',
//                                   'U', 'S', 'B', ' ', 'S', 't', 'o', 'r',
//                                    'a', 'g', 'e', '-', 'H', 'A', 'N', 'W','A','N','G',
//                                    '0', '1', '1', '0' };
volatile unsigned char  SenseData[18] = {0x70,0x00,0x00,0x00,0x00,0x00,0x00,0x0A,
								 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

volatile unsigned char  Read_Cap_SendBack[] = { 0x00, 0x00, 0x00, 0x3C, 0x00, 0x00, 0x02, 0x00 } ;  // 0x25, 30K, 512Byte/Page
volatile unsigned char  Op_23_SendBack[12];
volatile unsigned char  usb_block_addr[4];
volatile unsigned char trans_length[2];
volatile unsigned long  ram_addr;
volatile unsigned char page_count;
volatile unsigned char  buf_count;
volatile unsigned char  flash_block_addr[4];
volatile unsigned long  flash_count;
volatile unsigned char 	dma_finished_flag[2];
volatile unsigned char 	write_finish;
volatile unsigned long p_send_buf;
volatile unsigned long p_recv_buf;
volatile unsigned long send_residue;
volatile unsigned long recv_residue;

volatile char usb_csw_buf[USB_CSW_SIZE];

static unsigned long usb_in_stall_ix = 1;	
//========================= Bulk Tools ==============================
void SendCSWBack()
{
	unsigned long cnt = MIN(send_residue,EPn_MAX_PACKAGE_SIZE);
#ifdef USB_CBWCSW_DMA_MODE
	p_send_buf = device_tx_fifo(p_send_buf,cnt,TRUE);
#else
	p_send_buf = device_tx_fifo(p_send_buf,cnt,FALSE);
#endif
	send_residue -= cnt;
	if(send_residue == 0 )
	{
		usb_running_state = USB_IDLE ;
		p_recv_buf = (volatile unsigned long)usb_cbw_buf;
		recv_residue = sizeof(usb_cbw_buf);
		////USB_PRINTF_L0("S\r\n\r\n");
    }
}

void USB_Prepare_CSW( unsigned long  num, unsigned long  state )
{
	//usb_ep1_finish = 0;
	//dCSWSignature byte[3]--byte[0]
	usb_csw_buf[0]=0x55;
	usb_csw_buf[1]=0x53;
	usb_csw_buf[2]=0x42;
	usb_csw_buf[3]=0x53;
	//dCSWTag byte[7]--byte[4]
	usb_csw_buf[4]=dCBWTag[0];
	usb_csw_buf[5]=dCBWTag[1];
	usb_csw_buf[6]=dCBWTag[2];
	usb_csw_buf[7]=dCBWTag[3];
	//dCSWDataResidue byte[11]--byte[8]
	usb_csw_buf[8]=(num>>0)&0xff;
	usb_csw_buf[9]=(num>>8)&0xFF;
	usb_csw_buf[10]=(num>>16)&0xFF;
	usb_csw_buf[11]=(num>>24)&0xFF;
	//bCSWStatus byte[12]
	usb_csw_buf[12]=state;
	//
	p_send_buf = (volatile unsigned long)usb_csw_buf;
	send_residue = sizeof(usb_csw_buf);
}

//========================= BULK Command ======================================
void _sense6_handle( void)          // 0x06
{
    unsigned char   count ;

//    printf( "Bulk: Sense6, 0x%bx\n", usb_running_state ) ;
    switch( usb_running_state )
    {
        case    USB_BULK_CBW :
        {
            usb_running_state = USB_BULK_IN ;
            break ;
        }
        case    USB_BULK_IN :
        {   
			while(usb_read_mreg16(USB_EPn_CTRL(EPIN))&0x80);//wait last send data finish
            for( count = 0; count < 8; count++ )
            {
                 write_ahb32(USB_EPn_FIFO(EPIN),0x00);	
            }
	      	write_ahb32(USB_EPn_CTRL(EPIN),0x80|count);
	      	USB_Prepare_CSW(0x00,CSW_CMD_PASSED);
            usb_running_state = USB_BULK_CSW ;
            break ;
        }
        case    USB_BULK_CSW :
        {
            SendCSWBack() ;			
            break ;
        }
        default :
        {
            usb_running_state = USB_IDLE ;
            break ;
        }
    }            
}

void _read_capacity_handle( void)   // 0x25
{
//    unsigned char   count ;
	unsigned long   temp;
//    printf( "Bulk: Read_Cap, 0x%bx\n", usb_running_state ) ;
    switch( usb_running_state )
    {
        case    USB_BULK_CBW :
        {
			temp = ((USB_DISK_OFFSET_END - USB_DISK_OFFSET)>>9) - 1;
			Read_Cap_SendBack[0] = 	(unsigned char)((temp & 0xff000000) >> 24);
			Read_Cap_SendBack[1] = 	(unsigned char)((temp & 0x00ff0000) >> 16);
			Read_Cap_SendBack[2] = 	(unsigned char)((temp & 0x0000ff00) >>  8);
			Read_Cap_SendBack[3] = 	(unsigned char)((temp & 0x000000ff) >>  0);
			p_send_buf = (volatile unsigned long)Read_Cap_SendBack;
			send_residue = sizeof(Read_Cap_SendBack);
            usb_running_state = USB_BULK_IN ;
            break ;
        }
        case    USB_BULK_IN :
		{
			unsigned long cnt = MIN(send_residue,EPn_MAX_PACKAGE_SIZE);
			p_send_buf = device_tx_fifo(p_send_buf,cnt,FALSE);
			send_residue -= cnt;
			if(send_residue == 0 )
			{
				USB_Prepare_CSW(send_residue,CSW_CMD_PASSED);
	    		usb_running_state = USB_BULK_CSW;
		    }
            break ;
        }
        case    USB_BULK_CSW :
        {
            SendCSWBack() ;
            break ;
        }
        default :
        {
            usb_running_state = USB_IDLE ;
            break ;
        }
    }
}
void _op_code_23_handle( void)      // read format capacities
{
//    unsigned char   count ;
	unsigned long   temp;

    switch( usb_running_state )
    {
        case    USB_BULK_CBW :
        {
			Op_23_SendBack[0] = 0x00;
			Op_23_SendBack[1] = 0x00;
			Op_23_SendBack[2] = 0x00;
			Op_23_SendBack[3] = 0x08;		    

			temp = ((USB_DISK_OFFSET_END - USB_DISK_OFFSET)>>9);// xxx/512

			Op_23_SendBack[4] = 	(unsigned char)((temp & 0xff000000) >> 24);
			Op_23_SendBack[5] = 	(unsigned char)((temp & 0x00ff0000) >> 16);
			Op_23_SendBack[6] = 	(unsigned char)((temp & 0x0000ff00) >>  8);
			Op_23_SendBack[7] = 	(unsigned char)((temp & 0x000000ff) >>  0);

			Op_23_SendBack[8] = 0x01; //unformatted media
			Op_23_SendBack[9] = 0x00;
			Op_23_SendBack[10] = 0x02;
			Op_23_SendBack[11] = 0x00;
			p_send_buf = (volatile unsigned long)Op_23_SendBack;
			send_residue = sizeof(Op_23_SendBack);
            usb_running_state = USB_BULK_IN ;
            break ;
        }
        case    USB_BULK_IN :
        {
			unsigned long cnt = MIN(send_residue,EPn_MAX_PACKAGE_SIZE);
			p_send_buf = device_tx_fifo(p_send_buf,cnt,FALSE);
			send_residue -= cnt;
			if(send_residue == 0 )
			{
				USB_Prepare_CSW(send_residue,CSW_CMD_PASSED);
	    		usb_running_state = USB_BULK_CSW;
		    }
            break ;
        }
        case    USB_BULK_CSW :
        {
            SendCSWBack() ;
            break ;
        }
        default :
        {
            usb_running_state = USB_IDLE ;
            break ;
        }
    }    
}

void ScsiCmdInquiry( void)          // 0x12
{
//    unsigned char    count ;
    switch( usb_running_state )
    {
        case    USB_BULK_CBW :
        {
			p_send_buf = (volatile unsigned long)InquriySendBack;
			send_residue = sizeof(InquriySendBack);
            usb_running_state = USB_BULK_IN ;
            break ;
        }
        case    USB_BULK_IN :
		{
			unsigned long cnt = MIN(send_residue,EPn_MAX_PACKAGE_SIZE);

			//printf("0x%x,%d\r\n",p_send_buf,send_residue);
			p_send_buf = device_tx_fifo(p_send_buf,cnt,FALSE);
			send_residue -= cnt;
			if(send_residue == 0 )
			{
				USB_Prepare_CSW(send_residue,CSW_CMD_PASSED);
	    		usb_running_state = USB_BULK_CSW;
		    }
            break;
        }
        case    USB_BULK_CSW :
        {
            SendCSWBack() ;
            break ;
        }
        default :
        {
            usb_running_state = USB_IDLE ;
            break ;
        }
    }
}

void ScsiCmdRequestSense( void)          // 0x03
{
//    unsigned char    count ;
    switch( usb_running_state )
    {
        case    USB_BULK_CBW :
        {
			p_send_buf = (volatile unsigned long)SenseData;
			send_residue = sizeof(SenseData);
            usb_running_state = USB_BULK_IN ;
            break ;
        }
        case    USB_BULK_IN :
		{
			unsigned long cnt = MIN(send_residue,EPn_MAX_PACKAGE_SIZE);
			p_send_buf = device_tx_fifo(p_send_buf,cnt,FALSE);
			send_residue -= cnt;
			if(send_residue == 0 )
			{
				USB_Prepare_CSW(send_residue,CSW_CMD_PASSED);
	    		usb_running_state = USB_BULK_CSW;
		    }
            break;
        }
        case    USB_BULK_CSW :
        {
            SendCSWBack() ;
            break ;
        }
        default :
        {
            usb_running_state = USB_IDLE ;
            break ;
        }
    }
}
void _cmd_common_handle( void)     // 0x00
{
//    printf( "Bulk: Test Ready, 0x%bx\n", usb_running_state ) ;
    switch( usb_running_state )
    {
        case    USB_BULK_CBW :
        {
	        if( CBWCB[0] == TEST_UNIT_READY_OPCODE )
				USB_Prepare_CSW(0x00,CSW_CMD_PASSED);
			else
				USB_Prepare_CSW(0x00,CSW_CMD_PASSED);
			usb_running_state = USB_BULK_CSW ;			
            break ;
        }
		case 	USB_BULK_STALL:
		{
			if((usb_send_epn_stall[EPIN]==TRUE)||(usb_send_epn_stall[EPOUT]==TRUE))
				usb_running_state = USB_BULK_STALL;							
			else
				
				usb_running_state = USB_BULK_CSW ;			
			break;
		}
        case    USB_BULK_CSW :
        {
            SendCSWBack();
            break ;
        }
        default :
        {
            usb_running_state = USB_IDLE ;
            break ;
        }
    }    
}

//=================================== Sub Item Function =======================================
void covert_usb_2_flash( void)
{
    unsigned char count ;

    for( count = 0; count < 4; count++ )
    {
        usb_block_addr[count] = CBWCB[2 + count] ;
    }
    trans_length[0] = (unsigned char)CBWCB[7] ;
    trans_length[1] = (unsigned char)CBWCB[8] ;
	ram_addr = (((unsigned long)(usb_block_addr[0]))<<24) | 
			   (((unsigned long)(usb_block_addr[1]&0xff))<<16)| 
			   (((unsigned long)(usb_block_addr[2]&0xff))<< 8) |
			   (((unsigned long)(usb_block_addr[3]&0xff))<< 0);

    ram_addr = ((ram_addr<<9) + USB_DISK_OFFSET);

    page_count = ((trans_length[0]&0x00ff)<<8) | (trans_length[1] & 0x00ff);
    buf_count = USB_PAGE_SIZE/EPn_MAX_PACKAGE_SIZE;

}

//================= main item function =====================

void _write10_handle( unsigned long stand_requst)         // 0x2A
{
//	unsigned char	count ;
	switch( usb_running_state ) 
	{
		case    USB_BULK_CBW:
		{
			////USB_PRINTF_L0("%02X\r\n",CBWCB[0]);
        	covert_usb_2_flash();
			p_recv_buf = ram_addr;
			if(stand_requst)
				recv_residue = page_count*USB_PAGE_SIZE ;
			else
				recv_residue = page_count;
        	usb_running_state = USB_BULK_OUT;
        	break;
        }
        case    USB_BULK_OUT:
        {
			unsigned long cnt = MIN(recv_residue,EPn_MAX_PACKAGE_SIZE);
#ifdef USB_DMA_MODE			
			p_recv_buf = device_rx_fifo(p_recv_buf,cnt,TRUE);
#else
			p_recv_buf = device_rx_fifo(p_recv_buf,cnt,FALSE);
#endif
			recv_residue -= cnt;
			if(recv_residue == 0)
			{
				USB_Prepare_CSW(recv_residue, CSW_CMD_PASSED);
	    		usb_running_state = USB_BULK_CSW;
				////USB_PRINTF_L0("W\r\n");
		    }
        	break ;
        }
        case    USB_BULK_CSW:
        {
           	SendCSWBack();
            break;
        }
        default:
        {
            usb_running_state = USB_IDLE;
            break;
        }
    }       
}

void _read10_handle( unsigned long stand_request)      // 0x28
{

//	unsigned char	count ;
//	static unsigned long bulk_in_status = CSW_CMD_PASSED;
    switch( usb_running_state )
	{
		case    USB_BULK_CBW :
		{
			////USB_PRINTF_L0("%02X\r\n",CBWCB[0]);
	     	covert_usb_2_flash( ) ;
			p_send_buf = ram_addr;
			if(stand_request)
				send_residue = page_count*USB_PAGE_SIZE;
			else
				send_residue = page_count;
            usb_running_state = USB_BULK_IN ;
            break ;
        }
        case    USB_BULK_IN:
		{
			unsigned long cnt = MIN(send_residue,EPn_MAX_PACKAGE_SIZE);
#ifdef USB_DMA_MODE			
			p_send_buf = device_tx_fifo(p_send_buf,cnt,TRUE);
#else
			p_send_buf = device_tx_fifo(p_send_buf,cnt,FALSE);
#endif
			send_residue -= cnt;
			if(send_residue == 0 )
			{
				USB_Prepare_CSW(send_residue, CSW_CMD_PASSED);
	    		usb_running_state = USB_BULK_CSW;
				////USB_PRINTF_L0("R\r\n");
		    }
            break;
        }
        case    USB_BULK_CSW:
        {
        	SendCSWBack();
        	break ;
        }
        default :
        {
        	usb_running_state = USB_IDLE ;
        	break ;
        }
    }            
}

#endif //USB_TEST_EN
