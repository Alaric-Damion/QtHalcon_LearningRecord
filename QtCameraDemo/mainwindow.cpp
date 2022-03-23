#pragma execution_character_set("utf-8")
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->initCamera();
    this->initForm();


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initCamera()
{
    cameras = QCameraInfo::availableCameras();//获取可用摄像头设备列表
    int index = cameras.size();
    for(int i=0;i<index;i++)
    {
        qDebug()<<index<<cameras.at(i).description();//摄像头的设备名称
        ui->cmb_CameraName->addItem(cameras.at(i).description());
    }

    m_camera = new QCamera(this);//初始化摄像头设备
    m_camera->setCaptureMode(QCamera::CaptureStillImage);//设置捕捉模式为静态帧
//    m_camera->setCaptureMode(QCamera::CaptureVideo);//设置捕捉模式为视频
    m_camera->setViewfinder(ui->widgetCamera);//设置 摄像头画面的显示位置
    image_capture = new QCameraImageCapture(m_camera);
    m_camera->start();//开启摄像头

    connect(image_capture, SIGNAL(imageCaptured(int, QImage)), this, SLOT(processCapturedImage(int, QImage)));
    timer_image_capture = new QTimer(this);
    connect(timer_image_capture,&QTimer::timeout,this,&MainWindow::captureImage);
    SetFrameResolution();
}

void MainWindow::initForm()
{
    myTimer=new QTimer(this);
    myTimer->start(1000);
    QLabel * btn = new QLabel(this);
    connect(myTimer , &QTimer::timeout, [=](){
        QDateTime Date_Time =QDateTime::currentDateTime();
        QString Time_system = Date_Time.toString("yyyy-MM-dd hh:mm:ss ddd");
        btn->setText(Time_system);
    });
//    statusBar()->setStyleSheet(QString("QStatusBar::item{border: 2px}"));
    statusBar()->setSizeGripEnabled(false);
    ui->statusBar->addPermanentWidget(btn);
    m_status1 = new QLabel(this);
    m_status1->setGeometry(2,2,18,18);
    m_status2 = new QLabel(this);
    m_status3 = new QLabel(this);
    UpdateCameraStatus();
}

void MainWindow::UpdateCameraStatus()
{
    QString curCamName = ui->cmb_CameraName->currentText();
    QString curCamFrame = ui->cmb_MaxFrameRate->currentText();
    QString curCamRes = ui->cmb_Resolution->currentText();
    m_status1->setText("摄像头："+curCamName);
    ui->statusBar->addWidget(m_status1);
    m_status2->setText("帧率："+curCamFrame);
    ui->statusBar->addWidget(m_status2);
    m_status3->setText("分辨率："+curCamRes);
    ui->statusBar->addWidget(m_status3);
}

void MainWindow::on_btnSnap_clicked()
{
    image_capture->capture();
}

void MainWindow::on_btnVedio_clicked()
{
    int Intervalue = ui->spinBox->value();
    timer_image_capture->setInterval(Intervalue);
    timer_image_capture->start();
}

void MainWindow::processCapturedImage(int request_id, const QImage &img)
{
    QImage scaledImage = img.scaled(ui->label->size(),
                                    Qt::KeepAspectRatio,
                                    Qt::SmoothTransformation);
    ui->label->setPixmap(QPixmap::fromImage(scaledImage));
}

void MainWindow::captureImage()
{
    image_capture->capture();
}

void MainWindow::on_cmb_CameraName_activated(int index)
{
    index = ui->cmb_CameraName->currentIndex();
    qDebug()<<"Index"<< index <<": "<< ui->cmb_CameraName->currentText();
    if(m_camera->isAvailable())
    {
        m_camera->stop();
        delete m_camera;
    }

    m_camera = new QCamera(cameras[index]);
    m_camera->setCaptureMode(QCamera::CaptureStillImage);
    m_camera->setViewfinder(ui->widgetCamera);
    image_capture = new QCameraImageCapture(m_camera);
    m_camera->start();
    connect(image_capture, SIGNAL(imageCaptured(int, QImage)), this, SLOT(processCapturedImage(int, QImage)));
    SetFrameResolution();
        UpdateCameraStatus();
}

void MainWindow::SetFrameResolution()
{
    mResSize.clear();
    mResSize = m_camera->supportedViewfinderResolutions();
    ui->cmb_Resolution->clear();
    int i=0;
    foreach (QSize msize, mResSize) {
        qDebug()<<msize;
        ui->cmb_Resolution->addItem(QString::number(msize.width(),10)+"*"+QString::number(msize.height(),10), i++);
    }  //摄像头支持分辨率打印
    ui->cmb_Resolution->setCurrentIndex(0);

    mMaxFrame.clear();
    int j;
    for(j=0;j< m_camera->supportedViewfinderFrameRateRanges().length();j++)
    {
        qreal frameRates = m_camera->supportedViewfinderFrameRateRanges().at(j).maximumFrameRate;
        mMaxFrame.append(frameRates);
        ui->cmb_MaxFrameRate->addItem(QString::number(frameRates));
    }
    ui->cmb_MaxFrameRate->setCurrentIndex(j-1);
}

void MainWindow::on_cmb_Resolution_activated(int index)
{
    index = ui->cmb_Resolution->currentIndex();
    qDebug()<<"Index"<< index <<": "<< ui->cmb_Resolution->currentText();
    qDebug()<<"mResSize:"<<mResSize[index];
    //设置摄像头分辨率
    QCameraViewfinderSettings set;
    set.setResolution(mResSize[index]);
    m_camera->setViewfinderSettings(set);
    UpdateCameraStatus();
}

void MainWindow::on_cmb_MaxFrameRate_activated(int index)
{
    index = ui->cmb_MaxFrameRate->currentIndex();
    //设置摄像头最大帧率
    QCameraViewfinderSettings set;
    set.setMaximumFrameRate(mMaxFrame[index]);
    m_camera->setViewfinderSettings(set);
    UpdateCameraStatus();
}
