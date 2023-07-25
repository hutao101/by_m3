#include "sample_dma.h"

//p2m 32bit

void sample_timer_t22_fordma_t17(void)
{
//	u32 i;
//	TIM1_NVIC_Init(31);
//	
//  TIM3_Int_Init(199,9999,TIM_CounterMode_Up);  //10000/50M=200us  200*200us=40ms(up) 
//	TIM1_Int_Init(9999,9999,TIM_CounterMode_Up,2);  
//	TIM3_PWMOUT_Init(TIM_OCMode_PWM2);
////	TIM_SetIC1Prescaler(TIM3,TIM_OCPreload_Disable);  //需关闭预分频功能，不然CCR1为0，单脉冲不成功
////	TIM_SelectOnePulseMode(TIM3,TIM_OPMode_Single);
//	
//	TIM1_CaptureIN_Init(TIM_Channel_1,TIM_ICSelection_DirectTI);
//	TIM_ITConfig(TIM1, TIM_IT_CC1, ENABLE);	

//	TIM_SetCompare1(TIM3,100);	
//  TIM_Cmd(TIM1, ENABLE);
//	TIM_Cmd(TIM3, ENABLE);
	
	TIM1_NVIC_Init(31);
	TIM1_Int_Init(9999,9999,TIM_CounterMode_Up,0);  //10000/50M=200us  10000*200us=2s(up) 
	TIM_ITConfig(TIM1, TIM_IT_CC1, DISABLE);	
	
	TIM1_CaptureIN_Init(TIM_Channel_1,TIM_ICSelection_DirectTI);
  TIM_Cmd(TIM1, ENABLE);
	printf("\r\n");
}


int dmac_32bit_timer2mem_cfg_t17(int channel)
{
    u32 reg_value; 
	//configure the source address 

	write_ahb32(AHB_DMAC_BASE +DMAC_SAR_L(channel),0x40011c4c);  //0x40011c34 CCR1 32bit  0x40011c4c:TIM1 DMA address full transfe

	//configure the destination address
	write_ahb32(AHB_DMAC_BASE +DMAC_DAR_L(channel),0x20004A00);

	//configure the CTL_L
	reg_value =   (2 << 20)	   										// peripheral to memory,control by ahb-dmac
				 | (0 << 17)                                	 	// gather enable
				 | (0 << 18)										//scatter disable
				 | (1 << 11)| (0 << 14)		       //des 1items  src 4 items  
				 | (0 << 7) | (2 << 9)		 						//src addr no change,dst increments
				 | (2 << 4) | (2 << 1)								//src 32bit des 32bit
				 | 1;				   								//interrupt enable 
	write_ahb32(AHB_DMAC_BASE + DMAC_CTL_L(channel), reg_value);
	//configure the CTL_H	 
	reg_value =  8;	   	//一个块4个数据									 
	write_ahb32(AHB_DMAC_BASE + DMAC_CTL_H(channel), reg_value);
	//configure the CFG_L
	reg_value =    (0 << 19)| (0 << 18)								//handshake polarrity, src default 0, dest base on setting	   								
				 | (0 << 11)| (0 << 10)								//handshake select hardware
				 | (0 << 8 ) 										//DMA transfer from the source is not sspended
				 | (channel << 5);				    				//channel prority is set to the channel index,means the channel 7 is the hishgest channel
	write_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(channel), reg_value);
	//configure the CFG_H

	reg_value = read_ahb32(MCU_CFG);
	reg_value &= (~(7<<12));  //MCU_CFG[14:12]清0
  reg_value |= 4<<12; //TIMER1_CC1 MCU_CFG[14:12]=100
//	printf("!!!!!!reg_value = 0x%x\r\n",reg_value);       
	write_ahb32(MCU_CFG, reg_value);  
//	printf("MCU_CFG: 0x%x\r\n",read_ahb32(MCU_CFG));

	reg_value =    (3 << 7)| (0 << 11)		//Handshaking 3 :TIMER1_CC1 MCU_CFG[14:12]=100
				 | (1 << 2) 										//prot non-cache, non-buff,prvilge data
				 | (0 << 1 ) 										//Space/data available for single transfer 
				 | 0  ;				    							//source data pre-fetching is enable
	write_ahb32(AHB_DMAC_BASE + DMAC_CFG_H(channel), reg_value);

	return 0;
}


/****************************************************************************
 * @brief   sample_dma_t17
 * Function test the Timer1 to SRAM dma transfer of des is 32bit ，src can be 8/16/32
 * @author  he
 * @date    2022-06-16
 * @param   
		channel:DMA must set to 1
 * @return void
******************************************************************************/

void sample_dma_t17(int channel)
{
	
/*begin*****************************顺序一：DMA配置及使能、TIM配置及使能、握手*****************************************/	
	NVIC_InitTypeDef NVIC_InitStructure;
	
	ch_irq = channel;
	
	NVIC_InitStructure.NVIC_IRQChannel = 1;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 		//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure); //根据指定的参数初始化NVIC寄存器
	
	TIM1->DIER &= (~(1<<9));  // Capture/Compare 1 DMA request Disable	
	TIM_Cmd(TIM1, DISABLE); //关闭定时器1
	TIM_Cmd(TIM3, DISABLE); //关闭定时器3
	
	write_ahb32(0x20004A00,0xffffffff);
	write_ahb32(0x20004A04,0xffffffff);
	write_ahb32(0x20004A08,0xffffffff);
	write_ahb32(0x20004A0C,0xffffffff);
	write_ahb32(0x20004A10,0xffffffff);
	write_ahb32(0x20004A14,0xffffffff);
	write_ahb32(0x20004A18,0xffffffff);	
	write_ahb32(0x20004A1C,0xffffffff);
	
	write_ahb32(0x20004A2C,0xffffffff);
	
	printf("DMAC_CTL_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_L(channel)));
	printf("DMAC_CTL_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_H(channel)));

	printf("DMAC_CFG_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(channel)));
	printf("DMAC_CFG_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_H(channel)));
	
	printf("11111\r\n");
	dw_dmac_clearIrq(channel, IRQ_TYPE_ALL);
	dw_dmac_unmaskIrq(channel,IRQ_TYPE_ALL);
	
	dmac_32bit_timer2mem_cfg_t17(channel);  //DMA 初始化
	dw_dmac_enableChannel(channel);
	
	
	printf("3333\r\n");
	sample_timer_t22_fordma_t17();
	
	delay_ms(1);
	
	printf("2222\r\n");
	//timer dma 支持
	TIM1->DCR &= ~(31<<8);// 清空12：8 DMA burst length
	TIM1->DCR |= 0<<8 ;   // dma burst length is 1*4  对定时器读写一次数据所需要的字节
	TIM1->DCR &= (~31);  //清空0-4bit，
	TIM1->DCR |= TIM_DMABase_CCR1; //DMA base address = TIM_DMABase_CCR1
	TIM1->DIER |= 1<<9;  // Capture/Compare 1 DMA request enable
	delay_ms(10000);
	
	printf("TIM1-DMAR: 0x%x\r\n",TIM1->DMAR);
	printf("0x40011c34：0x%x\r\n",read_ahb32(0x40011c34));
	printf("CCR1：0x%x\r\n",TIM1->CCR1);
	printf("TIM1-DMAR: 0x%x\r\n",TIM1->DMAR);
	printf("0x40011c34：0x%x\r\n",read_ahb32(0x40011c34));
	printf("CCR1：0x%x\r\n",TIM1->CCR1);
	
	printf("\r\n\r\n");
	printf("\r\n0x20004A00：0x%x\r\n",read_ahb32(0x20004A00));
	printf("\r\n0x20004A04：0x%x\r\n",read_ahb32(0x20004A04));
	printf("\r\n0x20004A08：0x%x\r\n",read_ahb32(0x20004A08));
	printf("\r\n0x20004A0C：0x%x\r\n",read_ahb32(0x20004A0C));
	printf("\r\n0x20004A10：0x%x\r\n",read_ahb32(0x20004A10));
	printf("\r\n0x20004A14：0x%x\r\n",read_ahb32(0x20004A14));
	printf("\r\n0x20004A18：0x%x\r\n",read_ahb32(0x20004A18));
	printf("\r\n0x20004A1C：0x%x\r\n",read_ahb32(0x20004A1C));
	printf("\r\n0x20004A2C：0x%x\r\n",read_ahb32(0x20004A2C));
	printf("\r\n\r\n");
	
	printf("DMAC_CTL_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_L(channel)));
	printf("DMAC_CTL_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_H(channel)));

	printf("DMAC_CFG_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(channel)));
	printf("DMAC_CFG_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_H(channel)));
	
	delay_ms(10000);
	//	//第二次搬运  
	dmac_32bit_timer2mem_cfg_t17(channel);  //DMA 初始化
	dw_dmac_enableChannel(channel);
	delay_ms(10000);
	printf("\r\n0x20004A00：0x%x\r\n",read_ahb32(0x20004A00));
	printf("\r\n0x20004A04：0x%x\r\n",read_ahb32(0x20004A04));
	printf("\r\n0x20004A08：0x%x\r\n",read_ahb32(0x20004A08));
	printf("\r\n0x20004A0C：0x%x\r\n",read_ahb32(0x20004A0C));
	printf("\r\n0x20004A10：0x%x\r\n",read_ahb32(0x20004A10));
	printf("\r\n0x20004A14：0x%x\r\n",read_ahb32(0x20004A14));
	printf("\r\n0x20004A18：0x%x\r\n",read_ahb32(0x20004A18));
	printf("\r\n0x20004A1C：0x%x\r\n",read_ahb32(0x20004A1C));
	
	printf("\r\n0x20004A2C：0x%x\r\n",read_ahb32(0x20004A2C));
	
//	printf("DMAC_CTL_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_L(channel)));
	printf("DMAC_CTL_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_H(channel)));

	printf("DMAC_CFG_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(channel)));
//	printf("DMAC_CFG_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_H(channel)));

	while(1)
	{
		
	}
/**end***********************顺序一：DMA配置及使能、TIM配置及使能、握手***********/


/*begin**********************顺序二：TIM配置及使能、DMA配置以及使能、握手***********/	
//	NVIC_InitTypeDef NVIC_InitStructure;
//	ch_irq = channel;
	
//	NVIC_InitStructure.NVIC_IRQChannel = 1;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 		//IRQ通道使能
//	NVIC_Init(&NVIC_InitStructure); //根据指定的参数初始化NVIC寄存器
//	
//	TIM1->DIER &= (~(1<<9));  // Capture/Compare 1 DMA request Disable	
//	TIM_Cmd(TIM1, DISABLE); //关闭定时器1
//	TIM_Cmd(TIM3, DISABLE); //关闭定时器3
//	
//	write_ahb32(0x20004A00,0xffffffff);
//	write_ahb32(0x20004A04,0xffffffff);
//	write_ahb32(0x20004A08,0xffffffff);
//	write_ahb32(0x20004A0C,0xffffffff);
//	write_ahb32(0x20004A10,0xffffffff);
//	write_ahb32(0x20004A14,0xffffffff);
//	write_ahb32(0x20004A18,0xffffffff);	
//	write_ahb32(0x20004A1C,0xffffffff);
//	write_ahb32(0x20004A2C,0xffffffff);
//	
//	printf("3333\r\n");
//	sample_timer_t22_fordma_t17();
//	
//	delay_ms(1);
//	
//	printf("DMAC_CTL_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_L(channel)));
//	printf("DMAC_CTL_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_H(channel)));

//	printf("DMAC_CFG_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(channel)));
//	printf("DMAC_CFG_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_H(channel)));
//	
//	printf("11111\r\n");
//	dw_dmac_clearIrq(channel, IRQ_TYPE_ALL);
//	dw_dmac_unmaskIrq(channel,IRQ_TYPE_ALL);
//	
//	dmac_32bit_timer2mem_cfg_t17(channel);  //DMA 初始化
//	dw_dmac_enableChannel(channel);
//	
//	printf("2222\r\n");
//	//timer dma 支持
//	TIM1->DCR &= ~(31<<8);// 清空12：8 DMA burst length
//	TIM1->DCR |= 0<<8 ;   // dma burst length is 1*4  对定时器读写一次数据所需要的字节
//	TIM1->DCR &= (~31);  //清空0-4bit，
//	TIM1->DCR |= TIM_DMABase_CCR1; //DMA base address = TIM_DMABase_CCR1
//	TIM1->DIER |= 1<<9;  // Capture/Compare 1 DMA request enable
//	
//	delay_ms(500);
//	printf("TIM1-DMAR: 0x%x\r\n",TIM1->DMAR);
//	printf("0x40011c34：0x%x\r\n",read_ahb32(0x40011c34));
//	printf("CCR1：0x%x\r\n",TIM1->CCR1);

//	printf("TIM1-DMAR: 0x%x\r\n",TIM1->DMAR);
//	printf("0x40011c34：0x%x\r\n",read_ahb32(0x40011c34));
//	printf("CCR1：0x%x\r\n",TIM1->CCR1);
//	
//	delay_ms(10000);
//	
//	printf("\r\n\r\n");
//	printf("\r\n0x20004A00：0x%x\r\n",read_ahb32(0x20004A00));
//	printf("\r\n0x20004A04：0x%x\r\n",read_ahb32(0x20004A04));
//	printf("\r\n0x20004A08：0x%x\r\n",read_ahb32(0x20004A08));
//	printf("\r\n0x20004A0C：0x%x\r\n",read_ahb32(0x20004A0C));
//	printf("\r\n0x20004A10：0x%x\r\n",read_ahb32(0x20004A10));
//	printf("\r\n0x20004A14：0x%x\r\n",read_ahb32(0x20004A14));
//	printf("\r\n0x20004A18：0x%x\r\n",read_ahb32(0x20004A18));
//	printf("\r\n0x20004A1C：0x%x\r\n",read_ahb32(0x20004A1C));
//	printf("\r\n0x20004A2C：0x%x\r\n",read_ahb32(0x20004A2C));
//	
//	delay_ms(500);
//	
//	printf("\r\n\r\n");
//	
//	printf("DMAC_CTL_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_L(channel)));
//	printf("DMAC_CTL_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_H(channel)));

//	printf("DMAC_CFG_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(channel)));
//	printf("DMAC_CFG_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_H(channel)));
//	
//	delay_ms(10000);
//	
//	//第二次搬运  
//	dmac_32bit_timer2mem_cfg_t17(channel);  //DMA 初始化
//	dw_dmac_enableChannel(channel);
//	
//	delay_ms(2000);
//	printf("\r\n0x20004A00：0x%x\r\n",read_ahb32(0x20004A00));
//	printf("\r\n0x20004A04：0x%x\r\n",read_ahb32(0x20004A04));
//	printf("\r\n0x20004A08：0x%x\r\n",read_ahb32(0x20004A08));
//	printf("\r\n0x20004A0C：0x%x\r\n",read_ahb32(0x20004A0C));
//	printf("\r\n0x20004A10：0x%x\r\n",read_ahb32(0x20004A10));
//	printf("\r\n0x20004A14：0x%x\r\n",read_ahb32(0x20004A14));
//	printf("\r\n0x20004A18：0x%x\r\n",read_ahb32(0x20004A18));
//	printf("\r\n0x20004A1C：0x%x\r\n",read_ahb32(0x20004A1C));
//	printf("\r\n0x20004A2C：0x%x\r\n",read_ahb32(0x20004A2C));	
//	
////	printf("DMAC_CTL_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_L(channel)));
//	printf("DMAC_CTL_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_H(channel)));

//	printf("DMAC_CFG_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(channel)));
////	printf("DMAC_CFG_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_H(channel)));
//	
//	
//	while(1)
//	{
//		
//	}
/**end*************************顺序二：TIM配置及使能、DMA配置以及使能、握手**********/


/*begin**********************顺序三：TIM配置及使能、握手、DMA配置以及使能***********/	
//	NVIC_InitTypeDef NVIC_InitStructure;
//	ch_irq = channel;
//	NVIC_InitStructure.NVIC_IRQChannel = 1;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 		//IRQ通道使能
//	NVIC_Init(&NVIC_InitStructure); //根据指定的参数初始化NVIC寄存器
//	
//	TIM1->DIER &= (~(1<<9));  // Capture/Compare 1 DMA request Disable	
//	TIM_Cmd(TIM1, DISABLE); //关闭定时器1
//	TIM_Cmd(TIM3, DISABLE); //关闭定时器3
//	
//	write_ahb32(0x20004A00,0xffffffff);
//	write_ahb32(0x20004A04,0xffffffff);
//	write_ahb32(0x20004A08,0xffffffff);
//	write_ahb32(0x20004A0C,0xffffffff);
//	write_ahb32(0x20004A10,0xffffffff);
//	write_ahb32(0x20004A14,0xffffffff);
//	write_ahb32(0x20004A18,0xffffffff);	
//	write_ahb32(0x20004A1C,0xffffffff);
//	write_ahb32(0x20004A2C,0xffffffff);
//	printf("data = 0x%x\r\n",read_ahb32(0x20004A2C));
//	printf("3333\r\n");
//	sample_timer_t22_fordma_t17();
//	
//	delay_ms(1);
//	
//	printf("DMAC_CTL_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_L(channel)));
//	printf("DMAC_CTL_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_H(channel)));

//	printf("DMAC_CFG_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(channel)));
//	printf("DMAC_CFG_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_H(channel)));
//	
//	printf("2222\r\n");
//	//timer dma 支持
//	TIM1->DCR &= ~(31<<8);// 清空12：8 DMA burst length
//	TIM1->DCR |= 0<<8 ;   // dma burst length is 1*4  对定时器读写一次数据所需要的字节
//	TIM1->DCR &= (~31);  //清空0-4bit，
//	TIM1->DCR |= TIM_DMABase_CCR1; //DMA base address = TIM_DMABase_CCR1
//	TIM1->DIER |= 1<<9;  // Capture/Compare 1 DMA request enable
//	
//	printf("11111\r\n");
//	dw_dmac_clearIrq(channel, IRQ_TYPE_ALL);
//	dw_dmac_unmaskIrq(channel,IRQ_TYPE_ALL);
//	
//	dmac_32bit_timer2mem_cfg_t17(channel);  //DMA 初始化
//	dw_dmac_enableChannel(channel);
//	
//	delay_ms(500);
//	printf("TIM1-DMAR: 0x%x\r\n",TIM1->DMAR);
//	printf("0x40011c34：0x%x\r\n",read_ahb32(0x40011c34));
//	printf("CCR1：0x%x\r\n",TIM1->CCR1);
//	printf("TIM1-DMAR: 0x%x\r\n",TIM1->DMAR);
//	printf("0x40011c34：0x%x\r\n",read_ahb32(0x40011c34));
//	printf("CCR1：0x%x\r\n",TIM1->CCR1);
//	
//	while((read_ahb32(AHB_DMAC_BASE+DMAC_CH_EN_REG_L)) & (1<<channel));
//	
//	printf("\r\n\r\n");
//	printf("\r\n0x20004A00：0x%x\r\n",read_ahb32(0x20004A00));
//	printf("\r\n0x20004A04：0x%x\r\n",read_ahb32(0x20004A04));
//	printf("\r\n0x20004A08：0x%x\r\n",read_ahb32(0x20004A08));
//	printf("\r\n0x20004A0C：0x%x\r\n",read_ahb32(0x20004A0C));
//	printf("\r\n0x20004A10：0x%x\r\n",read_ahb32(0x20004A10));
//	printf("\r\n0x20004A14：0x%x\r\n",read_ahb32(0x20004A14));
//	printf("\r\n0x20004A18：0x%x\r\n",read_ahb32(0x20004A18));
//	printf("\r\n0x20004A1C：0x%x\r\n",read_ahb32(0x20004A1C));
//	printf("\r\n0x20004A2C：0x%x\r\n",read_ahb32(0x20004A2C));	
//	
//	printf("\r\n\r\n");
//	
//	printf("DMAC_CTL_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_L(channel)));
//	printf("DMAC_CTL_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_H(channel)));

//	printf("DMAC_CFG_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(channel)));
//	printf("DMAC_CFG_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_H(channel)));
//	
//	delay_ms(1000);
//	
//	//第二次搬运  
//	dmac_32bit_timer2mem_cfg_t17(channel);  //DMA 初始化
//	dw_dmac_enableChannel(channel);
//	while((read_ahb32(AHB_DMAC_BASE+DMAC_CH_EN_REG_L)) & (1<<channel));
//	delay_ms(500);
//	printf("\r\n0x20004A00：0x%x\r\n",read_ahb32(0x20004A00));
//	printf("\r\n0x20004A04：0x%x\r\n",read_ahb32(0x20004A04));
//	printf("\r\n0x20004A08：0x%x\r\n",read_ahb32(0x20004A08));
//	printf("\r\n0x20004A0C：0x%x\r\n",read_ahb32(0x20004A0C));
//	printf("\r\n0x20004A10：0x%x\r\n",read_ahb32(0x20004A10));
//	printf("\r\n0x20004A14：0x%x\r\n",read_ahb32(0x20004A14));
//	printf("\r\n0x20004A18：0x%x\r\n",read_ahb32(0x20004A18));
//	printf("\r\n0x20004A1C：0x%x\r\n",read_ahb32(0x20004A1C));
//	printf("\r\n0x20004A2C：0x%x\r\n",read_ahb32(0x20004A2C));	
//	
////	printf("DMAC_CTL_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_L(channel)));
//	printf("DMAC_CTL_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_H(channel)));

//	printf("DMAC_CFG_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(channel)));
////	printf("DMAC_CFG_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_H(channel)));
//	
//	while(1)
//	{
//		
//	}
/**end**************************顺序三：TIM配置及使能、握手、DMA配置以及使能************/
}


