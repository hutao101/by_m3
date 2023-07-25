//********************************************************************
//(Copyright 2012) ////P Inc. All Rights Reserved
//Company Confidential and Proprietary information
//This information may not be disclosed to unauthorized individual
//********************************************************************
//
//////P Microelectronics Co.,Ltd
//
//File name 	: ctrlitem.c
//Author		:
//Verision		: 
//Created		: 
//Modified		: 
//Description	:
//				  
//********************************************************************
//#include "platform.h"
#include "usb.h"
//#include "uart.h"

//static unsigned long debug_cnt=0;
#ifdef USB_TEST_EN

#ifdef DEVICE_HID
	#define MY_VID  		0x0461
	#define MY_PID  		0x4D15
#else //CBW/CSW
#ifdef DEVICE_HID_TEST
	#define MY_VID  		0x05E4
	#define MY_PID  		0x0101
	#define IFCLASS			IFCLASS_TEST
	#define IFSUBCLASS		IFSUBCLASS_TEST
	#define IFPROTOCOL      IFPROTOCOL_TEST 
#else //!DEVICE_HID_TEST
#ifdef USB_DEVICE_MASS_STORAGE
	#define MY_VID  		0x05E2
	#define MY_PID  		0x0101
	#define IFCLASS			IFCLASS_MASS
	#define IFSUBCLASS		IFSUBCLASS_MASS
	#define IFPROTOCOL      IFPROTOCOL_MASS 	
#else //!USB_DEVICE_MASS_STORAGE
	#define MY_VID  		0x05E4
	#define MY_PID  		0x0101
	#define IFCLASS			IFCLASS_TEST
	#define IFSUBCLASS		IFSUBCLASS_TEST
	#define IFPROTOCOL      IFPROTOCOL_TEST 	
#endif // MASS STORAGE
#endif //DEVICE_HID_TEST
#endif //DEVICE_HID

volatile unsigned char    ctrl_buf[256] ;
volatile unsigned char    *p_ctrl_buf=0;
volatile unsigned char    ctrl_count ;
#ifdef DEVICE_HID//HID 
/*(Mouse) information from Appendix E: Example USB Descriptors for HID Class Devices, in Page76 @ HID1.1*/
static unsigned char    DeviceDesc[] = {
0x12/*bLength*/,0x01/*bDescrType=DEVICE*/,0x10/*bcdUSB*/,0x01/*bcdUSB*/,0x00/*bDevClass*/,0x00/*bDevSubClass*/,0x00/*bDevProtocol*/,
EP0_MAX_PACKAGE_SIZE/*bMaxPacketSize*/,(MY_VID>>0)&0xFF/*idVendor*/,(MY_VID>>8)&0xFF/*idVendor*/,(MY_PID>>8)&0xFF/*idProduct*/,(MY_PID>>8)&0xFF/*idProduct*/,0x00/*bcdDev*/,0x02/*bcdDev*/,
0x00/*iManugacturer*/,0x02/*iProduct*/,0x00/*iSerialNum*/,0x01/*bNumConfig*/};  

static unsigned char    ConfigDesc[] = {
0x09/*bLength*/,0x02/*bDescrType=CONFIGURATION*/,0x22/*wTotalLength*/,0x00/*wTotalLength*/,0x01/*bNumIF*/,0x01/*bCfgValue*/,0x00/*iConfig*/,0xA0/*bmAttribute=bus Power, remote wakeup*/,0x32/*MaxPower*/,/*device configuration*/
0x09/*bLength*/,0x04/*bDescrType=INTERFACES*/,0x00/*bIFNum*/,0x00/*bAlternateSetting*/,0x01/*bNumEP*/,IFCLASS_HID/*bIFClass=HID*/,IFSUBCLASS_HID/*bIFSubClass=boot IF*/,IFPROTOCOL_HID/*bIFProtocol=Mouse*/,0x00/*biIF*/,/*interface configuration*/
0x09/*bLength*/,0x21/*bDescrType=HID*/,0x11/*bcdHID*/,0x01/*bcdHID*/,0x00/*bcountryCode*/,0x01/*bNumDescriptors=number of HID class Desriptors*/,0x22/*bDescriptorType=report*/,0x34/*wDescriptorLength=sizeof(IntefaceDes)*/,0x00/*wDescriptorLength*/,  /*HID Descriptor*/
0x07/*bLength*/,0x05/*bDescrType=ENDPOINT*/,EPn_INPORT(EPIN)/*bEPAddr*/,0x03/*bmAttributes=Interrupt*/,EPn_MAX_PACKAGE_SIZE/*wMaxPacketSize*/,0x00/*bInterval*/,0x0a/*bInterval*/};/*EndPoint Configuration Descriptor*/

static unsigned char    Str0Desc[] = {0x04, 0x03, 0x09, 0x04 };
static unsigned char    Str1Desc[] = {0x04, 0x03, ' ', 0x00};
static unsigned char    Str2Desc[] = {0x24,0x03,0x55,0x00,0x53,0x00,0x42,0x00,
							   0x20,0x00,0x4f,0x00,0x70,0x00,0x74,0x00,
							   0x69,0x00,0x63,0x00,0x61,0x00,0x6c,0x00,
							   0x20,0x00,0x4d,0x00,0x6f,0x00,0x75,0x00,
							   0x73,0x00,0x65,0x00};

static unsigned char    Str3Desc[] = {0x16, 0x03, '2', 0x00, '0', 0x00, '2', 0x00, '7', 0x00, '3', 0x00,
                            '0', 0x00, '0', 0x00, '4', 0x00, '1', 0x00, '3', 0x00};
/*Report Descriptor (Mouse) */
static unsigned char    IntefaceDes[] = {  0x05,0x01,0x09,0x02,0xa1,0x01,0x09,0x01,
								   0xa1,0x00,0x05,0x09,0x19,0x01,0x29,0x03,
								   0x15,0x00,0x25,0x01,0x75,0x01,0x95,0x03,
								   0x81,0x02,0x75,0x05,0x95,0x01,0x81,0x01,
								   0x05,0x01,0x09,0x30,0x09,0x31,0x09,0x38,
								   0x15,0x81,0x25,0x7f,0x75,0x08,0x95,0x03,
								   0x81,0x06,0xc0,0xc0};
#define InterfaceDesLen		(sizeof(IntefaceDes))
#else
#ifdef DEVICE_HID_TEST
/*(Mouse) information from Appendix E: Example USB Descriptors for HID Class Devices, in Page76 @ HID1.1*/
static unsigned char    DeviceDesc[] = {
0x12/*bLength*/,0x01/*bDescrType=DEVICE*/,0x10/*bcdUSB*/,0x01/*bcdUSB*/,0x00/*bDevClass*/,0x00/*bDevSubClass*/,0x00/*bDevProtocol*/,
EP0_MAX_PACKAGE_SIZE/*bMaxPacketSize*/,(MY_VID>>0)&0xFF/*idVendor*/,(MY_VID>>8)&0xFF/*idVendor*/,(MY_PID>>8)&0xFF/*idProduct*/,(MY_PID>>8)&0xFF/*idProduct*/,0x00/*bcdDev*/,0x02/*bcdDev*/,
0x00/*iManugacturer*/,0x02/*iProduct*/,0x00/*iSerialNum*/,0x01/*bNumConfig*/};  

static unsigned char    ConfigDesc[] = {
0x09/*bLength*/,0x02/*bDescrType=CONFIGURATION*/,0x29/*wTotalLength*/,0x00/*wTotalLength*/,0x01/*bNumIF*/,0x01/*bCfgValue*/,0x00/*iConfig*/,0xA0/*bmAttribute=bus Power, remote wakeup*/,0x32/*MaxPower*/,/*device configuration*/
0x09/*bLength*/,0x04/*bDescrType=INTERFACES*/,0x00/*bIFNum*/,0x00/*bAlternateSetting*/,0x02/*bNumEP*/,IFCLASS/*bIFClass=HID*/,IFSUBCLASS/*bIFSubClass=boot IF*/,IFPROTOCOL/*bIFProtocol=Mouse*/,0x00/*biIF*/,/*interface configuration*/
0x09/*bLength*/,0x21/*bDescrType=HID*/,0x11/*bcdHID*/,0x01/*bcdHID*/,0x00/*bcountryCode*/,0x01/*bNumDescriptors=number of HID class Desriptors*/,0x22/*bDescriptorType=report*/,0x34/*wDescriptorLength=sizeof(IntefaceDes)*/,0x00/*wDescriptorLength*/,  /*HID Descriptor*/
0x07/*bLength*/,0x05/*bDescrType=ENDPOINT*/,EPn_INPORT(EPIN)/*bEPAddr*/,0x03/*bmAttributes=Interrupt*/,EPn_MAX_PACKAGE_SIZE/*wMaxPacketSize*/,0x00/*bInterval*/,0x03/*bInterval*/,/*EndPoint Configuration Descriptor*/
0x07/*bLength*/,0x05/*bDescrType=ENDPOINT*/,EPn_OUTPORT(EPOUT)/*bEPAddr*/,0x03/*bmAttributes=Interrupt*/,EPn_MAX_PACKAGE_SIZE/*wMaxPacketSize*/,0x00/*bInterval*/,0x03/*bInterval*/};/*EndPoint Configuration Descriptor*/

static unsigned char    Str0Desc[] = {0x04, 0x03, 0x09, 0x04 };
static unsigned char    Str1Desc[] = {0x04, 0x03, ' ', 0x00};
static unsigned char    Str2Desc[] = {0x24,0x03,0x55,0x00,0x53,0x00,0x42,0x00,
							   0x20,0x00,0x4f,0x00,0x70,0x00,0x74,0x00,
							   0x69,0x00,0x63,0x00,0x61,0x00,0x6c,0x00,
							   0x20,0x00,0x4d,0x00,0x6f,0x00,0x75,0x00,
							   0x73,0x00,0x65,0x00};

static unsigned char    Str3Desc[] = {0x16, 0x03, '2', 0x00, '0', 0x00, '2', 0x00, '7', 0x00, '3', 0x00,
                            '0', 0x00, '0', 0x00, '4', 0x00, '1', 0x00, '3', 0x00};
/*Report Descriptor (Mouse) */
static unsigned char    IntefaceDes[] = {  0x05,0x01,0x09,0x02,0xa1,0x01,0x09,0x01,
								   0xa1,0x00,0x05,0x09,0x19,0x01,0x29,0x03,
								   0x15,0x00,0x25,0x01,0x75,0x01,0x95,0x03,
								   0x81,0x02,0x75,0x05,0x95,0x01,0x81,0x01,
								   0x05,0x01,0x09,0x30,0x09,0x31,0x09,0x38,
								   0x15,0x81,0x25,0x7f,0x75,0x08,0x95,0x03,
								   0x81,0x06,0xc0,0xc0};
#define InterfaceDesLen		(sizeof(IntefaceDes))
#else // Mass-storage
//=========================== Common Para  =============================
static unsigned char    DeviceDesc[] = {    
0x12/*bLength*/, 0x01/*bDescrType=DEVICE*/, 0x10/*bcdUSB*/, 0x01/*bcdUSB*/, 0x00/*bDevClass*/, 0x00/*bDevSubClass*/, 0x00/*bDevProtocol*/,  // USB Version 1.10
EP0_MAX_PACKAGE_SIZE/*bMaxPacketSize*/,(MY_VID>>0)&0xFF/*idVendor*/,(MY_VID>>8)&0xFF/*idVendor*/,(MY_PID>>8)&0xFF/*idProduct*/,(MY_PID>>8)&0xFF/*idProduct*/, 0x00/*bcdDev*/, 0x01/*bcdDev*/,
0x01/*iManugacturer*/, 0x02/*iProduct*/, 0x03/*iSerialNum*/, 0x01/*bNumConfig*/} ;   

static unsigned char    ConfigDesc[] = {    
0x09/*bLength*/,0x02/*bDescrType=CONFIGURATION*/,0x20/*wTotalLength*/,0x00/*wTotalLength*/,0x01/*bNumIF*/,0x01/*bCfgValue*/,0x00/*iConfig*/,0xA0/*bmAttribute=bus Power, remote wakeup*/,0x32/*MaxPower*/,/*device configuration*/
0x09/*bLength*/,0x04/*bDescrType=INTERFACES*/,0x00/*bIFNum*/,0x00/*bAlternateSetting*/,0x02/*bNumEP*/,IFCLASS/*bIFClass=Mass Storage*/,IFSUBCLASS/*bIFSubClass*/,IFPROTOCOL/*bIFProtocol=Bulk-Only*/,0x00/*biIF*/,/*interface configuration*/
0x07/*bLength*/,0x05/*bDescrType=ENDPOINT*/,EPn_INPORT(EPIN)/*bEPAddr*/,0x02/*bmAttribute=Bulk*/,EPn_MAX_PACKAGE_SIZE/*wMaxPacketSize*/,0x00/*wMaxPacketSize*/,0x00/*bInterval*/,/*ep configuration*/
0x07/*bLength*/,0x05/*bDescrType=ENDPOINT*/,EPn_OUTPORT(EPOUT)/*bEPAddr*/,0x02/*bmAttribute=Bulk*/,EPn_MAX_PACKAGE_SIZE/*wMaxPacketSize*/,0x00/*wMaxPacketSize*/,0x00/*bInterval*/}/*ep configuration*/;

static unsigned char    Str0Desc[] = {0x04/*bLength*/, 0x03/*bDescrType=STRING*/, 0x09, 0x04 };
/*manugacturer*/
static unsigned char    Str1Desc[] = {0x04/*bLength*/, 0x03/*bDescrType=STRING*/, ' ', 0x00};
/*Product*/
static unsigned char    Str2Desc[] = {0x1a/*bLength*/, 0x03/*bDescrType=STRING*/, 
							'S', 0x00, 'C', 0x00, 'C', 0x00, '5',
                            0x00, '1', 0x00, '.', 0x00, '1', 0x00, ' ', 0x00,
                            'D',  0x00, 'i', 0x00, 's', 0x00, 'k', 0x00};
//static unsigned char    Str2Desc[] = {0x20/*bLength*/, 0x03/*bDescrType=STRING*/, 
//							'H', 0x00, 'A', 0x00, 'N', 0x00, 'W',0x00, 'A',0x00, 'N',0x00, 'G',
//                            0x00, '1', 0x00, '.', 0x00, '1', 0x00, ' ', 0x00,
//                            'D',  0x00, 'i', 0x00, 's', 0x00, 'k', 0x00};

/*serial number at least 12 characters @4.1.1 */
static unsigned char    Str3Desc[] = {0x16/*bLength*/, 0x03/*bDescrType=STRING*/, 
							'2', 0x00, '0', 0x00, '2', 0x00, '7', 0x00, '3', 0x00,
                            '0', 0x00, '0', 0x00, '4', 0x00, '1', 0x00, '3', 0x00};
#endif //
#endif  ////2012.05.17 by P , for hid test device
#define DeviceDescLen       (sizeof(DeviceDesc))
#define ConfigDescLen       (sizeof(ConfigDesc))
#define Str0DescLen         (sizeof(Str0Desc))
#define Str1DescLen         (sizeof(Str1Desc))
#define Str2DescLen         (sizeof(Str2Desc))
#define Str3DescLen         (sizeof(Str3Desc))
static unsigned char    StrDescLenGroup[] = { Str0DescLen, Str1DescLen, Str2DescLen, Str3DescLen } ;
//=========================== Each Request =============================
void Chap9_SetConfiguration( void)	//0x09
{
    // //USB_PRINTF_L0( "SetConfiguration, 0x%bx\n\r", usb_running_ctrl_state ) ;
	if(req_dir == USB_REQ_DIR_OUT)
	{
		EP0_Send_Empty_Packet();
	}
	else
	{
		//usb_running_ctrl_state = USB_IDLE;
	}
}
void Chap9_SetAddress( void)	    //0x05
{	
	if(req_dir == USB_REQ_DIR_OUT)
	{
		EP0_Send_Empty_Packet();	
		usb_state = USB_STATE_ADDRESS;
	}
	else
	{

	}
	////USB_PRINTF_L0( "SetAddr 0x%lx \n\r",usb_read_mreg16(USB_ADDR)) ;
}

void Get_Desc_Prepare( void)
{	
	unsigned long type = wValue & 0xFF00;
	unsigned long index = wValue & 0x00FF;
//	unsigned char temp;
    switch(type)
    {
        case    DEVICE_DESC_TYPE :
        {
        	//USB_PRINTF_L0("device descript\r\n");
            if( wLength > DeviceDescLen )   wLength = DeviceDescLen ;
            p_ctrl_buf = DeviceDesc ;
            break ;
        }
        case    CONFIGURATION_DESC_TYPE :
        {
        	////USB_PRINTF_L0("configure descript\n");
            if( wLength > ConfigDescLen )   wLength = ConfigDescLen ;
            p_ctrl_buf = ConfigDesc ;
            break ;
        }
        case    STRING_DESC_TYPE :
        {
        	////USB_PRINTF_L0("string descript\n");
			//index = wValue & 0xFF;
            if( index < 4 )
            {
                if( wLength > StrDescLenGroup[index] )   
					wLength = StrDescLenGroup[index] ;
                switch(index)
                {
					case 0:
						p_ctrl_buf = Str0Desc;
						break;
					case 1:
						p_ctrl_buf = Str1Desc;
						break;
					case 2:
						p_ctrl_buf = Str2Desc;
						break;
					case 3:
						p_ctrl_buf = Str3Desc;
						break;
                }

            }
			else
            {
                wLength = 0 ;
            }
            break ;
        }
#ifdef DEVICE_HID		
		case HID_REPORT_TYPE:  // class des
		{
			if( wLength > ConfigDescLen )   wLength = InterfaceDesLen;
			p_ctrl_buf = IntefaceDes;
			break;
		}
#endif		
        default :
        {
            wLength = 0 ;
            break ;
        }
    }
}
void Chap9_GetDescriptor( void)     //0x06
{	 
	static unsigned char    ctrl_buf_ix = 0;
   	//printf( "0x%x\n\r", usb_running_ctrl_state ) ;
    switch( usb_running_ctrl_state )
    {
        case    USB_CTRL_SETUP :
        {	 
            Get_Desc_Prepare( ) ;
            usb_running_ctrl_state = USB_CTRL_IN ;
			ctrl_buf_ix = 0;
	        //USB_PRINTF_L0("a\r\n");			
            break ; 
        }
        case    USB_CTRL_IN :
        {	 
            unsigned char    count = 0;
            unsigned char    ix = 0 ;
			//USB_PRINTF_L0("b\r\n");	
			if(wLength>0)
			{
				count = MIN(EP0_MAX_PACKAGE_SIZE,wLength);
				while(usb_read_mreg16(USB_EP0_CTRL)&0x80);//wait last send data finish
				for(ix=0; ix < count; ix++)
	            {
	                write_ahb32(USB_EP0_FIFO,p_ctrl_buf[ctrl_buf_ix]);
					ctrl_buf_ix++;
	            }
	            write_ahb32(USB_EP0_CTRL,count|0x80);
				////while(usb_read_mreg16(USB_EP0_CTRL)&0x80);//wait last send data finish//2012.04.06 add by P 
	            wLength -= count;
				
			}			
			if( wLength == 0)
           	{
           		if(count==EP0_MAX_PACKAGE_SIZE)
       			{
					EP0_Send_Empty_Packet();
				}
				else
				{
				}
                usb_running_ctrl_state = USB_IDLE;
				ctrl_buf_ix = 0;
           	}
			
            break ;
        }
        default :
        {
            usb_running_ctrl_state = USB_IDLE ;
            break ;
        }
    }
	usb_state = USB_STATE_CONFIGURED;
}
void Class_Send_Max_Lun( void)      // bRequest = 0xFE        
{
	switch( usb_running_ctrl_state )
	{
	    case    USB_CTRL_SETUP :
	    {
			if(req_dir == USB_REQ_DIR_IN)
	            usb_running_ctrl_state = USB_CTRL_IN;
			else
	            usb_running_ctrl_state = USB_IDLE;
	        break ;
	    }
	    case    USB_CTRL_IN :
	    {
	
#ifdef USB_EP_STALL_TEST
			{
				write_ahb32(USB_EP_HALT,0x01);
		    	usb_send_epn_stall[0] = TRUE;
			}
#else
			{
				while(usb_read_mreg16(USB_EP0_CTRL)&0x80);//wait last send data finish
				write_ahb32(USB_EP0_FIFO,0);
		    	write_ahb32(USB_EP0_CTRL,0x81);//
	    	}
#endif
	    	usb_running_ctrl_state = USB_IDLE;   //
	        break ;
	    }		
	    default :
	    {
	        usb_running_ctrl_state = USB_IDLE ;
	        break ;
	    }
	} 
}
/*
void Class_BulkOnly_MassStorage_Reset( void)      // bRequest = 0xFF       
{
	EP0_Send_Empty_Packet();
}
*/

void Chap9_GetStatus( void)         // 0x00
{
    switch( usb_running_ctrl_state )
    {
        case    USB_CTRL_SETUP :
        {
			usb_running_ctrl_state = USB_CTRL_IN;
			break;
       	}
		case USB_CTRL_IN:
		{
			if(req_dir == USB_REQ_DIR_IN)
			{
				if(req_recip==USB_REQ_RECIP_EP)
				{		
					char ep = wIndex & 0x0F;
					write_ahb32(USB_EP0_FIFO,usb_ep_stall[ep]);
					write_ahb32(USB_EP0_FIFO,0x00);
				}
				else if(req_recip==USB_REQ_RECIP_DEV)
				{
					write_ahb32(USB_EP0_FIFO,0x02);
					write_ahb32(USB_EP0_FIFO,0x00);
				}
				else
				{
					write_ahb32(USB_EP0_FIFO,0x00);
					write_ahb32(USB_EP0_FIFO,0x00);
				}			
				write_ahb32(USB_EP0_CTRL,0x82);	        	
	        }		
			else
			{

			}
			usb_running_ctrl_state = USB_IDLE;   //======
			break;				
	    }
        default :
        {
            usb_running_ctrl_state = USB_IDLE ;
            break ;
        }
	}
}

void Chap9_ClearFeature( void)      // 0x01
{
	switch(usb_running_ctrl_state)
	{
        case    USB_CTRL_SETUP :
        {
			////USB_PRINTF_L0("ClearFeature\r\n");
			if(req_dir == USB_REQ_DIR_OUT)
			{
				unsigned char ix;
				if((req_recip==USB_REQ_RECIP_EP)&&(wValue==ENDPOINT_HALT))//ENDPOINT_HALT
				{
					write_ahb32(USB_EP_HALT,0x00); //clear epn halt
					stall_clear_flag[EPIN]=1;//2012.04.19 by P
		            //USB_PRINTF_L0( "Clear EP stall 0x%02X\r\n",usb_read_mreg16(USB_EP_HALT));
//					EP0_Send_Empty_Packet();
					/*for(ix = 0; ix < 5; ix++)
					{
						usb_ep_stall[ix] = FALSE;
						usb_send_epn_stall[ix] = FALSE;
						stall_clear_flag[ix]==TRUE;
						//usb_ep_in_stall[ix] = FALSE;
						//usb_ep_out_stall[ix] = FALSE;		
					}*/
				}
				else if((req_recip==USB_REQ_RECIP_DEV)&&(wValue==DEVICE_REMOTE_WAKEUP))//DEVICE_REMOTE_WAKEUP
				{
				
				
				}					
			}
			else
			{

			}
			usb_running_ctrl_state = USB_CTRL_IN;
			break;
		}
	case USB_CTRL_IN: //status stage
		{
			EP0_Send_Empty_Packet();
        }
	default:
		{
			usb_running_ctrl_state = USB_IDLE;
			break;
		}
	}
}
void MLsup_StallEP0( void)          // reserve      0x02
{
	if(req_dir == USB_REQ_DIR_IN)
	{
		EP0_Send_Empty_Packet();
	}
	else
	{

	}
}
void MLsup_StallEP1( void)          // reserve      0x04
{
	if(req_dir == USB_REQ_DIR_IN)
	{
		EP0_Send_Empty_Packet();
	}
	else
	{

	}
}
void Chap9_SetFeature( void)	    // 0x03
{
	if(req_dir == USB_REQ_DIR_OUT)
	{
		EP0_Send_Empty_Packet();
	}
	else
	{

	}
}

void Chap9_SetDescriptor( void)     //0x07
{
	if(req_dir == USB_REQ_DIR_OUT)
	{
		EP0_Send_Empty_Packet();
	}
	else
	{

	}
}
void Chap9_GetConfiguration( void)	//0x08
{
	if(req_dir == USB_REQ_DIR_IN)
	{
		EP0_Send_Empty_Packet();
	}
	else
	{

	}
}

void Chap9_GetInterface( void)		//0x0a
{
	if(req_dir == USB_REQ_DIR_IN)
	{
		EP0_Send_Empty_Packet();
	}
	else
	{

	}
}
void Chap9_SetInterface( void)		//0x0b
{
	if(req_dir == USB_REQ_DIR_OUT)
	{
		EP0_Send_Empty_Packet();
	}
	else
	{

	}
}


void EP0_Send_Empty_Packet()
{
    switch( usb_running_ctrl_state )
    {
        case    USB_CTRL_SETUP :
        {	
            usb_running_ctrl_state = USB_CTRL_IN;
            break ;//P.liu comment it
        }
        case    USB_CTRL_IN :
        {	
			while(usb_read_mreg16(USB_EP0_CTRL)&0x80);//wait last send data finish
        	write_ahb32(USB_EP0_CTRL,0x80);//send empty packet
        	usb_running_ctrl_state = USB_IDLE;   //======
        	////write_ahb32(EP0_INT_EN,EP_SETUP|EP_FINISH|EP_IN_NACK|EP_OUT_ACK|EP_STALL);//2012.04.24 reused by P
        	//2012.05.07 by P, need to do this bellow after all in-nack handler 
        	////usb_in_flag = FALSE;//2012.05.03 by P
        	////write_ahb32(EP0_INT_EN,EP_SETUP|EP_IN_NACK|EP_OUT_ACK|EP_STALL);//2012.04.24 reused by P//2012.05.03 by P
            break ;
        }		
        default :
        {
            usb_running_ctrl_state = USB_IDLE ;
            break ;
        }
    } 
}
#endif //USB_TEST_EN
