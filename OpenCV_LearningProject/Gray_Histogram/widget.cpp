#pragma execution_character_set("utf-8")
#include "widget.h"
#include "ui_widget.h"
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("OpenCV图像直方图处理");
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_btn_loadImg_clicked()
{
    Mat Img1 = imread("lena.png");
    cvtColor(Img1, grayImg1, COLOR_BGR2GRAY);
    QImage qImg_Gray1 = cvMat2QImage(grayImg1);
    ui->lbl_grayImg1->setPixmap(QPixmap::fromImage(qImg_Gray1.scaled(ui->lbl_grayImg1->size())));

    Mat Img2 = imread("mandril_color.tif");
    cvtColor(Img2, grayImg2, COLOR_BGR2GRAY);
    QImage qImg_Gray2 = cvMat2QImage(grayImg2);
    ui->lbl_grayImg2->setPixmap(QPixmap::fromImage(qImg_Gray2.scaled(ui->lbl_grayImg2->size())));

}


void Widget::on_btn_histogram_clicked()
{
//    //直方图
//    Mat hist1,hist2;
//    //获取图像直方图
//    int histsize = 256;
//    float ranges[] = { 0,256 };
//    const float* histRanges = { ranges };
//    calcHist(&grayImg1, 1, 0, Mat(), hist1, 1, &histsize, &histRanges, true, false);
//    calcHist(&grayImg2, 1, 0, Mat(), hist2, 1, &histsize, &histRanges, true, false);


//    //创建直方图显示图像
//    int hist_h = 300;//直方图的图像的高
//    int hist_w = 512; //直方图的图像的宽
//    int bin_w = hist_w / histsize;//直方图的等级
//    Mat histImage1(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));//绘制直方图显示的图像
//    Mat histImage2(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));

//    //绘制并显示直方图
//    normalize(hist1, hist1, 0, hist_h, NORM_MINMAX, -1, Mat());//归一化直方图
//    for (int i = 1; i < histsize; i++)
//    {
//        line(histImage1, Point((i - 1) * bin_w, hist_h - cvRound(hist1.at<float>(i - 1))),
//             Point((i)*bin_w, hist_h - cvRound(hist1.at<float>(i))), Scalar(255, 0, 0), 2, 8, 0);
//    }
    Mat histImage1 = getHistogramImg(grayImg1);
    // 显示直方图
    QImage qImg_hist1 = cvMat2QImage(histImage1);
    ui->lbl_histogram1->setPixmap(QPixmap::fromImage(qImg_hist1.scaled(ui->lbl_histogram1->size())));

//    normalize(hist2, hist2, 0, hist_h, NORM_MINMAX, -1, Mat());//归一化直方图
//    for (int i = 1; i < histsize; i++)
//    {
//        line(histImage2, Point((i - 1) * bin_w, hist_h - cvRound(hist2.at<float>(i - 1))),
//             Point((i)*bin_w, hist_h - cvRound(hist2.at<float>(i))), Scalar(255, 0, 0), 2, 8, 0);
//    }
    Mat histImage2 = getHistogramImg(grayImg2);
    QImage qImg_hist2 = cvMat2QImage(histImage2);
    ui->lbl_histogram2->setPixmap(QPixmap::fromImage(qImg_hist2.scaled(ui->lbl_histogram2->size())));
}


void Widget::on_btn_equilibriumImg_clicked()
{
    Mat enhancedImg1;
    equalizeHist(grayImg1, enhancedImg1);
    QImage qImg_enhanced = cvMat2QImage(enhancedImg1);
    ui->lbl_enhanced->setPixmap(QPixmap::fromImage(qImg_enhanced.scaled(ui->lbl_enhanced->size())));
    Mat enhancedHist = getHistogramImg(enhancedImg1);
    QImage qImg_enhancedHist = cvMat2QImage(enhancedHist);
    ui->lbl_enhancedHist->setPixmap(QPixmap::fromImage(qImg_enhancedHist.scaled(ui->lbl_enhancedHist->size())));
}


void Widget::on_btn_matchImg_clicked()
{
    //均衡化处理
    Mat enhancedImg1,enhancedImg2,hist1,hist2,image_match;
    equalizeHist(grayImg1, enhancedImg1);
    equalizeHist(grayImg2, enhancedImg2);


    //获取两个均衡化图像的直方图
    int histsize = 256;
    float ranges[] = { 0,256 };
    const float* histRanges = { ranges };
    calcHist(&enhancedImg1, 1, 0, Mat(), hist1, 1, &histsize, &histRanges, true, false);
    calcHist(&enhancedImg2, 1, 0, Mat(), hist2, 1, &histsize, &histRanges, true, false);

    //计算两个均衡化图像直方图的累积概率
    float hist1_cdf[256] = { hist1.at<float>(0) };
    float hist2_cdf[256] = { hist2.at<float>(0) };
    for (int i = 1; i < 256; i++)
    {
        hist1_cdf[i] = (hist1_cdf[i - 1] + hist1.at<float>(i));
        hist2_cdf[i] = (hist2_cdf[i - 1] + hist2.at<float>(i));
    }

    for (int i = 0; i < 256; i++)
    {
        hist1_cdf[i] = hist1_cdf[i] / (enhancedImg1.rows * enhancedImg1.cols);
        hist2_cdf[i] = hist2_cdf[i] / (enhancedImg2.rows * enhancedImg2.cols);
    }

    // 两个累计概率之间的差值，用于找到最接近的点
    float diff_cdf[256][256];
    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < 256; j++)
        {
            diff_cdf[i][j] = fabs(hist1_cdf[i] - hist2_cdf[j]);
        }
    }

    Mat lut(1, 256, CV_8U);
    for (int i = 0; i < 256; i++)
    {
        //查找源灰度级为i的映射灰度和i的累积概率差最小(灰度接近)的规定化灰度
        float min = diff_cdf[i][0];
        int index = 0;
        for (int j = 0; j < 256; j++) {
            if (min > diff_cdf[i][j]) {
                min = diff_cdf[i][j];
                index = j;
            }
        }
        lut.at<uchar>(i) = index;
    }
    LUT(enhancedImg1, lut, image_match);  //图像中进行映射
    QImage qImg_match = cvMat2QImage(image_match);
    ui->lbl_matchImg->setPixmap(QPixmap::fromImage(qImg_match.scaled(ui->lbl_matchImg->size())));
    Mat matchImgHist = getHistogramImg(image_match);
    QImage qImg_matchImgHist = cvMat2QImage(matchImgHist);
    ui->lbl_matchImgHist->setPixmap(QPixmap::fromImage(qImg_matchImgHist.scaled(ui->lbl_matchImgHist->size())));
}


void Widget::on_btn_resetImg_clicked()
{
    ui->lbl_grayImg1->clear();
    ui->lbl_grayImg2->clear();
    ui->lbl_histogram1->clear();
    ui->lbl_histogram2->clear();
    ui->lbl_enhanced->clear();
    ui->lbl_matchImg->clear();
    ui->lbl_enhancedHist->clear();
    ui->lbl_matchImgHist->clear();
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

Mat Widget::getHistogramImg(Mat img)
{
    //直方图
    Mat hist1;
    //获取图像直方图
    int histsize = 256;
    float ranges[] = { 0,256 };
    const float* histRanges = { ranges };
    calcHist(&img, 1, 0, Mat(), hist1, 1, &histsize, &histRanges, true, false);

    //创建直方图显示图像
    int hist_h = 300;//直方图的图像的高
    int hist_w = 512; //直方图的图像的宽
    int bin_w = hist_w / histsize;//直方图的等级
    Mat histImage1(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));//绘制直方图显示的图像

    //绘制并显示直方图
    normalize(hist1, hist1, 0, hist_h, NORM_MINMAX, -1, Mat());//归一化直方图
    for (int i = 1; i < histsize; i++)
    {
        line(histImage1, Point((i - 1) * bin_w, hist_h - cvRound(hist1.at<float>(i - 1))),
             Point((i)*bin_w, hist_h - cvRound(hist1.at<float>(i))), Scalar(255, 0, 0), 2, 8, 0);
    }
    return histImage1;
}
