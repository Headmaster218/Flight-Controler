#include "timer.h"

void TIM3_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //ʱ��ʹ��

  TIM_TimeBaseStructure.TIM_Period = arr; //�����Զ���װ�ؼĴ�������ֵ
  TIM_TimeBaseStructure.TIM_Prescaler =(psc-1);//����Ԥ��Ƶֵ
  TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���ģʽ
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;//�ظ���������
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //������ʼ��
  TIM_ClearFlag(TIM1, TIM_FLAG_Update);//���жϱ�־λ

  TIM_ITConfig(      //ʹ�ܻ���ʧ��ָ����TIM�ж�
    TIM1,            //TIM1
    TIM_IT_Update  | //TIM �����ж�Դ
    TIM_IT_Trigger,  //TIM �����ж�Դ 
    ENABLE  	     //ʹ��
    );
	
  //�������ȼ�
  NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;  
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//��ռ���ȼ�0��
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  	   //�����ȼ�0��
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); 

  TIM_Cmd(TIM1, ENABLE);  //ʹ��TIMx����
}
