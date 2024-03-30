#pragma execution_character_set("utf-8")
#include "widget.h"
#include "ui_widget.h"
#include "salt.h"
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("OpenCV高斯滤波");
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


void Widget::on_btn_saltPic_clicked()
{
    Salt tmp;
    int saltNum = ui->spb_saltPram->value();
    saltImg = tmp.setSalt(grayImg, saltNum);
    QImage qImg_Salt = cvMat2QImage(saltImg);
    ui->lbl_saltPic->setPixmap(QPixmap::fromImage(qImg_Salt.scaled(ui->lbl_saltPic->size())));
}


void Widget::on_spb_saltPram_valueChanged(int arg1)
{
    Salt tmp;
    saltImg = tmp.setSalt(grayImg, arg1);
    QImage qImg_Salt = cvMat2QImage(saltImg);
    ui->lbl_saltPic->setPixmap(QPixmap::fromImage(qImg_Salt.scaled(ui->lbl_saltPic->size())));
}


void Widget::on_btn_GaussFilterCV_clicked()
{
    int gaussPram = ui->spb_GaussPram->value();
    GaussianBlur(saltImg, gaussImg_CV, Size(gaussPram,gaussPram),2,2);
    QImage qImg_gaussCV = cvMat2QImage(gaussImg_CV);
    ui->lbl_GaussFilterCV->setPixmap(QPixmap::fromImage(qImg_gaussCV.scaled(ui->lbl_GaussFilterCV->size())));
}


void Widget::on_btn_GaussFilterOwn_clicked()
{
    int gaussPram = ui->spb_GaussPram->value();
    GaussFilterOwn(saltImg, gaussImg_Own, gaussPram);
    QImage qImg_gaussOwn = cvMat2QImage(gaussImg_Own);
    ui->lbl_GaussFilterOwn->setPixmap(QPixmap::fromImage(qImg_gaussOwn.scaled(ui->lbl_GaussFilterOwn->size())));
}


void Widget::on_spb_GaussPram_valueChanged(int arg1)
{
    GaussianBlur(saltImg, gaussImg_CV, Size(arg1,arg1),2,2);
    QImage qImg_gaussCV = cvMat2QImage(gaussImg_CV);
    ui->lbl_GaussFilterCV->setPixmap(QPixmap::fromImage(qImg_gaussCV.scaled(ui->lbl_GaussFilterCV->size())));
    GaussFilterOwn(saltImg, gaussImg_Own, arg1);
    QImage qImg_gaussOwn = cvMat2QImage(gaussImg_Own);
    ui->lbl_GaussFilterOwn->setPixmap(QPixmap::fromImage(qImg_gaussOwn.scaled(ui->lbl_GaussFilterOwn->size())));
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

void Widget::GaussFilterOwn(Mat img_input, Mat &img_output, int size)
{
    img_output = img_input.clone();
    int k = (size - 1)/2;
    for(int i=k; i<(img_input.rows-k); i++)
    {
        for(int j=k; j<(img_input.cols-k); j++)
        {
            double sum = 0.0;
            double sum1 = 0.0;
            double sigma = 7;
            double g;
            for(int n = -k; n < k + 1; n++)
            {
                for(int m = -k; m < k+1; m++)
                {
                    g = exp(-(m*m + n*n)/(2*sigma*sigma));
                    sum = sum + g*img_input.at<uchar>(i+m,j+n);
                    sum1 = sum1 + g;
                }
            }
            img_output.at<uchar>(i,j) = (int)round(sum/sum1);
        }
    }
}

void Widget::on_btn_resetPic_clicked()
{
    ui->spb_saltPram->setValue(1000);
    ui->spb_GaussPram->setValue(3);
    ui->lbl_grayPic->clear();
    ui->lbl_saltPic->clear();
    ui->lbl_GaussFilterCV->clear();
    ui->lbl_GaussFilterOwn->clear();
}

