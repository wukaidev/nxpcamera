//#define Speed_Control_Integral_MAX 2000
//#define Speed_Control_Integral_MIN -2000

#define Speed_Control_Out_MAX 9999
#define Speed_Control_Out_MIN 1

#define Direction_Control_Out_MAX   1605      //������1600
#define Direction_Control_Out_MIN   1285     //�����Сֵ1280

extern float Speed_Set;
//extern u16 Feedback_Speed_L;
//extern u16 Feedback_Speed_R;
extern s32 Left_Pulse;
extern s32 Right_Pulse;
extern u16  y_position;  //�ű�������
extern u16  y_position1;
extern int time_distance;  //��¼������������������ʱ��
extern u8 time_finish;  //�ɼ�ʱ����ɱ�־

extern void Speed_Control();
extern void Direction_Control();
//int Get_Direction(void);
//int Get_Direction1(void);