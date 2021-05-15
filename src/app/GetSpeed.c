#include "include.h"

/*************************************************************************
* �������ƣ�Get_Speed
* ����˵������ȡ���ұ������������Լ����ҵ��ת������
* ����˵����Left_Pulse      �������������
            Right_Pulse     �ұ�����������
            Left_Dir        ����ת������
            Right_Dir       �ҵ��ת������
* �������أ�Feedback_Speed  �����ٶ�
*************************************************************************/
//u16 Feedback_Speed = 0;
//u16 Feedback_Speed_L = 0;
u16 Feedback_Speed_R = 0;

s32 Left_Pulse=0;
s32 Right_Pulse=0;//����������Ǹ����ұߵ�
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


  Left_Dir_flag = gpio_get(PORTB,9);     //���ұ�����
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