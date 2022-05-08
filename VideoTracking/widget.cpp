#pragma execution_character_set("utf-8")
#include "widget.h"
#include "ui_widget.h"
#include <iostream>
#include <QDebug>
using namespace std;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    timer->stop();
    connect(timer,SIGNAL(timeout()),this,SLOT(readFrame()));
    openCam = true;

    cap = new VideoCapture(0);
    frameWidth = cap->get(CAP_PROP_FRAME_WIDTH);
    frameHeight = cap->get(CAP_PROP_FRAME_HEIGHT);
    fps = cap->get(CAP_PROP_FPS);
    qDebug()<<"width"<<frameWidth<<frameHeight<<fps;
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_btnOpenVedio_clicked()
{
    if(openCam)
    {
        ui->btnOpenVedio->setText("关闭摄像头");
        timer->start(30);
    }
    else {
        ui->btnOpenVedio->setText("打开摄像头");
        timer->stop();
    }
    openCam = !openCam;
}


QImage Widget::cvMat2QImage(const cv::Mat &mat)
{
    switch ( mat.type() )
    {
    // 8-bit  4 channel
    case CV_8UC4:
    {
        QImage image( (const uchar*)mat.data, mat.cols, mat.rows, static_cast<int>(mat.step), QImage::Format_RGB32 );
        return image;
    }

        // 8-bit  3 channel
    case CV_8UC3:
    {
        QImage image( (const uchar*)mat.data, mat.cols, mat.rows, static_cast<int>(mat.step), QImage::Format_RGB888 );
        return image.rgbSwapped();
    }

        // 8-bit  1 channel
    case CV_8UC1:
    {
        static QVector<QRgb>  sColorTable;
        // only create our color table once
        if ( sColorTable.isEmpty() )
        {
            sColorTable.resize( 256 );
            for ( int i = 0; i < 256; ++i )
            {
                sColorTable[i] = qRgb( i, i, i );
            }
        }
        QImage image( (const uchar*)mat.data, mat.cols, mat.rows, static_cast<int>(mat.step), QImage::Format_Indexed8 );
        image.setColorTable( sColorTable );
        return image;
    }

    default:
        qDebug("Image format is not supported: depth=%d and %d channels\n", mat.depth(), mat.channels());
        qWarning() << "cvMatToQImage - cv::Mat image type not handled in switch:" << mat.type();
        break;
    }

    return QImage();
}


void Widget::on_btnQuit_clicked()
{
    timer->stop();
    cap->release();
    close();
}

void Widget::readFrame()
{
    Mat matFrame;
    cap->read(matFrame);
    if(isTrack)
    {
        Mat grayframePre,frameDet;
        Mat frameNow,grayframeNow;
        cvtColor(matFrame,grayframeNow,COLOR_RGB2GRAY);
        cvtColor(framePre,grayframePre,COLOR_RGB2GRAY);
        GaussianBlur(grayframeNow,grayframeNow,Size(21,21),0,0);
        GaussianBlur(grayframePre,grayframePre,Size(21,21),0,0);
        absdiff(grayframeNow,grayframePre,frameDet);
        framePre = matFrame;
        threshold(frameDet,frameDet,20,255,THRESH_BINARY);
        Mat element = getStructuringElement(0,Size(3,3));
        vector<vector<Point>> contours;
        dilate(frameDet,frameDet,element);
        findContours(frameDet,contours,RETR_TREE,CHAIN_APPROX_SIMPLE,Point());
        qDebug()<<"Num"<<contours.size();
        QString SavePath = "D:/ImgPath/" + QString::number(VideoNum) + "_track.avi";
        if(contours.size()==0)
        {
            if(writer.isOpened())
            {
                writer.release();
            }

            if(isSaveFrame)
            {
                isSaveFrame = false;
                VideoNum++;
            }
        }
        else
        {
            for(int i=0;i<contours.size();i++)
            {
                double area = contourArea(contours[i]);
                if(area < 100)continue;
                else
                {
                    qDebug()<<"有物体运动！";
                    if(!isSaveFrame)
                    {
                        int fourcc = writer.fourcc('M', 'J', 'P', 'G');
                        writer.open(SavePath.toStdString(),fourcc,10,Size(frameWidth,frameHeight),true);
                        isSaveFrame = true;
                    }
                    else
                    {
                        writer.write(matFrame);
                    }
                    break;
                }
            }
        }
    }
    else
    {
        framePre = matFrame;
    }
    QImage Qimg = cvMat2QImage(matFrame);
    ui->picshow->setPixmap(QPixmap::fromImage(Qimg));

}

void Widget::on_ckb_Track_clicked(bool checked)
{
    if(checked)
    {
        isTrack = true;
    }
    else {
        isTrack = false;
    }

}
