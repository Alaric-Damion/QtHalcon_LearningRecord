#pragma execution_character_set("utf-8")
#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QTime>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("OpenCV图像拉普拉斯锐化");
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


void Widget::on_btn_LaplacianCV_clicked()
{
    QTime timer;
    timer.start();
    Mat tmpImage;
    Laplacian(grayImg, tmpImage, grayImg.depth());
    int takeTime = timer.elapsed();
    QImage qImg_LaplacianCV = cvMat2QImage(tmpImage);
    ui->lbl_LaplacianCV->setPixmap(QPixmap::fromImage(qImg_LaplacianCV.scaled(ui->lbl_LaplacianCV->size())));
    ui->lbl_takeTimeCV->setText(QString::number(takeTime).append("ms"));
}


void Widget::on_btn_LaplacianOwn4_clicked()
{
    QTime timer;
    timer.start();
    Mat tmpImage;
    LaplacianOwn(grayImg, tmpImage, 4);
    int takeTime = timer.elapsed();
    QImage qImg_LaplacianOwn4 = cvMat2QImage(tmpImage);
    ui->lbl_LaplacianOwn4->setPixmap(QPixmap::fromImage(qImg_LaplacianOwn4.scaled(ui->lbl_LaplacianOwn4->size())));
    ui->lbl_takeTimeOwn4->setText(QString::number(takeTime).append("ms"));
}


void Widget::on_btn_LaplacianOwn8_clicked()
{
    QTime timer;
    timer.start();
    Mat tmpImage;
    LaplacianOwn(grayImg, tmpImage, 8);
    int takeTime = timer.elapsed();
    QImage qImg_LaplacianOwn8 = cvMat2QImage(tmpImage);
    ui->lbl_LaplacianOwn8->setPixmap(QPixmap::fromImage(qImg_LaplacianOwn8.scaled(ui->lbl_LaplacianOwn8->size())));
    ui->lbl_takeTimeOwn8->setText(QString::number(takeTime).append("ms"));
}


void Widget::on_btn_resetPic_clicked()
{
    ui->lbl_grayPic->clear();
    ui->lbl_LaplacianCV->clear();
    ui->lbl_LaplacianOwn4->clear();
    ui->lbl_LaplacianOwn8->clear();
    ui->lbl_takeTimeCV->clear();
    ui->lbl_takeTimeOwn4->clear();
    ui->lbl_takeTimeOwn8->clear();
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

void Widget::LaplacianOwn(Mat img_input, Mat &img_output, int type)
{
    img_output = img_input.clone();
    int tmp;
    for (int i = 1; i < (img_input.rows-1); i++)
    {
        for (int j = 1; j < (img_input.cols - 1); j++)
        {
            if(type == 4)       //四邻域
            {
                tmp = 4 * img_input.at<uchar>(i, j) - img_input.at<uchar>(i + 1, j) - img_input.at<uchar>(i - 1, j) - img_input.at<uchar>(i, j + 1) - img_input.at<uchar>(i, j - 1);
                img_output.at<uchar>(i, j) = saturate_cast<uchar>(img_output.at<uchar>(i, j) + tmp);
            }
            else                //八邻域
            {
                tmp = 8 * img_input.at<uchar>(i, j) - img_input.at<uchar>(i + 1, j) - img_input.at<uchar>(i - 1, j) - img_input.at<uchar>(i, j + 1) - img_input.at<uchar>(i, j - 1)
                      - img_input.at<uchar>(i-1, j-1)- img_input.at<uchar>(i+1, j+1)- img_input.at<uchar>(i-1, j+1)- img_input.at<uchar>(i+1, j-1);
                img_output.at<uchar>(i, j) = saturate_cast<uchar>(img_output.at<uchar>(i, j) + tmp);
            }
        }
    }
    img_output = img_input - img_output;
}
