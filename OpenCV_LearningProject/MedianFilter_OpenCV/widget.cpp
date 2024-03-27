#pragma execution_character_set("utf-8")
#include "widget.h"
#include "ui_widget.h"
#include "salt.h"
#include <QTime>
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("OpenCV图像中值滤波");
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


void Widget::on_btn_MedianOpenCV_clicked()
{
    QTime timer;
    timer.start();
    medianBlur(grayImg, medianImg_OpenCV, 7);
    int takeTime = timer.elapsed();
    QImage qImg_medianOpencv = cvMat2QImage(medianImg_OpenCV);
    ui->lbl_MedianOpenCV->setPixmap(QPixmap::fromImage(qImg_medianOpencv.scaled(ui->lbl_MedianOpenCV->size())));
    ui->lbl_takeTimeCV->setText(QString::number(takeTime).append("ms"));
}


void Widget::on_btn_MedianBasic_clicked()
{
    QTime timer;
    timer.start();
    MedianFilterOwn(grayImg, medianImg_Own, 7);
    int takeTime = timer.elapsed();
    QImage qImg_medianOwn = cvMat2QImage(medianImg_Own);
    ui->lbl_MedianBasic->setPixmap(QPixmap::fromImage(qImg_medianOwn.scaled(ui->lbl_MedianBasic->size())));
    ui->lbl_takeTimeOwn->setText(QString::number(takeTime).append("ms"));
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

void Widget::on_btn_resetPic_clicked()
{
    ui->lbl_grayPic->clear();
    ui->lbl_saltPic->clear();
    ui->lbl_MedianOpenCV->clear();
    ui->lbl_MedianBasic->clear();
    ui->lbl_takeTimeCV->clear();
    ui->lbl_takeTimeOwn->clear();
}


void Widget::on_btn_saltPic_clicked()
{
    Salt tmp;
    saltImg = tmp.setSalt(grayImg, 1000);
    QImage qImg_Salt = cvMat2QImage(saltImg);
    ui->lbl_saltPic->setPixmap(QPixmap::fromImage(qImg_Salt.scaled(ui->lbl_saltPic->size())));
}

void Widget::MedianFilterOwn(Mat img_input, Mat &img_output, int size)
{
    img_output = img_input.clone();
    Mat sortArray(1, size*size, CV_8U);
    int k = (size - 1)/2;
    for(int i=k; i<(img_input.rows-k); i++)
    {
        for(int j=k; j<(img_input.cols-k); j++)
        {
            // 获取核的每个值
            int h=0;
            for(int m=-k; m<k+1; m++)
            {
                for(int n=-k;n<k+1; n++)
                {
                    sortArray.at<uchar>(h) = img_input.at<uchar>(i+m, j+n);
                    h++;
                }
            }
            // 获得核的中值，赋值给中间像素点
            img_output.at<uchar>(i,j) = sortCompute(sortArray,size*size);
        }
    }
}

int Widget::sortCompute(Mat& sortarray, int filterSize)
{
    int exchange, tmpNum;
    for(int i=1; i<=filterSize-1; i++)
    {
        for(int j=1; j<=filterSize-i; j++)
        {
            if(sortarray.at<uchar>(j-1) > sortarray.at<uchar>(j))
            {
                exchange = sortarray.at<uchar>(j-1);
                sortarray.at<uchar>(j-1) = sortarray.at<uchar>(j);
                sortarray.at<uchar>(j) = exchange;
            }
        }
    }
    tmpNum = sortarray.at<uchar>((filterSize + 1)/2);
    return tmpNum;

}
