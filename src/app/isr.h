/******************** (C) COPYRIGHT 2011 Ұ��Ƕ��ʽ���������� ********************
 * �ļ���       ��isr.h
 * ����         �����º궨���жϺţ���ӳ���ж�����������жϺ�����ַ��
 *                ʹ��ָ��������������жϷ������������жϷ�����
 *                ���棺ֻ����"vectors.c"���������ұ�����"vectors.h"�����ĺ��棡����
 *
 * ʵ��ƽ̨     ��Ұ��kinetis������
 * ��汾       ��
 * Ƕ��ϵͳ     ��
 *
 * ����         ��Ұ��Ƕ��ʽ����������
 * �Ա���       ��http://firestm32.taobao.com
 * ����֧����̳ ��http://www.ourdev.cn/bbs/bbs_list.jsp?bbs_id=1008
**********************************************************************************/



#ifndef __ISR_H
#define __ISR_H 1

#include  "include.h"
/*#undef  VECTOR_080
#define VECTOR_080 FTM2_IRQHandler //FTM2���벶׽�ж�*/

//#undef  VECTOR_078
//#define VECTOR_078 FTM0_IRQHandler //FTM0���벶׽�ж� FTM0_CH2 -- PTC3


#undef  VECTOR_084   
#define VECTOR_084  PIT0_IRQHandler    //���¶��� 84 ���ж�Ϊ PIT0_IRQHandler �ж� 
#undef  VECTOR_085   
#define VECTOR_085  PIT1_IRQHandler    //���¶��� 85 ���ж�Ϊ PIT1_IRQHandler �ж�
#undef  VECTOR_086   
#define VECTOR_086  PIT2_IRQHandler    //���¶��� 86 ���ж�Ϊ PIT2_IRQHandler �ж�

#undef  VECTOR_103    
#define VECTOR_103  PORTA_IRQHandler   //PORTA �ж�
#undef  VECTOR_104    
#define VECTOR_104  PORTB_IRQHandler   //PORTC �ж� 
#undef  VECTOR_105    
#define VECTOR_105  PORTC_IRQHandler   //PORTC �ж�

#undef  VECTOR_107
#define VECTOR_107  PORTE_IRQHandler   //PORTE �ж�
//#undef  VECTOR_065                         //Ҫ��ȡ���ˣ���Ϊ�� vectors.h ��Ĭ���Ƕ���Ϊ default_isr
//#define  VECTOR_065    USART2_IRQHandler   //���¶��� 63 ���жϵ� ISR��UART1   
//extern void USART2_IRQHandler(void);       //���� 1 �жϽ��պ��� 

#undef VECTOR_017
#define VECTOR_017 DMA_CH1_Handler
#undef VECTOR_019
#define VECTOR_019 DMA_CH3_Handler

//extern void FTM0_IRQHandler();
//extern void FTM2_IRQHandler();

extern void DMA_CH1_Handler(void);
extern void DMA_CH3_Handler(void);

extern void PORTA_IRQHandler(void);           //PORTA �жϷ�����  
extern void PORTB_IRQHandler(void);           //PORTB �жϷ����� 
extern void PORTC_IRQHandler(void);           //PORTC �жϷ�����
extern void PORTE_IRQHandler(void);           //PORTE �жϷ����� 

extern void PIT0_IRQHandler(void);            //PIT0 ��ʱ�жϷ�����  
extern void PIT1_IRQHandler(void);            //PIT1 ��ʱ�жϷ�����
extern void PIT2_IRQHandler(void);

extern u8 BufferData[ROW][LINE];
//extern u8 BufferData1[ROW][LINE];
extern u8 Start;
/*                          ���¶����ж�������
 *  ��ȡ��Ĭ�ϵ��ж�����Ԫ�غ궨��       #undef  VECTOR_xxx
 *  �����¶��嵽�Լ���д���жϺ���       #define VECTOR_xxx    xxx_IRQHandler
 *  ���磺
 *       #undef  VECTOR_003
 *       #define VECTOR_003    HardFault_Handler    ���¶���Ӳ���Ϸ��жϷ�����
 */

#endif  //__ISR_H

/* End of "isr.h" */