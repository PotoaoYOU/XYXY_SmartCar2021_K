#ifndef __BALANCE_H__
#define __BALANCE_H__

typedef struct Pid_Param
{
    float kp;
    float ki;
    float kd;
    int err;
} Pid_Param_t;

typedef struct Servo_Param
{
    char frequency;
    int mid_value;
    int left_value;
    int right_value;
    Pid_Param_t pid;
    unsigned long pwm;
} Servo_Param_t;

#include "include.h"

#endif
