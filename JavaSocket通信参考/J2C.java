package com.pax.android2native;

import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.net.UnknownHostException;

//当应用程序启动时，Android首先会开启一个主线程(也就是UI线程)

public class TcpClientManager {

    private static TcpClientManager mTcpClientConnector;    //类中自己定义自己，给自己创建一个对象名
    private Socket mClient;         //套接字的对象名
    private ConnectListener mListener;      //
    private Thread mConnectThread;          //新线程的对象名，用于连接
    private static final int HANDMESSAGE = 0;       //

    /*  public interface 接口名 {
            抽象方法1;
            抽象方法2;
            抽象方法3;
        }   */
    public interface ConnectListener {
        void onReceiveData(String data);
    }

    public void setOnConnectListener(ConnectListener listener) {
        this.mListener = listener;      //给
    }

    //创建本类的实例化对象
    public static TcpClientManager getInstance() {
        if (mTcpClientConnector == null)
            mTcpClientConnector = new TcpClientManager();
        return mTcpClientConnector;
    }

    //定义自己的Handler
    Handler mHandler = new Handler() {
        @Override
        //在定义自己的Handler的时候都会重写Handler的handleMessage()方法来对拿到的消息进行处理
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            switch (msg.what) {
            case HANDMESSAGE:       //最初定义HANDMESSAGE = 0;
                if (mListener != null) {
                    mListener.onReceiveData(msg.getData().getString("data"));
                }
                break;
            }
        }
    };

    //新建线程用于连接
    public void createConnect(final String mSerIP, final int mSerPort) {
        if (mConnectThread == null) {
            mConnectThread = new Thread(new Runnable() {
                @Override
                public void run() {
                    connect(mSerIP, mSerPort);
                }
            });
            mConnectThread.start();     //新建线程启动
        }
    }

    /**
     * 与服务端进行连接
     * 
     * @throws IOException
     */
    private void connect(String mSerIP, int mSerPort) {
        if (mClient == null) {
            try {
                mClient = new Socket(mSerIP, mSerPort);
            } catch (UnknownHostException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            } catch (IOException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }
        }
        if (mClient.isConnected()) {
            Message message = Message.obtain();
            message.what = HANDMESSAGE;
            Bundle bundle = new Bundle();
            bundle.putString("data", "Connect Server success\n");
            message.setData(bundle);
            mHandler.sendMessage(message);
        }

    }

    /**
     * 发送数据
     * 
     * @param data
     *            需要发送的内容
     */
    public void send(String data) throws IOException {
        OutputStream outputStream = mClient.getOutputStream();
        outputStream.write(data.getBytes());

        InputStream inputStream;
        try {
            inputStream = mClient.getInputStream();
            if (inputStream == null) {
                Log.e("TAG", "inputStream error");
                return;
            }
            byte[] buffer = new byte[1024];
            int len = -1;
            while ((len = inputStream.read(buffer)) != -1) {
                String recedata = new String(buffer, 0, len);
                Message message = Message.obtain();
                message.what = HANDMESSAGE;
                Bundle bundle = new Bundle();
                bundle.putString("data", recedata);
                message.setData(bundle);
                mHandler.sendMessage(message);
            }
        } catch (IOException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
    }

    /**
     * 断开连接
     * 
     * @throws IOException
     */
    public void disconnect() throws IOException {
        if (mClient != null) {
            mClient.close();
            mClient = null;
        }
    }
}

