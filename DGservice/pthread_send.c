#include "my_head.h"

//发送子线程函数
void *pthread_send(void *arg)
{
    char buf[N] = "";
    ssize_t bytes;      //数据类型重命名 typedef int ssize_t
    MSG msg = *(MSG *)arg;
    while (recv(msg.acceptfd, buf, N, MSG_DONTWAIT) != 0)   //使用非阻塞的网络数据接收函数 recv() 判断网络连接是否正常
    {
        bzero(buf, N);      //数组buf清零
        fgets(buf, N, stdin);

        /*
            ssize_t send(int sockfd, const void *buf, size_t len, int flags);
            功能：发送数据
            参数：
                sockfd：文件描述符
                    客户端：socket函数的返回值
                    服务器：accept函数的返回值
                buf：发送的数据
                len：buf的长度
                flags：标志位
                    0 阻塞
                    MSG_DONTWAIT 非阻塞
            返回值：
                成功：发送的字节数
                失败：‐1
        */
        if((bytes = send(msg.acceptfd, buf, N, 0)) < 0)
        {
            ERR_LOG("fail to send");
        }
        
    }
    
    //如果recv()返回值为0，表示连接中断
    //关闭用于通信的套接字acceptfd
    close(msg.acceptfd);
    printf("The client quited\n");

    pthread_exit(NULL); //退出该进程
}