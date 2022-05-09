#include "settingdialog.h"
#include "ui_settingdialog.h"
#include "JHCap.h"

#include "mainwindow.h"

#include <Windows.h>
#include <QMessageBox>
#include <QDateTime>
#include <QSettings>
#include <QFileDialog>

SettingDialog::SettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingDialog)
{
    ui->setupUi(this);
    deviceid = 0;

    ui->cbPowerFreq->addItem("50Hz");
    ui->cbPowerFreq->addItem("60Hz");   
}

SettingDialog::~SettingDialog()
{
    delete ui;
}

void SettingDialog::on_hsGamma_valueChanged(int value)
{
    double gamma = value / 100.0;
    CameraSetGamma(deviceid,gamma);

    ui->lblGamm->setText(QString("%1").arg(gamma));
}

void SettingDialog::on_hsContrast_valueChanged(int value)
{
    double contrast = value / 100.0;
    CameraSetContrast(deviceid,contrast);

    ui->lblContrast->setText(QString("%1").arg(contrast));
}

void SettingDialog::on_hsSaturation_valueChanged(int value)
{
    double saturation = value / 100.0;
    CameraSetSaturation(deviceid,saturation);

    ui->lblSaturation->setText(QString("%1").arg(saturation));
}

void SettingDialog::on_hsGain_valueChanged(int value)
{
    if(ui->hsGain->isEnabled())
    {
        CameraSetGain(deviceid,value);
        ui->lblGain->setText(QString("%1").arg(value));
    }
}

void SettingDialog::on_hsExposure_valueChanged(int value)
{
    if(ui->hsExposure->isEnabled())
    {
        CameraSetExposure(deviceid,value);
        ui->lblExposure->setText(QString("%1").arg(value));
    }
}

void SettingDialog::on_chkAutoExposure_toggled(bool checked)
{
    if(!checked)
    {
        int exposure=1;
        CameraGetExposure(deviceid, &exposure);
        ui->lblExposure->setText(QString("%1").arg(exposure));
        ui->hsExposure->setValue(exposure);
    }
    CameraSetAEC(deviceid,checked);
    ui->hsExposure->setEnabled(!checked);
}

void SettingDialog::on_chkAutoGain_toggled(bool checked)
{
    if(!checked)
    {
        int gain=1;
        CameraGetGain(deviceid, &gain);
        ui->lblGain->setText(QString("%1").arg(gain));
        ui->hsGain->setValue(gain);
    }
    CameraSetAGC(deviceid,checked);
    ui->hsGain->setEnabled(!checked);
}

void SettingDialog::on_cbMirrorX_toggled(bool checked)
{
    CameraSetMirrorX(deviceid,checked);
}

void SettingDialog::on_cbMirrorY_toggled(bool checked)
{
    CameraSetMirrorY(deviceid,checked);
}

void SettingDialog::on_btnSaveParam_clicked()
{
    CameraSaveParameter(deviceid,ui->rdParam0->isChecked()?0:1);
}

void SettingDialog::InitWBGain()
{
    double redGain=0, greenGain=0, blueGain=0;
    CameraGetWBGain(deviceid,&redGain, &greenGain, &blueGain);
    ui->lblRedGain->setText(QString("%1").arg(redGain,2));
    ui->lblGreenGain->setText(QString("%1").arg(greenGain,2));
    ui->lblBlueGain->setText(QString("%1").arg(blueGain,2));
    ui->hsRedGain->setValue(redGain*100);
    ui->hsGreenGain->setValue(greenGain*100);
    ui->hsBlueGain->setValue(blueGain*100);
}

void SettingDialog::InitParameter()
{
    ////////
    double gamma=1;
    CameraGetGamma(deviceid, &gamma);
    ui->lblGamm->setText(QString("%1").arg(gamma));
    ui->hsGamma->setValue(gamma*100);

    double contrast=1;
    CameraGetContrast(deviceid, &contrast);
    ui->lblContrast->setText(QString("%1").arg(contrast));
    ui->hsContrast->setValue(contrast*100);

    double saturation=1;
    CameraGetSaturation(deviceid, &saturation);
    ui->lblSaturation->setText(QString("%1").arg(saturation));
    ui->hsSaturation->setValue(saturation*100);

    bool enhancement=true;
    CameraGetEnhancement(deviceid, &enhancement);
    ui->chkEnhancement->setChecked(enhancement);
    on_chkEnhancement_toggled(enhancement);

    bool awb=false;
    CameraGetAWB(deviceid, &awb);
    ui->chkAWB->setChecked(awb);
    on_chkAWB_toggled(awb);

    int gain = 1;
    CameraGetGain(deviceid, &gain);
    ui->lblGain->setText(QString("%1").arg(gain));
    ui->hsGain->setValue(gain);

    bool agc = true;
    CameraGetAGC(deviceid, &agc);
    ui->chkAutoGain->setChecked(agc);
    ui->hsGain->setEnabled(!agc);

    int exposure = 1;
    CameraGetExposure(deviceid, &exposure);
    ui->lblExposure->setText(QString("%1").arg(exposure));
    ui->hsExposure->setValue(exposure);

    bool aec = true;
    CameraGetAEC(deviceid, &aec);
    ui->chkAutoExposure->setChecked(aec);
    ui->hsExposure->setEnabled(!aec);

    int black = 0;
    CameraGetBlackLevel(deviceid, &black);
    ui->hsBlackLevel->setValue(black);
    ui->lblBlack->setText(QString("%1").arg(black));

    int delay = 0;
    CameraGetDelay(deviceid, &delay);
    delay = delay / 100;
    ui->hsFrameSpeed->setValue(delay);
    ui->leFrameSpeed->setText(QString("%1").arg(delay));

    int target = 90;
    CameraGetAETarget(deviceid, &target);
    ui->hsAETarget->setValue(target);
    ui->lblTarget->setText(QString("%1").arg(target));

    bool mx, my;
    CameraGetMirrorX(deviceid, &mx);
    CameraGetMirrorY(deviceid, &my);
    ui->cbMirrorX->setChecked(mx);
    ui->cbMirrorY->setChecked(my);

    int interpolation = 0;
    CameraGetInterpolation(deviceid, &interpolation);
    if(interpolation == 0) ui->rbBilinear->setChecked(true);
    else if(interpolation == 1) ui->rbAdaptive->setChecked(true);
    else ui->rbHaminton->setChecked(true);

    int resolution_mode = 1;
    CameraGetResolutionMode(deviceid, &resolution_mode);
    if(resolution_mode == 0) ui->rbCrop->setChecked(true);
    else if(resolution_mode == 1) ui->rbSkip->setChecked(true);
    else ui->rbBin->setChecked(true);


    int flicker = 0;
    CameraGetAntiFlicker(deviceid, &flicker);
    if(flicker)
    {
        if(flicker==1 || flicker == 2)
        {
            ui->cbPowerFreq->setCurrentIndex(flicker-1);
            ui->chkAntiFlicker->setChecked(true);
        }
    }
    else ui->chkAntiFlicker->setChecked(false);       
}

void SettingDialog::on_rdParam0_clicked()
{
    CameraLoadParameter(deviceid,0);
    InitParameter();
    InitWBGain();
}

void SettingDialog::on_rdParam1_clicked()
{
    CameraLoadParameter(deviceid,1);
    InitParameter();
    InitWBGain();
}

void SettingDialog::on_btnWB_clicked()
{
    CameraOnePushWB(deviceid);
    InitWBGain();
}

void SettingDialog::on_hsRedGain_valueChanged(int value)
{
    CameraSetWBGain(deviceid,value/100.0, ui->hsGreenGain->value()/100.0, ui->hsBlueGain->value()/100.0);
    //InitWBGain();
    ui->lblRedGain->setText(QString("%1").arg(value/100.0,2));
}

void SettingDialog::on_hsGreenGain_valueChanged(int value)
{
    CameraSetWBGain(deviceid,ui->hsRedGain->value()/100.0, value/100.0, ui->hsBlueGain->value()/100.0);
    //InitWBGain();
    ui->lblGreenGain->setText(QString("%1").arg(value/100.0,2));
}

void SettingDialog::on_hsBlueGain_valueChanged(int value)
{
    CameraSetWBGain(deviceid,ui->hsRedGain->value()/100.0, ui->hsGreenGain->value()/100.0, value/100.0);
    //InitWBGain();
    ui->lblBlueGain->setText(QString("%1").arg(value/100.0,2));
}
void SettingDialog::switchLanguage()
{
    ui->retranslateUi(this);
}

void SettingDialog::on_hsBlackLevel_valueChanged(int value)
{
    CameraSetBlackLevel(deviceid,value);
    ui->lblBlack->setText(QString("%1").arg(value));
}

void SettingDialog::on_hsAETarget_valueChanged(int value)
{
    CameraSetAETarget(deviceid,value);
    ui->lblTarget->setText(QString("%1").arg(value));
}


void SettingDialog::on_btnDefault_clicked()
{
}

void SettingDialog::on_rbCrop_clicked()
{
    CameraSetResolutionMode(deviceid,0);
}

void SettingDialog::on_rbSkip_clicked()
{
    CameraSetResolutionMode(deviceid,1);
}

void SettingDialog::on_rbBin_clicked()
{
    CameraSetResolutionMode(deviceid,2);
}


void SettingDialog::on_hsFrameSpeed_valueChanged(int value)
{
    ui->leFrameSpeed->setText(QString("%1").arg(value));
    CameraSetDelay(deviceid,value*100);
}


void SettingDialog::on_chkEnhancement_toggled(bool checked)
{
    CameraSetEnhancement(deviceid,checked);
}


void SettingDialog::on_chkAWB_toggled(bool checked)
{
    ui->hsRedGain->setEnabled(!checked);
    ui->hsGreenGain->setEnabled(!checked);
    ui->hsBlueGain->setEnabled(!checked);
    CameraSetAWB(deviceid, checked);
}


void SettingDialog::on_chkAntiFlicker_toggled(bool checked)
{
    if(checked)
    {
        int type = ui->cbPowerFreq->currentIndex()+1;
        CameraSetAntiFlicker(deviceid, type);
    }
    else
        CameraSetAntiFlicker(deviceid, 0);
}

void SettingDialog::on_rbBilinear_clicked()
{
    CameraSetInterpolation(deviceid,0);
}

void SettingDialog::on_rbAdaptive_clicked()
{
    CameraSetInterpolation(deviceid,1);
}

void SettingDialog::on_rbHaminton_clicked()
{
    CameraSetInterpolation(deviceid,2);
}

void SettingDialog::on_cbHighspeed_toggled(bool checked)
{
    CameraSetHighspeed(deviceid, checked);
}
