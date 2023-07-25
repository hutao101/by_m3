#include "sys.h"
#include "delay.h"
#include "sample_usart.h"
#include "stm32f10x.h"
  

/****************************************宏定义***************************************/
//C文件内使用宏定义



/****************************************枚举***************************************/
//C文件内使用枚举



/****************************************结构体***************************************/
//C文件内使用结构体


#define read_ahb32(mreg)   *(volatile unsigned int *)(mreg)




/**********************************************************************************/

u32 Baudrate[USART_BUAD_COUNT] = {1200,2400,4800,9600,14400,19200,38400,43000,57600,76800,115200,128000,230400,256000,
								  460800,921600,1000000,2250000,4500000};
u16 Prt[USART_PRT_COUNT] = {USART_Parity_No,USART_Parity_Even,USART_Parity_Odd};
u16 Stpb[USART_STPB_COUNT] = {USART_StopBits_1,USART_StopBits_2};
u16 WL[USART_WL_COUNT] = {USART_WordLength_8b,USART_WordLength_9b};


//初始化配置参数,修改此结构体赋值，可以改变初始化参数
USART_InitTypeDef USART_InitStructure = 
								{
									eUSART_BAUD_9600,		//波特率
									USART_WordLength_8b,	//字长
		                            USART_StopBits_1,		//停止位
		                            USART_Parity_No,    	//校验位
		                            USART_Mode_Rx | USART_Mode_Tx,	//通信模式
		                            USART_HardwareFlowControl_None	//硬件流控
								};
								
/**********************************************************************************/


#if 0
/*******************************************************************************
 * @brief   uart_gpio_cfg
 * Function gpio配置函数
 * @author  taohu
 * @date    2022-1-6
 * @param   
		tx_port:tx对应GPIO端口
		rx_port:rx对应GPIO端口
 		tx_pin:tx对应GPIO引脚
 		rx_pin:rx对应GPIO引脚
 * @return void
*******************************************************************************/
void uart_gpio_cfg(GPIO_TypeDef * tx_port,GPIO_TypeDef* rx_port,u16 tx_pin,u16 rx_pin)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = tx_pin; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
	GPIO_Init(tx_port, &GPIO_InitStructure);
	
	
	GPIO_InitStructure.GPIO_Pin = rx_pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(rx_port, &GPIO_InitStructure);
}


/*******************************************************************************
 * @brief   uart_gpio_clk_cfg
 * Function gpio和端口时钟配置函数
 * @author  taohu
 * @date    2022-1-6
 * @param   
		USARTx:USART基地址;通过USART基地址配置对应端口和时钟
 * @return void
*******************************************************************************/
void uart_gpio_clk_cfg(u32 USARTx)
{
	
	//u32 temp = (u32)USARTx;
	switch(USARTx)
    {
		case (u32)USART1:
	    {
	    	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟

			//USART1_TX   GPIOA.9
			//USART1_RX   GPIOA.10初始化
			uart_gpio_cfg(GPIOA,GPIOA,GPIO_Pin_9,GPIO_Pin_10);	
	    }
        break;
	    case (u32)USART2:
	    {
	    	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//使能USART2
	    	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//GPIOA时钟
	    	
			//USART2_TX   GPIOA.2
			//USART2_RX   GPIOA.3初始化
			uart_gpio_cfg(GPIOA,GPIOA,GPIO_Pin_2,GPIO_Pin_3);	
	    }
        break;
        case (u32)USART3:
	    {
	    	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//使能USART3
	    	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//GPIOB时钟
	
			//USART3_TX   GPIOB.10
			//USART3_RX   GPIOB.11初始化
			uart_gpio_cfg(GPIOB,GPIOB,GPIO_Pin_10,GPIO_Pin_11);
	    }
        break;
        case (u32)UART4:
	    {
	        RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);	//使能USART4
	    	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	//GPIOC时钟

			//UART4_TX   GPIOC.10
			//UART4_RX   GPIOC.11初始化
			uart_gpio_cfg(GPIOC,GPIOC,GPIO_Pin_10,GPIO_Pin_11);
	    }
        break;
        //case (u32)UART5: //FPGA MAP中 UART4和UART5地址一样
	    //{
	   	//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);	//使能USART5
	    //	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	//GPIOC时钟
	    //	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	//GPIOD时钟
	
			
			//UART5_TX   GPIOC.12
			//UART5_RX   GPIOC.2初始化
		//	uart_gpio_cfg(GPIOC,GPIOD,GPIO_Pin_12,GPIO_Pin_2);
	    //}
        //break;
		default:
			return;
	}
}

#endif

/*******************************************************************************
 * @brief   uart_nvic_cfg
 * Function UART的NVIC配置
 * @author  taohu
 * @date    2022-1-6
 * @param   
		USARTx:USART基地址；通过基地址选择中断通道号
 * @return void
*******************************************************************************/
void sample_uart_nvic_cfg(u32 USARTx, FunctionalState state)
{

	NVIC_InitTypeDef NVIC_InitStructure;

	//拿到USART通道号文档再更新
    switch(USARTx)
    {
		case (u32)USART1:
			NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
			break;
		
		case (u32)USART2:
			NVIC_InitStructure.NVIC_IRQChannel = 12;//USART2_IRQn;
			break;

		case (u32)USART3:
			NVIC_InitStructure.NVIC_IRQChannel = 13;//USART3_IRQn;
			break;
		
       	case (u32)UART4:
			NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
			break;
		
		//case (u32)UART5:    //FPGA MAP中 UART4和UART5地址一样
		//	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
		//	break;
		default:
			break;
	}
	//Usart1 NVIC 配置
	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = state; 		//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure); //根据指定的参数初始化NVIC寄存器
}

/*******************************************************************************
 * @brief   sample_uart_init
 * Function UART初始化接口
 * @author  taohu
 * @date    2022-1-6
 * @param   
		USARTx:USART基地址
 * @return void
*******************************************************************************/
void sample_uart_init(USART_TypeDef* USARTx)
{
	
    //GPIO端口设置
	//uart_gpio_clk_cfg((u32)USARTx);
	
	USART_Init(USARTx, &USART_InitStructure); //初始化串口
	USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);
	USART_Cmd(USARTx, ENABLE);  
	
	//nvic设置
	sample_uart_nvic_cfg((u32)USARTx,ENABLE);
	//使能串口
	printf("Baud rate:%d\r\nStop bit：0x%x\r\n",USART_InitStructure.USART_BaudRate,USART_InitStructure.USART_StopBits);
	printf("Data length:0x%x\r\nParity：0x%x\r\n",USART_InitStructure.USART_WordLength,USART_InitStructure.USART_Parity);
	printf("uart address:0x%x\r\n",USARTx);
}

/*******************************************************************************
 * @brief   sample_uart_deinit
 * Function UART deinit接口
 * @author  taohu
 * @date    2022-1-6
 * @param   
		USARTx:USART基地址
 * @return void
*******************************************************************************/
void sample_uart_deinit(USART_TypeDef* USARTx)
{
	USART_DeInit(USARTx);
}

/*******************************************************************************
 * @brief   sample_uart_set_buad
 * Function 修改配置参数接口
 * @author  taohu
 * @date    2022-1-6
 * @param   
		type:修改参数类型
		param：参数值
 * @return void
*******************************************************************************/
void sample_uart_set_param(T_eUART_PARAM type,u32 param)
{
	switch(type)
	{
		case eUSART_BAUD:
			USART_InitStructure.USART_BaudRate = param;
			break;
		case eUSART_WORDLEN:
			USART_InitStructure.USART_WordLength = param;
  			break;
		case eUSART_STPB:
			USART_InitStructure.USART_StopBits = param;
			break;
		case eUSART_PRT:
			USART_InitStructure.USART_Parity = param;
  			break;
		case eUSART_MODE:
			USART_InitStructure.USART_Mode = param;
			break;
		case eUSART_FLOW:
			USART_InitStructure.USART_HardwareFlowControl = param;	
		default:
			return;
	}
}


/*******************************************************************************
 * @brief   sample_uart_read_buad
 * Function 读取配置参数接口
 * @author  taohu
 * @date    2022-1-6
 * @param   
		type:读取参数类型
 * @return void
*******************************************************************************/
u32 sample_uart_read_param(T_eUART_PARAM type)
{
	switch(type)
	{
		case eUSART_BAUD:
			return USART_InitStructure.USART_BaudRate;
	
		case eUSART_WORDLEN:
			return USART_InitStructure.USART_WordLength;
			
		case eUSART_STPB:
			return USART_InitStructure.USART_StopBits;

		case eUSART_PRT:
			return USART_InitStructure.USART_Parity;

		case eUSART_MODE:
			return USART_InitStructure.USART_Mode;
			
		case eUSART_FLOW:
			return USART_InitStructure.USART_HardwareFlowControl;	

		default:
			return 0;
	}
}

/*******************************************************************************
 * @brief   sample_uart_struct_init
 * Function 设置USART_InitStructure为默认参数
 * @author  taohu
 * @date    2022-1-6
 * @param   
		type:修改参数类型
		param：参数值
 * @return void
*******************************************************************************/
void sample_uart_struct_init(void)
{
	/* USART_InitStruct members default value */
	USART_StructInit(&USART_InitStructure);
}

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
ErrorStatus sample_uart_chk_data(u16 send_data, u16 recv_data)
{
	u8 i,cnt = 0;
	u16 temp;
	//如果是8位数据结构，清除第9位
	if(USART_InitStructure.USART_WordLength == USART_WordLength_8b)
		send_data &= 0x00FF;

	temp = send_data;
	
	switch(USART_InitStructure.USART_Parity)
	{
		case USART_Parity_No:
		{
			
		}
		break;
		case USART_Parity_Even:
		{
			for (i=0; i<(USART_InitStructure.USART_WordLength/0x1000 + 7); i++)
			{
				if (temp & 0x01)
					cnt++;

				temp >>= 1;
			}
			
			if(cnt % 2 == 0)
				send_data &= ~(1 << (USART_InitStructure.USART_WordLength/0x1000 + 7));
			else
				send_data |= 1 << (USART_InitStructure.USART_WordLength/0x1000 + 7);
		}
		break;
		case USART_Parity_Odd:
		{
			for (i=0; i<(USART_InitStructure.USART_WordLength/0x1000 + 7); i++)
			{
				if (temp & 0x01)
					cnt++;

				temp >>= 1;
			}
			
			if(cnt % 2 == 0)
				send_data |= 1 << (USART_InitStructure.USART_WordLength/0x1000 + 7);
			else
				send_data &= ~(1 << (USART_InitStructure.USART_WordLength/0x1000 + 7));
		}
		break;
		
		default:
		{
			printf("sample uart parity param is error\r\n");
		}
	}
	
	//printf("send_data:%x\r\n",send_data);
	if(send_data == recv_data)
		return SUCCESS;
	else
		return ERROR;
			
}


