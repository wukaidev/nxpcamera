#include "include.h"

/*************************************************************************
* 函数名称：Get_Bound
* 功能说明：图像处理程序
* 参数说明：value_hang  有效行
* 函数返回：Error       偏差
*************************************************************************/
#define YUZHI 250

int TURN[ROW]={0};  //放置每一行黑点的位置
int TURN1[ROW]={0}; 
u16 y_position=0,y_position1=0;  //记录信标的垂直位置

u16 y_position_last=0,y_position_last2=0;
int dir0=0;
int dir1=0;
int dir=0;  //方向标志变量
int dir_last=0;
u8 nuer=0;

int Get_Direction(void)  //图像1处理得到前向方向偏差
{
   int i,j,k0=0,k1=0,turn=0;
   y_position=0;
   
   for(i=0;i<40;i++)  
   {  //左右各去宽80高40的直角三角形
      for(j=0;j<40-2*i;j++)
      {  BufferData[i][j]=0;  }
      for(j=LINE-1;j>300+2*i;j--)
      {  BufferData[i][j]=0;  }
   }
   
   for(i=0;i<ROW;i++)  /*从第20行开始处理图像，到倒数第30行*/
   {
      for(j=0;j<LINE;j++)//只从左边寻找到第一个点为止
      {
        if(j%2==0)  continue; //隔列处理
        
         if(BufferData[i][j] > YUZHI)
         { k0=j; break; }  //k0记录从左边数第一个黑点数		
      }
      if(k0!=0)  //如果从左边找到k0, 再开始从右边找k1
      {
         for(j=LINE-1;j>=0;j--)
         {
           if(j%2==0)  continue; //隔列处理
           
           if(BufferData[i][j] > YUZHI)
           { k1=j; y_position=i; break; }//k1记录从右边数第一个黑点数			
         }    //y_position记录前摄像头信标最底端纵坐标
      }
      TURN[i]=(k0+k1)/2;//放置第i行黑点的位置
      k0=0;k1=0;               
   }
   j=0;
   for(i=0;i<ROW;i++)
   {
      if(TURN[i]!=0) //处理完一行后接着判断这一行有没有找到信标
      { turn=turn+TURN[i]; j++;  TURN[i]=0;/*数组清零*/}
   }
   turn=turn/j;  //算出黑点的平均水平位置
   turn=turn-LINE/2; //偏离中心线的位置
   
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

/******************后置摄像头图像处理**********************************/
/*int Get_Direction1(void)  //图像2处理得到后退方向偏差
{
    int i,j,k0=0,k1=0,turn1=0;
    y_position1=0;
        
    for(i=0;i<40;i++)  
    {  //左右各去宽80高40的直角三角形
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
	    { k0=j; break; }//k0记录从左边数第一个黑点数			
	}
        if(k0!=0)  //如果从左边找到k0, 再开始从右边找k1
        {
	    for(j=LINE-1;j>=0;j--)
	    {
		if(BufferData1[i][j] > YUZHI)
		{ k1=j; y_position1=i; break; }//k1记录从右边数第一个黑点数	
            }           //y_position1记录后摄像头信标最底端纵坐标
	}
	TURN1[i]=(k0+k1)/2;//放置第i行黑点的位置
        k0=0;k1=0;   
    }
    j=0;
    for(i=0;i<ROW;i++)
    {
        if(TURN1[i]!=0)
	{ turn1=turn1+TURN1[i]; j++;  TURN1[i]=0; }
    }
    turn1=turn1/j;  //算出黑点的平均位置
    turn1=turn1-LINE/2; //偏离中心线的位置

    return turn1; 
}
*/

/************************舵机控制*********************************/

extern int move;  //move是图像左移的距离  75

void GetWay(void)  //先处理图像判断方向
{  
  //if( y_position > 120 )
    //move=90;
  //else
    //move=60;
   dir0=Get_Direction()-move;  //move是图像左移的距离
   
   
   Direction_Control(); 
}


