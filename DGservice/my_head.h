#ifndef _MY_HEAD_H_
#define _MY_HEAD_H_

    /* 1.头文件 */
    #include <stdio.h>
    #include <stdlib.h>
    #include <errno.h>
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/socket.h> //网络
    #include <arpa/inet.h>  
    #include <netinet/in.h> 
    #include <string.h>     //字符串处理
    #include <strings.h>
    #include <signal.h>     //信号
    #include <pthread.h>    //线程
    #include <wiringPi.h>    //用于树莓派GPIO引脚控制、串口通信、SPI通信及I2C通信等功能

    #define N 128

    /* 2.宏定义 */
    #define ERR_LOG(errmsg) do{ perror(errmsg); exit(1); }while(0)  //用于出错提示的宏定义
    #define OPEN 1      //门打开状态
    #define CLOSE 0     //门关闭状态

    /* 3.数据类型重命名 */
    typedef struct{         //该结构体存储 套接字信息的结构体 套接字文件描述符
        struct sockaddr_in addr;
        int acceptfd;
    }MSG;

    /* 4.函数声明 */
    void service();                     //服务器网络配置函数 (service.c)
    void *pthread_recv(void *arg);      //接收子线程函数声明 (pthread_recv.c)
    void *pthread_send(void *arg);      //发送子线程函数声明 (pthread_send.c)
    void steering_gear_init();          //舵机初始化函数 (steering_gear.c)
    void open_lock();                   //开门锁函数 舵机转动(steering_gear.c)
    void close_lock();                  //关门锁函数 舵机转回原位(steering_gear.c)


    /* 5.全局变量声明 */
    extern int lock_state;             //表示门锁开关的状态量
    extern int door_state;             //表示大门开关的状态量
    extern int pwmPin;                 //使用 wiringPi引脚编号 为1的pin脚产生PWM波

#endif
