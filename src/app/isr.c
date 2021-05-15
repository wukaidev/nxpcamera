#include "include.h"

extern int dir;  //方向标志变量

u8 BufferData[ROW][LINE];
u8 HERF_Count=0;
extern u8 Is_Finish;

u8 BufferData1[ROW][LINE];
//u8 HERF_Count1=0;
//extern u8 Is_Finish1;

void DMA_CH1_Handler(void)
{
    DMA_IRQ_CLEAN(DMA_CH1);  //清除通道传输中断标志位(这样才能再次进入中断)
      
    DMA_DIS(DMA_CH1);        //采集完H个数据后进入这个DMA中断，停止DMA传输。行中断中打开DMA传输 
     
    if(HERF_Count >= ROW)         //串口调试用到
    {
        Is_Finish=1;
        disable_irq(91); //关行中断
        enable_irq(87); //开场中断
        gpio_set(PORTE,26,0);
    }

}

void DMA_CH3_Handler(void)
{
    DMA_IRQ_CLEAN(DMA_CH3);  //清除通道传输中断标志位(这样才能再次进入中断)
       
    DMA_DIS(DMA_CH3);        //采集完H个数据后进入这个DMA中断，停止DMA传输。行中断中打开DMA传输 
     
    /*if(HERF_Count1 >= ROW)         //串口调试用到
    {
      Is_Finish1 = 1;
      disable_irq(89);               //关行中断2
      enable_irq(88);                //开场中断2
      gpio_set(PORTE,27,0);
    } */
}



/*************************************************************************
* 函数名称：PORTA_IRQHandler
* 功能说明：场中断服务函数，PTA26
* 参数说明：无
* 函数返回：无
*************************************************************************/

void PORTA_IRQHandler(void)
{  
  u8  n=26;//引脚号
  
    if(PORTA_ISFR & (1<<n))            //PTA26 触发中断
    {    
       PORTA_ISFR |= (1<<n);           //写 1 清中断标志位
       
       disable_irq(87);            //关场中断
       enable_irq(91);             //开行中断
  
       DMA_TCD1_DADDR = (u32)BufferData;

       Is_Finish = 0;
       HERF_Count=0;             //行采集计数清零 
    }

}

/*************************************************************************
* 函数名称：PORTE_IRQHandler
* 功能说明：行中断服务函数，PTE10
* 参数说明：无
* 函数返回：无
*************************************************************************/

void PORTE_IRQHandler(void)    
{  
  u8 n=10;                             //引脚号
  if(PORTE_ISFR & (1<<n))              //PTE10 触发中断
  {
    PORTE_ISFR |= (1<<n);              //写1清中断标志位
    { 
       //DMA_TCD1_DADDR = (u32)BufferData + (u32)(LINE * HERF_Count);
       HERF_Count++;
       DMA_EN(DMA_CH1);  //使能通道CHn 硬件请求
    }
  }
} 

/*************************************************************************
* 函数名称：PORTB_IRQHandler
* 功能说明：场中断2服务函数，PTB19
* 参数说明：无
* 函数返回：无
*************************************************************************/

void PORTB_IRQHandler(void)/*摄像头2*/
{  
    u8 n=19;//引脚号
                                    
    if(PORTB_ISFR & (1<<n))            //PTB19 触发中断
    {    
      PORTB_ISFR |= (1<<n);           //写 1 清中断标志位
       
       /*disable_irq(88);            //关场中断2  PORTB
       enable_irq(89);             //开行中断2   PORTC
       
       DMA_TCD3_DADDR = (u32)BufferData1;

       Is_Finish1=0;
       HERF_Count1=0;             //行采集计数清零*/
     
    }
  
}

/*************************************************************************
* 函数名称：PORTC_IRQHandler
* 功能说明：行中断2服务函数，PTC19
* 参数说明：无
* 函数返回：无
*************************************************************************/

void PORTC_IRQHandler(void)    
{  
  u8 n=19;                             //引脚号
  if(PORTC_ISFR & (1<<n))              //PTC19 触发中断
  {    
    PORTC_ISFR |= (1<<n);              //写1清中断标志位
         
     /* HERF_Count1++;
      DMA_EN(DMA_CH3);  //使能通道CHn 硬件请求*/
  } 
}

/*************************************************************************
* 函数名称：PIT0_IRQHandler
* 功能说明：PIT0 定时中断服务函数，周期10ms
* 参数说明：无
* 函数返回：无
*************************************************************************/
u16 time=0,time_copy=0;  //记录超声波传感器返回来时间,time_copy为所加传感器计时时间
u16 time_last=0,time_copy_last=0;

u8 dangerous=0;  
u8 dangerous_copy=0;

u8 sample_start=0;  //开始计时标志
u8 sample_start_copy=0;

/*test*/
u8 distance_turn=0;  //中断函数里处理超声波侧回来值危险标志
u8 tt=0;
u8 voice_sum=0;
u8 ook=0;
u8 voice[5]={0};
/*test*/

u16 Int_1ms=0;

void PIT0_IRQHandler(void)  //周期1ms
{
   Int_1ms++;
   PIT_Flag_Clear(PIT0);//清中断标志 
  
   if(Int_1ms==1)  //  超声波测距在第1ms-第4ms以内进行
   {
      time_copy_last=time_copy;time_last=time;
      
         gpio_set(PORTA,17,1);  //给trig输出高电平
         gpio_set(PORTC,0,1);
         delayus(20);//添加20us延时
         gpio_set(PORTA,17,0);
         gpio_set(PORTC,0,0);
         sample_start=0; sample_start_copy=0;
         time=0; time_copy=0;  //时间累加变量清0
         dangerous=0; dangerous_copy=0;
         enable_irq(69); //定时器中断1打开
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
     
     voice[tt++]=ook;  //记录最近5次ook的值
     if(tt==5)
     {  tt=0;  } 
     
     u8 ti=0;
     voice_sum=0;
     for(ti=0;ti<5;ti++) 
     {  voice_sum=voice_sum+voice[ti];  }
     
     if(voice_sum>2)
     {  distance_turn=1; }//中断函数里处理超声波侧回来值危险标志
     else
     {  distance_turn=0; }
  /*test*/
     
   }
   if(Int_1ms==8)
   {
      if(Start)
      {  Get_Speed();   //电机控制前获取速度
         Speed_Control();  //电机控制，周期10ms
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
* 函数名称：PIT1_IRQHandler
* 功能说明：PIT1 定时中断服务函数，周期100us
* 参数说明：无
* 函数返回：无
*************************************************************************/
//u8 a7_in=0;  //前边超声波 ECHO 脚
//u8 a6_in=0;

u8 b0_in=0;  //后边超声波 ECHO 脚
u8 c6_in=0;

void PIT1_IRQHandler(void)  //周期100us
{
   PIT_Flag_Clear(PIT1);//清中断标志
  
      b0_in=gpio_get(PORTB, 0);
      if( b0_in==1 ) 
      {  sample_start=1;
         time++;  //记录超声波传感器返回来时间
         if(time>40) //所测物体距离过远
         {  //time=0;
            disable_irq(69); //关闭PIT1定时器中断
         }
       }
       else if( sample_start==1 && time<40 && b0_in==0)  
       {  dangerous=1;  //危险距离标志
          //time=0;
          disable_irq(69); //关闭PIT1定时器中断
       }
}

void PIT2_IRQHandler(void)  //周期100us
{
   PIT_Flag_Clear(PIT2);//清中断标志
  
   c6_in=gpio_get(PORTC, 6);
   if( c6_in==1 )
   {  sample_start_copy=1;
      time_copy++;  //记录超声波传感器返回来时间
      if(time_copy>40) //所测物体距离过远
      {  //time_copy=0;
         disable_irq(70); //关闭PIT2定时器中断
      }
    }
    else if( sample_start_copy==1 && time_copy<40 && c6_in==0)  
    {  dangerous_copy=1;  //危险距离标志
       //time_copy=0;
       disable_irq(70); //关闭PIT2定时器中断
    }
}
/*************************************************************************
* 野火嵌入式开发工作室 
* 函数名称：FTM1_IRQHandler
* 功能说明：FTM1输入捕捉中断服务函数
* 参数说明：无
* 函数返回：无
* 修改时间：2012-2-25
* 备 注：引脚号需要根据自己初始化来修改，参考现有的代码添加自己的功能

*************************************************************************/ 
//u8 red_right=0;
u8 red_right_flag=0;

/*void FTM2_IRQHandler()
{}*/
/*{
    u8 s=FTM2_STATUS; //读取捕捉和比较状态 
    //All CHnF bits can be checked using only one read of STATUS.
    //u8 CHn; 
    FTM2_STATUS=0x00; //清中断标志位

    //CHn=4;  
    if( s & (  (1<<6) || (1<<7) )  ) //FTM2-CH4,CH5,CH6,CH7
    { 
        //FTM_IRQ_DIS(FTM2,4); //禁止输入捕捉中断 
        //FTM_IRQ_DIS(FTM2,5);
     // FTM_IRQ_DIS(FTM2,6);FTM_IRQ_DIS(FTM2,7);
       
        //red_right_flag=1;  //检测到信号给1
        
        
        //FTM_IRQ_EN(FTM2,4); //开启输入捕捉中断
        //FTM_IRQ_EN(FTM2,5); 
        //FTM_IRQ_EN(FTM2,6); FTM_IRQ_EN(FTM2,7); 
     }
 
        //FTM_CnSC_REG(FTM2_BASE_PTR,CHn) &= ~FTM_CnSC_CHIE_MASK; //禁止输入捕捉中断

    
}
*/