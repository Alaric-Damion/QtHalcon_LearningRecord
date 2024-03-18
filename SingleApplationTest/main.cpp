#include "widget.h"
#include "singleapplication.h"
#include <QApplication>
#include <QLockFile>
#include <QMessageBox>

bool CheckSoftExist()
{
    // 防止程序重复启动
    QString lockFilePath;
    lockFilePath = "./singleApp.lock";

    QLockFile *lockFile = new QLockFile(lockFilePath);
    if (!lockFile ->tryLock(2000)) {
        QMessageBox::warning(NULL, QObject::tr("Warning"), QObject::tr("The software is running!"));
        return false;
    }
    return true;
}

int main(int argc, char *argv[])
{
    // 方法一 本地socket 使用SingleApplication 代替原来的QApplication类
    SingleApplication a(argc, argv);
    if(!a.isRunning()){
        Widget w;
        a.w = &w;
        w.show();
        return a.exec();
    }

    // 方法二 文件锁方法
//    QApplication a(argc, argv);
//    if(CheckSoftExist() == true)
//    {
//        Widget w;
//        w.show();
//        return a.exec();
//    }
}
