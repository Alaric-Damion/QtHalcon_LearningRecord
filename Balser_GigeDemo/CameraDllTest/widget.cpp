#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QMessageBox>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    initSystem();
}

Widget::~Widget()
{
    if(m_control){
           m_control->deleteAll();
    }

    delete ui;
}

void Widget::on_btnOpenCamera_clicked()
{
    m_control->initCamera();
    initCameraParm();
}

void Widget::on_btnGrabImage_clicked()
{
    m_control->StartAcquire();
}

void Widget::on_btnSnapImage_clicked()
{
    m_control->StartSnap();
}

void Widget::on_btnAutoExTime_clicked()
{
    m_control->OneKeyAutoExTime();
    int curFps = m_control->getFrameRate();
    ui->txt_FrameRate->setText(QString::number(curFps));
}

void Widget::on_btnStopGrab_clicked()
{
    m_control->StopAcquire();
}


void Widget::initSystem()
{

    m_control = new Balser_GigeCamera(this);
    connect(m_control,&Balser_GigeCamera::canShowImg  ,this,&Widget::on_DisplayImg);

    connect(ui->spinBox_ExTime,SIGNAL(valueChanged(int)),this,SLOT(SetExTime()));
    connect(ui->hSliderExTime,SIGNAL(valueChanged(int)), ui->spinBox_ExTime,SLOT(setValue(int)));
    connect(ui->spinBox_ExTime,SIGNAL(valueChanged(int)), ui->hSliderExTime,SLOT(setValue(int)));

    connect(ui->spinBox_Gain,SIGNAL(valueChanged(int)),this,SLOT(SetGain()));
    connect(ui->hSlider_Gain,SIGNAL(valueChanged(int)), ui->spinBox_Gain,SLOT(setValue(int)));
    connect(ui->spinBox_Gain,SIGNAL(valueChanged(int)), ui->hSlider_Gain,SLOT(setValue(int)));
}

void Widget::on_DisplayImg(QImage img)
{
    qDebug()<<"显示图像";
    QPixmap m_pix = QPixmap::fromImage(img);
    m_pix = m_pix.scaled(ui->lbl_PicShow->size(), Qt::KeepAspectRatio);
    ui->lbl_PicShow->setPixmap(m_pix);
}

void Widget::initCameraParm()
{
   int curExTime = m_control->getExposureTime();
   ui->spinBox_ExTime->setValue(curExTime);
   int curGain = m_control->getGain();
   ui->spinBox_Gain->setValue(curGain);
   int curFps = m_control->getFrameRate();
   ui->txt_FrameRate->setText(QString::number(curFps));
   ui->txt_FrameRate->setReadOnly(true);
}

void Widget::SetExTime()
{
    int ExTime_Value = ui->spinBox_ExTime->value();
    m_control->setExposureTime(ExTime_Value);
}

void Widget::SetGain()
{
    int Gain_Value = ui->spinBox_Gain->value();
    m_control->setGain(Gain_Value);
}

void Widget::on_ckb_ModifyFps_stateChanged(int arg1)
{
    if(arg1==Qt::CheckState::Checked)
    {
        ui->txt_FrameRate->setReadOnly(false);
        DefaultFps = ui->txt_FrameRate->text();
    }
    else
    {
        ui->txt_FrameRate->setReadOnly(true);
    }
}

void Widget::on_txt_FrameRate_editingFinished()
{
    if(ui->txt_FrameRate->text().trimmed().isEmpty()){
        // 知道为空后就重置为默认值
        ui->txt_FrameRate->setText(DefaultFps);
        QMessageBox::warning(this, "Warning", "Invalid data!!");
    }
    else
    {
       m_control->setFrameRate(ui->txt_FrameRate->text().toInt());
    }
}
