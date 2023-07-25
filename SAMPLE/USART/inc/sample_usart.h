#ifndef __SAMPLE_USART_H
#define __SAMPLE_USART_H
#include "stdio.h"	
#include "sys.h" 
#include "sample_usart_conf.h" 


//USART相关宏定义
#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收

#define USART_BUAD_COUNT  		19  //定义最大接收字节数 200
#define USART_PRT_COUNT  		3  	//定义最大接收字节数 200
#define USART_STPB_COUNT 	    2  	//定义最大接收字节数 200
#define USART_WL_COUNT 		    2  	//定义最大接收字节数 200


extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         		//接收状态标记	
extern u32 Baudrate[USART_BUAD_COUNT];
extern u16 Prt[USART_PRT_COUNT];
extern u16 Stpb[USART_STPB_COUNT];
extern u16 WL[USART_WL_COUNT];



//USART枚举类型

//UART波特率枚举
typedef enum
{
    eUSART_BAUD_1200     = 1200, 
    eUSART_BAUD_2400     = 2400,
    eUSART_BAUD_4800     = 4800,
    eUSART_BAUD_9600     = 9600,
    eUSART_BAUD_14400    = 14400,
    eUSART_BAUD_19200    = 19200,
    eUSART_BAUD_38400    = 38400,
    eUSART_BAUD_43000    = 43000 ,
    eUSART_BAUD_57600    = 57600,
    eUSART_BAUD_76800    = 76800,
    eUSART_BAUD_115200   = 115200,
    eUSART_BAUD_128000   = 128000,
    eUSART_BAUD_230400   = 230400,
    eUSART_BAUD_256000   = 256000,
    eUSART_BAUD_460800   = 460800,
    eUSART_BAUD_921600   = 921600 ,
    eUSART_BAUD_1M       = 1000000,
    eUSART_BAUD_2M25  	 = 2250000,
    eUSART_BAUD_4M5      = 4500000
} T_eUART_BAUDRATE;

//UART配置参数枚举
typedef enum
{
    eUSART_BAUD     = 0,
    eUSART_WORDLEN  = 1,   
    eUSART_STPB     = 2,
    eUSART_PRT		= 3, 
    eUSART_MODE     = 4, 
    eUSART_FLOW     = 5,
} T_eUART_PARAM;



//USART结构体类型


//对APP层开发接口

/*********************************UART功能函数*********************************/

/*******************************************************************************
 * @brief   sample_uart_init
 * Function UART初始化接口
 * @author  taohu
 * @date    2022-1-6
 * @param   
		USARTx:USART基地址
 * @return void
*******************************************************************************/
void sample_uart_init(USART_TypeDef* USARTx);


/*******************************************************************************
 * @brief   sample_uart_deinit
 * Function UART deinit接口
 * @author  taohu
 * @date    2022-1-6
 * @param   
		USARTx:USART基地址
 * @return void
*******************************************************************************/
void sample_uart_deinit(USART_TypeDef* USARTx);


/*******************************************************************************
 * @brief   sample_uart_nvic_cfg
 * Function 中断使能配置
 * @author  taohu
 * @date    2022-1-6
 * @param   
		USARTx:USART基地址
 * @return void
*******************************************************************************/
void sample_uart_nvic_cfg(u32 USARTx, FunctionalState state);



/*******************************************************************************
 * @brief   sample_uart_set_param
 * Function 修改配置参数接口
 * @author  taohu
 * @date    2022-1-6
 * @param   
		type:修改参数类型
		param：参数值
 * @return void
*******************************************************************************/
void sample_uart_set_param(T_eUART_PARAM type,u32 param);


/*******************************************************************************
 * @brief   sample_uart_set_buad
 * Function 读取配置参数接口
 * @author  taohu
 * @date    2022-1-6
 * @param   
		type:读取参数类型
 * @return void
*******************************************************************************/
u32 sample_uart_read_param(T_eUART_PARAM type);

/*******************************************************************************
 * @brief   sample_uart_chk_data
 * Function 对比发送的数据和接收的数据
 * @author  taohu
 * @date    2022-2-23
 * @param   
		send_data:发送的数据
		recv_data：接收的数据
 * @return ErrorStatus
 		   SUCCESS:对比成功
 		   ERROR：对比失败
 			
*******************************************************************************/
ErrorStatus sample_uart_chk_data(u16 send_data, u16 recv_data);

//test case
void sample_uart_t1(USART_TypeDef* USARTx);
void sample_uart_t2(USART_TypeDef* USARTx);
void sample_uart_t3(USART_TypeDef* USARTx);
void sample_uart_t4(USART_TypeDef* USARTx);
void sample_uart_t5(USART_TypeDef* USARTx);
void sample_uart_t6(USART_TypeDef* USARTx);
void sample_uart_t7(USART_TypeDef* USARTx);
void sample_uart_t8(USART_TypeDef* USARTx);
void sample_uart_t9(USART_TypeDef* USARTx);
void sample_uart_t10(USART_TypeDef* USARTx);
void sample_uart_t11(USART_TypeDef* USARTx);
void sample_uart_t12(USART_TypeDef* USARTx);
void sample_uart_t13(USART_TypeDef* USARTx);
void sample_uart_t14(USART_TypeDef* USARTx);
void sample_uart_t15(USART_TypeDef* USARTx);
void sample_uart_t16(USART_TypeDef* USARTx);
void sample_uart_t17(USART_TypeDef* USARTx);
void sample_uart_t18(USART_TypeDef* USARTx);
void sample_uart_t19(USART_TypeDef* USARTx);
void sample_uart_t20(USART_TypeDef* USARTx);
void sample_usart_t21(USART_TypeDef* USARTx);
void sample_usart_t22(USART_TypeDef* USARTx);
void sample_uart_t23(USART_TypeDef* USARTx);
void sample_uart_t24(USART_TypeDef* USARTx);
void sample_uart_t25(USART_TypeDef* USARTx);
void sample_uart_t26(USART_TypeDef* USARTx);
void sample_uart_t27(USART_TypeDef* USARTx);
void sample_uart_t28(USART_TypeDef* USARTx);

#endif




