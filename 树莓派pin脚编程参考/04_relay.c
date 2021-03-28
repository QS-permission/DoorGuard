//继电器模块实验

#include <wiringPi.h>
#include <stdio.h>

#define makerobo_RelayPin      0   // 继电器GPIO管脚

//-------------主程序-----------------
int main(void)
{
    //初始化连接失败时，将消息打印到屏幕	
	if(wiringPiSetup() == -1){
	/*
	int wiringPiSetup (void) ：
	当使用这个函数初始化树莓派引脚时，程序使用的是wiringPi 引脚编号表。引脚的编号为 0~16。
	需要root权限
	返回值：执行状态，-1表示失败

	int wiringPiSetupGpio (void) ：
	当使用这个函数初始化树莓派引脚时，程序中使用的是BCM GPIO 引脚编号表。
	需要root权限
	返回：执行状态，-1表示失败
	*/
		printf("setup wiringPi failed !");
		return 1; 
	}	
	// 继电器管脚设置为输出模式
	pinMode(makerobo_RelayPin, OUTPUT);
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

	while(1){
			/*
			void digitalWrite (int pin, int value)	
			作用：让对一个已近配置为输出模式的 引脚  输出指定的电平信号
				pin：控制的引脚
				value：引脚输出的电平值。
					可取的值：HIGH，LOW分别代表高低电平
			*/
			digitalWrite(makerobo_RelayPin, HIGH); // 打开继电器
			delay(1000);                           // 延时1s		
			digitalWrite(makerobo_RelayPin, LOW);  // 关闭继电器			
			delay(1000);                           // 延时1s
	}
	return 0;
}

