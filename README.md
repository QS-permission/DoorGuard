本课题设计参照物联网系统的三层架构，传感层主要是主控机树莓派4B及其外接的传感器，网络层选择WiFi技术交互通信，应用层选择安卓APP作为终端控制，最终构建一个简易的物联网系统。
主要实现以下四个要求：
1.	多种大门解锁方式。
可以通过密码解锁、指纹解锁、手机远程无线解锁等。
2.	实时记录与通知
可以将大门的实时情况传送给安卓终端。比如此时大门的开闭情况，来客到访，或者大门解锁的方式等。
3.	门铃以及安全示警
来客到访按下门铃或大门异常开启（例如被撬锁），均能够立即发送对应信息至安卓终端，并根据不同情况播放音效等。
4.	Android 应用
设计一个安卓APP，综合管理该门禁系统，可以远程解锁大门，查看大门实时状况，查看大门开合历史日志，接收来客到访信息以及大门异常报警信号等。
