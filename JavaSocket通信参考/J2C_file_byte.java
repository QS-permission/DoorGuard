//https://blog.csdn.net/yutianzuijin/article/details/24807417
//Java与C之间的socket通信，基于指纹的音乐检索就是让用户录制一段正在播放的音乐上传服务器，服务器通过提取指纹进行检索获得相应的歌名返回给用户
//InputStream：继承自InputStream的流都是用于向程序中输入数据的，且数据单位都是字节（8位）。
//OutputSteam：继承自OutputStream的流都是程序用于向外输出数据的，且数据单位都是字节（8位）。

import java.io.*;
import java.net.*;
 
public class Client
{
    void query(String file,String ip,int port)      //query（查询）
    {
        FileInputStream fileInputStream;        //文件输入流
        DataInputStream netInputStream;         //数据输入流
        DataOutputStream netOutputStream;       //数据输出流
        Socket sc;                              //套接字sc
        int fileLength;                         //文件长度
        byte[] buffer=new byte[1023];           //临时装载文件的缓存数组buffer，用于发送
        byte[] readLen=new byte[10];            //记录接收方返回判断结果0/1，由接收方传回
        byte[] readResult=new byte[2000];       //记录接收方返回结果，由接收方传回
        int len;                                //记录写入成功的字节数 line47
        int result_count=0;
 
        //判断文件是否存在
        File f=new File(file);      //给构造方法File()传递一个文件名字符串file，创建一个文件对象f
        if(f.exists())
        {
            fileLength=(int)f.length();     //如果文件对象存在，将文件长度赋值给成员变量fileLength
        }
        else
        {
            System.out.println("No such file");     //否则输出错误信息
            return;
        }
 
        try
        {
            fileInputStream=new FileInputStream(file);      //传入文件名，创建对象文件输入流fileInputStream
            sc=new Socket(ip,port);                         //Socket(String host, int port)
            netInputStream=new DataInputStream(sc.getInputStream());  //InputStream get*putStream()返回一个套接字的输入流对象
            netOutputStream=new DataOutputStream(sc.getOutputStream()); //传入Data*putStream()构造方法，创建一个数据输入/输出流对象
 
            //传输文件长度
            //将整形成员变量fileLength文件长度转为字符串，再转为Byte[]类型的数组，使用写方法通过网络输出
            netOutputStream.write(Integer.toString(fileLength).getBytes());
 
            //传输文件
            //将文件输入流中的数据读入缓存数组buffer中，并返回成功读取的字节数
            while((len=fileInputStream.read(buffer))>0)
            {
                netOutputStream.write(buffer,0,len);    //将数组buffer中的内容写入网络数据输出流中，该输出流与套接字输出流嵌合
            }
 
            //从网络数据输入流（该输入流与套接字输入流嵌合）中读取数据并存入Byte[]数组readLen中
            netInputStream.read(readLen);
 
            while(((char)readLen[0])=='1')      //取数组Byte[] realLen的第一个元素判断是否为1
            {
				/4. Read result//
                netInputStream.read(readResult);    //将最终结果写入数组Byte[] readResult中
                String result=new String(readResult);   //将数组Byte[] readResult转为字符串result
                String[] ss=result.split(",");      //以字符串中的逗号为界，分割成多段小字符串，并存于数组
 
                int score=Integer.parseInt(ss[3]);
                int startTime=Integer.parseInt(ss[4]);
                double confidence=Double.parseDouble(ss[5]);
 
                System.out.println("name:"+ss[0].trim());
                System.out.println("artist:"+ss[1].trim());
                System.out.println("album:"+ss[2].trim());
                System.out.println("score:"+score);
                System.out.println("startTime:"+startTime);
                System.out.println("confidence:"+confidence);
 
                result_count++;                 //标志位，记录是否有返回结果
 
                netInputStream.read(readLen);
            }
 
            if(result_count==0)
            {
                System.out.println("No match music");
            }
 
            //将I/O流和套接字资源关闭
            fileInputStream.close();
            netInputStream.close();
            netOutputStream.close();
            sc.close();
        }
        catch(Exception e)
        {
            e.printStackTrace();
        }
    }
 
    public static void main(String[] args)
    {
        //创建对象，调用方法
        Client client=new Client();
        client.query(args[0],args[1],9527);
    }
}