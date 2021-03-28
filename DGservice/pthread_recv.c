#include "my_head.h"

//接收子线程函数
void *pthread_recv(void *arg)
{
    char buf[N] = "";
    ssize_t bytes;      //数据类型重命名 typedef int ssize_t
    MSG msg = *(MSG *)arg;
    while (1)
    {
        /*
            ssize_t recv(int sockfd, void *buf, size_t len, int flags);
            功能：接收数据
            参数：
                sockfd：文件描述符
                    客户端：socket函数的返回值
                    服务器：accept函数的返回值
                buf：保存接收到的数据
                len：buf的长度
                flags：标志位
                    0 阻塞
                    MSG_DONTWAIT 非阻塞
            返回值：
                成功：接收的字节数
                失败：‐1
                如果发送端关闭文件描述符或者关闭进程，则recv函数会返回0
        */
        if((bytes = recv(msg.acceptfd, buf, N, 0)) < 0)
        {
            close(msg.acceptfd);    //关闭用于通信的套接字acceptfd
            ERR_LOG("fail to recv");
        }
        else if(bytes == 0)         //如果发送端关闭文件描述符或者关闭进程，则recv函数会返回0
        {
            close(msg.acceptfd);    //关闭用于通信的套接字acceptfd
            printf("The client quited\n");
            pthread_exit(NULL);
        }

        printf("[%s - %d]: %s\n", inet_ntoa(msg.addr.sin_addr), ntohs(msg.addr.sin_port), buf);

        /*
        int strcmp(const char *str1, const char *str2) 
        功能：把 str1 所指向的字符串和 str2 所指向的字符串进行比较。
        参数：
            str1：要进行比较的第一个字符串。
            str2：要进行比较的第二个字符串。
        返回值：
            如果返回值小于 0，则表示 str1 小于 str2。
            如果返回值大于 0，则表示 str1 大于 str2。
            如果返回值等于 0，则表示 str1 等于 str2。
        */
        if (strcmp(buf, "1") == 0)  //判断安卓终端发送的信号 发送字符串"1" 表示开门
        {
            if (lock_state == OPEN) //如果锁的状态是已经打开
            {
                bzero(buf, N);      //数组buf清零
                strcpy(buf ,"锁已开，请不要重复开锁");
                if((bytes = send(msg.acceptfd, buf, N, 0)) < 0)
                {
                    ERR_LOG("fail to send");
                }
            }
            else                    //如果锁没有开，则开锁
            {
                open_lock();
                lock_state = OPEN;
            }
            
        }

        if (strcmp(buf, "2") == 0)
        {
            if (lock_state == CLOSE)
            {
                bzero(buf, N);      //数组buf清零
                strcpy(buf ,"锁已关，请不要重复关锁");
                if((bytes = send(msg.acceptfd, buf, N, 0)) < 0)
                {
                    ERR_LOG("fail to send");
                }
            }
            else
            {
                close_lock();
                lock_state = CLOSE;
            }
            
        }

        bzero(buf, N);  //清空数组
    }
}