#include "include.h"

u8 Is_Finish=0;
u8 Is_Finish1=0;
extern u8 BufferData[ROW][LINE];
extern u8 BufferData1[ROW][LINE];
#define TIME 1000  //��ʱ���ж�1 ����10us

/*��ʼ��*/
void pro_init(void)
{
  set_irq_priority(87, 0);             //PORTA  ���ó��ж����ȼ�
  set_irq_priority(91, 0);             //PORTE  �������ж����ȼ�
  set_irq_priority(64, 0);
  //set_irq_priority(88, 0);             //PORTB  ���ó��ж�2���ȼ�
  //set_irq_priority(89, 0);             //PORTC  �������ж�2���ȼ�
  
  set_irq_priority(68, 2);             //PIT0   ���ö�ʱ���ж����ȼ�
  set_irq_priority(69, 1);             //PIT1   ���ö�ʱ��1�ж����ȼ� 
  set_irq_priority(70, 1);             //PIT1   ���ö�ʱ��1�ж����ȼ�
  
  DisableInterrupts;                   //�ر����ж�
  
  pit_init_ms(PIT0,1);                //��ʱ����ʼ��������1ms
  pit_init(PIT1,10000);  //����100us
  pit_init(PIT2,10000);
  
  FTM_PWM_init(FTM0, CH0, 10000, 5000);  //������PWM��10KHz��50%ռ�ձȣ�PTC1
  FTM_PWM_init(FTM0, CH1, 10000, 5000);  //�ҵ����PWM��10KHz��50%ռ�ձȣ�PTC3
  
  FTM_PWM_init(FTM1, CH0, 100, 1445);    //�����PWM ��100HZ��PTA8  1450����
  
  //FTM_Input_init(FTM2,CH4,Falling);  // PTD4  ��׽�������ͷ�½���
  //FTM_Input_init(FTM2,CH5,Falling);  // PTD5
  //FTM_Input_init(FTM2,CH6,Rising_or_Falling);  // PTD6
  //FTM_Input_init(FTM2,CH7,Rising_or_Falling);  // PTD7
                                   
  DMA_count_Init(DMA_CH4, PTB22, 0xFFFE, DMA_rising);   //����������������PTB22  
  DMA_count_Init(DMA_CH5, PTC5, 0xFFFE, DMA_rising);   //�ұ��������������PTC5
 
   gpio_init(PORTE, 28, GPO, 0);  //������
  
  //gpio_init (PORTA, 7, GPI, LOW);
   //gpio_init (PORTA, 6, GPI, LOW);  //��ӵڶ�����������ʱ
  //gpio_init (PORTA, 17, GPO, LOW); //TRIG��A17, ECHO��A7

  gpio_init (PORTB, 0, GPI, LOW);
   gpio_init (PORTC, 6, GPI, LOW);  //��ӵڶ�����������ʱ
  gpio_init (PORTC, 0, GPO, LOW); //TRIG1��C0, ECHO1��B0
  
  gpio_init(PORTB, 7, GPI, 0);         //��������������룬PTB7
  gpio_init(PORTB, 9, GPI, 0);         //�ұ������������룬PTB9
   
  gpio_init(PORTE, 0, GPI, 0);         //����ͷ8�����ݿڣ�PTE0~PTE7
  gpio_init(PORTE, 1, GPI, 0);
  gpio_init(PORTE, 2, GPI, 0);
  gpio_init(PORTE, 3, GPI, 0);
  gpio_init(PORTE, 4, GPI, 0);
  gpio_init(PORTE, 5, GPI, 0);
  gpio_init(PORTE, 6, GPI, 0);
  gpio_init(PORTE, 7, GPI, 0);
  
  /*gpio_init(PORTD, 0, GPI, 0);         //����ͷ2 8�����ݿڣ�PTD0~PTD7
  gpio_init(PORTD, 1, GPI, 0);
  gpio_init(PORTD, 2, GPI, 0);
  gpio_init(PORTD, 3, GPI, 0);
  gpio_init(PORTD, 4, GPI, 0);
  gpio_init(PORTD, 5, GPI, 0);*/
  gpio_init(PORTD, 6, GPI, 0);
  gpio_init(PORTD, 7, GPI, 0);
  
  uart_init(UART4,115200);       //���ڳ�ʼ����������:9600��TXD��PTD2��RXD��PTD3
  
  gpio_init (PORTA, 28, GPO, LOW);
  
  gpio_init(PORTE, 12, GPI, 0);         //PCLK--PTE12
  PORTE_PCR12|=PORT_PCR_IRQC(1);   
  DMA_transmit_init((void *)&GPIOE_PDIR, BufferData,LINE,53);
  //DMA_PORTx2BUFF_Init (DMA_CH1, (void *)&PTE_BYTE0_IN, BufferData[0], PTE12, DMA_BYTE1, LINE, DMA_rising_keepon);
  
  /*gpio_init(PORTD, 9, GPI, 0);         //PCLK2--PTD9
  PORTD_PCR9|=PORT_PCR_IRQC(1);   //ȷ������ģʽ
  DMA_transmit_init1((void *)&GPIOD_PDIR, BufferData1, LINE ,52);*/
  
  exti_init(PORTA, 26, rising_up);     //VSYNC,���жϣ�PTA26
  exti_init(PORTE, 10, rising_up);     //HERF,���жϣ�PTE10 
  
  //exti_init(PORTB, 19, rising_up);     //VSYNC,���жϣ�PTB19
  //exti_init(PORTC, 19, rising_up);     //HERF,���жϣ�PTC18  
 
  EnableInterrupts;                    //�������ж�

  disable_irq(91);                     //�����ж� PORTE
  //disable_irq(89);                     //�����ж�2 PORTC
  
  disable_irq(69); //�ر�PIT1��ʱ���ж�
  disable_irq(70);
  
  // ����ͷ RST ����
  gpio_init (PORTE, 26, GPO, LOW);  gpio_init (PORTE, 27, GPO, LOW);
     time_delay_ms(1000);
  gpio_set(PORTE,26,1); gpio_set(PORTE,27,1);
     time_delay_ms(1000);
  gpio_set(PORTE,26,0); gpio_set(PORTE,27,0);
  //���뿪��
  gpio_init(PORTA, 9, GPI, 0);  // ��1λ
  gpio_init(PORTA, 11, GPI, 0);
  gpio_init(PORTA, 13, GPI, 0);
  gpio_init(PORTA, 15, GPI, 0); 

}

/****����ͼ��****/
void SendImage(void)
{
 // DisableInterrupts;                 //�ر����ж�  
  u16 i=0,j=0;
  uart_putchar(UART4,0xFF);//ͼ��ͷ  
  for(i=0;i<ROW;i++)  
  {
    for(j=0;j<LINE;j++)
    {
       if(BufferData[i][j] == 0xFF) 
          BufferData[i][j]--;
      uart_putchar (UART4,BufferData[i][j]);
      //time_delay_ms(1);
    }    
  }
 // EnableInterrupts;                    //�������ж�
}

void SendImage1(void)
{
  //DisableInterrupts;                   //�ر����ж� 
  u16 i=0,j=0;
  uart_putchar(UART4,0xFF);//ͼ��ͷ 
  for(i=0;i<ROW;i++) 
  {  
    for(j=0;j<LINE;j++)
    {
       if(BufferData1[i][j] == 0xFF)
             BufferData1[i][j]--;
      uart_putchar (UART4,BufferData1[i][j]);
      //time_delay_ms(1);
      
    }    
  }
 // EnableInterrupts;                    //�������ж�
  
}
