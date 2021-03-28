//双色LED灯实验

#include <wiringPi.h>
#include <softPwm.h>	//使用软件PWM需要加该头文件
#include <stdio.h>

#define uchar unsigned char

#define makerobo_Led_PinRed    0 // 红色LED 管脚
#define makerobo_Led_PinGreen  1 // 绿色LED 管脚

// LED 初始化
void makerobo_led_Init(void)
{
	softPwmCreate(makerobo_Led_PinRed,  0, 100);		
	softPwmCreate(makerobo_Led_PinGreen,0, 100);
	/*
	int softPwmCreate (int pin, int initialValue, int pwmRange) ：
	功能：使用一个指定的pin引脚创建一个模拟的PWM输出引脚。
	参数：
		pin：用来作为软件PWM输出的引脚。
		initalValue：引脚输出的初始值。
		pwmRange：PWM值的范围上限。建议使用100。对于pwmRange使用100 ，那么对于给定的引脚，该值可以是0（关闭）到100（完全打开）之间的任何值。
	返回值：0表示成功。
	*/
}
// 设置LED 亮度PWM调节范围是0x00-0xff
void makerobo_led_ColorSet(uchar r_val, uchar g_val)
{
	softPwmWrite(makerobo_Led_PinRed,   r_val);
	softPwmWrite(makerobo_Led_PinGreen, g_val);
	/*
	void softPwmWrite (int pin, int value) ：
	功能：更新引脚输出的PWM值。
	参数：
		pin：通过softPwmCreate创建的引脚。
		value：PWM引脚输出的值。
	*/
}
//-------------主程序-----------------
int main(void)
{
    //初始化连接失败时，将消息打印到屏幕
	if(wiringPiSetup() == -1){ 
		printf("setup wiringPi failed !");
		return 1; 
	}

	makerobo_led_Init(); // LED 初始化

	while(1){
		makerobo_led_ColorSet(0xff,0x00);   //红色	
		delay(500);                         //延时500ms
		makerobo_led_ColorSet(0x00,0xff);   //绿色
		delay(500);
		makerobo_led_ColorSet(0xff,0x45);	
		delay(500);
		makerobo_led_ColorSet(0xff,0xff);	
		delay(500);
		makerobo_led_ColorSet(0x7c,0xfc);	
		delay(500);
	}
	return 0;
}
