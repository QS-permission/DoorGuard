#include "my_head.h"

/*
使用舵机表示门锁开关
舵机的控制信号由接收机的通道进入信号调制芯片获得 直流偏置电压 
再与舵机内部周期为20ms，宽度为1.5ms的基准信号比较，由此决定舵机角度
舵机的控制信号一般需要一个20ms左右的时基脉冲，该脉冲的高电平部分一般为0.5ms-2.5ms范围内
一般来说，1ms对应舵机0°，1.5ms对应舵机90°，2ms对应舵机180°。具体舵机具体分析
*/

//舵机初始化函数
void steering_gear_init()
{
    pinMode (pwmPin, PWM_OUTPUT);
    /*
	void pinMode (int pin, int mode)
	作用：配置引脚的IO模式	
	参数：
		pin：配置的引脚
		mode:指定引脚的IO模式
			可取的值：INPUT、OUTPUT、PWM_OUTPUT，GPIO_CLOCK
	注意：
		只有wiringPi 引脚编号下的1脚（BCM下的18脚） 支持PWM输出
		只有wiringPi编号下的7（BCM下的4号）支持GPIO_CLOCK输出
	*/
    pwmSetMode (PWM_MODE_MS);
    /*
    void pwmSetMode(int mode)
    作用：设置pwm的工作模式
    参数：
        mode： PWM_MODE_MS(传统的PWM模式,设置占空比输出)  PWM_MODE_BAL(树莓派默认的PWM模式,传输N/M数据)
    */
    pwmSetRange (2000);
    /*
    void pwmSetRange(unsigned int range)
    功能：设置PWM发生器的数值范围（相当于STM32的定时器计数值）
    参数：
        range：范围的最大值，0~range，默认是0~1024。
    */
    pwmSetClock (384);  //此处虽然设置工作频率为50kHz，但只要在每20ms内 高电平持续的时间为0.5ms-2.5ms范围内，一样可以控制舵机
    /*
    void pwmSetClock(int divisor)
    功能：设置pwm波的工作频率。19.2MHz晶振为pwm提供时钟输入，默认32分频(divisor=32)，pwm的默认工作频率为600KHz.
    参数：
        divisor：设置分频，最终产生的工作频率为 19.2M/divisor
    */
}

//开锁函数，具体动作为控制舵机转动一定角度
void open_lock()
{
    pwmWrite(pwmPin, 185);  //此处控制每20ms内 高电平持续时间为1.85ms (185/2000*20ms)
    /*void pwmWrite (int pin, int value)	
    功能：设置pwm的输出占空比。前面 pwmSetRange(range) 设置了范围，占空比的范围为0～range.
    参数：
        pin：引脚
        value：写入到PWM寄存器的值，范围在0~1024之间。
    注意：对于树莓派，pin只能是wiringPi 引脚编号下的1脚（BCM下的18脚）
    */
}

//关锁函数，具体动作为控制舵机转回原位
void close_lock()
{
    pwmWrite(pwmPin, 50);   //此处控制每20ms内 高电平持续时间为0.5ms (50/2000*20ms)
}