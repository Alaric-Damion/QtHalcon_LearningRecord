#ifndef CAMERACTRL_H
#define CAMERACTRL_H

#include <QObject>

#include <Halcon.h>
#include <HalconCpp.h>
using namespace HalconCpp;


class CameraCtrl : public QObject
{
    Q_OBJECT
public:
    explicit CameraCtrl(QObject *parent = nullptr);

    void HandleContinuousGrab();

    void Delay_MSec(unsigned int msec);
    unsigned int tInter;
    bool isFocus;

private:

signals:

     void hobjectReady(HObject);
     void hobjectFocus(HObject);

};

#endif // CAMERACTRL_H
