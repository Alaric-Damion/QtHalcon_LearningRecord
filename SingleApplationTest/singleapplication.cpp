#include "SingleApplication.h"
#include <QtNetwork/QLocalSocket>
#include <QFileInfo>

#define TIME_OUT   500

SingleApplication::SingleApplication(int &argc, char *argv[])
    :QApplication(argc, argv),
    isRunnings(false),
    w(NULL),
    localserver(NULL)
{
    //取应用程序名作为localServer的名字
    serverName = QFileInfo(QApplication::applicationFilePath()).fileName();
    InitLocalConnection();
}


/*****************************************************************
* 检查是否已有一个实例在运行, true有实例运行, false没有实例运行.
******************************************************************/
bool SingleApplication::isRunning()
{
    return isRunnings;
}


/*****************************************************************
* 通过socket通讯实现程序单实例运行,监听到新的连接时触发该函数.
******************************************************************/
void SingleApplication::NewLocalConnection()
{
    QLocalSocket *localSocket = localserver->nextPendingConnection();
    if(localSocket)
    {
        localSocket->waitForReadyRead(TIME_OUT * 2);
        delete localSocket;

        //其他处理,如:读取启动参数
        ActivateWindow();
    }
}


/*****************************************************************
* 通过socket通讯实现程序单实例运行，
* 初始化本地连接，如果连接不上server，则创建，否则退出
******************************************************************/
void SingleApplication::InitLocalConnection()
{
    isRunnings = false;
    QLocalSocket socket;
    socket.connectToServer(serverName);
    if(socket.waitForConnected(TIME_OUT))
    {
        isRunnings = true;

        //其他处理,如:将启动参数发送到服务端
        return;
    }

    //连接不上服务器,就创建一个
    NewLocalServer();
}

/*****************************************************************
* 创建localserver
******************************************************************/
void SingleApplication::NewLocalServer()
{
    localserver = new QLocalServer(this);
    connect(localserver, &QLocalServer::newConnection, this, &SingleApplication::NewLocalConnection);
    if(!localserver->listen(serverName))
    {
        // 此时监听失败,可能是程序崩溃时,残留进程服务导致的,移除
        if(localserver->serverError() == QAbstractSocket::AddressInUseError)
        {
            localserver->removeServer(serverName);
            localserver->listen(serverName); //重新监听
        }
    }
}

/*****************************************************************
* 激活主窗口
******************************************************************/
void SingleApplication::ActivateWindow()
{
    if(w)
    {
        w->raise();
        //        w->activateWindow();
        w->showNormal();

        //        这个可以显示原窗口的大小（即最小化前的大小）（windowsAPI唤醒窗口）
        //        HWND hwnd = reinterpret_cast<HWND>(m->winId());
        //        ShowWindow(hwnd, SW_RESTORE);
    }
}
