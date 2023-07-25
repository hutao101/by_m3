#include "sample_dma.h"

//p2m 32bit  TIM CC3

void sample_timer_t22_fordma_t25(void)
{

	TIM1_NVIC_Init(33);
	TIM1_Int_Init(9999,9999,TIM_CounterMode_Up,0);  //10000/50M=200us  10000*200us=2s(up) 
	TIM_ITConfig(TIM1, TIM_IT_CC3, DISABLE);	
	
	TIM1_CaptureIN_Init(TIM_Channel_3,TIM_ICSelection_DirectTI);
  TIM_Cmd(TIM1, ENABLE);
	printf("\r\n");
}


int dmac_32bit_timer2mem_cfg_t25(int channel)
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
				 | (1 << 11)| (0 << 14)		       //des 4items  src 4 items  
				 | (0 << 7) | (2 << 9)		 						//src addr no change,dst increments
				 | (2 << 4) | (2 << 1)								//src 32bit des 32bit
				 | 1;				   								//interrupt enable 
	write_ahb32(AHB_DMAC_BASE + DMAC_CTL_L(channel), reg_value);
	//configure the CTL_H	 
	reg_value =  12;	   	//һ����4������									 
	write_ahb32(AHB_DMAC_BASE + DMAC_CTL_H(channel), reg_value);
	//configure the CFG_L
	reg_value =    (0 << 19)| (0 << 18)								//handshake polarrity, src default 0, dest base on setting	   								
				 | (0 << 11)| (0 << 10)								//handshake select hardware
				 | (0 << 8 ) 										//DMA transfer from the source is not sspended
				 | (channel << 5);				    				//channel prority is set to the channel index,means the channel 7 is the hishgest channel
	write_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(channel), reg_value);
	//configure the CFG_H

	reg_value = read_ahb32(MCU_CFG);
	reg_value &= (~(7<<12));  //MCU_CFG[14:12]��0
  reg_value |= 4<<12; //TIMER1_CC1 MCU_CFG[14:12]=100
//	printf("!!!!!!reg_value = 0x%x\r\n",reg_value);       
	write_ahb32(MCU_CFG, reg_value);  
//	printf("MCU_CFG: 0x%x\r\n",read_ahb32(MCU_CFG));

	reg_value =    (5 << 7)| (0 << 11)		//Handshaking 5 :TIMER1_CC3 MCU_CFG[14:12]=100
				 | (1 << 2) 										//prot non-cache, non-buff,prvilge data
				 | (0 << 1 ) 										//Space/data available for single transfer 
				 | 0  ;				    							//source data pre-fetching is enable
	write_ahb32(AHB_DMAC_BASE + DMAC_CFG_H(channel), reg_value);

	return 0;
}


// close ��case����

void sample_dma_t25(int channel)
{
	
/*begin*****************************˳��һ��DMA���ü�ʹ�ܡ�TIM���ü�ʹ�ܡ�����*****************************************/	
	NVIC_InitTypeDef NVIC_InitStructure;
	ch_irq = channel;
	NVIC_InitStructure.NVIC_IRQChannel = 1;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 		//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure); //����ָ���Ĳ�����ʼ��NVIC�Ĵ���
	
	TIM1->DIER &= (~(1<<11));  // Capture/Compare 3 DMA request Disable	
	TIM_Cmd(TIM1, DISABLE); //�رն�ʱ��1
	TIM_Cmd(TIM3, DISABLE); //�رն�ʱ��3
	
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
	
	dmac_32bit_timer2mem_cfg_t25(channel);  //DMA ��ʼ��
	dw_dmac_enableChannel(channel);
	
	
	printf("3333\r\n");
	sample_timer_t22_fordma_t25();
	
	delay_ms(1);
	
	printf("2222\r\n");
	//timer dma ֧��
	TIM1->DCR &= ~(31<<8);// ���12��8 DMA burst length
	TIM1->DCR |= 0<<8 ;   // dma burst length is 1*4  �Զ�ʱ����дһ����������Ҫ���ֽ�
	TIM1->DCR &= (~31);  //���0-4bit��
	TIM1->DCR |= TIM_DMABase_CCR3; //DMA base address = TIM_DMABase_CCR3
	TIM1->DIER |= 1<<11;  // Capture/Compare 3 DMA request enable
	delay_ms(500);
	
	printf("TIM1-DMAR: 0x%x\r\n",TIM1->DMAR);
	printf("0x40011c3C��0x%x\r\n",read_ahb32(0x40011c3C));
	printf("CCR3��0x%x\r\n",TIM1->CCR3);
	printf("TIM1-DMAR: 0x%x\r\n",TIM1->DMAR);
	printf("0x40011c3C��0x%x\r\n",read_ahb32(0x40011c3C));
	printf("CCR3��0x%x\r\n",TIM1->CCR3);
	
	printf("\r\n\r\n");
	printf("\r\n0x20004A00��0x%x\r\n",read_ahb32(0x20004A00));
	printf("\r\n0x20004A04��0x%x\r\n",read_ahb32(0x20004A04));
	printf("\r\n0x20004A08��0x%x\r\n",read_ahb32(0x20004A08));
	printf("\r\n0x20004A0C��0x%x\r\n",read_ahb32(0x20004A0C));
	printf("\r\n0x20004A10��0x%x\r\n",read_ahb32(0x20004A10));
	printf("\r\n0x20004A14��0x%x\r\n",read_ahb32(0x20004A14));
	printf("\r\n0x20004A18��0x%x\r\n",read_ahb32(0x20004A18));
	printf("\r\n0x20004A1C��0x%x\r\n",read_ahb32(0x20004A1C));
	printf("\r\n0x20004A2C��0x%x\r\n",read_ahb32(0x20004A2C));
	printf("\r\n\r\n");
	
	printf("DMAC_CTL_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_L(channel)));
	printf("DMAC_CTL_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CTL_H(channel)));

	printf("DMAC_CFG_L:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_L(channel)));
	printf("DMAC_CFG_H:0x%x\r\n",read_ahb32(AHB_DMAC_BASE + DMAC_CFG_H(channel)));
	
	while(1)
	{
		
	}
/**end***********************˳��һ��DMA���ü�ʹ�ܡ�TIM���ü�ʹ�ܡ�����***********/

}

