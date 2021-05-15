//#define Speed_Control_Integral_MAX 2000
//#define Speed_Control_Integral_MIN -2000

#define Speed_Control_Out_MAX 9999
#define Speed_Control_Out_MIN 1

#define Direction_Control_Out_MAX   1605      //舵机最大1600
#define Direction_Control_Out_MIN   1285     //舵机最小值1280

extern float Speed_Set;
//extern u16 Feedback_Speed_L;
//extern u16 Feedback_Speed_R;
extern s32 Left_Pulse;
extern s32 Right_Pulse;
extern u16  y_position;  //信标纵坐标
extern u16  y_position1;
extern int time_distance;  //记录超声波传感器返回来时间
extern u8 time_finish;  //采集时间完成标志

extern void Speed_Control();
extern void Direction_Control();
//int Get_Direction(void);
//int Get_Direction1(void);