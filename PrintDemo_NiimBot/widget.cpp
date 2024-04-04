#include "widget.h"
#include "ui_widget.h"
#include "jcPrinter.h"
#include <QTimer>
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    // 初始化SDK
    bool loadDllResult = Printer::loadPrinterApiDll();
    if(loadDllResult)
    {
        isSDKInit = Printer::initSdk("./font");
    }
    else
    {
        isSDKInit = false;
    }
    refreshPrinters();
    QTimer timer;
    timer.setInterval(500);
    connect(&timer,SIGNAL(timeout()),this, SLOT(getConnectStatus()));
    timer.start();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::refreshPrinters()
{
    printers.clear();
    char* pAllPrinter = Printer::getAllPrinters();
    if(pAllPrinter != NULL)
    {
        qDebug()<<"pAllPrinter"<<pAllPrinter;
    }

}


void Widget::on_btn_findPrinter_clicked()
{
    if(!isSDKInit)
    {
        ui->textEdit->append("SDK初始化失败！");
        return;
    }

}


void Widget::on_btn_Print_clicked()
{

}

void Widget::getConnectStatus()
{
    bool curStatus = Printer::isConnected();
    if(curStatus)
    {
        ui->lbl_connectStatus->setText("在线");
    }
    else
    {
        ui->lbl_connectStatus->setText("离线");
    }
}

