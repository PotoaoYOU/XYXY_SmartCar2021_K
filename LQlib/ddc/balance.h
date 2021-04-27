/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】北京龙邱智能科技自平衡摩托车
【编    写】chiusir
【E-mail  】chiusir@163.com
【软件版本】V1.1 
【最后更新】2020年4月10日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://longqiu.taobao.com
------------------------------------------------
【dev.env.】Keil5.2及以上版本
【Target 】 STM32F103
【Crystal】 8.000Mhz
【SYS PLL】 72MHz
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#ifndef __CONTROL_H
#define __CONTROL_H
#include "include.h"

int SBB_Get_BalancePI(float angle, float gyro);
int SBB_Get_Servo_Turn(int servo_input);
int turn(int encoder_left, int encoder_right, float gyro);
void SBB_Set_Motor_Servo_Pwm(int moto1);
void Key(void);
void Xianfu_Pwm(void);
u8 Turn_Off(float angle, int volt);
void SBB_Get_Angle(u8 way);
int myabs(int a);
int Put_Down(float Angle, int encoder);
int SBB_Get_MotorPI(int Encoder, int Target);
void Demo_SBB_Car(void);
void OLED_Show(void);
#endif
