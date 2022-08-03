#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <opencv2/opencv.hpp>
//#include <windows.h>
#include<QtWinExtras/QtWinExtras>
#include <opencv2/highgui/highgui_c.h>

#include "Halcon.h"
#include "HalconCpp.h"
//#include <stdio.h>
#include <QMessageBox>

using namespace HalconCpp;
using namespace cv;

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:

    void on_btn_LoadImg_clicked();

    void on_btn_TransImg_clicked();

private:
    Ui::Widget *ui;
    //Halcon窗口句柄
    HTuple hv_WindowHandleCurrent;
    //OpenCV窗口名称
    std::string cv_winName;
    //输入图像格式
    HObject InPut_HobjectImg;
    cv::Mat InPut_MatImg;
    QImage InPut_QImg;
    //输出图像格式
    HObject OutPut_HobjectImg;
    cv::Mat OutPut_MatImg;
    QImage OutPut_QImg;

private:
    //初始化显示窗口(绑定格式)
    void init_DisplayWindow();
    //显示QImage格式图像
    void DispQImage(QImage QImg);
    //显示Mat格式图像
    void DispMat(Mat cvImg);
    //显示HObject格式图像
    void DispHObject(HObject hvImg);

    //QImage格式转Mat格式
    Mat QImage2Mat(QImage image);
    //Mat格式转QImage格式
    QImage cvMat2QImage(const cv::Mat &mat);
    //QImage格式转HObject格式
    HObject QImage2HObject(QImage qimage);
    //HObject格式转QImage格式
    void HObjectToQImage(HObject hv_image, QImage &qimage);
    void rgb3_to_interleaved (HObject ho_ImageRGB, HObject *ho_ImageInterleaved);
    //Mat格式转HObject格式
    HObject Mat2Hobject(Mat& image);
    //HObject格式转Mat格式
    Mat HObject2Mat(HObject Hobj);
    //HObject格式转QImage格式
    QImage HObject2QImage(const HalconCpp::HObject &Hobj);
};

#endif // WIDGET_H
