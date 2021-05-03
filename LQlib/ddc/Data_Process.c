#include "Data_Process.h"

#define ACC_RATIO 2.4  //加速度计比例
#define GYRO_RATIO 7.2 //陀螺仪比例
#define DT 0.005      //采样周期

void refreshGyroscopeRawData(short *gyroscope_data)
{
    ICM_Get_Raw_data(&(gyroscope_data[aacx]),
                     &(gyroscope_data[aacy]),
                     &(gyroscope_data[aacz]),
                     &(gyroscope_data[gyrox]),
                     &(gyroscope_data[gyroy]),
                     &(gyroscope_data[gyroz]));
    gyroscope_data[gyroy] = 0 - gyroscope_data[gyroy];
}

//----------------------------------------------------------------
//  @brief      一阶互补滤波
//  @param      angle_m     加速度计数据
//  @param      gyro_m      陀螺仪数据
//  @return     float       数据融合后的角度
//----------------------------------------------------------------
float firstOrderComplementaryFilter(float angle_m, float gyro_m)
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

//end
