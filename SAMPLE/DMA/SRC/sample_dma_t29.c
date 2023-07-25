#include "sample_dma.h"

//m 2 p 32bit ccr4 pwm  des_msize = 4

void sample_timer_t22_toDma_t29(void)
{
	TIM1_NVIC_Init(33);
	TIM1_Int_Init(1999,4999,TIM_CounterMode_Up,0);  //10000/50M=200us  200*200us=40ms(up) 
	TIM_ITConfig(TIM1, TIM_IT_CC4, DISABLE);	
	
	TIM1_PWMOUT_Init(TIM_OCMode_PWM1);
	TIM_SetCounter(TIM1,0);
	TIM_SetCompare1(TIM1,999);
  TIM_Cmd(TIM1, ENABLE);

	printf("\r\n");
}

int dmac_32bit_mem2iic_cfg_t29(int channel)
{
    u32 reg_value; 
	//configure the source address 
	write_ahb32(AHB_DMAC_BASE +DMAC_SAR_L(channel),0x20004A00);
	//configure the destination address
	write_ahb32(AHB_DMAC_BASE +DMAC_DAR_L(channel), 0x40011c4c);  //0x40011c34 CCR1 32bit  0x40011c4c:TIM1 DMA address full transfe
	//configure the CTL_L
	
	reg_value =   (1 << 20)	   										// memory to peripheral ,control by ahb-dmac
				 | (0 << 17)                                	 	// gather enable
				 | (0 << 18)										//scatter disable
				 | (1 << 11)| (1 << 14)		         //source burst length is 4	and dest also is 4 
				 | (2 << 7) | (0 << 9)		 						//increment the source and no change destination address
				 | (2 << 4) | (2 << 1)								//source and destination transfer width is 32bits
				 | 1;				   								//interrupt enable 
	write_ahb32(AHB_DMAC_BASE + DMAC_CTL_L(channel), reg_value);
	//configure the CTL_L	 
	reg_value = 14;	   										 
	write_ahb32(AHB_DMAC_BASE + DMAC_CTL_H(channel), reg_value);
	
	//configure the CFG_L
	reg_value =    (0 << 19)| (0 << 18)								//handshake polarrity, src default 0, dest base on setting	   										// memory to memory ,control by ahb-dmac
				 | (0 << 11)| (0 << 10)								//handshake select hardware
				 | (0 << 8 ) 										//DMA transfer from the source is not sspended
				 | (channel << 5);				    				//channel prority is set to the channel index,means the channel 7 is the hishgest channel
	write_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(channel), reg_value);
	//configure the CFG_H
	
	reg_value = read_ahb32(MCU_CFG);
	reg_value &= (~(7<<12));  //MCU_CFG[14:12]清0
  reg_value |= 4<<12; //TIMER1_CC4 MCU_CFG[14:12]=100
//	printf("!!!!!!reg_value = 0x%x\r\n",reg_value);  

	write_ahb32(MCU_CFG, reg_value);  

//	printf("MCU_CFG: 0x%x\r\n",read_ahb32(MCU_CFG));

	reg_value =    (0 << 7)| (12 << 11)		//Handshaking 12:TIMER1_CC4 MCU_CFG[14:12]=100			
				 | (1 << 2) 										//prot non-cache, non-buff,prvilge data
				 | (0 << 1 ) 										//Space/data available for single transfer 
				 | 0  ;				    							//source data pre-fetching is enable
	write_ahb32(AHB_DMAC_BASE + DMAC_CFG_H(channel), reg_value);

	return 0;
}


void sample_dma_t29(int channel)
{
/*begin******************************顺序一 ：DMA 握手 TIM***************************/
	NVIC_InitTypeDef NVIC_InitStructure;
	ch_irq = channel;
	
	NVIC_InitStructure.NVIC_IRQChannel = 1;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 		//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure); //根据指定的参数初始化NVIC寄存器
	
	TIM1->DIER &= (~(1<<12));  // Capture/Compare 4 DMA request Disable	
	TIM_Cmd(TIM1, DISABLE); //关闭定时器1
	
	write_ahb32(0x20004A00,99);
	write_ahb32(0x20004A04,199);
	write_ahb32(0x20004A08,299);
	write_ahb32(0x20004A0C,399);
	write_ahb32(0x20004A10,499);
	write_ahb32(0x20004A14,599);
	write_ahb32(0x20004A18,699);	
	write_ahb32(0x20004A1C,799);
	write_ahb32(0x20004A20,899);
	write_ahb32(0x20004A24,999);	
	write_ahb32(0x20004A28,1099);
	write_ahb32(0x20004A2C,1199);
	write_ahb32(0x20004A30,665);
	write_ahb32(0x20004A34,887);
	
	printf("DMAC_CTL_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_L(channel)));
	printf("DMAC_CTL_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_H(channel)));

	printf("DMAC_CFG_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(channel)));
	printf("DMAC_CFG_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_H(channel)));
	
	printf("11111\r\n");
	dw_dmac_clearIrq(channel, IRQ_TYPE_ALL);
	dw_dmac_unmaskIrq(channel,IRQ_TYPE_ALL);
	
	dmac_32bit_mem2iic_cfg_t29(channel);  //DMA 初始化
	dw_dmac_enableChannel(channel);
	
		printf("2222\r\n");
	//timer dma 支持
	TIM1->DCR &= ~(31<<8);// 清空12：8 DMA burst length
	TIM1->DCR |= 0<<8 ;   // dma burst length is 1*4  对定时器读写一次数据所需要的字节
	TIM1->DCR &= (~31);  //清空0-4bit，
	TIM1->DCR |= TIM_DMABase_CCR4; //DMA base address = TIM_DMABase_CCR4
	TIM1->DIER |= 1<<12;  // Capture/Compare 3 DMA request enable
	
	
	printf("3333\r\n");
	sample_timer_t22_toDma_t29();
	
	delay_ms(10000);
	
	printf("DMAC_CTL_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_L(channel)));
	printf("DMAC_CTL_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_H(channel)));

	printf("DMAC_CFG_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(channel)));
	printf("DMAC_CFG_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_H(channel)));
	

	while(1);
/*begin******************************顺序一 ：DMA 握手 TIM***************************/



}


