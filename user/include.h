#ifndef __LQINCLUDES_H
#define __LQINCLUDES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "STC16f.h"
#include "intrins.h"

#include "math.h"
#include "Data_Process.h"
#include "balance.h"

typedef unsigned char u8;
typedef unsigned char uint8;
typedef unsigned char uint8_t;
typedef unsigned int u16;
typedef unsigned int uint16;
typedef unsigned int uint16_t;
typedef unsigned long u32;
typedef unsigned long uint32;
typedef unsigned long uint32_t;
typedef signed char int8;
typedef signed char int8_t;
typedef signed int int16;
typedef signed int int16_t;
typedef signed long int32;
typedef signed long int32_t;

#include <LQ_GPIO_KEY.h>
#include <LQ_GPIO_LED.h>
#include <LQ_GPIO_EXIT.h>
#include <LQ_OLED096.h>
#include <LQ_MotorServo.h>
#include "LQ_ICM20689.h"
//#include "LQ_ICM42605.h"
#include <LQ_UART.h>
#include <LQ_ADC.h>
#include "LQ_PWM.h"
#include <LQ_I2C.h>
#include <LQ_I2C_soft.h>
#include <LQ_TIMER.h>
#include <LQ_SPI.h>
#include <LQ_SPI_soft.h>
#include <LQ_Encoder.h>
#include <LQ_UART.h>

#define EnableInterrupts (EA = 1)  //开总中断
#define DisableInterrupts (EA = 0) //关总中断

#define MAIN_Fosc 30000000UL
#define PI 3.14159265

typedef struct Car_Data_
{
    int16_t gyroscope_data[6]; //陀螺仪六轴原始数据
    float aacx_with_mix;
    int16_t uart_send_buffer[3];
    Servo_Param_t Servo;
} Car_Data_t;

extern Car_Data_t data Car;

#endif
