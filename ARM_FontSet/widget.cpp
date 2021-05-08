#pragma execution_character_set("utf-8")
#include "widget.h"
#include "ui_widget.h"
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->initForm();
//    this->resize(800,600);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::initForm()
{
    try {
        Hlong WinIDcurrent = (Hlong)ui->picShow->winId();
        qDebug()<<ui->picShow->width()<<ui->picShow->height()<<endl;
        OpenWindow(0,0,ui->picShow->width(),ui->picShow->height(),WinIDcurrent,"visible","",&hv_WindowHandleCurrent);
        HTuple hv_Type;
        GetWindowType(hv_WindowHandleCurrent,&hv_Type);
        qDebug()<<"hv_Type"<<hv_Type.S()<<endl;


    //设定Halcon窗口文字字体
    HTuple hv_OS;
    GetSystem("operating_system",&hv_OS);
    if(hv_OS.TupleSubstr(0,2) == HTuple("Win")){
        //方法一：直接设置Font参数
        QString Font_win = "-Arial-20-*-1-*-*-1-";
        QByteArray ba = Font_win.toLocal8Bit();
        const char *string = ba.data();
        SetFont(hv_WindowHandleCurrent,string);
    }
    else if(hv_OS.TupleSubstr(0,4) == HTuple("Linux")){
        //方法二：获取系统下的字体，选择第一个加载使用
        HTuple hv_Font,hv_FontWithSize;
        QueryFont(hv_WindowHandleCurrent,&hv_Font);
        hv_FontWithSize = HTuple(hv_Font[0]) + "-40";
        SetFont(hv_WindowHandleCurrent,hv_FontWithSize);
    }
    }      catch(HalconCpp::HException &except){
        qDebug()<<except.ProcName().Text();             //实际的HALCON算子的名字
        qDebug()<<except.ErrorMessage().Text();         //错误文本
        qDebug()<<except.ErrorCode();                   //错误代码
    }

}

void Widget::resizeEvent(QResizeEvent *)
{
    HTuple WindowWidth,WindowHeight;
        WindowWidth = ui->picShow->width();
        WindowHeight = ui->picShow->height();
        SetWindowExtents(hv_WindowHandleCurrent,0,0,WindowWidth,WindowHeight);
}

void Widget::on_btnShowText_clicked()
{
//    resize(800,600);
    HObject hv_TestImg;
    QString imgPath = "D:/VirtualFile/ImageTest.jpg";
//    QString imgPath = "/home/yhl/qtcode/ImageTest.jpg";
    QByteArray ba1 = imgPath.toLatin1();
    const char *string1 = ba1.data();
    HTuple  ImagePart_CurrentName(string1);
    ReadImage(&hv_TestImg,ImagePart_CurrentName);
    dispOnWindow(hv_TestImg,hv_WindowHandleCurrent);

    QString Text = ui->txtShow->text();
    QByteArray ba_Text = Text.toLatin1();
    const char *str_Text = ba_Text.data();
    SetTposition(hv_WindowHandleCurrent,200,200);

        SetColor(hv_WindowHandleCurrent,"red");
        WriteString(hv_WindowHandleCurrent,str_Text);

}

void Widget::on_btnModifyFont_clicked()
{
    QString Font_Size = ui->sBox_FontSize->text();
    HTuple hv_OS;
    GetSystem("operating_system",&hv_OS);
    if(hv_OS.TupleSubstr(0,2) == HTuple("Win")){
        QString Font_win = "-Arial-"+ Font_Size +"-";
        QByteArray ba = Font_win.toLocal8Bit();
        const char *string = ba.data();
        SetFont(hv_WindowHandleCurrent,string);
    }
    else if(hv_OS.TupleSubstr(0,4) == HTuple("Linux")){
        HTuple hv_Font,hv_FontWithSize;
        QueryFont(hv_WindowHandleCurrent,&hv_Font);
        hv_FontWithSize = HTuple(hv_Font[0]) + "-40";
        SetFont(hv_WindowHandleCurrent,hv_FontWithSize);
    }

    QString Text = ui->txtShow->text();
    QByteArray ba_Text = Text.toLatin1();
    const char *str_Text = ba_Text.data();
    SetTposition(hv_WindowHandleCurrent,300,300);

        SetColor(hv_WindowHandleCurrent,"black");
        WriteString(hv_WindowHandleCurrent,str_Text);

}

void Widget::dispOnWindow(const HObject &img, HTuple hv_WindowHandle)
{
    HTuple hv_Width,hv_Height;
    GetImageSize(img,&hv_Width,&hv_Height);
    HTuple Win_RowStart    = 0;
  HTuple Win_RowEnd      = hv_Height-1;
  HTuple Win_ColumnStart = 0;
  HTuple Win_ColumnEnd   = hv_Width-1;
    SetPart(hv_WindowHandle, Win_RowStart, Win_ColumnStart, Win_RowEnd, Win_ColumnEnd);
    DispObj(img,hv_WindowHandle);
    //    DispCross(hv_WindowHandle,300,400,50,0);
}
