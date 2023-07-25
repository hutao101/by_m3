#include "sys.h"
#include "delay.h"
#include "sample_adc.h"
#include "stm32f10x_tim.h"

/*******************************************************************************
 * @brief   sample_adc_t7_7
 * Function 间断模式:EXTI触发(需打开中断屏蔽位)
 * @author  licuihuang
 * @date    2022-9-21
 * @param   void
 * @return  void
*******************************************************************************/
#define GPIO_TEST_BASE GPIOD_BASE

void gpio_nvic_init(u32 channel)
{	
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = channel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		  //子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 		  //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure); 	
}

void sample_adc_t7_7(void)
{	
	u32 i=0,j=0,temp;
	write_ahb32(0x4001b074,3<<4);//[6:4]:0-4:ABCDE
#if 1
	temp = ADC_CTRL_FIFO_10 | ADC_EXTTRIG_Set | ADC_TrigConv_EXTI | (2<<13);  //选择EXTI11触发，每轮转换3组通道
	write_ahb32(ADC_CTRL,temp);              //FIFO阈值 
	write_ahb32(ADC_CH2,ADC_CH2_CHALEN_5);   //转换通道个数
	write_ahb32(ADC_CH0,0xc520c41);          //需转换的通道0xc520c41(54321通道)	
	write_ahb32(ADC_MODE,ADC_MODE_DISCON);   //间断模式	
    printf("ADC_STAT:0x%x ADC_MODE:0x%x ADC_CTRL:0x%x\r\n",read_ahb32(ADC_STAT),read_ahb32(ADC_MODE),read_ahb32(ADC_CTRL));
#endif

#if 1
	//GPIO配置
	write_ahb32(GPIO_TEST_BASE+0x08,0x0);     //SW_CTRL寄存器为0，配置为GPIO软件模式
	write_ahb32(GPIO_TEST_BASE+0x04,0x0);     //DDR寄存器为1，配置为输出模式
	write_ahb32(GPIO_TEST_BASE+0x00,0xFFFF);  //GPIO输出为1
	
	printf("GPIO:0x%x\r\n",read_ahb32(GPIO_TEST_BASE+0x50));//GPIO输入数据寄存器的值
	//中断配置
	write_ahb32(GPIO_TEST_BASE+0x44,0xFFFF);     //DEBOUNCE
	write_ahb32(GPIO_TEST_BASE+0x38,0xFFFF);     //GPIO_INTTYPE_LEVEL，边沿触发
	write_ahb32(GPIO_TEST_BASE+0x3c,0x0);        //GPIO_INT_POLARITY，上升沿触发
	write_ahb32(GPIO_TEST_BASE+0x34,~(1<<11));   //GPIO_INTMASK，UN MASK
	write_ahb32(GPIO_TEST_BASE+0x30,1<<11);      //使能位打开

//	gpio_nvic_init(0x08);

	printf("raw:0x%x\r\n",read_ahb32(GPIO_TEST_BASE+0x44));    //读RAW中断
	printf("status:0x%x\r\n",read_ahb32(GPIO_TEST_BASE+0x40)); //读status中断
	printf("GPIO:0x%x\r\n",read_ahb32(GPIO_TEST_BASE+0x50));   //GPIO输入数据寄存器的值

	//write_ahb32(GPIO_TEST_BASE+0x00,0xFFFF);  //GPIO输出为0
	delay_ms(1);
	//write_ahb32(GPIO_TEST_BASE+0x00,0x0);     //GPIO输出为0	
#endif

	
#if 1
	while(1)
	{
		write_ahb32(GPIO_TEST_BASE+0x4C,0xFFFF);  //清EOI		
		while((read_ahb32(ADC_STAT)&0xf8)!=0x0)  //判断FIFO计数值是否为0
		{
		    j=read_ahb32(ADC_STAT);
			i=read_ahb32(ADC_DATA);
			printf("data0:0x%x stat0:0x%x\r\n",i,j);  //每打印一次数据FIFO计数值就会递减
		}
	}
#endif
}



