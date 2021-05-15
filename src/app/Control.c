#include "include.h"

/*************************************************************************
* �������ƣ�Speed_Control
* ����˵�����ٶȿ��ƣ�����PID���ƣ�û�в��ò��٣�
* ����˵����Speed_Set  �趨�ٶ�
            Spe_Kp     �ٶȻ�������
            Spe_Ki     �ٶȻ�������
            Spe_Kd     �ٶȻ�΢����
* �������أ���
*************************************************************************/
//extern u16 y_position,y_position_last,y_position_last2;

float Direction_Control_Out = 0;  //������ֵ
extern u8 distance_turn;

extern float Speed_Set; //�趨�ٶ�  210
float Speed_Set_L;
float Speed_Set_R;
float Speed_Kp =15;//7.5
extern float Speed_Ki;
float Speed_Kd = 10;
extern int brake;

int S_err_sum_L=0;            //�ٶȻ�I�����
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
extern int move;  //move��ͼ�����Ƶľ���
/********���򻷱���******/
u16 jiansujuli=100;  //��ƽ�ʱ����


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

     
  
    Speed_Error_L = Speed_Set_L -Left_Pulse; // Feedback_Speed_L ;//������������������ֵ����ֵ��ֱ������ж�ƫ��
    Speed_Error_R = Speed_Set_R -Right_Pulse;//Right_Pulse;  
    Speed_nP_L = Speed_Error_L;
    Speed_nP_R = Speed_Error_R; 
    /**********�ٶȿ���I��**********/   
    S_err_sum_L+=Speed_nP_L;    
    if(S_err_sum_L>5000)S_err_sum_L=5000;                      //�����޷�
    if(S_err_sum_L<-5000)S_err_sum_L=-5000;
    S_err_sum_R+=Speed_nP_R;
    if(S_err_sum_R>5000)S_err_sum_R=5000;                      //�����޷�
    if(S_err_sum_R<-5000)S_err_sum_R=-5000;
    /**********�ٶȿ���D��**********/ 
    Speed_nD_L= Speed_Error_L - Speed_Error_L_Last;
    Speed_nD_R= Speed_Error_R - Speed_Error_R_Last;
    
    Speed_Control_Out_L += Speed_Kp*Speed_nP_L + Speed_Ki*S_err_sum_L + Speed_Kd*Speed_nD_L;  //���ҵ���ٶ����
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
 
  
  /*test  ���ϵ���*/
  if( (time_copy<3 && time_copy_last<3 ) || ( time<3 && time_last<3 ) )  //ײ��ʱ
  {  bizhangcishu=1;  }
  
  if( bizhangcishu!=0 )
  {
    bizhangcishu++;
    if( bizhangcishu==16 )  //����5��
    {  bizhangcishu=0;  }
    //daoche=1;
    FTM_PWM_Duty(FTM0, CH1, 3500 );  //����
    FTM_PWM_Duty(FTM0, CH0, 3500 );
  }
  else
  {
    FTM_PWM_Duty(FTM0, CH1, (u16)( 5000 + Speed_Control_Out_L ) );
    FTM_PWM_Duty(FTM0, CH0, (u16)( 5000 + Speed_Control_Out_R ) );
  }
  
  //FTM_PWM_Duty(FTM0, CH1, (u16)( 5000 + Speed_Control_Out_L ) );  //��ִ�б���
 // FTM_PWM_Duty(FTM0, CH0, (u16)( 5000 + Speed_Control_Out_R ) );

}

/*************************************************************************
* �������ƣ�Direction_Control
* ����˵����������ƣ�����PDλ��ʽ����
* ����˵����Dir_Kp     ���򻷱�����
            Dir_Kd     ����΢����
* �������أ���
*************************************************************************/

//u8 tt=0;
//u8 voice_sum=0;
//u8 ook=0;
//u8 voice[5]={0};


float Direction_P=0;  //��Ҫ������Ķ�û����
extern float Direction_dP;  //���뿪�ظı�Direction_P��ֵ
float Direction_kD=4;

float Direction_Error;

float Direction_Error_Last = 0;

float Direction_nP = 0;
float Direction_nD = 0;

//u16 location=92;//��ֵ����40��ʱ�����ԼΪ30cm
//u16 location1=30;//�����55 �͹�����ڴ� 60ͦ�õ�
extern u8 dangerous; 
extern u8 dangerous_copy; 

extern u8 red_right_flag;
int red_right_sum=0;  //��⵽ʱ�ҹ�ֱ�������ű�
extern u8 pianyi;

void Direction_Control()
{
  Direction_Error=dir0; 

  Direction_nD=Direction_Error - Direction_Error_Last;

 if( Direction_Error > 0 )  //ͼ���������ƺ��ҹ�ʱ���� P ������(����)
  {  Direction_P=Direction_dP;  }  //2.0
  else  Direction_P=0.8;
 
   if(pianyi==2||pianyi==3)
   {
      if( Direction_Error > 0 )  //ͼ���������ƺ��ҹ�ʱ���� P ������(����)
      {  Direction_P=0.8;  }  //2.0
      else  Direction_P=Direction_dP;
   }
  Direction_Control_Out = 1445-(Direction_P * Direction_Error + Direction_kD * Direction_nD);
  Direction_Error_Last=Direction_Error;
  
  if( (pianyi==2||pianyi==3) &&  dir0 == -(LINE/2) - move  )
   {  Direction_Control_Out=Direction_Control_Out_MIN; } //��������ʱ�ҹ�
  /*�������˲����*/
  //��locationС��ĳһֵʱ��������
 
      /*if(dangerous==1 && dangerous_copy==1 ) //turn right
      {
          //Direction_Control_Out = Direction_Control_Out_MAX;
          //gpio_set (PORTE, 28, 1);  //�������øߵ�ƽ
          ook=1;   
      }
      else
      {  //gpio_set (PORTE, 28, 0);
         ook=0;
      }

      
   voice[tt++]=ook;  //��¼���5��ook��ֵ
    
   if(tt==5)
   {  tt=0;  } 
  u8 ti=0;
  voice_sum=0;
  for(ti=0;ti<5;ti++) 
  {  voice_sum=voice_sum+voice[ti];  }
  
  if(voice_sum>2)  //�ۼ�����д������� dangerousΪ1 
  { 
  //   if(y_position<50)
  //   {
        Direction_Control_Out = Direction_Control_Out_MAX;
        gpio_set (PORTE, 28, 1);  //�������øߵ�ƽ    
   //  }
    // else  gpio_set (PORTE, 28, 0);
  }
  else
  {  gpio_set (PORTE, 28, 0);  }

  */
  
  if( distance_turn==1 )  //Σ�վ���
  {  Direction_Control_Out = Direction_Control_Out_MAX;
  
       if(pianyi==2||pianyi==3)  //�ҹձ���
         Direction_Control_Out=Direction_Control_Out_MIN;
     //gpio_set (PORTE, 28, 1);  //�������øߵ�ƽ     
  }
  //else
  //{   gpio_set (PORTE, 28, 0);  
  //}
  
  //����
  if( bizhangcishu!=0 )  //����ʱ�ҹ�, bizhangcishu ֵ�ڵ�����������ı�
  {  Direction_Control_Out=Direction_Control_Out_MIN;
     //gpio_set (PORTE, 28, 1);
     
     if(pianyi==2||pianyi==3)  //�������  ����
        Direction_Control_Out=Direction_Control_Out_MAX;
  }
  /*else
  {   //gpio_set (PORTE, 28, 0);
  }*/
  
  if(pianyi!=2 && pianyi!=3)  // pianyi=0,1ʱ��ִ��
  {
     if( (dir0 == -(LINE/2) - move )  &&  red_right_flag==1)//����ͷ��������ʱ
     { 
       red_right_sum=1;
     }
     if(red_right_sum!=0  )
     {
       if(dir0 == -(LINE/2) - move )
       {  Direction_Control_Out=Direction_Control_Out_MIN; } //�ҹ�
       if(dir0 != -(LINE/2) - move )
       { red_right_sum=0; }
     }
  }
  


  //red_right_flag=0;  //����ܼ���źű�����0
    
  /*������ֵ�޷�*/
  if (Direction_Control_Out > Direction_Control_Out_MAX)   //1550 
      Direction_Control_Out = Direction_Control_Out_MAX;
  else if (Direction_Control_Out < Direction_Control_Out_MIN)  //1330
      Direction_Control_Out = Direction_Control_Out_MIN;


  FTM_PWM_Duty(FTM1, CH0, (u16)(Direction_Control_Out));
  
}

    