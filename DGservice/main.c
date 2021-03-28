#include "my_head.h"

/* 全局变量定义 */
int lock_state = CLOSE;             //表示门锁开关的状态量
int door_state = CLOSE;             //表示大门开关的状态量
int pwmPin = 1;                     //使用 wiringPi引脚编号 为1的pin脚产生PWM波

int main(int argc, char const *argv[])
{
    /*
    初始化树莓派pin脚，使用wiringPi引脚编号表
	int wiringPiSetup (void) ：
	作用：当使用这个函数初始化树莓派引脚时，程序使用的是wiringPi 引脚编号表。引脚的编号为 0~16。需要root权限
	返回值：执行状态，-1表示失败
	*/
    if (wiringPiSetup() == -1)
    {
        ERR_LOG("fail to wiringPiSetup()");           //用于出错提示的宏定义
    }

    steering_gear_init();

    service();  //服务器网络配置函数,启动服务器等待连接

    return 0;
}
