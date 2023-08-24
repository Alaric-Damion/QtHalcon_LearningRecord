#include "widget.h"
#include "ui_widget.h"
#include "dialog.h"
#include <QDebug>
#include<QKeyEvent>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    p_dialog = NULL;
    p_form = NULL;
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_Load_clicked()
{
    if(p_dialog == NULL)
    {
        p_dialog =new Dialog();
        p_dialog->setWindowTitle(QString::fromLocal8Bit("虚拟键盘测试"));
        p_dialog->show();
        qDebug()<<"Test"<<endl;
    }
    else
    {
        p_dialog->show();

    }
}

void Widget::on_pushButton_test_clicked()
{
    if(p_form == NULL)
    {
        p_form =new Form();
        p_form->setWindowTitle(QString::fromLocal8Bit("测试"));
        p_form->show();
//        qDebug()<<"Test"<<endl;
    }
    else
    {
        p_form->show();

    }
}

void Widget::keyPressEvent(QKeyEvent* event)
{
    switch(event->key())
        {
            case Qt::Key_Down:
            {
                qDebug()<<"ok";
            }
                break;
            case Qt::Key_F1:
            {
                qDebug()<<"ssss";
            }
        }


//    if (0x30 <= event->key() && 0x39 >= event->key())
//       {
//        char st=event->key();
//        QString str_ascii = QString(st);
//        qDebug()<<str_ascii;

//           qDebug()<<event->key();
//       }
//       else if (0x41 <= event->key() && 0x48 >= event->key())
//       {

//       qDebug()<<event->key();
//       }
//       else
//       {

//       qDebug()<<event->key();
//       }

}
