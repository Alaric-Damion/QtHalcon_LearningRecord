#pragma execution_character_set("utf-8")
#include "widget.h"
#include "ui_widget.h"
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("OpenCV图像变换");
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_btn_loadPic_clicked()
{
    Mat Img = imread("lena.png");
    cvtColor(Img, grayImg, COLOR_BGR2GRAY);

    QImage qImg_Gray = cvMat2QImage(grayImg);
    ui->lbl_grayPic->setPixmap(QPixmap::fromImage(qImg_Gray.scaled(ui->lbl_grayPic->size())));
}


void Widget::on_btn_InversionTrans_clicked()
{
    Mat InversionImg = grayImg.clone();
    for(int i=0;i<grayImg.rows;i++)
    {
        for(int j=0;j<grayImg.cols;j++)
        {
            InversionImg.at<uchar>(i,j) = 255 - grayImg.at<uchar>(i,j);
        }
    }
    QImage qImg_Inversion = cvMat2QImage(InversionImg);
    ui->lbl_InversionPic->setPixmap(QPixmap::fromImage(qImg_Inversion.scaled(ui->lbl_InversionPic->size())));
}


void Widget::on_btn_logarithmTrans_clicked()
{
    Mat LogarithmImg = grayImg.clone();
    for(int i=0;i<grayImg.rows;i++)
    {
        for(int j=0;j<grayImg.cols;j++)
        {
            LogarithmImg.at<uchar>(i,j) = 6*log((double)grayImg.at<uchar>(i,j) + 1);
        }
    }
    normalize(LogarithmImg, LogarithmImg, 0, 255,NORM_MINMAX);
    convertScaleAbs(LogarithmImg,LogarithmImg);
    QImage qImg_Logarithm = cvMat2QImage(LogarithmImg);
    ui->lbl_LogPic->setPixmap(QPixmap::fromImage(qImg_Logarithm.scaled(ui->lbl_LogPic->size())));
}


void Widget::on_btn_gammaTrans_clicked()
{
    Mat gammaImg = grayImg.clone();
    for(int i=0;i<grayImg.rows;i++)
    {
        for(int j=0;j<grayImg.cols;j++)
        {
            gammaImg.at<uchar>(i,j) = 6*pow((double)grayImg.at<uchar>(i,j), 0.5);
        }
    }
    normalize(gammaImg, gammaImg, 0, 255,NORM_MINMAX);
    convertScaleAbs(gammaImg,gammaImg);
    QImage qImg_Gamma = cvMat2QImage(gammaImg);
    ui->lbl_GammaPic->setPixmap(QPixmap::fromImage(qImg_Gamma.scaled(ui->lbl_GammaPic->size())));
}


void Widget::on_btn_resetPic_clicked()
{
    ui->lbl_grayPic->clear();
    ui->lbl_InversionPic->clear();
    ui->lbl_LogPic->clear();
    ui->lbl_GammaPic->clear();
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
