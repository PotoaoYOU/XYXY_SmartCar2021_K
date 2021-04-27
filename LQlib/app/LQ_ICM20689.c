/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
��ƽ    ̨�������������ܿƼ���ƽ��Ħ�г�
����    д��chiusir
��E-mail  ��chiusir@163.com
�������汾��V1.1 ��Ȩ���У���λʹ��������ϵ��Ȩ
�������¡�2020��4��10��
�������Ϣ�ο����е�ַ��
����    վ��http://www.lqist.cn
���Ա����̡�http://longqiu.taobao.com
------------------------------------------------
��dev.env.��Keil5.2�����ϰ汾
��Target �� STC16F40K
��SYS PLL�� 36MHz
=================================================================
����������Ƶ��ַ��https://space.bilibili.com/95313236
����ʱ, ѡ��ʱ�� 30MHZ (�û��������޸�Ƶ��).
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#include "include.h"

#define USE_SOFT_SPI //ʹ��ģ��SPI,����ʹ��Ӳ��SPI

float temp_p = 0;

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
����  �ߡ�chiusir
������˵������ʼ�� ICM20689
�������汾��V1.0
�������¡�2020��12��21�� 
����������
������ֵ���� 0����ʼ���ɹ�   1��ʧ��
������ֵ���� 
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
u8 ICM20689_Init(void)
{
  char txt[10];
  u8 res;

  ICM_SPI_Init();
  delayms(100);
  res = ICM_Read_Byte(WHO_AM_I); //��ȡICM20689��ID
  sprintf(txt, "ID:0x%02X", res);
  OLED_P6x8Str(1, 1, txt);

  if (res != ICM20689_ID) //����ID��ȷ
  {
    //printf("ID=%#X\r\n",res);
    //OLED_P6x8Str(1,4,"ID is fail!");
    return 1;
  }
  else
  {
    //printf("ICM20689 is OK!\n");
  }

  ICM_Write_Byte(ICM_PWR_MGMT1_REG, 0X80); //��λ
  delayms(100);                            //��ʱ100ms
  ICM_Write_Byte(ICM_PWR_MGMT1_REG, 0X00); //����
  delayms(100);                            //��ʱ100ms

  ICM_Set_Gyro_Fsr(3);  //�����Ǵ�����,��2000dps
  ICM_Set_Accel_Fsr(1); //���ٶȴ�����,0:��2g   1:��4g    2:��8g
  ICM_Set_Rate(1000);   //���ò�����1000Hz
  //ICM_Write_Byte(ICM_CFG_REG,0x02);    //�������ֵ�ͨ�˲���   98hz
  ICM_Write_Byte(ICM_CFG_REG, 4);          //��ͨ�˲�Ƶ�ʣ�����ֵ��0x07(3600Hz)�˼Ĵ����ھ���Internal_Sample_Rate==8K
  ICM_Write_Byte(ICM_INT_EN_REG, 0X00);    //�ر������ж�
  ICM_Write_Byte(ICM_FIFO_EN_REG, 0X00);   //�ر�FIFO
  ICM_Write_Byte(ICM_USER_CTRL_REG, 0X00); //I2C��ģʽ�ر�
  ICM_Write_Byte(ICM_PWR_MGMT1_REG, 0X01); //����CLKSEL,PLL X��Ϊ�ο�
  ICM_Write_Byte(ICM_PWR_MGMT2_REG, 0X00); //���ٶ��������Ƕ�����

  return 0;
}
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
����  �ߡ�chiusir
������˵������ȡ ICM20689 ��ԭʼ����
�������汾��V1.0
�������¡�2020��12��21�� 
����������
������ֵ���� 0����ʼ���ɹ�   1��ʧ��
������ֵ���� 
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Test_ICM20689(void)
{
  char txt[16];
  short data aacx, aacy, aacz;    //���ٶȴ�����ԭʼ����
  short data gyrox, gyroy, gyroz; //������ԭʼ����

  OLED_CLS(); //LCD����
  OLED_P6x8Str(1, 0, "LQ20689M Test");
  delayms(100);
  if (ICM20689_Init())
  {
    OLED_P6x8Str(1, 1, "LQ20689M Fail");
    while (1)
      ;
  }

  while (1)
  {

    //����ԭʼ��ֵ
    ICM_Get_Raw_data(&aacx, &aacy, &aacz, &gyrox, &gyroy, &gyroz); //�õ����ٶȴ���������

    sprintf((char *)txt, "angle:%f", atan2((float)aacz, (float)aacx) * 180.0 / PI); //�����м��ٶȼƵõ��Ƕ�ֵ
    OLED_P6x8Str(0, 1, (u8 *)txt);

    /*
    Kalman_Filter((float)aacz,(float)(0-gyroy)); //������
    sprintf((char*)txt,"g_angle:%f",Angle); 
    OLED_P6x8Str(0,2,(u8*)txt);
    sprintf((char*)txt,"Ang_Y:%f",angle_dot);//X����ٶ�ֵ											
    OLED_P6x8Str(0,3,(u8*)txt);
    */

    sprintf((char *)txt, "ax:%06d", aacx); //X����ٶ�ֵ
    OLED_P6x8Str(0, 2, (u8 *)txt);
    sprintf((char *)txt, "ay:%06d", aacy); //Y����ٶ�ֵ
    OLED_P6x8Str(0, 3, (u8 *)txt);
    sprintf((char *)txt, "az:%06d", aacz); //Z����ٶ�ֵ
    OLED_P6x8Str(0, 4, (u8 *)txt);
    sprintf((char *)txt, "gx:%06d", gyrox); //X��Ƕ�ֵ
    OLED_P6x8Str(0, 5, (u8 *)txt);
    sprintf((char *)txt, "gy:%06d", gyroy); //Y��Ƕ�ֵ
    OLED_P6x8Str(0, 6, (u8 *)txt);
    sprintf((char *)txt, "gz:%06d", gyroz); //Z��Ƕ�ֵ
    OLED_P6x8Str(0, 7, (u8 *)txt);

    /*
    //���Լ��ںϽ��
    g_angle = ICM20689_ACC_angle_X_Z();
    ICM20689_GYRO_Y_Ang();
    ICM20689_GYRO_ACCEL_Ang_Y();
    sprintf((char*)txt,"g_angle:%f",g_angle); 
    OLED_P6x8Str(0,2,(u8*)txt);
    sprintf((char*)txt,"Ang_Y:%f",GYRO_Ang_Y);//X����ٶ�ֵ											
    OLED_P6x8Str(0,3,(u8*)txt);
    */

    /*
    printf("\r\nAX: %d  ",aacx); 
    printf("\r\nAY: %d  ",aacy);
    printf("\r\nAZ: %d  ",aacz); 
    printf("\r\nGX: %d  ",gyrox);
    printf("\r\nGY: %d  ",gyroy); 
    printf("\r\nGZ: %d  ",gyroz);
    printf("*********************");
    */
    delayms(100);
  }
}

/*************************************************************************
*  �������ƣ�void ICM_SPI_Init(void)
*  ����˵����ģ��SPI�ڳ�ʼ��
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020��8��10��
*  ��    ע��   
*************************************************************************/
void ICM_SPI_Init(void)
{
#ifdef USE_SPI
  SPI_init();
#elif defined USE_SOFT_SPI
  Soft_SPI_Init();
#endif
}

/*************************************************************************
*  �������ƣ�void ICM_Read_Len(u8 reg,u8 len,u8 *buf)
*  ����˵����u8 reg��ʼ�Ĵ���,u8 *buf����ָ��,u16 len����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020��8��10��
*  ��    ע��   
*************************************************************************/
void ICM_Read_Len(u8 reg, u8 len, u8 *buf)
{
#ifdef USE_SOFT_SPI
  buf[0] = reg | 0x80;
  Soft_SPI_ReadWriteNbyte(buf, len + 1);
#else
  SPI_Read_Nbytes(reg | 0x80, buf, len);
#endif
}
/*************************************************************************
*  �������ƣ�void ICM_Get_Gyroscope(short *gx,short *gy,short *gz)
*  ����˵����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020��8��10��
*  ��    ע��   
*************************************************************************/
void ICM_Get_Gyroscope(short *gx, short *gy, short *gz)
{
  u8 buf[7];
  ICM_Read_Len(ICM_GYRO_XOUTH_REG, 6, buf);

  *gx = ((u16)buf[1] << 8) | buf[2];
  *gy = ((u16)buf[3] << 8) | buf[4];
  *gz = ((u16)buf[5] << 8) | buf[6];
}
/*************************************************************************
*  �������ƣ�void ICM_Get_Accelerometer(short *ax,short *ay,short *az)
*  ����˵����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020��8��10��
*  ��    ע��   
*************************************************************************/
void ICM_Get_Accelerometer(short *ax, short *ay, short *az)
{
  u8 buf[7];
  ICM_Read_Len(ICM_ACCEL_XOUTH_REG, 6, buf);

  *ax = ((u16)buf[1] << 8) | buf[2];
  *ay = ((u16)buf[3] << 8) | buf[4];
  *az = ((u16)buf[5] << 8) | buf[6];
}

/*************************************************************************
*  �������ƣ�void ICM_Get_Raw_data(short *ax,short *ay,short *az,short *gx,short *gy,short *gz)
*  ����˵������ȡ���ٶ�����������
*  ����˵��������
*  �������أ���
*  �޸�ʱ�䣺2020��8��10��
*  ��    ע��   
*************************************************************************/
void ICM_Get_Raw_data(short *ax, short *ay, short *az, short *gx, short *gy, short *gz)
{
  u8 buf[16];
  ICM_Read_Len(ICM_ACCEL_XOUTH_REG, 14, buf);

  *ax = ((u16)buf[1] << 8) | buf[2];
  *ay = ((u16)buf[3] << 8) | buf[4];
  *az = ((u16)buf[5] << 8) | buf[6];
  *gx = ((u16)buf[9] << 8) | buf[10];
  *gy = ((u16)buf[11] << 8) | buf[12];
  *gz = ((u16)buf[13] << 8) | buf[14];
}

/*************************************************************************
*  �������ƣ�void ICM_Write_Byte(u8 reg,u8 value)
*  ����˵������Ĵ���д����
*  ����˵����u8 reg�Ĵ���,u8 value����
*  �������أ���
*  �޸�ʱ�䣺2020��8��10��
*  ��    ע��   
*************************************************************************/
void ICM_Write_Byte(u8 reg, u8 value)
{
#ifdef USE_SOFT_SPI
  uint8_t buff[2];
  buff[0] = reg;                    //�ȷ��ͼĴ���
  buff[1] = value;                  //�ٷ�������
  Soft_SPI_ReadWriteNbyte(buff, 2); //����buff�����ݣ����ɼ��� buff��

#else
  SPI_CS = 0;
  SPI_WriteByte(reg | 0x80);
  SPI_WriteByte(value);
  SPI_CS = 1;
#endif
}

/*************************************************************************
*  �������ƣ�void ICM_Write_Byte(u8 reg,u8 value)
*  ����˵������Ĵ���д����
*  ����˵����u8 reg�Ĵ���,
*  �������أ�u8 value����
*  �޸�ʱ�䣺2020��8��10��
*  ��    ע��   
*************************************************************************/
u8 ICM_Read_Byte(u8 reg)
{
  u8 buff[2];
#ifdef USE_SOFT_SPI
  buff[0] = reg | 0x80; //�ȷ��ͼĴ���
  Soft_SPI_ReadWriteNbyte(buff, 2);
  return buff[1];
#else
  SPI_CS = 0;
  SPI_WriteByte(reg | 0x80);
  buff[0] = SPI_ReadByte();
  SPI_CS = 1;
  return buff[0];
#endif
}
/*************************************************************************
*  �������ƣ�void ICM_Set_Gyro_Fsr(u8 fsr)
*  ����˵�������������Ǵ����������̷�Χ
*  ����˵����Fsr:0,��250dps;1,��500dps;2,��1000dps;3,��2000dps
*  �������أ�
*  �޸�ʱ�䣺2020��8��10��
*  ��    ע��   
*************************************************************************/
void ICM_Set_Gyro_Fsr(u8 fsr)
{
  ICM_Write_Byte(ICM_GYRO_CFG_REG, fsr << 3); //���������������̷�Χ
}
/*************************************************************************
*  �������ƣ�void ICM_Set_Accel_Fsr(u8 fsr)
*  ����˵��������LQ20689�����Ǵ����������̷�Χ
*  ����˵����fsr:0,��2g;1,��4g;2,��8g;3,��16g
*  �������أ�
*  �޸�ʱ�䣺2020��8��10��
*  ��    ע��   
*************************************************************************/
void ICM_Set_Accel_Fsr(u8 fsr)
{
  ICM_Write_Byte(ICM_ACCEL_CFG_REG, fsr << 3); //���ü��ٶȴ����������̷�Χ
}
/*************************************************************************
*  �������ƣ�void ICM_Set_LPF(u16 lpf)
*  ����˵��������LQ20689���ֵ�ͨ�˲���
*  ����˵����lpf:���ֵ�ͨ�˲�Ƶ��(Hz)
*  �������أ�
*  �޸�ʱ�䣺2020��8��10��
*  ��    ע��   
*************************************************************************/
void ICM_Set_LPF(u16 lpf)
{
  u8 dat = 0;
  if (lpf >= 188)
    dat = 1;
  else if (lpf >= 98)
    dat = 2;
  else if (lpf >= 42)
    dat = 3;
  else if (lpf >= 20)
    dat = 4;
  else if (lpf >= 10)
    dat = 5;
  else
    dat = 6;
  ICM_Write_Byte(ICM_CFG_REG, dat); //�������ֵ�ͨ�˲���
}
/*************************************************************************
*  �������ƣ�void ICM_Set_Rate(u16 rate)
*  ����˵��������LQ20689�����Ǵ����������̷�Χ
*  ����˵����rate:4~1000(Hz)
*  �������أ�
*  �޸�ʱ�䣺2020��8��10��
*  ��    ע��   
*************************************************************************/
void ICM_Set_Rate(u16 rate)
{
  u8 dat;
  if (rate > 1000)
    rate = 1000;
  if (rate < 4)
    rate = 4;
  dat = 1000 / rate - 1;
  ICM_Write_Byte(ICM_SAMPLE_RATE_REG, dat); //�������ֵ�ͨ�˲���
  ICM_Set_LPF(rate / 2);                    //�Զ�����LPFΪ�����ʵ�һ��
}
/*************************************************************************
*  �������ƣ�u16 ICM_Get_Temperature(void)
*  ����˵�����õ��¶�ֵ
*  ����˵����
*  �������أ�����ֵ:�¶�ֵ(������100��)
*  �޸�ʱ�䣺2020��8��10��
*  ��    ע������ֵ:�¶�ֵ(������100��)   
*************************************************************************/
u16 ICM_Get_Temperature(void)
{
  u8 buf[3];
  u16 raw;
  float temp;
  ICM_Read_Len(ICM_TEMP_OUTH_REG, 2, buf);
  raw = ((u16)buf[0] << 8) | buf[1];
  temp = 21 + ((double)raw) / 333.87;
  return (u16)temp * 100;
}

//**********************************************************************************************************************************************************
void icm20689_reset()
{
  ICM_Write_Byte(ICM_PWR_MGMT1_REG, 0X80); //��λ
  delayms(100);
  ICM_Write_Byte(ICM_PWR_MGMT1_REG, 0x00); //�������״̬
}

//