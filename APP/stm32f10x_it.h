/**
  ******************************************************************************
  * @file    GPIO/IOToggle/stm32f10x_it.h 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   This file contains the headers of the interrupt handlers.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F10x_IT_H
#define __STM32F10x_IT_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

struct T_IRQ_CBFUNC
{
	void (*wwdg)(void);
	void (*dma)(void);
    void (*usb)(void);
	void (*efc)(void);
	void (*spi1_m)(void);	
	void (*spi2_m)(void);	
	void (*tim1)(void);
	void (*exti1)(void);	
	void (*exti2)(void);
	void (*exti3)(void);	
	void (*exti4)(void);	
	void (*usart1)(void);	
	void (*usart2)(void);	
	void (*usart3)(void);
	void (*i2c1)(void);	
	void (*i2c2)(void);	
	void (*tim2)(void);	
	void (*tim3)(void);	
	void (*tim4)(void);	
	void (*rtc)(void);	
	void (*can)(void);	
	void (*adc1)(void);
	void (*adc2)(void);
	void (*dac1)(void);
	void (*dac2)(void);
	void (*spi1_s)(void);
	void (*spi2_s)(void);
	void (*exti5)(void);
	void (*pvd)(void);
	void (*tamper)(void);
	void (*tim1_ud)(void);
	void (*tim1_cc1)(void);
	void (*tim1_cc2)(void);
	void (*tim1_cc3)(void);
	void (*tim1_cc4)(void);
	void (*tim1_com)(void);
	void (*tim1_trg)(void);
	void (*tim1_bk)(void);
	void (*tim1_cc1of)(void);
	void (*tim1_cc2of)(void);
	void (*tim1_cc3of)(void);
	void (*tim1_cc4of)(void);
};

extern struct T_IRQ_CBFUNC irq_cb;


void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);

#endif /* __STM32F10x_IT_H */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
