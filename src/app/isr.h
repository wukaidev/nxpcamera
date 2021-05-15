/******************** (C) COPYRIGHT 2011 野火嵌入式开发工作室 ********************
 * 文件名       ：isr.h
 * 描述         ：重新宏定义中断号，重映射中断向量表里的中断函数地址，
 *                使其指向我们所定义的中断服务函数。声明中断服务函数
 *                警告：只能在"vectors.c"包含，而且必须在"vectors.h"包含的后面！！！
 *
 * 实验平台     ：野火kinetis开发板
 * 库版本       ：
 * 嵌入系统     ：
 *
 * 作者         ：野火嵌入式开发工作室
 * 淘宝店       ：http://firestm32.taobao.com
 * 技术支持论坛 ：http://www.ourdev.cn/bbs/bbs_list.jsp?bbs_id=1008
**********************************************************************************/



#ifndef __ISR_H
#define __ISR_H 1

#include  "include.h"
/*#undef  VECTOR_080
#define VECTOR_080 FTM2_IRQHandler //FTM2输入捕捉中断*/

//#undef  VECTOR_078
//#define VECTOR_078 FTM0_IRQHandler //FTM0输入捕捉中断 FTM0_CH2 -- PTC3


#undef  VECTOR_084   
#define VECTOR_084  PIT0_IRQHandler    //重新定义 84 号中断为 PIT0_IRQHandler 中断 
#undef  VECTOR_085   
#define VECTOR_085  PIT1_IRQHandler    //重新定义 85 号中断为 PIT1_IRQHandler 中断
#undef  VECTOR_086   
#define VECTOR_086  PIT2_IRQHandler    //重新定义 86 号中断为 PIT2_IRQHandler 中断

#undef  VECTOR_103    
#define VECTOR_103  PORTA_IRQHandler   //PORTA 中断
#undef  VECTOR_104    
#define VECTOR_104  PORTB_IRQHandler   //PORTC 中断 
#undef  VECTOR_105    
#define VECTOR_105  PORTC_IRQHandler   //PORTC 中断

#undef  VECTOR_107
#define VECTOR_107  PORTE_IRQHandler   //PORTE 中断
//#undef  VECTOR_065                         //要先取消了，因为在 vectors.h 里默认是定义为 default_isr
//#define  VECTOR_065    USART2_IRQHandler   //重新定义 63 号中断的 ISR：UART1   
//extern void USART2_IRQHandler(void);       //串口 1 中断接收函数 

#undef VECTOR_017
#define VECTOR_017 DMA_CH1_Handler
#undef VECTOR_019
#define VECTOR_019 DMA_CH3_Handler

//extern void FTM0_IRQHandler();
//extern void FTM2_IRQHandler();

extern void DMA_CH1_Handler(void);
extern void DMA_CH3_Handler(void);

extern void PORTA_IRQHandler(void);           //PORTA 中断服务函数  
extern void PORTB_IRQHandler(void);           //PORTB 中断服务函数 
extern void PORTC_IRQHandler(void);           //PORTC 中断服务函数
extern void PORTE_IRQHandler(void);           //PORTE 中断服务函数 

extern void PIT0_IRQHandler(void);            //PIT0 定时中断服务函数  
extern void PIT1_IRQHandler(void);            //PIT1 定时中断服务函数
extern void PIT2_IRQHandler(void);

extern u8 BufferData[ROW][LINE];
//extern u8 BufferData1[ROW][LINE];
extern u8 Start;
/*                          重新定义中断向量表
 *  先取消默认的中断向量元素宏定义       #undef  VECTOR_xxx
 *  在重新定义到自己编写的中断函数       #define VECTOR_xxx    xxx_IRQHandler
 *  例如：
 *       #undef  VECTOR_003
 *       #define VECTOR_003    HardFault_Handler    重新定义硬件上访中断服务函数
 */

#endif  //__ISR_H

/* End of "isr.h" */