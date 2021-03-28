#include"my_head.h"

//服务器网络配置函数
void service()
{   

    int sockfd, acceptfd;   //定义用于连接的套接字sockfd，用于通信的套接字acceptfd
    struct sockaddr_in serveraddr, clientaddr;  //分别定义存储服务器和客户端信息的网络信息结构体
    socklen_t addrlen = sizeof(serveraddr); //计算网络信息结构体的大小 数据类型重命名typedef unsigned int socklen_t
    /*
        struct in_addr     //用于存储ip地址的结构体
        {
        in_addr_t s_addr;   //ip地址 4字节 数据类型重命名typedef uint32_t in_addr_t
        };

        在网络编程(TCP/UDP)中经常使用如下的 网络信息结构体 sockaddr_in{}
        struct sockaddr_in  
        {
        sa_family_t sin_family;     //协议族 2字节 数据类型重命名typedef unsigned short sa_family_t
        in_port_t sin_port;         //端口号 2字节 数据类型重命名typedef uint16_t in_port_t
        struct in_addr sin_addr;    //ip地址 4字节 存储在上面定义的结构体当中
        char sin_zero[8]            //填充，不起什么作用 8字节 为了与通用网络信息结构体对应
        };
    */

    //第一步：创建套接字
    
    /*
        int socket(int domain, int type, int protocol);
        功能：创建一个套接字，返回一个文件描述符
        参数：
            domain：通信域，协议族
                AF_UNIX 本地通信8 AF_INET ipv4网络协议
                AF_INET6 ipv6网络协议
                AF_PACKET 底层接口
            type：套接字的类型
                SOCK_STREAM 流式套接字（tcp）
                SOCK_DGRAM 数据报套接字（udp）
                SOCK_RAW 原始套接字（用于链路层）
            protocol：附加协议，如果不需要，则设置为0
        返回值：
            成功：文件描述符
            失败：‐1
    */
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        ERR_LOG("fail to socket");  //出错提示，头文件中已有宏定义
    }

    //将套接字设置为允许重复使用本机地址或者设置为端口复用

    /*
        int setsockopt(int sock, int level, int optname, const void *optval, socklen_t optlen);
        参数：  
            sock：将要被设置或者获取选项的套接字。
            level：选项所在的协议层。
            optname：需要访问的选项名。
            optval：指向包含新选项值的缓冲。
            optlen：现选项的长度。
        返回值：  
            成功：0
            失败：-1
    */
    int on = 1;
    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
    {
        ERR_LOG("fail to setsockopt");
    }

    //第二步：填充服务器网络信息结构体 前文已经定义
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY); //INADDR_ANY转换过来就是0.0.0.0，泛指本机的意思，也就是表示本机的所有IP
    serveraddr.sin_port =htons(8080);   //选择端口，此处为8080

    //第三步：将套接字与服务器网络信息结构体绑定

    /*
        为了使不同格式地址(ipv4/6)能被传入套接字函数, 网络信息结构体 必须要强制转换成 通用网络信息结构体
        网络信息结构体 sockaddr{}
        struct sockaddr
        {
        sa_family_t sa_family; // 2字节
        char sa_data[14] //14字节
        };

        int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
        功能：将套接字与网络信息结构体绑定
        参数：
            sockfd：文件描述符，socket()的返回值
            addr：网络信息结构体
                通用结构体（一般不用） struct sockaddr
                网络信息结构体  struct sockaddr_in
            addrlen：addr的长度
        返回值：
            成功：0
            失败：‐1
    */
    if(bind(sockfd, (struct sockaddr *)&serveraddr, addrlen) < 0)
    {
        ERR_LOG("fail to bind");
    }

    //第四步：将套接字设置为被动监听状态

    /*
        int listen(int sockfd, int backlog);
        功能：将套接字设置为被动监听状态，这样做之后就可以接收到连接请求
        参数：
            sockfd：文件描述符，socket函数返回值
            backlog：允许通信连接的主机个数，一般设置为5、10
        返回值：
            成功：0
            失败：‐1
    */
    if(listen(sockfd, 5) < 0)
    {
        ERR_LOG("fail to listen");
    }

    while(1)
    {
        //第五步：阻塞等待客户端的连接请求
        
        /*
            int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
            功能：阻塞等待客户端的连接请求
            参数：
                sockfd：文件描述符，socket函数的返回值
                addr：接收到的客户端的信息结构体
                addrlen：addr的长度
            返回值：
                成功：新的文件描述符（只要有客户端连接，就会产生新的文件描述符，这个新的文件描述符专门与指定的客户端进行通信的）
                失败：‐1
        */
        if((acceptfd = accept(sockfd, (struct sockaddr *)&clientaddr, &addrlen)) < 0)
        {
            ERR_LOG("fail to accept");
        }
        
        //打印客户端的信息
        printf("%s -- %d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

        //创建子线程与客户端进行通信
        MSG msg;    //存储 套接字信息的结构体、套接字文件描述符 的结构体 头文件中已经定义
        msg.addr = clientaddr;      //填充该结构体
        msg.acceptfd = acceptfd;
        pthread_t recv_thread, send_thread;   //定义收发线程标识符
        /*
            int pthread_create ((pthread_t *thread, pthread_attr_t *attr, void *(*start_routine)(void *), void *arg))
            参数：
                thread：线程标识符
                attr：线程属性设置 一般置空
                start_routine：线程函数的起始地址
                arg：传递给 start_routine 的参数
            返回值	
                成功:0
                失败:-1
        */
        if(pthread_create(&recv_thread, NULL, pthread_recv, &msg) != 0) //创建接收线程
        {
            ERR_LOG("fail to pthread_create recv");
        }
        pthread_detach(recv_thread);     //分离接收子线程
        //可连接的线程 能够被其他线程回收或杀死，在其被杀死前，内存空间不会自动被释放。
        //可分离的线程 不能被其他线程回收或杀死，其内存空间在它终止时由系统自动释放。

        if(pthread_create(&send_thread, NULL, pthread_send, &msg) != 0) //创建发送线程
        {
            ERR_LOG("fail to pthread_create send");
        }
        pthread_detach(send_thread);     //分离发送线程
    }

    //关闭用于连接的套接字sockfd 退出线程
    close(sockfd);

}