#include "include.h"

/*************************************************************************
* 函数名称：Get_Speed
* 功能说明：获取左右编码器脉冲数以及左右电机转动方向
* 参数说明：Left_Pulse      左编码器脉冲数
            Right_Pulse     右编码器脉冲数
            Left_Dir        左电机转动方向
            Right_Dir       右电机转动方向
* 函数返回：Feedback_Speed  车体速度
*************************************************************************/
//u16 Feedback_Speed = 0;
//u16 Feedback_Speed_L = 0;
u16 Feedback_Speed_R = 0;

s32 Left_Pulse=0;
s32 Right_Pulse=0;//版子下面的那个是右边的
u32 Left_Dir_flag = 0;
u32 Right_Dir_flag = 0;
  u8 Left_Dir = 0;
  u8 Right_Dir = 1;
  
void Get_Speed(void)
{
  /*u32 Left_Dir_flag = 0;
  u32 Right_Dir_flag = 0;
  s32 Left_Dir = 1;
  s32 Right_Dir = 1;
 */
 
  Left_Pulse = (s32)(DMA_count_get(DMA_CH4));
  Right_Pulse = (s32)(DMA_count_get(DMA_CH5));
  DMA_count_reset(DMA_CH4);
  DMA_count_reset(DMA_CH5);


  Left_Dir_flag = gpio_get(PORTB,9);     //左右编码器
  Right_Dir_flag = gpio_get(PORTB,7);
  /*
  if(Left_Dir_flag == 1)
  {
    Left_Dir = -1;
  }
  else if(Left_Dir_flag == 0)
  {
    Left_Dir = 1;
  }
  
  if(Right_Dir_flag == 0)
  {
    Right_Dir = -1;
  }
  else if(Right_Dir_flag == 1)
  {
    Right_Dir = 1;
  }
  
  Feedback_Speed = (float)((Left_Dir*Left_Pulse + Right_Dir*Right_Pulse)/2);
  Feedback_Speed_L=(float)(Left_Dir*Left_Pulse);
  Feedback_Speed_R=(float)(Right_Dir*Right_Pulse);
*/
  
  
}