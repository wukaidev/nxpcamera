#include "include.h"

/*************************************************************************
* �������ƣ�Get_Bound
* ����˵����ͼ�������
* ����˵����value_hang  ��Ч��
* �������أ�Error       ƫ��
*************************************************************************/
#define YUZHI 250

int TURN[ROW]={0};  //����ÿһ�кڵ��λ��
int TURN1[ROW]={0}; 
u16 y_position=0,y_position1=0;  //��¼�ű�Ĵ�ֱλ��

u16 y_position_last=0,y_position_last2=0;
int dir0=0;
int dir1=0;
int dir=0;  //�����־����
int dir_last=0;
u8 nuer=0;

int Get_Direction(void)  //ͼ��1����õ�ǰ����ƫ��
{
   int i,j,k0=0,k1=0,turn=0;
   y_position=0;
   
   for(i=0;i<40;i++)  
   {  //���Ҹ�ȥ��80��40��ֱ��������
      for(j=0;j<40-2*i;j++)
      {  BufferData[i][j]=0;  }
      for(j=LINE-1;j>300+2*i;j--)
      {  BufferData[i][j]=0;  }
   }
   
   for(i=0;i<ROW;i++)  /*�ӵ�20�п�ʼ����ͼ�񣬵�������30��*/
   {
      for(j=0;j<LINE;j++)//ֻ�����Ѱ�ҵ���һ����Ϊֹ
      {
        if(j%2==0)  continue; //���д���
        
         if(BufferData[i][j] > YUZHI)
         { k0=j; break; }  //k0��¼���������һ���ڵ���		
      }
      if(k0!=0)  //���������ҵ�k0, �ٿ�ʼ���ұ���k1
      {
         for(j=LINE-1;j>=0;j--)
         {
           if(j%2==0)  continue; //���д���
           
           if(BufferData[i][j] > YUZHI)
           { k1=j; y_position=i; break; }//k1��¼���ұ�����һ���ڵ���			
         }    //y_position��¼ǰ����ͷ�ű���׶�������
      }
      TURN[i]=(k0+k1)/2;//���õ�i�кڵ��λ��
      k0=0;k1=0;               
   }
   j=0;
   for(i=0;i<ROW;i++)
   {
      if(TURN[i]!=0) //������һ�к�����ж���һ����û���ҵ��ű�
      { turn=turn+TURN[i]; j++;  TURN[i]=0;/*��������*/}
   }
   turn=turn/j;  //����ڵ��ƽ��ˮƽλ��
   turn=turn-LINE/2; //ƫ�������ߵ�λ��
   
   /* for(i=0;i<ROW;i++)
    {
        if(BufferData[i][turn]>YUZHI)
        { k0=i;break; }
               
     }
     for(i=ROW-1;i>=0;i--)
     {
         if(BufferData[i][turn]>YUZHI)
         { k1=i;break; }
     }
    y_position=(k0+k1)/2;
    */
        
   return turn;  
}

/******************��������ͷͼ����**********************************/
/*int Get_Direction1(void)  //ͼ��2����õ����˷���ƫ��
{
    int i,j,k0=0,k1=0,turn1=0;
    y_position1=0;
        
    for(i=0;i<40;i++)  
    {  //���Ҹ�ȥ��80��40��ֱ��������
       for(j=0;j<80-2*i;j++)
       {  BufferData1[i][j]=0;  }
       for(j=319;j>240+2*i;j--)
       {  BufferData1[i][j]=0;  }
    }
    
    for(i=0;i<ROW;i++)
    { 
	for(j=0;j<LINE ;j++)
	{
	    if(BufferData1[i][j] > YUZHI)
	    { k0=j; break; }//k0��¼���������һ���ڵ���			
	}
        if(k0!=0)  //���������ҵ�k0, �ٿ�ʼ���ұ���k1
        {
	    for(j=LINE-1;j>=0;j--)
	    {
		if(BufferData1[i][j] > YUZHI)
		{ k1=j; y_position1=i; break; }//k1��¼���ұ�����һ���ڵ���	
            }           //y_position1��¼������ͷ�ű���׶�������
	}
	TURN1[i]=(k0+k1)/2;//���õ�i�кڵ��λ��
        k0=0;k1=0;   
    }
    j=0;
    for(i=0;i<ROW;i++)
    {
        if(TURN1[i]!=0)
	{ turn1=turn1+TURN1[i]; j++;  TURN1[i]=0; }
    }
    turn1=turn1/j;  //����ڵ��ƽ��λ��
    turn1=turn1-LINE/2; //ƫ�������ߵ�λ��

    return turn1; 
}
*/

/************************�������*********************************/

extern int move;  //move��ͼ�����Ƶľ���  75

void GetWay(void)  //�ȴ���ͼ���жϷ���
{  
  //if( y_position > 120 )
    //move=90;
  //else
    //move=60;
   dir0=Get_Direction()-move;  //move��ͼ�����Ƶľ���
   
   
   Direction_Control(); 
}


