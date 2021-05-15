#include "include.h"

/*************************************************************************
* 函数名称：主函数
* 功能说明：无
* 参数说明：无
* 函数返回：无
*************************************************************************/
int move=0;
int brake=0;
float Speed_Set=0;
float Direction_dP=0;
float Speed_Ki = 0;
u8 boma0=0,boma1=0,boma2=0,boma3=0,sudu=0,pianyi=0;

extern u16 jiansujuli;

void bomakaiguan()
{ 
   boma0=gpio_get(PORTA, 9);
   boma1=gpio_get(PORTA, 11);
   boma2=gpio_get(PORTA, 13);
   boma3=gpio_get(PORTA, 15);
     
   //速度选择
   sudu=boma0*2+boma1;
   if(sudu==0)
   { //Speed_Set=220;brake=120;Direction_dP=0.95;Speed_Ki = 0.05; jiansujuli=90;
     Speed_Set=320;brake=110;Direction_dP=1.6;Speed_Ki = 0.02; jiansujuli=100;
   }
   else if(sudu==1)
  {Speed_Set=350;brake=110;Direction_dP=1.6;Speed_Ki = 0.02; jiansujuli=100;}
   else if(sudu==2)
   { Speed_Set=270;brake=120;Direction_dP=1.2;Speed_Ki = 0.05; jiansujuli=80;}//70
   else
   { Speed_Set=300;brake=100;Direction_dP=1.6;Speed_Ki = 0.02; jiansujuli=100;}
  //Speed_Set;
   //左右偏移move值 在速度是250 270的时候move等于65都是合适的 300的时候move等于60 320 同为60
   pianyi=boma2*2+boma3;
   if(pianyi==0)
   {  move=70; 
      jiansujuli=jiansujuli-5;
   }
   else if(pianyi==1)
   {  move=75; 
      jiansujuli=jiansujuli-5;
   }
   else if(pianyi==2)
   { move=-65; }
   else
   { move=-70; }
  
}

u8 Start=0;
extern u8 Is_Finish;

//extern u8 dangerous;
int dir0_start=0;  //处理前图像得到偏差

extern int Get_Direction(void);
extern u8 red_right_flag;
void main()
{
  pro_init();
  //断电后更改拨码开关才是有意义的？
  bomakaiguan();
   
  for(;;)
  {
      if(Is_Finish==1)
      {  Is_Finish=0;
         dir0_start=Get_Direction();
         if(dir0_start != -( LINE/2))
         {  Start=1; 
              break; 
          }
       }
  }
  for(;;)
  {
    //读取红外接收头电平
    if(PTD6_IN==0||PTD7_IN==0)  
    {  
      
      red_right_flag=1;  
    }
    else {  red_right_flag=0;  }
    

    if( Is_Finish==1)
    {
       Is_Finish=0;
     // SendImage();
      
       GetWay();
    }

  }

}

