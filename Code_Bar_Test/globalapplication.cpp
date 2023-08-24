#include "globalapplication.h"
#include <QApplication>
#include <QKeyEvent>
#include <QDebug>

GlobalApplication::GlobalApplication(int &argc,char **argv):
QApplication(argc,argv)
{

}

GlobalApplication::~GlobalApplication()
{

}

void GlobalApplication::setWindowInstance(QWidget *wnd)
{
    widget = wnd;
}

bool GlobalApplication::notify(QObject *obj, QEvent *e)
{
    const QMetaObject* objMeta = obj->metaObject();
    QString clName = objMeta->className();
//    qDebug()<<clName;

//if(clName == "Form")
//    {
    if(e->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(e);
            if(keyEvent->key() == 16777220)
            {
                qDebug()<<strinput;
                qDebug() << clName;
                strinput.clear();
            }
            else
            {
                char st=keyEvent->key();
                QString str_ascii = QString(st);
                strinput +=str_ascii;
 //               qDebug()<<strinput;
            }
            return true;

         }

//}

//if(clName == "QWidgetWindow")
//{
//    if(e->type() == QEvent::KeyPress)
//    {
//        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(e);
//        if(keyEvent->key() == Qt::Key_F1)
//        {
//            qDebug() << clName;
//            qDebug() << "F1";
//        }
// //     widget->keyPress(keyEvent);
//     }
//    else if(e->type() == QEvent::MouseButtonPress)
//    {
//        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(e);
//        if(mouseEvent->buttons() == Qt::LeftButton)
//        qDebug() << "left";
//    }
//}


    return QApplication::notify(obj,e);

}
