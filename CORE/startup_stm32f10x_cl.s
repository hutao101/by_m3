;******************** (C) COPYRIGHT 2009 STMicroelectronics ********************
;* File Name          : startup_stm32f10x_cl.s
;* Author             : MCD Application Team
;* Version            : V3.1.2
;* Date               : 09/28/2009
;* Description        : STM32F10x Connectivity line devices vector table for RVMDK 
;*                      toolchain. 
;*                      This module performs:
;*                      - Set the initial SP
;*                      - Set the initial PC == Reset_Handler
;*                      - Set the vector table entries with the exceptions ISR address
;*                      - Branches to __main in the C library (which eventually
;*                        calls main()).
;*                      After Reset the CortexM3 processor is in Thread mode,
;*                      priority is Privileged, and the Stack is set to Main.
;* <<< Use Configuration Wizard in Context Menu >>>   
;*******************************************************************************
; THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
; WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
; AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
; INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
; CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
; INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
;*******************************************************************************

; Amount of memory (in bytes) allocated for Stack
; Tailor this value to your application needs
; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Stack_Size      EQU     0x00000200

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp	EQU     0x20000600


; <h> Heap Configuration
; <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size       EQU     0x00000200

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit

                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset
                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size
				IMPORT main
__Vectors       DCD     __initial_sp              ; Top of Stack
                DCD     Reset_Handler             ; Reset Handler
                DCD     NMI_Handler               ; NMI Handler
                DCD     HardFault_Handler         ; Hard Fault Handler
                DCD     MemManage_Handler         ; MPU Fault Handler
                DCD     BusFault_Handler          ; Bus Fault Handler
                DCD     UsageFault_Handler        ; Usage Fault Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     SVC_Handler               ; SVCall Handler
                DCD     DebugMon_Handler          ; Debug Monitor Handler
                DCD     0                         ; Reserved
                DCD     PendSV_Handler            ; PendSV Handler
                DCD     SysTick_Handler           ; SysTick Handler

                ; External Interrupts
                DCD     WWDG_IRQHandler            ; Window Watchdog //0
                DCD     DMA_IRQHandler             ; DMA_init		 //1
                DCD     USB_MC_IRQHandler          ; USB_init		 //2
				DCD     EFC_IRQHandler             ; EFC_init        //3
	            DCD     SPI1M_IRQHandler           ; SPI1_M          //4
                DCD     SPI2M_IRQHandler           ; SPI2_M		     //5
				DCD     TIM1_IRQHandler            ; TIM1            //6
                DCD     EXTI1_IRQHandler           ; EXTI Line 1     //7
                DCD     EXTI2_IRQHandler           ; EXTI Line 2     //8
                DCD     EXTI3_IRQHandler           ; EXTI Line 3     //9
                DCD     EXTI4_IRQHandler           ; EXTI Line 4     //10
	            DCD     USART1_IRQHandler          ; USART1	         //11
                DCD     USART2_IRQHandler          ; USART2          //12
                DCD     USART3_IRQHandler          ; USART3			 //13
                DCD     I2C1_IRQHandler            ; I2C1		     //14
                DCD     I2C2_IRQHandler            ; I2C2			 //15
				DCD     TIM2_IRQHandler            ; TIM1            //16
				DCD     TIM3_IRQHandler            ; TIM1            //17
				DCD     TIM4_IRQHandler            ; TIM1            //18
			    DCD     RTC_IRQHandler             ; RTC             //19
				DCD     CAN_IRQHandler             ; CAN		   	 //20
				DCD     ADC1_IRQHandler            ; CAN		   	 //21
				DCD     ADC2_IRQHandler            ; CAN		   	 //22
				DCD     DAC1_IRQHandler            ; CAN		   	 //23
				DCD     DAC2_IRQHandler            ; CAN		   	 //24
	            DCD     SPI1S_IRQHandler           ; SPI1_M          //25
                DCD     SPI2S_IRQHandler           ; SPI2_M		     //26
                DCD     EXTI5_IRQHandler           ; EXTI Line 4     //27
				DCD     VDT_IRQHandler             ; EXTI Line 4     //28
				DCD     TAMP_IRQHandler            ; EXTI Line 4     //29
				DCD     TIM1_UD_IRQHandler         ; EXTI Line 4     //30
                DCD     TIM1_CC1_IRQHandler        ; EXTI Line 4     //31
				DCD     TIM1_CC2_IRQHandler        ; EXTI Line 4     //32
				DCD     TIM1_CC3_IRQHandler        ; EXTI Line 4     //33
				DCD     TIM1_CC4_IRQHandler        ; EXTI Line 4     //34
				DCD     TIM1_COM_IRQHandler        ; EXTI Line 4     //35
				DCD     TIM1_TRG_IRQHandler        ; EXTI Line 4     //36
				DCD     TIM1_BK_IRQHandler         ; EXTI Line 4     //37
                DCD     TIM1_CC1OF_IRQHandler      ; EXTI Line 4     //38
				DCD     TIM1_CC2OF_IRQHandler      ; EXTI Line 4     //39
				DCD     TIM1_CC3OF_IRQHandler      ; EXTI Line 4     //40
				DCD     TIM1_CC4OF_IRQHandler      ; EXTI Line 4     //41			
__Vectors_End

__Vectors_Size  EQU  __Vectors_End - __Vectors

                AREA    |.text|, CODE, READONLY

; Reset handler routine
Reset_Handler    PROC
                 EXPORT  Reset_Handler             [WEAK]
        IMPORT  __main
                 LDR     R0, = __main
				 ;LDR     R0, = main
                 BX      R0
                 ENDP

; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC
                EXPORT  NMI_Handler                [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler          [WEAK]
                B       .
                ENDP
MemManage_Handler\
                PROC
                EXPORT  MemManage_Handler          [WEAK]
                B       .
                ENDP
BusFault_Handler\
                PROC
                EXPORT  BusFault_Handler           [WEAK]
                B       .
                ENDP
UsageFault_Handler\
                PROC
                EXPORT  UsageFault_Handler         [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler                [WEAK]
                B       .
                ENDP
DebugMon_Handler\
                PROC
                EXPORT  DebugMon_Handler           [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler             [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler            [WEAK]
                B       .
                ENDP

Default_Handler PROC	
                EXPORT     WWDG_IRQHandler         [WEAK]   ; Window Watchdog //0
                EXPORT     DMA_IRQHandler          [WEAK]   ; DMA_init		 //1
                EXPORT     USB_MC_IRQHandler       [WEAK]   ; USB_init		 //2
				EXPORT     EFC_IRQHandler          [WEAK]  ; EFC_init        //3
	            EXPORT     SPI1M_IRQHandler        [WEAK]  ; SPI1_M          //4
                EXPORT     SPI2M_IRQHandler        [WEAK]  ; SPI2_M		     //5
				EXPORT     TIM1_IRQHandler         [WEAK] ; TIM1            //6
                EXPORT     EXTI1_IRQHandler        [WEAK]   ; EXTI Line 1     //7
                EXPORT     EXTI2_IRQHandler        [WEAK]   ; EXTI Line 2     //8
                EXPORT     EXTI3_IRQHandler        [WEAK]   ; EXTI Line 3     //9
                EXPORT     EXTI4_IRQHandler        [WEAK]   ; EXTI Line 4     //10
	            EXPORT     USART1_IRQHandler       [WEAK]   ; USART1	         //11
                EXPORT     USART2_IRQHandler       [WEAK]   ; USART2          //12
                EXPORT     USART3_IRQHandler       [WEAK]  ; USART3			 //13
                EXPORT     I2C1_IRQHandler         [WEAK]   ; I2C1		     //14
                EXPORT     I2C2_IRQHandler         [WEAK]   ; I2C2			 //15
				EXPORT     TIM2_IRQHandler         [WEAK]   ; TIM1            //16
				EXPORT     TIM3_IRQHandler         [WEAK]   ; TIM1            //17
				EXPORT     TIM4_IRQHandler         [WEAK]   ; TIM1            //18
			    EXPORT     RTC_IRQHandler          [WEAK]   ; RTC             //19
				EXPORT     CAN_IRQHandler          [WEAK]   ; CAN		   	 //20
				EXPORT     ADC1_IRQHandler         [WEAK]   ; CAN		   	 //21
				EXPORT     ADC2_IRQHandler         [WEAK]   ; CAN		   	 //22
				EXPORT     DAC1_IRQHandler         [WEAK]   ; CAN		   	 //23
				EXPORT     DAC2_IRQHandler         [WEAK]    ; CAN		   	 //24
	            EXPORT     SPI1S_IRQHandler        [WEAK]   ; SPI1_M          //25
                EXPORT     SPI2S_IRQHandler        [WEAK]   ; SPI2_M		     //26
                EXPORT     EXTI5_IRQHandler        [WEAK]   ; EXTI Line 4     //27
				EXPORT     VDT_IRQHandler          [WEAK]   ; EXTI Line 4     //28
				EXPORT     TAMP_IRQHandler         [WEAK]   ; EXTI Line 4     //29
				EXPORT     TIM1_UD_IRQHandler      [WEAK]   ; EXTI Line 4     //30
                EXPORT     TIM1_CC1_IRQHandler     [WEAK]   ; EXTI Line 4     //31
				EXPORT     TIM1_CC2_IRQHandler     [WEAK]   ; EXTI Line 4     //32
				EXPORT     TIM1_CC3_IRQHandler     [WEAK]   ; EXTI Line 4     //33
				EXPORT     TIM1_CC4_IRQHandler     [WEAK]   ; EXTI Line 4     //34
				EXPORT     TIM1_COM_IRQHandler     [WEAK]   ; EXTI Line 4     //35
				EXPORT     TIM1_TRG_IRQHandler     [WEAK]   ; EXTI Line 4     //36
				EXPORT     TIM1_BK_IRQHandler      [WEAK]   ; EXTI Line 4     //37
                EXPORT     TIM1_CC1OF_IRQHandler   [WEAK]   ; EXTI Line 4     //38
				EXPORT     TIM1_CC2OF_IRQHandler   [WEAK]   ; EXTI Line 4     //39
				EXPORT     TIM1_CC3OF_IRQHandler   [WEAK]   ; EXTI Line 4     //40
				EXPORT     TIM1_CC4OF_IRQHandler   [WEAK]   ; EXTI Line 4     //41					

WWDG_IRQHandler      
DMA_IRQHandler        
USB_MC_IRQHandler       
EFC_IRQHandler         
SPI1M_IRQHandler       
SPI2M_IRQHandler       
TIM1_IRQHandler        
EXTI1_IRQHandler
EXTI2_IRQHandler       
EXTI3_IRQHandler  
EXTI4_IRQHandler    
USART1_IRQHandler     
USART2_IRQHandler      
USART3_IRQHandler     
I2C1_IRQHandler    
I2C2_IRQHandler      
TIM2_IRQHandler       
TIM3_IRQHandler   
TIM4_IRQHandler    
RTC_IRQHandler        
CAN_IRQHandler      
ADC1_IRQHandler 
ADC2_IRQHandler   
DAC1_IRQHandler 
DAC2_IRQHandler 
SPI1S_IRQHandler  
SPI2S_IRQHandler 
EXTI5_IRQHandler   
VDT_IRQHandler   
TAMP_IRQHandler     
TIM1_UD_IRQHandler    
TIM1_CC1_IRQHandler   
TIM1_CC2_IRQHandler   
TIM1_CC3_IRQHandler    
TIM1_CC4_IRQHandler     
TIM1_COM_IRQHandler
TIM1_TRG_IRQHandler 
TIM1_BK_IRQHandler  
TIM1_CC1OF_IRQHandler  
TIM1_CC2OF_IRQHandler   
TIM1_CC3OF_IRQHandler  
TIM1_CC4OF_IRQHandler  	   

                B       .

                ENDP

                ALIGN

;*******************************************************************************
; User Stack and Heap initialization
;*******************************************************************************
                 IF      :DEF:__MICROLIB
                
                 EXPORT  __initial_sp
                 EXPORT  __heap_base
                 EXPORT  __heap_limit
                
                 ELSE
                
                 IMPORT  __use_two_region_memory
                 EXPORT  __user_initial_stackheap
                 
__user_initial_stackheap PROC
                 LDR     R0, =  Heap_Mem
                 LDR     R1, =(Stack_Mem + Stack_Size)
                 LDR     R2, = (Heap_Mem +  Heap_Size)
                 LDR     R3, = Stack_Mem
                 BX      LR
                 ENDP
                 ALIGN

                 ENDIF

                 END

;******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE*****
