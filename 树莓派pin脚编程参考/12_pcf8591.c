//PCF8591 模数转换器传感器实验
//PCF8591是一款单芯片，单电源，低功耗8位CMOS数据采集设备，具有四个模拟输入，一个模拟输出和一个串行I2C接口

#include <stdio.h>
#include <wiringPi.h>
#include <pcf8591.h>

#define makerobo_PCF       120  // 设置PCF8591基础管脚为120 使用PCF8591哪个pin脚就在基准地址上 +n

// 主函数
int main (void)
{
	int pcf_value ;       // 定义一个变量存储AIN的输入值
	wiringPiSetup () ;    // 调用初始化函数
	pcf8591Setup (makerobo_PCF, 0x48) ;// 设置pcf8591基本引脚120，连接地址0x48
	//PCF8951通过I2C与树莓派连接，其自身硬件地址为 1001,000b 即0x48 

	while(1) // 无限循环
	{
		pcf_value = analogRead  (makerobo_PCF + 0) ;    // 获取AIN0上的值，插上跳线帽之后，采用的是内部的电位器，
		//使用第PCF8951第0个pin脚就在基准地址上 +0
		/*
		int analogRead (int pin)	
		功能：模拟量输入
		参数：
			pin：引脚
		返回值：
			引脚上读取的模拟量
		注意：
			树莓派的引脚本身是不支持AD转换的，也就是不能使用模拟量的API，需要增加另外的模块
		*/
		printf("%d\n", pcf_value);                  // 打印出该值            
		analogWrite (makerobo_PCF + 0, pcf_value) ; // 控制AOUT输出电平控制LED灯
		/*
		void analogWrite(int pin, int value)	
		功能：模拟量输出
		参数：
			pin:引脚
			value：输出的模拟量
		注意：
			树莓派的引脚本身是不支持AD转换的，也就是不能使用模拟量的API，需要增加另外的模块
		*/
		delay (10) ;                                // 延时10ms
	} 
	return 0 ;
}
