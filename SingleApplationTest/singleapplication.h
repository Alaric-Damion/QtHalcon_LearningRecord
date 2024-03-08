#ifndef SINGLEAPPLICATION_H
#define SINGLEAPPLICATION_H

#include <QObject>
#include <QWidget>
#include <QApplication>
#include <QtNetwork/QLocalServer>

class SingleApplication : public QApplication
{
    Q_OBJECT
public:
    SingleApplication(int &argc, char *argv[]);
    // 判断是否已有实例在运行
    bool isRunning();
    QWidget *w;

private slots:
    //有新连接时触发
    void NewLocalConnection();

private:
    //初始化本地连接
    void InitLocalConnection();
    //创建服务端
    void NewLocalServer();
    //激活窗口
    void ActivateWindow();

    // 本地socket Server
    QLocalServer *localserver;
    // 服务名称
    QString       serverName;
    // 运行状态
    bool          isRunnings;
};

#endif // SINGLEAPPLICATION_H
