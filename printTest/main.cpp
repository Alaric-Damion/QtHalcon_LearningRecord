#include "widget.h"
#include <QApplication>
#include <QIcon>
#include <QSplashScreen>
#include <windows.h>
#include <QMovie>
#include <QLabel>

Widget *w = nullptr;

void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    //QtMsgType类型的输出都会自动触发该函数。
    if(w){
        w->outputMessage(type,context,msg);
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qInstallMessageHandler(outputMessage);

//    QPixmap pixmap(":/image/poker.gif");
//        QSplashScreen splash(pixmap);
//        splash.setWindowOpacity(0.8);
//        splash.show();

//        QMovie mv(":/image/poker.gif");

//        a.processEvents();
//        Sleep(3000);

//        QPixmap pixmap(700,400);//gif动画的尺寸大小
//        QSplashScreen splash(pixmap);//利用QPixmap对象创建一个QSplashScreen对象
//        QLabel label(&splash);

        //设置label大小和位置
//        QMovie mv(":image/light.gif");
//        label.setMovie(&mv);
//        mv.start();
//        //显示此启动图片
//        splash.show();
//        splash.setCursor(Qt::BlankCursor);
//        for (int i = 0; i < 8000; i += mv.speed()) {
//          a.processEvents(); //使程序在显示启动画面的同时仍能响应鼠标等其他事件
//          Sleep(mv.speed()); // 延时
//        }

    w = new Widget;
    w->setWindowIcon(QIcon(":/image/Printer.bmp"));
    w->show();
//    splash.finish(w);

    return a.exec();
}
