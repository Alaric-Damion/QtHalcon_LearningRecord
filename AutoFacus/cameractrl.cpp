#include "cameractrl.h"
#include <QTimer>
#include <QEventLoop>
#include <QDebug>
//#include "HalconCpp.h"
//using namespace HalconCpp
#include <QThread>
CameraCtrl::CameraCtrl(QObject *parent) : QObject(parent)
{
    tInter=500;
    isFocus=false;
}

void CameraCtrl::Delay_MSec(unsigned int msec)
{
    QEventLoop loop;//定义一个新的事件循环
    QTimer::singleShot(msec, &loop, SLOT(quit()));//创建单次定时器，槽函数为事件循环的退出函数
    loop.exec();//事件循环开始执行，程序会卡在这里，直到定时时间到，本循环被退出
}

//读取图像并调用Brenner算法函数
void CameraCtrl::HandleContinuousGrab()
{

    HTuple  hv_I;
    HObject  ho_Image;

    for (hv_I=1; hv_I<=30; hv_I+=1)
    {
        //        time_t ref_time=time(NULL);
        ReadImage(&ho_Image, ("E:/Qt_Test/AutoFacus/Buddha/"+hv_I)+".png");
        Delay_MSec(tInter);
        qDebug()<<"eeeeeeeeeeeee"<<tInter;
        //        if(difftime(time(NULL),ref_time)>0.8)
        //        {
        emit hobjectReady(ho_Image);
        if(isFocus==true)
        {
            emit hobjectFocus(ho_Image);
        }
        //        }
    }
}

