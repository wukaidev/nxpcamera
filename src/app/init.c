#include "include.h"

u8 Is_Finish=0;
u8 Is_Finish1=0;
extern u8 BufferData[ROW][LINE];
extern u8 BufferData1[ROW][LINE];
#define TIME 1000  //定时器中断1 周期10us

/*初始化*/
void pro_init(void)
{
  set_irq_priority(87, 0);             //PORTA  设置场中断优先级
  set_irq_priority(91, 0);             //PORTE  设置行中断优先级
  set_irq_priority(64, 0);
  //set_irq_priority(88, 0);             //PORTB  设置场中断2优先级
  //set_irq_priority(89, 0);             //PORTC  设置行中断2优先级
  
  set_irq_priority(68, 2);             //PIT0   设置定时器中断优先级
  set_irq_priority(69, 1);             //PIT1   设置定时器1中断优先级 
  set_irq_priority(70, 1);             //PIT1   设置定时器1中断优先级
  
  DisableInterrupts;                   //关闭总中断
  
  pit_init_ms(PIT0,1);                //定时器初始化，周期1ms
  pit_init(PIT1,10000);  //周期100us
  pit_init(PIT2,10000);
  
  FTM_PWM_init(FTM0, CH0, 10000, 5000);  //左电机，PWM，10KHz，50%占空比，PTC1
  FTM_PWM_init(FTM0, CH1, 10000, 5000);  //右电机，PWM，10KHz，50%占空比，PTC3
  
  FTM_PWM_init(FTM1, CH0, 100, 1445);    //舵机，PWM ，100HZ，PTA8  1450待定
  
  //FTM_Input_init(FTM2,CH4,Falling);  // PTD4  捕捉红外接收头下降沿
  //FTM_Input_init(FTM2,CH5,Falling);  // PTD5
  //FTM_Input_init(FTM2,CH6,Rising_or_Falling);  // PTD6
  //FTM_Input_init(FTM2,CH7,Rising_or_Falling);  // PTD7
                                   
  DMA_count_Init(DMA_CH4, PTB22, 0xFFFE, DMA_rising);   //左编码器脉冲计数，PTB22  
  DMA_count_Init(DMA_CH5, PTC5, 0xFFFE, DMA_rising);   //右编码器脉冲计数，PTC5
 
   gpio_init(PORTE, 28, GPO, 0);  //蜂鸣器
  
  //gpio_init (PORTA, 7, GPI, LOW);
   //gpio_init (PORTA, 6, GPI, LOW);  //添加第二个超声波计时
  //gpio_init (PORTA, 17, GPO, LOW); //TRIG接A17, ECHO接A7

  gpio_init (PORTB, 0, GPI, LOW);
   gpio_init (PORTC, 6, GPI, LOW);  //添加第二个超声波计时
  gpio_init (PORTC, 0, GPO, LOW); //TRIG1接C0, ECHO1接B0
  
  gpio_init(PORTB, 7, GPI, 0);         //左编码器辨向输入，PTB7
  gpio_init(PORTB, 9, GPI, 0);         //右编码器辨向输入，PTB9
   
  gpio_init(PORTE, 0, GPI, 0);         //摄像头8个数据口，PTE0~PTE7
  gpio_init(PORTE, 1, GPI, 0);
  gpio_init(PORTE, 2, GPI, 0);
  gpio_init(PORTE, 3, GPI, 0);
  gpio_init(PORTE, 4, GPI, 0);
  gpio_init(PORTE, 5, GPI, 0);
  gpio_init(PORTE, 6, GPI, 0);
  gpio_init(PORTE, 7, GPI, 0);
  
  /*gpio_init(PORTD, 0, GPI, 0);         //摄像头2 8个数据口，PTD0~PTD7
  gpio_init(PORTD, 1, GPI, 0);
  gpio_init(PORTD, 2, GPI, 0);
  gpio_init(PORTD, 3, GPI, 0);
  gpio_init(PORTD, 4, GPI, 0);
  gpio_init(PORTD, 5, GPI, 0);*/
  gpio_init(PORTD, 6, GPI, 0);
  gpio_init(PORTD, 7, GPI, 0);
  
  uart_init(UART4,115200);       //串口初始化，波特率:9600，TXD：PTD2，RXD：PTD3
  
  gpio_init (PORTA, 28, GPO, LOW);
  
  gpio_init(PORTE, 12, GPI, 0);         //PCLK--PTE12
  PORTE_PCR12|=PORT_PCR_IRQC(1);   
  DMA_transmit_init((void *)&GPIOE_PDIR, BufferData,LINE,53);
  //DMA_PORTx2BUFF_Init (DMA_CH1, (void *)&PTE_BYTE0_IN, BufferData[0], PTE12, DMA_BYTE1, LINE, DMA_rising_keepon);
  
  /*gpio_init(PORTD, 9, GPI, 0);         //PCLK2--PTD9
  PORTD_PCR9|=PORT_PCR_IRQC(1);   //确定触发模式
  DMA_transmit_init1((void *)&GPIOD_PDIR, BufferData1, LINE ,52);*/
  
  exti_init(PORTA, 26, rising_up);     //VSYNC,场中断，PTA26
  exti_init(PORTE, 10, rising_up);     //HERF,行中断，PTE10 
  
  //exti_init(PORTB, 19, rising_up);     //VSYNC,场中断，PTB19
  //exti_init(PORTC, 19, rising_up);     //HERF,行中断，PTC18  
 
  EnableInterrupts;                    //开启总中断

  disable_irq(91);                     //关行中断 PORTE
  //disable_irq(89);                     //关行中断2 PORTC
  
  disable_irq(69); //关闭PIT1定时器中断
  disable_irq(70);
  
  // 摄像头 RST 引脚
  gpio_init (PORTE, 26, GPO, LOW);  gpio_init (PORTE, 27, GPO, LOW);
     time_delay_ms(1000);
  gpio_set(PORTE,26,1); gpio_set(PORTE,27,1);
     time_delay_ms(1000);
  gpio_set(PORTE,26,0); gpio_set(PORTE,27,0);
  //拨码开关
  gpio_init(PORTA, 9, GPI, 0);  // 第1位
  gpio_init(PORTA, 11, GPI, 0);
  gpio_init(PORTA, 13, GPI, 0);
  gpio_init(PORTA, 15, GPI, 0); 

}

/****发送图像****/
void SendImage(void)
{
 // DisableInterrupts;                 //关闭总中断  
  u16 i=0,j=0;
  uart_putchar(UART4,0xFF);//图像头  
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
 // EnableInterrupts;                    //开启总中断
}

void SendImage1(void)
{
  //DisableInterrupts;                   //关闭总中断 
  u16 i=0,j=0;
  uart_putchar(UART4,0xFF);//图像头 
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
 // EnableInterrupts;                    //开启总中断
  
}
