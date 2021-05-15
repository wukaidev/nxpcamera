#ifndef __INCLUDE_H__
#define __INCLUDE_H__


#include  "common.h"

/*
 * Include �û��Զ����ͷ�ļ�
 */

#include  "gpio.h"      //IO�ڲ���
#include  "LED.H"       //��ˮ��
#include  "exti.h"      //EXTI�ⲿGPIO�ж�
#include  "uart.h"      //����
#include  "lptmr.h"     //�͹��Ķ�ʱ��(��ʱ)
#include  "adc.h"       //ADת��
#include  "PIT.h"       //PIT��ʱ��
#include  "FTM.h"       //FTM PWM�����岶׽            
#include  "init.h" 
#include  "dma.h"       //DMAģ�� 
#include  "isr.h"

#include  "Control.h"
#include  "GetBound.h"
#include  "GetSpeed.h"

#endif  //__INCLUDE_H__
