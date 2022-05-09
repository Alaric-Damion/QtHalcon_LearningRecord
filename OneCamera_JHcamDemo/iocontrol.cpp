#include "JHCap.h"
#include "iocontrol.h"
#include "ui_iocontrol.h"

IOControl::IOControl(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IOControl)
{
    ui->setupUi(this);

    settings = new QSettings(".\\setting.ini",QSettings::IniFormat,0);

    InitControlParameter();
}

void IOControl::InitControlParameter()
{
    int io;
    CameraGetGPIO(0, &io);
    ui->cbIn->setChecked(io & CAMERA_IO_IN);

    int strobe = settings->value("Control/EnableStrobe", "0").toInt();
    int strobeInvert = settings->value("Control/StrobePolarity", "0").toInt();
    CameraEnableStrobe(0, strobe==1);
    CameraSetStrobePolarity(0, strobeInvert==1);
    ui->cbEnableStrobe->setChecked(strobe==1);
    ui->cbStrobePolarity->setChecked(strobeInvert==1);

    int triggerMode = settings->value("Control/TriggerMode", "0").toInt();
    int triggerInvert = settings->value("Control/TriggerPolarity", "0").toInt();
    CameraSetSnapMode(0, triggerMode);
    CameraSetTriggerPolarity(0, triggerInvert==1);
    ui->cbTriggerMode->setChecked(triggerMode==CAMERA_SNAP_TRIGGER);
    ui->cbTriggerPolarity->setChecked(triggerInvert==1);

    int out = settings->value("Control/Out", "0").toInt();
    CameraSetGPIO(0, CAMERA_IO_OUT, CAMERA_IO_OUT);
    ui->cbOut->setChecked(out==1);
}

IOControl::~IOControl()
{
    settings->sync();
    delete settings;
    delete ui;
}

void IOControl::on_btnRefresh_clicked()
{
    int io;
    CameraGetGPIO(0, &io);

    ui->cbIn->setChecked(io & CAMERA_IO_IN);
}

void IOControl::on_btnTrigger_clicked()
{
    CameraTriggerShot(0);
}

void IOControl::on_cbOut_clicked(bool checked)
{
    CameraSetGPIO(0,CAMERA_IO_OUT, checked?CAMERA_IO_OUT:0);

    settings->setValue("Control/Out", checked?1:0);
}

void IOControl::on_cbTriggerMode_clicked(bool checked)
{
    CameraSetSnapMode(0, checked?CAMERA_SNAP_TRIGGER:CAMERA_SNAP_CONTINUATION);

    settings->setValue("Control/TriggerMode", checked?CAMERA_SNAP_TRIGGER:CAMERA_SNAP_CONTINUATION);
}

void IOControl::on_cbEnableStrobe_clicked(bool checked)
{
    CameraEnableStrobe(0, checked);

    settings->setValue("Control/EnableStrobe", checked?1:0);
}

void IOControl::on_cbStrobePolarity_clicked(bool checked)
{
    CameraSetStrobePolarity(0, checked);

    settings->setValue("Control/StrobePolarity", checked?1:0);
}

void IOControl::on_cbTriggerPolarity_clicked(bool checked)
{
    CameraSetTriggerPolarity(0, checked);

    settings->setValue("Control/TriggerPolarity", checked?1:0);
}
