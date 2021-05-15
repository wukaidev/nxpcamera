#ifndef __INCLUDE_H__
#define __INCLUDE_H__


#include  "common.h"

/*
 * Include 用户自定义的头文件
 */

#include  "gpio.h"      //IO口操作
#include  "LED.H"       //流水灯
#include  "exti.h"      //EXTI外部GPIO中断
#include  "uart.h"      //串口
#include  "lptmr.h"     //低功耗定时器(延时)
#include  "adc.h"       //AD转换
#include  "PIT.h"       //PIT定时器
#include  "FTM.h"       //FTM PWM、脉冲捕捉            
#include  "init.h" 
#include  "dma.h"       //DMA模块 
#include  "isr.h"

#include  "Control.h"
#include  "GetBound.h"
#include  "GetSpeed.h"

#endif  //__INCLUDE_H__
