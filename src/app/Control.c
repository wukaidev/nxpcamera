#include "include.h"

/*************************************************************************
* 函数名称：Speed_Control
* 功能说明：速度控制，采用PID控制（没有采用差速）
* 参数说明：Speed_Set  设定速度
            Spe_Kp     速度环比例项
            Spe_Ki     速度环积分项
            Spe_Kd     速度环微分项
* 函数返回：无
*************************************************************************/
//extern u16 y_position,y_position_last,y_position_last2;

float Direction_Control_Out = 0;  //舵机输出值
extern u8 distance_turn;

extern float Speed_Set; //设定速度  210
float Speed_Set_L;
float Speed_Set_R;
float Speed_Kp =15;//7.5
extern float Speed_Ki;
float Speed_Kd = 10;
extern int brake;

int S_err_sum_L=0;            //速度环I项积分
int S_err_sum_R=0;

float Speed_Error_L = 0;
float Speed_Error_R = 0;
float static Speed_Error_L_Last=0;
float static Speed_Error_R_Last=0;
float Speed_nP_L = 0;
float Speed_nP_R = 0;
float Speed_nD_L=0;
float Speed_nD_R=0;
float  Speed_Control_Out_L=0;
float  Speed_Control_Out_R=0;
extern  u16 time,time_copy,time_last,time_copy_last;
u8 bizhangcishu=0;
extern int move;  //move是图像左移的距离
/********方向环变量******/
u16 jiansujuli=100;  //离灯近时减速


void Speed_Control()
{    
   //if(dir0==-260)

  if( (dir0 == -(LINE/2) - move)|| (distance_turn==1 ) || y_position > jiansujuli  )    
    {
      Speed_Set_L=brake;//Speed_Set*(0.4);
      Speed_Set_R=brake;//Speed_Set*(0.4);
    }
    else
    {
      Speed_Set_L=Speed_Set;
      Speed_Set_R=Speed_Set;
    }

     
  
    Speed_Error_L = Speed_Set_L -Left_Pulse; // Feedback_Speed_L ;//编码器读进来的脉冲值是正值，直接相减判断偏差
    Speed_Error_R = Speed_Set_R -Right_Pulse;//Right_Pulse;  
    Speed_nP_L = Speed_Error_L;
    Speed_nP_R = Speed_Error_R; 
    /**********速度控制I项**********/   
    S_err_sum_L+=Speed_nP_L;    
    if(S_err_sum_L>5000)S_err_sum_L=5000;                      //积分限幅
    if(S_err_sum_L<-5000)S_err_sum_L=-5000;
    S_err_sum_R+=Speed_nP_R;
    if(S_err_sum_R>5000)S_err_sum_R=5000;                      //积分限幅
    if(S_err_sum_R<-5000)S_err_sum_R=-5000;
    /**********速度控制D项**********/ 
    Speed_nD_L= Speed_Error_L - Speed_Error_L_Last;
    Speed_nD_R= Speed_Error_R - Speed_Error_R_Last;
    
    Speed_Control_Out_L += Speed_Kp*Speed_nP_L + Speed_Ki*S_err_sum_L + Speed_Kd*Speed_nD_L;  //左右电机速度输出
    Speed_Control_Out_R += Speed_Kp*Speed_nP_R + Speed_Ki*S_err_sum_R + Speed_Kd*Speed_nD_R; 

    Speed_Error_L_Last=Speed_Error_L;
    Speed_Error_R_Last=Speed_Error_R;

  
  if(Speed_Control_Out_L>4500.)
    Speed_Control_Out_L=4500.;
  
  if(Speed_Control_Out_L<0.)
    Speed_Control_Out_L=0.;
  
  if(Speed_Control_Out_R>4500.)
    Speed_Control_Out_R=4500.;
  if(Speed_Control_Out_R<0.)
    Speed_Control_Out_R=0.;

  // FTM_PWM_Duty(FTM0, CH1, 8000 );
  // FTM_PWM_Duty(FTM0, CH0, 8000 );
 
  
  /*test  避障倒车*/
  if( (time_copy<3 && time_copy_last<3 ) || ( time<3 && time_last<3 ) )  //撞灯时
  {  bizhangcishu=1;  }
  
  if( bizhangcishu!=0 )
  {
    bizhangcishu++;
    if( bizhangcishu==16 )  //倒车5次
    {  bizhangcishu=0;  }
    //daoche=1;
    FTM_PWM_Duty(FTM0, CH1, 3500 );  //倒车
    FTM_PWM_Duty(FTM0, CH0, 3500 );
  }
  else
  {
    FTM_PWM_Duty(FTM0, CH1, (u16)( 5000 + Speed_Control_Out_L ) );
    FTM_PWM_Duty(FTM0, CH0, (u16)( 5000 + Speed_Control_Out_R ) );
  }
  
  //FTM_PWM_Duty(FTM0, CH1, (u16)( 5000 + Speed_Control_Out_L ) );  //不执行避障
 // FTM_PWM_Duty(FTM0, CH0, (u16)( 5000 + Speed_Control_Out_R ) );

}

/*************************************************************************
* 函数名称：Direction_Control
* 功能说明：方向控制，采用PD位置式控制
* 参数说明：Dir_Kp     方向环比例项
            Dir_Kd     方向环微分项
* 函数返回：无
*************************************************************************/

//u8 tt=0;
//u8 voice_sum=0;
//u8 ook=0;
//u8 voice[5]={0};


float Direction_P=0;  //不要在这里改动没有用
extern float Direction_dP;  //拨码开关改变Direction_P的值
float Direction_kD=4;

float Direction_Error;

float Direction_Error_Last = 0;

float Direction_nP = 0;
float Direction_nD = 0;

//u16 location=92;//数值给定40的时候距离约为30cm
//u16 location1=30;//如果给55 就拐弯过于大 60挺好的
extern u8 dangerous; 
extern u8 dangerous_copy; 

extern u8 red_right_flag;
int red_right_sum=0;  //检测到时右拐直到看到信标
extern u8 pianyi;

void Direction_Control()
{
  Direction_Error=dir0; 

  Direction_nD=Direction_Error - Direction_Error_Last;

 if( Direction_Error > 0 )  //图像处理结果左移后，右拐时拐弯 P 项增大(待测)
  {  Direction_P=Direction_dP;  }  //2.0
  else  Direction_P=0.8;
 
   if(pianyi==2||pianyi==3)
   {
      if( Direction_Error > 0 )  //图像处理结果左移后，右拐时拐弯 P 项增大(待测)
      {  Direction_P=0.8;  }  //2.0
      else  Direction_P=Direction_dP;
   }
  Direction_Control_Out = 1445-(Direction_P * Direction_Error + Direction_kD * Direction_nD);
  Direction_Error_Last=Direction_Error;
  
  if( (pianyi==2||pianyi==3) &&  dir0 == -(LINE/2) - move  )
   {  Direction_Control_Out=Direction_Control_Out_MIN; } //看不到灯时右拐
  /*超声波滤波添加*/
  //当location小于某一值时才起作用
 
      /*if(dangerous==1 && dangerous_copy==1 ) //turn right
      {
          //Direction_Control_Out = Direction_Control_Out_MAX;
          //gpio_set (PORTE, 28, 1);  //蜂鸣器置高电平
          ook=1;   
      }
      else
      {  //gpio_set (PORTE, 28, 0);
         ook=0;
      }

      
   voice[tt++]=ook;  //记录最近5次ook的值
    
   if(tt==5)
   {  tt=0;  } 
  u8 ti=0;
  voice_sum=0;
  for(ti=0;ti<5;ti++) 
  {  voice_sum=voice_sum+voice[ti];  }
  
  if(voice_sum>2)  //累计五次中大于俩次 dangerous为1 
  { 
  //   if(y_position<50)
  //   {
        Direction_Control_Out = Direction_Control_Out_MAX;
        gpio_set (PORTE, 28, 1);  //蜂鸣器置高电平    
   //  }
    // else  gpio_set (PORTE, 28, 0);
  }
  else
  {  gpio_set (PORTE, 28, 0);  }

  */
  
  if( distance_turn==1 )  //危险距离
  {  Direction_Control_Out = Direction_Control_Out_MAX;
  
       if(pianyi==2||pianyi==3)  //右拐避障
         Direction_Control_Out=Direction_Control_Out_MIN;
     //gpio_set (PORTE, 28, 1);  //蜂鸣器置高电平     
  }
  //else
  //{   gpio_set (PORTE, 28, 0);  
  //}
  
  //避障
  if( bizhangcishu!=0 )  //倒车时右拐, bizhangcishu 值在电机输出函数里改变
  {  Direction_Control_Out=Direction_Control_Out_MIN;
     //gpio_set (PORTE, 28, 1);
     
     if(pianyi==2||pianyi==3)  //倒车左拐  反向
        Direction_Control_Out=Direction_Control_Out_MAX;
  }
  /*else
  {   //gpio_set (PORTE, 28, 0);
  }*/
  
  if(pianyi!=2 && pianyi!=3)  // pianyi=0,1时才执行
  {
     if( (dir0 == -(LINE/2) - move )  &&  red_right_flag==1)//摄像头看不到灯时
     { 
       red_right_sum=1;
     }
     if(red_right_sum!=0  )
     {
       if(dir0 == -(LINE/2) - move )
       {  Direction_Control_Out=Direction_Control_Out_MIN; } //右拐
       if(dir0 != -(LINE/2) - move )
       { red_right_sum=0; }
     }
  }
  


  //red_right_flag=0;  //红外管检测信号变量置0
    
  /*舵机输出值限幅*/
  if (Direction_Control_Out > Direction_Control_Out_MAX)   //1550 
      Direction_Control_Out = Direction_Control_Out_MAX;
  else if (Direction_Control_Out < Direction_Control_Out_MIN)  //1330
      Direction_Control_Out = Direction_Control_Out_MIN;


  FTM_PWM_Duty(FTM1, CH0, (u16)(Direction_Control_Out));
  
}

    