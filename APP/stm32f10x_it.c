/**
  ******************************************************************************
  * @file    GPIO/IOToggle/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h" 
#include "usart.h"	

struct T_IRQ_CBFUNC irq_cb = {0};

 
void NMI_Handler(void)
{
}
 
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}
 
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

 
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}
 
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}
 
void SVC_Handler(void)
{
}
 
void DebugMon_Handler(void)
{
}
 
void PendSV_Handler(void)
{
}
 
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
void WWDG_IRQHandler(void)
{
    if(irq_cb.wwdg == 0)
		printf("error:wwdg irq is null");
	else
		irq_cb.wwdg();
}

void DMA_IRQHandler(void)
{
	if(irq_cb.dma == 0)
		printf("error:dma irq is null");
	else
		irq_cb.dma();
}

void USB_MC_IRQHandler(void)
{
	if(irq_cb.usb == 0)
		printf("error:usb irq is null");
	else
		irq_cb.usb();
}

void EFC_IRQHandler(void)
{
	if(irq_cb.efc == 0)
		printf("error:efc irq is null");
	else
		irq_cb.efc();
}

void SPI1M_IRQHandler(void)
{
	if(irq_cb.spi1_m == 0)
		printf("error:spi1_m irq is null");
	else
		irq_cb.spi1_m();
}

void SPI2M_IRQHandler(void)
{
	if(irq_cb.spi2_m == 0)
		printf("error:spi2_m irq is null");
	else
		irq_cb.spi2_m();
}

void TIM1_IRQHandler(void)
{
	if(irq_cb.tim1 == 0)
		printf("error:tim1 irq is null");
	else
		irq_cb.tim1();
}

void EXTI1_IRQHandler(void)
{
	if(irq_cb.exti1 == 0)
		printf("error:exti1 irq is null");
	else
		irq_cb.exti1();
}

void EXTI2_IRQHandler(void)
{
	if(irq_cb.exti2 == 0)
		printf("error:exti2 irq is null");
	else
		irq_cb.exti2();
}

void EXTI3_IRQHandler(void)
{
	if(irq_cb.exti3 == 0)
		printf("error:exti3 irq is null");
	else
		irq_cb.exti3();
}

void EXTI4_IRQHandler(void)
{
	if(irq_cb.exti4 == 0)
		printf("error:exti4 irq is null");
	else
		irq_cb.exti4();
}

//void USART1_IRQHandler(void)
//{

//}

//void USART2_IRQHandler(void)
//{

//}

//void USART3_IRQHandler(void)
//{

//}

void I2C1_IRQHandler(void)
{
	if(irq_cb.i2c1 == 0)
		printf("error:i2c1 irq is null");
	else
		irq_cb.i2c1();
}

void I2C2_IRQHandler(void)
{
	if(irq_cb.i2c2 == 0)
		printf("error:i2c2 irq is null");
	else
		irq_cb.i2c2();
}

void TIM2_IRQHandler(void)
{
	if(irq_cb.tim2 == 0)
		printf("error:tim2 irq is null");
	else
		irq_cb.tim2();
}

void TIM3_IRQHandler(void)
{
	if(irq_cb.tim3 == 0)
		printf("error:tim3 irq is null");
	else
		irq_cb.tim3();
}

//void TIM4_IRQHandler(void)
//{
//	irq_cb.tim4();
//}

void RTC_IRQHandler(void)
{
	if(irq_cb.rtc == 0)
		printf("error:rtc irq is null");
	else
		irq_cb.rtc();
}

void CAN_IRQHandler(void)
{
	if(irq_cb.can == 0)
		printf("error:can1_sce irq is null");
	else
		irq_cb.can();
}

void ADC1_IRQHandler(void)
{
	if(irq_cb.adc1 == 0)
		printf("error:adc1 irq is null");
	else
		irq_cb.adc1();
}

void ADC2_IRQHandler(void)
{
	if(irq_cb.adc2 == 0)
		printf("error:adc2 irq is null");
	else
		irq_cb.adc2();
}

void DAC1_IRQHandler(void)
{
	if(irq_cb.dac1 == 0)
		printf("error:dac1 irq is null");
	else
		irq_cb.dac1();
}

void DAC2_IRQHandler(void)
{
	if(irq_cb.dac2 == 0)
		printf("error:dac2 irq is null");
	else
		irq_cb.dac2();
}

void SPI1S_IRQHandler(void)
{
	if(irq_cb.spi1_s == 0)
		printf("error:spi1_s irq is null");
	else
		irq_cb.spi1_s();
}

void SPI2S_IRQHandler(void)
{
	if(irq_cb.spi2_m == 0)
		printf("error:spi2_s irq is null");
	else
		irq_cb.spi2_s();
}

void EXTI5_IRQHandler(void)
{
	if(irq_cb.exti5 == 0)
		printf("error:exti5 irq is null");
	else
		irq_cb.exti5();
}

void PVD_IRQHandler(void)
{
    if(irq_cb.pvd == 0)
		printf("error:pvd irq is null");
	else
		irq_cb.pvd();
}

void TAMPER_IRQHandler(void)
{
    if(irq_cb.tamper == 0)
		printf("error:tamper irq is null");
	else
		irq_cb.tamper();
}

void TIM1_UD_IRQHandler(void)
{
	if(irq_cb.tim1_ud == 0)
		printf("error:tim1_ud irq is null\r\n");
	else
		irq_cb.tim1_ud();
}

void TIM1_CC1_IRQHandler(void)
{
	if(irq_cb.tim1_cc1 == 0)
		printf("error:tim1_cc1 irq is null");
	else
		irq_cb.tim1_cc1();
}

void TIM1_CC2_IRQHandler(void)
{
	if(irq_cb.tim1_cc2 == 0)
		printf("error:tim1_cc2 irq is null");
	else
		irq_cb.tim1_cc2();
}

void TIM1_CC3_IRQHandler(void)
{
	if(irq_cb.tim1_cc3 == 0)
		printf("error:tim1_cc3 irq is null");
	else
		irq_cb.tim1_cc3();
}

void TIM1_CC4_IRQHandler(void)
{
	if(irq_cb.tim1_cc4 == 0)
		printf("error:tim1_cc4 irq is null");
	else
		irq_cb.tim1_cc4();
}

void TIM1_COM_IRQHandler(void)
{
	if(irq_cb.tim1_com == 0)
		printf("error:tim1_com irq is null");
	else
		irq_cb.tim1_com();
}

void TIM1_TRG_IRQHandler(void)
{
	if(irq_cb.tim1_trg == 0)
		printf("error:tim1_trg irq is null");
	else
		irq_cb.tim1_trg();
}

void TIM1_BK_IRQHandler(void)
{
	if(irq_cb.tim1_bk == 0)
		printf("error:tim1_bk irq is null");
	else
		irq_cb.tim1_bk();
}

void TIM1_CC1OF_IRQHandler(void)
{
	if(irq_cb.tim1_cc1of == 0)
		printf("error:tim1_cc1of irq is null");
	else
		irq_cb.tim1_cc1of();
}

void TIM1_CC2OF_IRQHandler(void)
{
	if(irq_cb.tim1_cc2of == 0)
		printf("error:tim1_cc2of irq is null");
	else
		irq_cb.tim1_cc2of();
}

void TIM1_CC3OF_IRQHandler(void)
{
	if(irq_cb.tim1_cc3of == 0)
		printf("error:tim1_cc3of irq is null");
	else
		irq_cb.tim1_cc3of();
}

void TIM1_CC4OF_IRQHandler(void)
{
	if(irq_cb.tim1_cc4of == 0)
		printf("error:tim1_cc4of irq is null");
	else
		irq_cb.tim1_cc4of();
}


