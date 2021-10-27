#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qRegisterMetaType<HObject>("HObject");
    this->show();
    hv_PreDeviation=0.00;
    hv_I=0;

    camera = new CameraCtrl();
    liveThread = new QThread();
    camera->moveToThread(liveThread);
    connect(liveThread,&QThread::finished,camera,&QObject::deleteLater);
    connect(this,&MainWindow::ContinuousGrab,camera,&CameraCtrl::HandleContinuousGrab);
    liveThread->start();


    connect(camera,&CameraCtrl::hobjectReady,this,&MainWindow::handleHObject);
    connect(camera,&CameraCtrl::hobjectFocus,this,&MainWindow::AutoFocus);

    try{
        winID =(Hlong)ui->lblImage->winId();
        OpenWindow(0,0,(Hlong)ui->lblImage->width(),(Hlong)ui->lblImage->height(),winID,"visible","",&hv_WindowHandle);

    }
    catch(HalconCpp::HException &except)
    {
        qDebug()<<except.ProcName().Text()<<endl;
        qDebug()<<except.ErrorMessage().Text()<<endl;
        qDebug()<<except.ErrorCode()<<endl;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::AutoFocus(HObject ho_Image)
{
    // Local iconic variables
    HObject  ho_ImagePart00, ho_ImagePart20;
    HObject  ho_ImageSub, ho_ImageResult, ho_ImagePart01, ho_ImagePart10;
    HObject  ho_ImageSub1, ho_ImageResult1, ho_ImageSub2, ho_ImageResult2;

    // Local control variables
    HTuple  hv_I, hv_Width, hv_Height, hv_WindowID;
    HTuple  hv_Value, hv_Deviation;
    try
    {
        GetImageSize(ho_Image,&hv_Width,&hv_Height);
//        SetPart(hv_WindowHandle,0,0,hv_Height-1,hv_Width-1);
//        DispColor(ho_Image,hv_WindowHandle);

        CropPart(ho_Image, &ho_ImagePart00, 0, 0, hv_Width, hv_Height-2);
        ConvertImageType(ho_ImagePart00, &ho_ImagePart00, "real");
        CropPart(ho_Image, &ho_ImagePart20, 2, 0, hv_Width, hv_Height-2);
        ConvertImageType(ho_ImagePart20, &ho_ImagePart20, "real");
        SubImage(ho_ImagePart20, ho_ImagePart00, &ho_ImageSub, 1, 0);
        MultImage(ho_ImageSub, ho_ImageSub, &ho_ImageResult, 1, 0);
        Intensity(ho_ImageResult, ho_ImageResult, &hv_Value, &hv_Deviation);

        double d=hv_Deviation.D();
        QString strDev=QString::number(d,'f',3);
        ui->labDev->setText(strDev);

        //记录最大偏差值
        if(hv_PreDeviation<hv_Deviation)
        {
            hv_PreDeviation=hv_Deviation;
        }
    }
    catch(HalconCpp::HException &except)
    {
        qDebug()<<except.ProcName().Text()<<endl;
        qDebug()<<except.ErrorMessage().Text()<<endl;
        qDebug()<<except.ErrorCode()<<endl;
    }
    //show
}



void MainWindow::on_btnAutoFocus_clicked()
{
    bool r=true;
    camera->isFocus=r;
    emit(ContinuousGrab());
}

void MainWindow::on_btnMaxDev_clicked()
{

    double d=hv_PreDeviation.D();
    QString strMaxDev=QString::number(d,'f',3);
    ui->labmaxDev->setText(strMaxDev);
}


void MainWindow::focusTimeout()
{
//    HObject  ho_Image;
//    // Local iconic variables
//    HObject  ho_ImagePart00, ho_ImagePart20;
//    HObject  ho_ImageSub, ho_ImageResult, ho_ImagePart01, ho_ImagePart10;
//    HObject  ho_ImageSub1, ho_ImageResult1, ho_ImageSub2, ho_ImageResult2;

//    // Local control variables
//    HTuple  hv_Width, hv_Height, hv_WindowID;
//    HTuple  hv_Value, hv_Deviation;

//    try
//    {
//        hv_I=hv_I+1;
//        ReadImage(&ho_Image, ("/home/yhl/Buddha/"+hv_I)+".png");
//        GetImageSize(ho_Image,&hv_Width,&hv_Height);
//        SetPart(hv_WindowHandle,0,0,hv_Height-1,hv_Width-1);
//        DispColor(ho_Image,hv_WindowHandle);

//        CropPart(ho_Image, &ho_ImagePart00, 0, 0, hv_Width, hv_Height-2);
//        ConvertImageType(ho_ImagePart00, &ho_ImagePart00, "real");
//        CropPart(ho_Image, &ho_ImagePart20, 2, 0, hv_Width, hv_Height-2);
//        ConvertImageType(ho_ImagePart20, &ho_ImagePart20, "real");
//        SubImage(ho_ImagePart20, ho_ImagePart00, &ho_ImageSub, 1, 0);
//        MultImage(ho_ImageSub, ho_ImageSub, &ho_ImageResult, 1, 0);
//        Intensity(ho_ImageResult, ho_ImageResult, &hv_Value, &hv_Deviation);

////        HString s=hv_Deviation.S();
////        const char* t=s.Text();
////        QString strDev = QString(t);

//        double d=hv_Deviation.D();
//        QString strDev=QString::number(d,'f',3);
//        ui->labDev->setText(strDev);

//        if(hv_PreDeviation<hv_Deviation)
//        {
//            hv_PreDeviation=hv_Deviation;
//        }
//    }
//    catch(HalconCpp::HException &except)
//    {
//        qDebug()<<except.ProcName().Text()<<endl;
//        qDebug()<<except.ErrorMessage().Text()<<endl;
//        qDebug()<<except.ErrorCode()<<endl;
//    }
//    if(hv_I==30)
//    {

//     hv_I=0;

//    }


}

void MainWindow::on_btnTimer_clicked()
{
//    t_focus->start(500);
}

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
//    QString str=ui->editInter->text();
//    camera->tInter=str.toUInt();
//    qDebug()<<"ttttt"<<camera->tInter;
}

void MainWindow::on_editInter_returnPressed()
{
    QString str=ui->editInter->text();
    camera->tInter=str.toUInt();
    qDebug()<<"ttttt"<<camera->tInter;
}



void MainWindow::handleHObject(HObject ho_Image)
{
    HTuple  hv_I, hv_Width, hv_Height;

    GetImageSize(ho_Image,&hv_Width,&hv_Height);
    SetPart(hv_WindowHandle,0,0,hv_Height-1,hv_Width-1);
    DispColor(ho_Image,hv_WindowHandle);


}
