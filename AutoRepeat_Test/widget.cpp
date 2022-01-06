#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->lineEdit->setText("0.0");
    curNum = 0.0;

    ui->btnReduce->setAutoRepeat(true); //启用长按
    ui->btnReduce->setAutoRepeatDelay(400);//触发长按的时间
    ui->btnReduce->setAutoRepeatInterval(50);//长按时click信号间隔
    connect(ui->btnReduce,SIGNAL(clicked()),this,SLOT(ReduceNum()));

    ui->btnAdd->setAutoRepeat(true); //启用长按
    ui->btnAdd->setAutoRepeatDelay(400);//触发长按的时间
    ui->btnAdd->setAutoRepeatInterval(50);//长按时click信号间隔
    connect(ui->btnAdd,SIGNAL(clicked()),this,SLOT(AddNum()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::AddNum()
{
    curNum += 0.1;
    ui->lineEdit->setText(QString::number(curNum));
}

void Widget::ReduceNum()
{
    if(curNum>0)
    {
        curNum -= 0.1;
    }
    else
    {
        curNum = 0.0;
    }
    ui->lineEdit->setText(QString::number(curNum));

}
