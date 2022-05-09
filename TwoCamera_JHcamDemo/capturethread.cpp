#include "capturethread.h"
#include "JHCap.h"
#include "Windows.h"

#include <QDebug>
#include <QDateTime>

CaptureThread::CaptureThread(QObject *parent) :
    QThread(parent)
{
    work = false;
    term = false;
    deviceid = 0;
    viewid = 0;


    for(int i = 0; i < 256; i++)
    {
       grayColourTable.append(qRgb(i, i, i));
    }
}

//#define SAVERAW

void CaptureThread::run()
{
    while(1)
    {

        int width, height, size;

        if(work)
        {
            CameraGetImageSize(deviceid, &width, &height);
            CameraGetImageBufferSize(deviceid, &size, CAMERA_IMAGE_RGB24);

            unsigned char *buffer = new unsigned char[size];
            if(!CameraQueryImage(deviceid, buffer, &size, CAMERA_IMAGE_RGB24))
            {
               // qDebug()<<"captured:"<<deviceid;
                if(term) break;

#ifdef SAVERAW
                int static s=0;
                FILE *fp;
                fp = fopen("c:\\a.raw", "wb");
                fwrite(buffer, 2592*1944,1,fp);
                if(s++==3)
                    work=false;
#endif

                //RAW8 GRAY8
                //QImage img(buffer, width, height, QImage::Format_Indexed8);
                //img.setColorTable(grayColourTable);
                //RGB24
                QImage img(buffer, width, height, QImage::Format_RGB888);

                /*if(get_trigger_mode())
                {
                    static int i=0;
                    QDateTime d = QDateTime::currentDateTime();
                    QString suggestFileName = QString("snapshot-%1-%2.bmp").arg( d.toString("yyyyMMddhhmmss")).arg(i++);
                    img.save(suggestFileName);
                }*/
                qint64 timestamp = QDateTime::currentMSecsSinceEpoch();
                emit captured(viewid, img, buffer, timestamp);
            }
            else
            {
                delete [] buffer;
            }
        } else usleep(1000);
        if(term) break;
    }

    //qDebug() << "exit capture";
}

void CaptureThread::stream()
{
    work = true;
}

void CaptureThread::pause()
{
    work = false;
}

void CaptureThread::stop()
{
    work = false;
    term = true;
}
