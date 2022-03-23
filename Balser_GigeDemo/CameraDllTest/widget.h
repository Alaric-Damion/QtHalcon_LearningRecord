#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "../include/balser_gigecamera.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_btnOpenCamera_clicked();

    void on_btnGrabImage_clicked();

    void on_btnSnapImage_clicked();

    void on_btnAutoExTime_clicked();

    void on_btnStopGrab_clicked();

    void on_DisplayImg(QImage img);

    void SetExTime();
    void SetGain();

    void on_ckb_ModifyFps_stateChanged(int arg1);

    void on_txt_FrameRate_editingFinished();

private:
    Ui::Widget *ui;

    void initSystem();
    void initCameraParm();

    Balser_GigeCamera *m_control = NULL;

    QString DefaultFps;


};

#endif // WIDGET_H
