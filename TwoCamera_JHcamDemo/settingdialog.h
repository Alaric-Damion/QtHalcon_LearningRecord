#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QDialog>
#include <QSettings>

namespace Ui {
    class SettingDialog;
}

class SettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingDialog(QWidget *parent = 0);
    ~SettingDialog();
    void switchLanguage();
    void InitWBGain();
    void InitParameter();
    void set_deviceid(int id) {deviceid = id; }

    void addResolutionItem(int width, int height);
    void clearResolutionBox();

private slots:
    void on_hsGamma_valueChanged(int value);
    void on_hsContrast_valueChanged(int value);
    void on_hsGain_valueChanged(int value);
    void on_hsExposure_valueChanged(int value);
    void on_chkAutoExposure_toggled(bool checked);
    void on_chkAutoGain_toggled(bool checked);
    void on_cbMirrorX_toggled(bool checked);
    void on_cbMirrorY_toggled(bool checked);
    void on_btnSaveParam_clicked();
    void on_rdParam0_clicked();
    void on_rdParam1_clicked();
    void on_btnWB_clicked();
    void on_hsRedGain_valueChanged(int value);
    void on_hsGreenGain_valueChanged(int value);
    void on_hsBlueGain_valueChanged(int value);
    void on_hsBlackLevel_valueChanged(int value);
    void on_hsAETarget_valueChanged(int value);
    void on_hsSaturation_valueChanged(int value);
    void on_btnDefault_clicked();

    void on_rbCrop_clicked();
    void on_rbSkip_clicked();
    void on_rbBin_clicked();

    void on_hsFrameSpeed_valueChanged(int value);

    void on_chkEnhancement_toggled(bool checked);
    void on_chkAWB_toggled(bool checked);

    void on_chkAntiFlicker_toggled(bool checked);

    void on_rbBilinear_clicked();

    void on_rbAdaptive_clicked();

    void on_rbHaminton_clicked();

    void on_cbHighspeed_toggled(bool checked);

private:
    Ui::SettingDialog *ui;

    int deviceid;
public:
    int m_captureResoIndex;
};

#endif // SETTINGDIALOG_H
