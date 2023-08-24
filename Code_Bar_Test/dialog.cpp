#include "dialog.h"
#include "ui_dialog.h"
#include<QKeyEvent>
#include <QDebug>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::keyPressEvent(QKeyEvent* event)
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
                qDebug()<<"sssss";
            }
        }
}
