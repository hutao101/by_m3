#include "sys.h"
#include "delay.h"
#include "sample_timer2.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_timer_t15   
 * Function ����������(tim2��CH1��tim3��CH1,CH2���������,�۲�tim1��CH1������)
 * @author  licuihuang
 * @date    2022-8-24
 * @param   void
 * @return  void
*******************************************************************************/
void sample_timer2_t15(void)
{
	u32 i=2; 
//	for(i=2;i<4;i++) //CH2/3��������������ܳ������β���
	{
	//	TIM_GPIO_Init();
		TIM_OCInitTypeDef  TIM_OCInitStructure={0};
		TIM1_Int_Init(1999,3599,TIM_CounterMode_Up,0);	//5000/50M=100us  1000*100us=100ms(up)
		TIM3_Int_Init(4999,3599,TIM_CounterMode_Up);  //5000/50M=100us  5000*100us=500ms(up)
		TIM2_Int_Init(5799,3599,TIM_CounterMode_Up);  //5000/50M=100us  5800*100us=580ms(up)	
		TIM1_NVIC_Init(35);
		TIM_ITConfig(TIM1, TIM_IT_COM, ENABLE);//TIM_IT_COM TIM_IT_Trigger

		TIM3_PWMOUT_Init(TIM_OCMode_PWM2);
		TIM_SetCompare1(TIM3,2000);
		TIM_SetIC1Prescaler(TIM3,TIM_OCPreload_Disable);  //��ر�Ԥ��Ƶ���ܣ���ȻCCR1Ϊ0�������岻�ɹ�
		TIM_SelectOnePulseMode(TIM3,TIM_OPMode_Single); 
		
		TIM2_CaptureIN_Init(TIM_Channel_1,TIM_ICSelection_TRC);	

		TIM_SelectHallSensor(TIM2,ENABLE);//TIM_CR2_TI1S);    //���ʱֻ��1ͨ���в��Σ����԰��������Ȼ���timͨ��2������
		TIM_SelectInputTrigger(TIM2, TIM_TS_TI1F_ED);//TIM_TS_TI1F_ED);	  //��ͨ��������������� TIM_TS_TI2FP2
		TIM_SelectSlaveMode(TIM2, TIM_SlaveMode_Reset);  // ѡ���ģʽ: ��λģʽ
		
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;//TIM_OCMode_Active;//TIM_ForcedAction_InActive;//TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
		switch(i)
		{
			case 1:   //ͨ��1��������ģʽ���Ͳ��������������
				printf("\r\nTIM2 CH1 as trigger output:\r\n");
				TIM_OC1Init(TIM2, &TIM_OCInitStructure);
				TIM_SelectOCxM(TIM2, TIM_Channel_1, TIM_OCMode_PWM2);
				TIM_SetCompare1(TIM2,1500);
			    TIM_SelectOutputTrigger(TIM2,TIM_TRGOSource_OC1Ref);
				break;
			case 2: 
				printf("\r\nTIM2 CH2 as trigger output:\r\n");
				TIM_OC2Init(TIM2, &TIM_OCInitStructure);
				TIM_SelectOCxM(TIM2, TIM_Channel_2, TIM_OCMode_PWM2);
				TIM_SetCompare2(TIM2,1500);
			    TIM_SelectOutputTrigger(TIM2,TIM_TRGOSource_OC2Ref);
				break;
			case 3: 
				printf("\r\nTIM2 CH3 as trigger output:\r\n");
				TIM_OC3Init(TIM2, &TIM_OCInitStructure);
				TIM_SelectOCxM(TIM2, TIM_Channel_3, TIM_OCMode_PWM2);
				TIM_SetCompare3(TIM2,1500);
			    TIM_SelectOutputTrigger(TIM2,TIM_TRGOSource_OC3Ref);
				break;
			default:
				break;
		}		
//		TIM2_PWMOUT_Init(TIM_OCMode_PWM2);
//		TIM_SelectOCxM(TIM2, TIM_Channel_2, TIM_OCMode_PWM2);
//		TIM_CCxCmd(TIM2, TIM_Channel_2, TIM_CCx_Enable);	//��Ӧ��ʹ�ܣ�����������ŷ���������ᷴ��������
//		TIM_SetCompare2(TIM2,1500);
		
		TIM_SelectInputTrigger(TIM1, TIM_TS_ITR1);	  //TIM1��Ϊ��ģʽ��TIM3��Ϊ��ģʽʱӦѡITR2
//		TIM_SelectOutputTrigger(TIM2,TIM_TRGOSource_OC2Ref);   //ѡ��PWM�ο��ź�0C2REF��Ϊ����
		TIM_SelectSlaveMode(TIM1, TIM_SlaveMode_Reset);
		TIM_SelectMasterSlaveMode(TIM2,TIM_MasterSlaveMode_Enable);  //��������ģʽ	
		
		TIM1_PWMOUT_Init(TIM_OCMode_PWM1);
		TIM_SetCompare1(TIM1,400);	
		TIM_CCPreloadControl(TIM1, ENABLE);  //��CC1E CC1NE OC1MԤװ��ʹ�ܴ�
		TIM_SelectCOM(TIM1,ENABLE);  //��com��TRGI�����ظ��²���/�ȽϿ���λ
		
		TIM_Cmd(TIM1, ENABLE);
		TIM_Cmd(TIM3, ENABLE);
		TIM_Cmd(TIM2, ENABLE);

		delay_ms(1000);
		TIM_Cmd(TIM1, DISABLE);
		TIM_Cmd(TIM3, DISABLE);
		TIM_Cmd(TIM2, DISABLE);
	}
}	
	
	   