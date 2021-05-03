#ifndef __DATA_PROCESS_H__
#define __DATA_PROCESS_H__

#include "include.h"

/*陀螺仪六轴原始数据枚举*/
typedef enum
{
    aacx = 0,
    aacy,
    aacz,
    gyrox,
    gyroy,
    gyroz,
} GYROSCOP_DATA;

float firstOrderComplementaryFilter(float angle_m, float gyro_m);
void refreshGyroscopeRawData(short *gyroscope_data);

#endif
