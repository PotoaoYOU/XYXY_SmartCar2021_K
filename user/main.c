/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 【平    台】北京龙邱智能科技STC16核心板
 【编    写】chiusir
 【E-mail  】chiusir@163.com
 【软件版本】V1.1 版权所有，单位使用请先联系授权
 【最后更新】2020年10月28日
 【相关信息参考下列地址】
 【网    站】http://www.lqist.cn
 【淘宝店铺】http://longqiu.taobao.com
 ------------------------------------------------
 【IDE】STC16
 【Target 】 keil5.2及以上
 【SYS PLL】 30MHz使用内部晶振，通过
=================================================================
程序配套视频地址：https://space.bilibili.com/95313236
下载时, 选择时钟 30MHZ (用户可自行修改频率).
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include "include.h"

float data angle_c;    //数据融合后的角度
#define ACC_RATIO 5.6  //加速度计比例
#define GYRO_RATIO 7.6 //陀螺仪比例
#define DT 0.01        //采样周期

void sysinit(void);

//----------------------------------------------------------------
//  @brief      一阶互补滤波
//  @param      angle_m     加速度计数据
//  @param      gyro_m      陀螺仪数据
//  @return     float       数据融合后的角度
//----------------------------------------------------------------

float angle_calc(float angle_m, float gyro_m)
{
    float temp_angle;
    float gyro_now;
    float error_angle;
    static float last_angle;
    static uint8 first_angle;

    if (!first_angle) //判断是否为第一次运行本函数
    {
        //如果是第一次运行，则将上次角度值设置为与加速度值一致
        first_angle = 1;
        last_angle = angle_m;
    }

    gyro_now = gyro_m * GYRO_RATIO;
    //根据测量到的加速度值转换为角度之后与上次的角度值求偏差
    error_angle = (angle_m - last_angle) * ACC_RATIO;
    //根据偏差与陀螺仪测量得到的角度值计算当前角度值
    temp_angle = last_angle + (error_angle + gyro_now) * DT;
    //保存当前角度值
    last_angle = temp_angle;

    return temp_angle;
}
/******************** 主函数 **************************/
void main(void)
{
    sysinit();
    UART1_config(1); //调试
    UART1_PutStr("uart1...\n\r");
    UART4_config(2);

    if (ICM20689_Init())
        UART1_PutStr("ICM20689 INIT FAIL!!!\n\r");
    else
        UART1_PutStr("ICM20689 INIT SUCCESS\n\r");

    while (1)
    {
        uint16_t data aacx, aacy, aacz;    //加速度传感器原始数据
        uint16_t data gyrox, gyroy, gyroz; //陀螺仪原始数据
        uint16_t data icm_raw_data[3] = {0};

        ICM_Get_Raw_data(&aacx, &aacy, &aacz, &gyrox, &gyroy, &gyroz); //得到加速度传感器数据
        gyroy = -gyroy;
        angle_c = angle_calc(aacx, gyroy);
        icm_raw_data[0] = aacx;
        icm_raw_data[1] = gyroy;
        icm_raw_data[2] = (uint16_t)angle_c;

        // printf("angle:%f, aacx:%d, angle:%f\n\r", atan2((float)aacz, (float)aacx) * 180.0 / PI, aacx, angle_c); //反正切加速度计得到角度值

        vcan_sendware((uint8_t *)icm_raw_data, sizeof(icm_raw_data));

        delayms(10);
    }
}

void sysinit(void)
{
    WTST = 0;
    P_SW2 |= 0x80;
    IRCBAND = 1; //1：35M频段；0：24M频段
                 //IRTRIM =0;
                 //LIRTRIM=0;
    CLKDIV = 0;  //24MHz主频，分频设置
    P_SW2 &= 0x7F;
    RSTCFG |= 1 << 4; //P5.4复位管脚
    P0M1 = 0x40;
    P0M0 = 0;    //设置为准双向口,P0.6  0.7为输入口，编码器用
    P0PU = 0x40; //P0.6  0.7使能内部4.1K上拉电阻
    P1M1 = 0;
    P1M0 = 0; //设置为准双向口
    P3M1 = 0;
    P3M0 = 0; //设置为准双向口
    P4M1 = 0;
    P4M0 = 0; //设置为准双向口
    P5M1 = 0;
    P5M0 = 0; //设置为准双向口
    P6M1 = 0;
    P6M0 = 0; //设置为准双向口
    P7M1 = 0xFF;
    P7M0 = 0;    //设置为准双向口
    P7PU = 0xFF; //设置为准双向口,P7.0123为输入口接按键,P7.4567为输出口接电机驱动
    P40 = 0;     //打开LED电源
    EA = 1;
}
