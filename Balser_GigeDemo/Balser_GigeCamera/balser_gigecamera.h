#ifndef BALSER_GIGECAMERA_H
#define BALSER_GIGECAMERA_H
/***************************
* @projectName   camera_test
* @brief         Basler相机图像采集及参数设置功能；
* @author        Alaric
* @date          2018-11-04
**************************/

#include <QImage>
#include "balser_gigecamera_global.h"
#include <QObject>

#include <pylon/PylonIncludes.h>
#include <pylon/BaslerUniversalInstantCamera.h>
#include <pylon/_BaslerUniversalCameraParams.h>
#include <QMutex>

using namespace std;
using namespace Pylon;
using namespace GenApi;
using namespace Basler_UniversalCameraParams;

class BALSER_GIGECAMERASHARED_EXPORT Balser_GigeCamera : public QObject, public CImageEventHandler
{
    Q_OBJECT
public:
    Balser_GigeCamera(QObject *parent = nullptr);
    ~Balser_GigeCamera();

    enum BaslerCamera_Type{
        Type_Basler_Freerun, //设置相机的内触发
        Type_Basler_Line1, //设置相机的外触发
        Type_Basler_ExposureTimeAbs, //设置相机的曝光时间
        Type_Basler_GainRaw, //设置相机的增益
        Type_Basler_AcquisitionFrameRateAbs, //设置相机的频率
        Type_Basler_Width, //图片的宽度
        Type_Basler_Height, //图片的高度
        Type_Basler_LineSource, //灯的触发信号
    };

    void initCamera();                   //初始化相机
    void OpenCamera();                   //打开相机
    void CloseCamera();                   //关闭相机
    void deleteAll();                    //删除相机

    int getExposureTime();                //获得曝光时间
    void setExposureTime(int time);       //设置曝光时间
    int getGain();                        //获得增益
    void setGain(int value);              //设置增益
    void setFrameRate(int value);         //设置帧率
    int getFrameRate();                   //获得帧率
    void OneKeyAutoExTime();              //一键自动曝光
    void SetCamera(Balser_GigeCamera::BaslerCamera_Type index, int tmpValue = 0); // 设置各种参数
    int GetCamera(Balser_GigeCamera::BaslerCamera_Type index); // 获取各种参数
    void AutoExposureOnce(CBaslerUniversalInstantCamera& camera);      //自动曝光

    void StartAcquire();  //开始采集
    void StopAcquire();   //结束采集
    void StartSnap();     //抓图

    bool GrabOnLine_Signal;     //实时采图信号

signals:
    void canShowImg(QImage img);         //发送图像数据

public slots:
    void WorkTypeSignal(int work_type);         //接收图像处理信号，0为Once,1为Online;
private:
    CBaslerUniversalInstantCamera m_basler;
    CGrabResultPtr m_ptrGrabResult;
    CImageFormatConverter m_formatConverter;//Basler 图片格式转换类
    CPylonImage pylonImage; //Basler 图像格式
    QMutex m_mutexLock;

    int TestImg_WorkType = -1;          //图像处理方式，默认-1为不处理，0为Once，1为Online；
    QImage CopyImgToQImage(CGrabResultPtr ptrGrabResult);

protected:
    virtual void OnImageGrabbed(CInstantCamera &camera, const CGrabResultPtr &grabResult);
};

#endif // BALSER_GIGECAMERA_H
