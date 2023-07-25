#include "sys.h"
#include "delay.h"
#include "sample_adc.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_adc_t7
 * Function ���ģʽ
 * @author  licuihuang
 * @date    2022-6-21
 * @param   void
 * @return  void
*******************************************************************************/
void sample_adc_t7(void)
{	
	u32 i=0,j=0,k,temp;
	temp = ADC_CTRL_FIFO_10 | ADC_EXTTRIG_Set | ADC_TrigConv_SW | (2<<13) | ADC_EXTTRIG_SWSTART_Set; 
//	write_ahb32(ADC_CTRL,temp);  
	write_ahb32(ADC_CH2,ADC_CH2_CHALEN_5);  
	write_ahb32(ADC_CH0,0xc520c41);         //0x520c41��54321ͨ��(1��ת)     
	write_ahb32(ADC_MODE,ADC_MODE_DISCON);  //���ģʽ
	write_ahb32(ADC_CTRL,temp);  	        //CTRL[26]���ʹ��λ���Զ���0,�����������
    printf("ADC_STAT:0x%x ADC_MODE:0x%x ADC_CTRL:0x%x\r\n",read_ahb32(ADC_STAT),read_ahb32(ADC_MODE),read_ahb32(ADC_CTRL));

	while(1)
	{   
		if((read_ahb32(ADC_STAT)&0xf8)!=0x0)  
		{
			while((read_ahb32(ADC_STAT)&ADC_STAT_BUSY));
		    j=read_ahb32(ADC_STAT);
			i=read_ahb32(ADC_DATA);
			printf("data0:0x%x stat0:0x%x\r\n\r\n",i,j);  
		}
		else
	    {
			printf("START CONV:\r\n"); 
			printf("0x%x 0x%x\r\n",read_ahb32(ADC_MODE),read_ahb32(ADC_CTRL));  //����CTRL[26]���ʹ��λ���Զ���0������λ������
		    temp = read_ahb32(ADC_CTRL);
			temp |= ADC_EXTTRIG_SWSTART_Set;//(ADC_EXTTRIG_SWSTART_Set | ADC_EXTTRIG_Set | ADC_TrigConv_SW);
			write_ahb32(ADC_CTRL,temp);
			delay_ms(1);
		}
	}
}	
	
	