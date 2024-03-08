#pragma execution_character_set("utf-8")
#include "widget.h"
#include "ui_widget.h"
#include <QPixmap>
#include <QIcon>


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/image/myIcon.ico"));
    this->setWindowTitle("Qt程序单例化测试");
    QPixmap *pix = new QPixmap("./lena.png");
    pix->scaled(ui->lbl_pic->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->lbl_pic->setPixmap(*pix);
}

Widget::~Widget()
{
    delete ui;
}

