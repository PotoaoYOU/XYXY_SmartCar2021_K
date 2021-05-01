/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】北京龙邱智能科技自平衡摩托车
【编    写】chiusir
【E-mail  】chiusir@163.com
【软件版本】V1.1 版权所有，单位使用请先联系授权
【最后更新】2020年12月10日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://longqiu.taobao.com
------------------------------------------------
【dev.env.】Keil5.2及以上版本
【Target 】 STC16F40K
【SYS PLL】 36MHz
------------------------------------------------
程序配套视频地址：https://space.bilibili.com/95313236
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include "include.h"

u8 Uart4_Receive;

#define Baudrate1 (65536 - MAIN_Fosc / 115200 / 4)
#define Baudrate4 (65536 - MAIN_Fosc / 115200 / 4)

//========================================================================
// 函数: void UART1_int (void) interrupt UART1_VECTOR
// 描述: UART1中断函数。
// 参数: nine.
// 返回: none.
// 版本: VER1.0
// 日期: 2014-11-28
// 备注:
//========================================================================
void UART1_int(void) interrupt 4
{
    if (RI)
    {
        RI = 0;
        //
    }

    if (TI)
    {
        TI = 0;
        //
    }
}
//========================================================================
// 函数: void UART4_int (void) interrupt UART4_VECTOR
// 描述: UART4中断函数。
// 参数: nine.
// 返回: none.
// 版本: VER1.0
// 日期: 2014-11-28
// 备注:
//========================================================================
void UART4_int(void) interrupt 18
{
    static int uart_receive = 0; //蓝牙接收相关变量
    static u8 Flag_PID, i, j, Receive[50];
    static float Data;

    if ((S4CON & 0x01) != 0)
    {
        S4CON &= ~0x01; //Clear Rx flag

        uart_receive = S4BUF;
        Uart4_Receive = uart_receive;

        if (uart_receive == 'A')
        {
            Flag_Add = 1, Flag_Dec = 0; //0x41--65
            if (ForwardBackFlag < 500)
                ForwardBackFlag += 50; //===加速
        }
        else if (uart_receive == 'D') //减速
        {
            Flag_Add = 0, Flag_Dec = 1; //0x45--69低速挡（默认值）
            if (ForwardBackFlag > -1000)
                ForwardBackFlag -= 50; //===减速
        }
        else if (uart_receive == 'L')
            Flag_Left = 1, Flag_Right = 0; //左转向0x4C--76
        else if (uart_receive == 'R')
            Flag_Left = 0, Flag_Right = 1; //右转向0x52--82
        else if (uart_receive == 'T')
            Trace_BT = 0; //线阵CCD循迹
        else if (uart_receive == 'B')
            Trace_BT = 1; //蓝牙遥控
        else if (uart_receive == 'P')
        {
            Flag_Left = 0, Flag_Right = 0; //停车
            Flag_Add = 0, Flag_Dec = 0;
            MotorCtrl(0, 0);         //用龙邱DRV8701或者BTN7971驱动板
            ServoCtrl(Servo_Center); //舵机控制PWM 舵机范围：【0，10000】
            Flag_Stop = 1;
        }
    }

    if ((S4CON & 0x02) != 0)
    {
        S4CON &= ~0x02; //Clear Tx flag
                        //
    }
}
//========================================================================
// 函数: SetTimer2Baudraye(u32 dat)
// 描述: 设置Timer2做波特率发生器。
// 参数: dat: Timer2的重装值.
// 返回: none.
// 版本: VER1.0
// 日期: 2014-11-28
// 备注:
//========================================================================
void SetTimer2Baudraye(u32 dat)
{
    AUXR &= ~(1 << 4); //Timer stop
    AUXR &= ~(1 << 3); //Timer2 set As Timer
    AUXR |= (1 << 2);  //Timer2 set as 1T mode
    T2H = (u8)(dat / 256);
    T2L = (u8)(dat % 256);
    IE2 &= ~(1 << 2); //禁止中断
    AUXR |= (1 << 4); //Timer run enable
}
//========================================================================
// 函数: void   UART1_config(u16 tmr)
// 描述: UART1初始化函数。
// 参数: tmr: 选择波特率, 2: 使用Timer2做波特率, 其它值: 使用Timer1做波特率.
// 返回: none.
// 版本: VER1.0
// 日期: 2014-11-28
// 备注:
//========================================================================
void UART1_config(u16 tmr)
{
    /*********** 波特率使用定时器2 *****************/
    if (tmr == 2)
    {
        AUXR |= 0x01; //S1 tmr Use Timer2;
        SetTimer2Baudraye(Baudrate1);
    }
    /*********** 波特率使用定时器1 *****************/
    else
    {
        TR1 = 0;
        AUXR &= ~0x01;     //S1 tmr Use Timer1;
        AUXR |= (1 << 6);  //Timer1 set as 1T mode
        TMOD &= ~(1 << 6); //Timer1 set As Timer
        TMOD &= ~0x30;     //Timer1_16bitAutoReload;
        TH1 = (u8)((Baudrate1) / 256);
        TL1 = (u8)((Baudrate1) % 256);
        ET1 = 0;          //禁止中断
        INTCLKO &= ~0x02; //不输出时钟
        TR1 = 1;
    }
    /*************************************************/

    SCON = (SCON & 0x3f) | 0x40; //UART1模式, 0x00: 同步移位输出, 0x40: 8位数据,可变波特率, 0x80: 9位数据,固定波特率, 0xc0: 9位数据,可变波特率
                                 //  PS  = 1;                                                  //高优先级中断
    ES = 1;                      //允许中断
    REN = 1;                     //允许接收
    P_SW1 &= 0x3f;
    P_SW1 |= 0x00; //UART1 switch to, 0x00: P3.0 P3.1, 0x40: P3.6 P3.7, 0x80: P1.6 P1.7, 0xC0: P4.3 P4.4
}
void UART4_config(u16 tmr) // 选择波特率, 2: 使用Timer2做波特率, 其它值: 使用Timer4做波特率.
{
    if (tmr == 2)
    {
        SetTimer2Baudraye(Baudrate4);
        S4CON = 0x10; //8位数据, 使用Timer2做波特率发生器, 允许接收
    }
    else
    {
        S4CON = 0x50; //8位数据, 使用Timer4做波特率发生器, 允许接收
        T4H = (Baudrate4) / 256;
        T4L = (Baudrate4) % 256;
        T4T3M = 0xA0;
    }
    IE2 |= 0x10; //允许UART4中断
    // P_SW2 &= ~0x04;                                            	//UART4 switch bit2 to: 0: P0.2 P0.3
    P_SW2 |= 0x04; //UART4 switch bit2 to: 1: P5.2 P5.3
}

void Uart_Delay()
{
    unsigned char i, j;

    i = 3;
    j = 35;
    do
    {
        while (--j)
            ;
    } while (--i);
}
/*************************************************************************
*  函数名称：void UART_PutChar(UART_t  uratn, char ch)
*  功能说明：UART发送字节函数,使用前请先初始化对应串口
*  参数说明：uratn ： UART0 - UART3/ ch    ： 要打印的字符
*  函数返回：无
*  修改时间：2020年3月10日
*  备    注：UART_PutChar(UART0, 'a');  //打印字符a
*************************************************************************/
void UART1_PutChar(char ch)
{
    SBUF = ch;
    Uart_Delay();
}
void UART4_PutChar(char ch)
{
    S4BUF = ch;
    Uart_Delay();
}

//printf 函数支持
char putchar(char c)
{
    SBUF = c;
    Uart_Delay();
}
/*************************************************************************
*  函数名称：void UART_PutStr(UART_t  uratn, char *st)
*  功能说明：UART发送字符串函数(遇 NULL 停止发送),使用前请先初始化对应串口
*  参数说明：uratn ： UART0 - UART3/ str   ： 要打印的字符串地址
*  函数返回：无
*  修改时间：2020年3月10日
*  备    注：UART_PutStr(UART3, "123456789"); //发送9个字节
*************************************************************************/
void UART1_PutStr(char *st)
{
    while (*st)
    {
        UART1_PutChar(*st++);
    }
}
void UART4_PutStr(char *st)
{
    while (*st)
    {
        UART4_PutChar(*st++);
    }
}
/*************************************************************************
*  函数名称：char UART_GetChar(UART_t  uratn)
*  功能说明：UART读取字节 使用前请先初始化对应串口 调用前请确认有接收到数据 否则会等待数据到来
*  参数说明：uratn ： UART0 - UART3
*  函数返回：读取字节
*  修改时间：2020年3月10日
*  备    注：UART_GetChar(UART3); //接收一个字符
*************************************************************************/
char UART1_GetChar(void)
{
    return (SBUF);
}
char UART4_GetChar(void)
{
    return (S4BUF);
}

void UART1_PutCmdBuff(char *buff, char buff_size)
{
    char i = 0;
    for (i = 0; i < buff_size; i++)
    {
        UART1_PutChar(*(buff + i));
    }
}

void UART1_PutDataBuff(char *buff, char buff_size)
{
    char i = 0;
    for (i = 0; i < buff_size / 2; i++)
    {
        UART1_PutChar(*(buff + 2 * i + 1));
        UART1_PutChar(*(buff + 2 * i));
    }
}

// use: vcan_sendware((uint8_t *)var, sizeof(var));
void vcan_sendware(char *wareaddr, char waresize)
{
    char cmdf[2] = {0x03, 0xfc}; //串口调试 使用的前命令
    char cmdr[2] = {0xfc, 0x03}; //串口调试 使用的后命令

    UART1_PutCmdBuff(cmdf, sizeof(cmdf));  //先发送前命令
    UART1_PutDataBuff(wareaddr, waresize); //发送数据
    UART1_PutCmdBuff(cmdr, sizeof(cmdr));  //发送后命令
}

//
