#include "include.h"

extern int dir;  //�����־����

u8 BufferData[ROW][LINE];
u8 HERF_Count=0;
extern u8 Is_Finish;

u8 BufferData1[ROW][LINE];
//u8 HERF_Count1=0;
//extern u8 Is_Finish1;

void DMA_CH1_Handler(void)
{
    DMA_IRQ_CLEAN(DMA_CH1);  //���ͨ�������жϱ�־λ(���������ٴν����ж�)
      
    DMA_DIS(DMA_CH1);        //�ɼ���H�����ݺ�������DMA�жϣ�ֹͣDMA���䡣���ж��д�DMA���� 
     
    if(HERF_Count >= ROW)         //���ڵ����õ�
    {
        Is_Finish=1;
        disable_irq(91); //�����ж�
        enable_irq(87); //�����ж�
        gpio_set(PORTE,26,0);
    }

}

void DMA_CH3_Handler(void)
{
    DMA_IRQ_CLEAN(DMA_CH3);  //���ͨ�������жϱ�־λ(���������ٴν����ж�)
       
    DMA_DIS(DMA_CH3);        //�ɼ���H�����ݺ�������DMA�жϣ�ֹͣDMA���䡣���ж��д�DMA���� 
     
    /*if(HERF_Count1 >= ROW)         //���ڵ����õ�
    {
      Is_Finish1 = 1;
      disable_irq(89);               //�����ж�2
      enable_irq(88);                //�����ж�2
      gpio_set(PORTE,27,0);
    } */
}



/*************************************************************************
* �������ƣ�PORTA_IRQHandler
* ����˵�������жϷ�������PTA26
* ����˵������
* �������أ���
*************************************************************************/

void PORTA_IRQHandler(void)
{  
  u8  n=26;//���ź�
  
    if(PORTA_ISFR & (1<<n))            //PTA26 �����ж�
    {    
       PORTA_ISFR |= (1<<n);           //д 1 ���жϱ�־λ
       
       disable_irq(87);            //�س��ж�
       enable_irq(91);             //�����ж�
  
       DMA_TCD1_DADDR = (u32)BufferData;

       Is_Finish = 0;
       HERF_Count=0;             //�вɼ��������� 
    }

}

/*************************************************************************
* �������ƣ�PORTE_IRQHandler
* ����˵�������жϷ�������PTE10
* ����˵������
* �������أ���
*************************************************************************/

void PORTE_IRQHandler(void)    
{  
  u8 n=10;                             //���ź�
  if(PORTE_ISFR & (1<<n))              //PTE10 �����ж�
  {
    PORTE_ISFR |= (1<<n);              //д1���жϱ�־λ
    { 
       //DMA_TCD1_DADDR = (u32)BufferData + (u32)(LINE * HERF_Count);
       HERF_Count++;
       DMA_EN(DMA_CH1);  //ʹ��ͨ��CHn Ӳ������
    }
  }
} 

/*************************************************************************
* �������ƣ�PORTB_IRQHandler
* ����˵�������ж�2��������PTB19
* ����˵������
* �������أ���
*************************************************************************/

void PORTB_IRQHandler(void)/*����ͷ2*/
{  
    u8 n=19;//���ź�
                                    
    if(PORTB_ISFR & (1<<n))            //PTB19 �����ж�
    {    
      PORTB_ISFR |= (1<<n);           //д 1 ���жϱ�־λ
       
       /*disable_irq(88);            //�س��ж�2  PORTB
       enable_irq(89);             //�����ж�2   PORTC
       
       DMA_TCD3_DADDR = (u32)BufferData1;

       Is_Finish1=0;
       HERF_Count1=0;             //�вɼ���������*/
     
    }
  
}

/*************************************************************************
* �������ƣ�PORTC_IRQHandler
* ����˵�������ж�2��������PTC19
* ����˵������
* �������أ���
*************************************************************************/

void PORTC_IRQHandler(void)    
{  
  u8 n=19;                             //���ź�
  if(PORTC_ISFR & (1<<n))              //PTC19 �����ж�
  {    
    PORTC_ISFR |= (1<<n);              //д1���жϱ�־λ
         
     /* HERF_Count1++;
      DMA_EN(DMA_CH3);  //ʹ��ͨ��CHn Ӳ������*/
  } 
}

/*************************************************************************
* �������ƣ�PIT0_IRQHandler
* ����˵����PIT0 ��ʱ�жϷ�����������10ms
* ����˵������
* �������أ���
*************************************************************************/
u16 time=0,time_copy=0;  //��¼������������������ʱ��,time_copyΪ���Ӵ�������ʱʱ��
u16 time_last=0,time_copy_last=0;

u8 dangerous=0;  
u8 dangerous_copy=0;

u8 sample_start=0;  //��ʼ��ʱ��־
u8 sample_start_copy=0;

/*test*/
u8 distance_turn=0;  //�жϺ����ﴦ�����������ֵΣ�ձ�־
u8 tt=0;
u8 voice_sum=0;
u8 ook=0;
u8 voice[5]={0};
/*test*/

u16 Int_1ms=0;

void PIT0_IRQHandler(void)  //����1ms
{
   Int_1ms++;
   PIT_Flag_Clear(PIT0);//���жϱ�־ 
  
   if(Int_1ms==1)  //  ����������ڵ�1ms-��4ms���ڽ���
   {
      time_copy_last=time_copy;time_last=time;
      
         gpio_set(PORTA,17,1);  //��trig����ߵ�ƽ
         gpio_set(PORTC,0,1);
         delayus(20);//���20us��ʱ
         gpio_set(PORTA,17,0);
         gpio_set(PORTC,0,0);
         sample_start=0; sample_start_copy=0;
         time=0; time_copy=0;  //ʱ���ۼӱ�����0
         dangerous=0; dangerous_copy=0;
         enable_irq(69); //��ʱ���ж�1��
         enable_irq(70);
   }
   if(Int_1ms==7)
   {
       disable_irq(69); disable_irq(70);
       //uart_putchar (UART4,time_copy);
     /*test*/
     if(dangerous==1 && dangerous_copy==1 ) //turn right
     {  ook=1;  }
     else
     {  ook=0;  }
     
     voice[tt++]=ook;  //��¼���5��ook��ֵ
     if(tt==5)
     {  tt=0;  } 
     
     u8 ti=0;
     voice_sum=0;
     for(ti=0;ti<5;ti++) 
     {  voice_sum=voice_sum+voice[ti];  }
     
     if(voice_sum>2)
     {  distance_turn=1; }//�жϺ����ﴦ�����������ֵΣ�ձ�־
     else
     {  distance_turn=0; }
  /*test*/
     
   }
   if(Int_1ms==8)
   {
      if(Start)
      {  Get_Speed();   //�������ǰ��ȡ�ٶ�
         Speed_Control();  //������ƣ�����10ms
      }
      else
      {  FTM_PWM_Duty(FTM0, CH0, 5000 );
         FTM_PWM_Duty(FTM0, CH1, 5000 );
      } 
     
     
   }
   if(Int_1ms==10)
   {
      Int_1ms=0;
   }
}

/*************************************************************************
* �������ƣ�PIT1_IRQHandler
* ����˵����PIT1 ��ʱ�жϷ�����������100us
* ����˵������
* �������أ���
*************************************************************************/
//u8 a7_in=0;  //ǰ�߳����� ECHO ��
//u8 a6_in=0;

u8 b0_in=0;  //��߳����� ECHO ��
u8 c6_in=0;

void PIT1_IRQHandler(void)  //����100us
{
   PIT_Flag_Clear(PIT1);//���жϱ�־
  
      b0_in=gpio_get(PORTB, 0);
      if( b0_in==1 ) 
      {  sample_start=1;
         time++;  //��¼������������������ʱ��
         if(time>40) //������������Զ
         {  //time=0;
            disable_irq(69); //�ر�PIT1��ʱ���ж�
         }
       }
       else if( sample_start==1 && time<40 && b0_in==0)  
       {  dangerous=1;  //Σ�վ����־
          //time=0;
          disable_irq(69); //�ر�PIT1��ʱ���ж�
       }
}

void PIT2_IRQHandler(void)  //����100us
{
   PIT_Flag_Clear(PIT2);//���жϱ�־
  
   c6_in=gpio_get(PORTC, 6);
   if( c6_in==1 )
   {  sample_start_copy=1;
      time_copy++;  //��¼������������������ʱ��
      if(time_copy>40) //������������Զ
      {  //time_copy=0;
         disable_irq(70); //�ر�PIT2��ʱ���ж�
      }
    }
    else if( sample_start_copy==1 && time_copy<40 && c6_in==0)  
    {  dangerous_copy=1;  //Σ�վ����־
       //time_copy=0;
       disable_irq(70); //�ر�PIT2��ʱ���ж�
    }
}
/*************************************************************************
* Ұ��Ƕ��ʽ���������� 
* �������ƣ�FTM1_IRQHandler
* ����˵����FTM1���벶׽�жϷ�����
* ����˵������
* �������أ���
* �޸�ʱ�䣺2012-2-25
* �� ע�����ź���Ҫ�����Լ���ʼ�����޸ģ��ο����еĴ�������Լ��Ĺ���

*************************************************************************/ 
//u8 red_right=0;
u8 red_right_flag=0;

/*void FTM2_IRQHandler()
{}*/
/*{
    u8 s=FTM2_STATUS; //��ȡ��׽�ͱȽ�״̬ 
    //All CHnF bits can be checked using only one read of STATUS.
    //u8 CHn; 
    FTM2_STATUS=0x00; //���жϱ�־λ

    //CHn=4;  
    if( s & (  (1<<6) || (1<<7) )  ) //FTM2-CH4,CH5,CH6,CH7
    { 
        //FTM_IRQ_DIS(FTM2,4); //��ֹ���벶׽�ж� 
        //FTM_IRQ_DIS(FTM2,5);
     // FTM_IRQ_DIS(FTM2,6);FTM_IRQ_DIS(FTM2,7);
       
        //red_right_flag=1;  //��⵽�źŸ�1
        
        
        //FTM_IRQ_EN(FTM2,4); //�������벶׽�ж�
        //FTM_IRQ_EN(FTM2,5); 
        //FTM_IRQ_EN(FTM2,6); FTM_IRQ_EN(FTM2,7); 
     }
 
        //FTM_CnSC_REG(FTM2_BASE_PTR,CHn) &= ~FTM_CnSC_CHIE_MASK; //��ֹ���벶׽�ж�

    
}
*/