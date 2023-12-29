#pragma execution_character_set("utf-8")
#include "widget.h"
#include "ui_widget.h"
#include "form.h"
#include <QMessageBox>


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_clicked()
{
    Form *form = new Form();
    form->show();
}


void Widget::on_pushButton_3_clicked()
{
    if(tranIndex == 0){
    m_translator->load(":/translation/English.qm");
        tranIndex = 1;
    ui->pushButton_3->setText("中文");
    }
    else{
        m_translator->load(":/translation/Chinese.qm");
        tranIndex = 0;
        ui->pushButton_3->setText("English");
    }
//    qApp->installTranslator(m_translator);
    ui->pushButton_3->setText("语言");
}


void Widget::on_pushButton_2_clicked()
{
    QMessageBox::information(NULL, tr("提示"), tr("是否删除该孔模型？"),QMessageBox::Yes | QMessageBox::Cancel,QMessageBox::Yes);
}

void Widget::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
        if(tranIndex == 0){
            ui->pushButton_3->setText("English");
        }
        else
        {
            ui->pushButton_3->setText("中文");
        }
    }
    else
    {
        QWidget::changeEvent(event);
    }
}


void Widget::setTranslator(QTranslator *translator)
{
    this->m_translator = translator;
}

void Widget::setTranSet()
{
    if(tranIndex == 0){
        ui->pushButton_3->setText("English");
    }
    else{
       ui->pushButton_3->setText("中文");
    }
}
