#include "balser_gigecamera.h"
#include <QDebug>

Balser_GigeCamera::Balser_GigeCamera(QObject *parent) : QObject(parent)
{

}

Balser_GigeCamera::~Balser_GigeCamera()
{

}

void Balser_GigeCamera::initCamera()
{
    PylonInitialize();

    m_basler.RegisterImageEventHandler(this, RegistrationMode_Append, Cleanup_Delete);
    m_basler.Attach(CTlFactory::GetInstance().CreateFirstDevice(),Cleanup_Delete);
    qDebug()<<"Using device " << m_basler.GetDeviceInfo().GetModelName()<<endl;
    m_basler.Open();
    if (!m_basler.IsOpen() || m_basler.IsGrabbing())
    {
        qDebug()<<"camera open failed"<<endl;
        return;
    }
}

void Balser_GigeCamera::CloseCamera()
{
    if(m_basler.IsOpen()) {
        m_basler.DetachDevice();
        m_basler.Close();
        m_basler.DestroyDevice();
        m_ptrGrabResult.Release();
    }
}

void Balser_GigeCamera::deleteAll()
{
    //停止采集
    if(GrabOnLine_Signal) {
        StopAcquire();
    }
    //关闭摄像头
    try
    {
        CloseCamera();
        m_basler.DeregisterImageEventHandler(this);

        //关闭库
        qDebug() << "SBaslerCameraControl deleteAll: PylonTerminate" ;
        PylonTerminate();
    }
    catch (const Pylon::GenericException& e)
    {
        qDebug() << e.what();
    }
}

int Balser_GigeCamera::getExposureTime()
{
    return GetCamera(Type_Basler_ExposureTimeAbs);
}

void Balser_GigeCamera::setExposureTime(int time)
{
    SetCamera(Type_Basler_ExposureTimeAbs, time);
}

void Balser_GigeCamera::setGain(int value)
{
    SetCamera(Type_Basler_GainRaw, value);
}

int Balser_GigeCamera::getGain()
{
    return GetCamera(Type_Basler_GainRaw);
}

int Balser_GigeCamera::getFrameRate()
{
    return GetCamera(Type_Basler_AcquisitionFrameRateAbs);
}

void Balser_GigeCamera::setFrameRate(int value)
{
    SetCamera(Type_Basler_AcquisitionFrameRateAbs, value);
}

void Balser_GigeCamera::OneKeyAutoExTime()
{
    AutoExposureOnce(m_basler);
}

void Balser_GigeCamera::SetCamera(Balser_GigeCamera::BaslerCamera_Type index, int tmpValue)
{
    INodeMap &cameraNodeMap = m_basler.GetNodeMap();
    switch (index) {
    case Type_Basler_Freerun: {
        CEnumerationPtr  ptrTriggerSel = cameraNodeMap.GetNode ("TriggerSelector");
        ptrTriggerSel->FromString("FrameStart");
        CEnumerationPtr  ptrTrigger  = cameraNodeMap.GetNode ("TriggerMode");
#ifdef Real_Freerun
        ptrTrigger->SetIntValue(0);
#else //Software
        ptrTrigger->SetIntValue(1);
        CEnumerationPtr  ptrTriggerSource = cameraNodeMap.GetNode ("TriggerSource");
        ptrTriggerSource->FromString("Software");
#endif
    } break;
    case Type_Basler_Line1: {
        CEnumerationPtr  ptrTriggerSel = cameraNodeMap.GetNode ("TriggerSelector");
        ptrTriggerSel->FromString("FrameStart");
        CEnumerationPtr  ptrTrigger  = cameraNodeMap.GetNode ("TriggerMode");
        ptrTrigger->SetIntValue(1);
        CEnumerationPtr  ptrTriggerSource = cameraNodeMap.GetNode ("TriggerSource");
        ptrTriggerSource->FromString("Line1");
    } break;
    case Type_Basler_ExposureTimeAbs: {
        const CFloatPtr exposureTime = cameraNodeMap.GetNode("ExposureTimeAbs");
        exposureTime->SetValue(tmpValue);
    } break;
    case Type_Basler_GainRaw: {
        const CIntegerPtr cameraGen = cameraNodeMap.GetNode("GainRaw");
        cameraGen->SetValue(tmpValue);
    } break;
    case Type_Basler_AcquisitionFrameRateAbs: {
        const CBooleanPtr frameRate = cameraNodeMap.GetNode("AcquisitionFrameRateEnable");
        frameRate->SetValue(TRUE);
        const CFloatPtr frameRateABS = cameraNodeMap.GetNode("AcquisitionFrameRateAbs");
        frameRateABS->SetValue(tmpValue);
    } break;
    case Type_Basler_Width: {
        const CIntegerPtr widthPic = cameraNodeMap.GetNode("Width");
        widthPic->SetValue(tmpValue);
    } break;
    case Type_Basler_Height: {
        const CIntegerPtr heightPic = cameraNodeMap.GetNode("Height");
        heightPic->SetValue(tmpValue);
    } break;
    case Type_Basler_LineSource: {
        CEnumerationPtr  ptrLineSource = cameraNodeMap.GetNode ("LineSource");
        ptrLineSource->SetIntValue(2);
    } break;
    default:
        break;
    }
}

int Balser_GigeCamera::GetCamera(Balser_GigeCamera::BaslerCamera_Type index)
{
    INodeMap &cameraNodeMap = m_basler.GetNodeMap();
    switch (index) {
    case Type_Basler_ExposureTimeAbs: {
        const CFloatPtr exposureTime = cameraNodeMap.GetNode("ExposureTimeAbs");
        return exposureTime->GetValue();
    } break;
    case Type_Basler_GainRaw: {
        const CIntegerPtr cameraGen = cameraNodeMap.GetNode("GainRaw");
        return cameraGen->GetValue();
    } break;
    case Type_Basler_AcquisitionFrameRateAbs: {
        const CBooleanPtr frameRate = cameraNodeMap.GetNode("AcquisitionFrameRateEnable");
        frameRate->SetValue(TRUE);
        const CFloatPtr frameRateABS = cameraNodeMap.GetNode("AcquisitionFrameRateAbs");
        return frameRateABS->GetValue();
    } break;
    case Type_Basler_Width: {
        const CIntegerPtr widthPic = cameraNodeMap.GetNode("Width");
        return widthPic->GetValue();
    } break;
    case Type_Basler_Height: {
        const CIntegerPtr heightPic = cameraNodeMap.GetNode("Height");
        return heightPic->GetValue();
    } break;
    default:
        return -1;
        break;
    }
}

void Balser_GigeCamera::OnImageGrabbed(CInstantCamera &camera, const CGrabResultPtr &grabResult)
{
    m_mutexLock.lock();
    qDebug() <<"Capturest"<<endl;
    if (grabResult->GrabSucceeded())
    {
        m_ptrGrabResult = grabResult;//将捕获到的图像传递出去
        qDebug() <<"Captureok"<<endl;
        QImage CurrentImg;
        CurrentImg = CopyImgToQImage(m_ptrGrabResult);
        if(TestImg_WorkType == 1)
        {
            emit canShowImg(CurrentImg);
            //            startImgProcess(CurrentImg);
            qDebug() <<"Captureok"<<endl;
        }
        else{
            emit canShowImg(CurrentImg);
        }
    }

    m_mutexLock.unlock();
}

void Balser_GigeCamera::StartAcquire()
{

    if ( !m_basler.IsGrabbing() ){
        qDebug()<<"grabstart"<<endl;
        GrabOnLine_Signal = true;
        m_basler.StartGrabbing(GrabStrategy_LatestImageOnly,GrabLoop_ProvidedByInstantCamera);
    }
    else {
        qDebug()<<"error"<<endl;
    }
}

void Balser_GigeCamera::StartSnap()
{
    m_basler.StartGrabbing(1);
    CBaslerUniversalGrabResultPtr ptrGrabResult;
    m_basler.RetrieveResult( 5000, ptrGrabResult, TimeoutHandling_ThrowException);
    if (ptrGrabResult->GrabSucceeded())
    {
        qDebug()<<"snapok"<<endl;
        QImage CurrentImg = CopyImgToQImage(ptrGrabResult);
        emit canShowImg(CurrentImg);
        if(TestImg_WorkType == 0){
            //            startImgProcess(CurrentImg);
            TestImg_WorkType = -1;
        }
    }
}

void Balser_GigeCamera::StopAcquire()
{
    if ( m_basler.IsGrabbing()){
        GrabOnLine_Signal = false;
        m_basler.StopGrabbing();  // 停止抓取图像
    }
}

void Balser_GigeCamera::AutoExposureOnce(CBaslerUniversalInstantCamera& camera)
{
    if ( !camera.ExposureAuto.IsWritable())
    {
        cout << "The camera does not support Exposure Auto." << endl << endl;
        return;
    }

    camera.ExposureAuto.SetValue(ExposureAuto_Once);
    int n = 0;
    while (camera.ExposureAuto.GetValue() != ExposureAuto_Off)
    {
        ++n;

        //For demonstration purposes only. Wait until the image is shown.
        WaitObject::Sleep(100);

        //Make sure the loop is exited.
        if (n > 100)
        {
            throw TIMEOUT_EXCEPTION( "The adjustment of auto exposure did not finish.");
        }
    }
}

void Balser_GigeCamera::WorkTypeSignal(int work_type)
{
    TestImg_WorkType = work_type;
}

QImage Balser_GigeCamera::CopyImgToQImage(CGrabResultPtr ptrGrabResult)
{
    QImage Qimg;
    //格式转换
    m_formatConverter.Convert(pylonImage, ptrGrabResult);
    uchar * din = (uchar *)(pylonImage.GetBuffer()); //数据指针

    //单通道Momo
    if(IsMono(pylonImage.GetPixelType()))
    {
        qDebug()<<"黑白图像";
        Qimg = QImage(din, ptrGrabResult->GetWidth(), ptrGrabResult->GetHeight(), QImage::Format_Indexed8);
    }
    else
    {
        //此处可以根据相机的格式获取特定格式的彩色图像
        if(IsRGB(pylonImage.GetPixelType()))
        {
            Qimg = QImage(din, ptrGrabResult->GetWidth(), ptrGrabResult->GetHeight(), QImage::Format_RGB888);
        }
    }

    return Qimg;
}
