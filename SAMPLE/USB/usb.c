//********************************************************************
//(Copyright 2012) usb Inc. All Rights Reserved
//Company confidential and Properietary information.
//This information may not be disclosed to unauthorized individual.
//********************************************************************
//
// P Microelectronics Co.,Ltd.
//
// File Name: usb.c
// Author:    
// Version:   
// Created:   
// Modified:
// Description:
//********************************************************************
#include "../CM3/MCUF103.h"
#include "usb.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
unsigned int usb_val;
unsigned int int_state;
unsigned int ep_int_state;
unsigned int ep0_int_state;
unsigned int usb_setupcnt;
void usb_init(void)
{
	  int value;
	  ep0_int_state = 0;
	  ep_int_state = 0;	
	  int_state = 0;
	  usb_setupcnt = 0;
//	  write_ahb32(0x4001ac18, 0x287);

      write_ahb32(USB_TOP,0x08);// reset ep and fifo
	  write_ahb32(USB_EP_EN,0x00);
      write_ahb32(USB_TOP,0x80);//  active bus and release ep fifo 	
      write_ahb32(USB_TOP,0x82);//  active bus and release ep fifo 	
	  write_ahb32(USB_INT_EN,0x40);//interrupt mask
	  write_ahb32(USB_EP0_CTRL,0x80);//release bus
	
	  usb_val = read_ahb32(USB_INT_STATE); //bit 3 for sof received
	  while(usb_val == 0x0)
	  {
	  	usb_val = read_ahb32(USB_INT_STATE); 
      }
		if(usb_val && 0x1) //bus reset received
		{
				  int_state |= 0x1;
    }
		else if (usb_val && 0x2) //bus suspend received
		{
				  int_state |= 0x2;
    }
		else if (usb_val && 0x4) //bus resume received
		{
				  int_state |= 0x4;
    }
		else if (usb_val && 0x8) //sof received
		{
				  int_state |= 0x8;}
		else if (usb_val && 0x10) //ep interrupt received
		{
				  int_state |= 0x10;}
			
	  usb_val = read_ahb32(USB_EP_INT_STATE); //bit 0 for ep
	  if(usb_val && 0x1) // ep int received
		{
				  ep_int_state |= 0x1;}
	  usb_val = read_ahb32(USB_EP0_INT_STATE);
	  while(usb_val == 0x0)
		{
			usb_val = read_ahb32(USB_EP0_INT_STATE); 
    }

		if(usb_val && 0x1) //setup packet received
		{
				  ep0_int_state |= 0x1;}
		else if (usb_val && 0x2) //status stage finished received
		{
				  ep0_int_state |= 0x2;}
		else if (usb_val && 0x4) //in nack received
		{
				  ep0_int_state |= 0x4;}
		else if (usb_val && 0x8) //in ack received
		{
				  ep0_int_state |= 0x8;}
		else if (usb_val && 0x10) //in stall received
		{
				  ep0_int_state |= 0x10;}
		else if (usb_val && 0x20) //out nack received
		{
				  ep0_int_state |= 0x20;}
		else if (usb_val && 0x40) //out ack received
		{
				  ep0_int_state |= 0x40;}
		else if (usb_val && 0x80) //out stall received
		{
				  ep0_int_state |= 0x80;}
	  usb_val = read_ahb32(USB_ADDR);
	//SETUP0-7
}
void usb_tx(u32 tx_num, u32 * tx_buf)
{

}
void usb_rx(u32 *rx_buf)
{

}
void usb_test(void)
{

}
