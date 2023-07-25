#include "sys.h"
#include "delay.h"
#include "sample_adc.h"
#include "stm32f10x_tim.h"

/*******************************************************************************
 * @brief   sample_adc_t7_7
 * Function ���ģʽ:EXTI����(����ж�����λ)
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
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		  //�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 		  //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure); 	
}

void sample_adc_t7_7(void)
{	
	u32 i=0,j=0,temp;
	write_ahb32(0x4001b074,3<<4);//[6:4]:0-4:ABCDE
#if 1
	temp = ADC_CTRL_FIFO_10 | ADC_EXTTRIG_Set | ADC_TrigConv_EXTI | (2<<13);  //ѡ��EXTI11������ÿ��ת��3��ͨ��
	write_ahb32(ADC_CTRL,temp);              //FIFO��ֵ 
	write_ahb32(ADC_CH2,ADC_CH2_CHALEN_5);   //ת��ͨ������
	write_ahb32(ADC_CH0,0xc520c41);          //��ת����ͨ��0xc520c41(54321ͨ��)	
	write_ahb32(ADC_MODE,ADC_MODE_DISCON);   //���ģʽ	
    printf("ADC_STAT:0x%x ADC_MODE:0x%x ADC_CTRL:0x%x\r\n",read_ahb32(ADC_STAT),read_ahb32(ADC_MODE),read_ahb32(ADC_CTRL));
#endif

#if 1
	//GPIO����
	write_ahb32(GPIO_TEST_BASE+0x08,0x0);     //SW_CTRL�Ĵ���Ϊ0������ΪGPIO���ģʽ
	write_ahb32(GPIO_TEST_BASE+0x04,0x0);     //DDR�Ĵ���Ϊ1������Ϊ���ģʽ
	write_ahb32(GPIO_TEST_BASE+0x00,0xFFFF);  //GPIO���Ϊ1
	
	printf("GPIO:0x%x\r\n",read_ahb32(GPIO_TEST_BASE+0x50));//GPIO�������ݼĴ�����ֵ
	//�ж�����
	write_ahb32(GPIO_TEST_BASE+0x44,0xFFFF);     //DEBOUNCE
	write_ahb32(GPIO_TEST_BASE+0x38,0xFFFF);     //GPIO_INTTYPE_LEVEL�����ش���
	write_ahb32(GPIO_TEST_BASE+0x3c,0x0);        //GPIO_INT_POLARITY�������ش���
	write_ahb32(GPIO_TEST_BASE+0x34,~(1<<11));   //GPIO_INTMASK��UN MASK
	write_ahb32(GPIO_TEST_BASE+0x30,1<<11);      //ʹ��λ��

//	gpio_nvic_init(0x08);

	printf("raw:0x%x\r\n",read_ahb32(GPIO_TEST_BASE+0x44));    //��RAW�ж�
	printf("status:0x%x\r\n",read_ahb32(GPIO_TEST_BASE+0x40)); //��status�ж�
	printf("GPIO:0x%x\r\n",read_ahb32(GPIO_TEST_BASE+0x50));   //GPIO�������ݼĴ�����ֵ

	//write_ahb32(GPIO_TEST_BASE+0x00,0xFFFF);  //GPIO���Ϊ0
	delay_ms(1);
	//write_ahb32(GPIO_TEST_BASE+0x00,0x0);     //GPIO���Ϊ0	
#endif

	
#if 1
	while(1)
	{
		write_ahb32(GPIO_TEST_BASE+0x4C,0xFFFF);  //��EOI		
		while((read_ahb32(ADC_STAT)&0xf8)!=0x0)  //�ж�FIFO����ֵ�Ƿ�Ϊ0
		{
		    j=read_ahb32(ADC_STAT);
			i=read_ahb32(ADC_DATA);
			printf("data0:0x%x stat0:0x%x\r\n",i,j);  //ÿ��ӡһ������FIFO����ֵ�ͻ�ݼ�
		}
	}
#endif
}



