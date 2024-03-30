#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "opencv2/opencv.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

using namespace cv;
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_btn_loadPic_clicked();

    void on_btn_saltPic_clicked();

    void on_spb_saltPram_valueChanged(int arg1);

    void on_btn_GaussFilterCV_clicked();

    void on_btn_GaussFilterOwn_clicked();

    void on_spb_GaussPram_valueChanged(int arg1);

    void on_btn_resetPic_clicked();

private:
    Ui::Widget *ui;
    // 灰度图像
    Mat grayImg;
    // 椒盐噪声图
    Mat saltImg;
    // opencv高斯滤波图像
    Mat gaussImg_CV;
    // 自计算高斯滤波图像
    Mat gaussImg_Own;
    // Mat图像类型转换为QImage
    QImage cvMat2QImage(const cv::Mat &mat);
    // 自计算高斯滤波
    void GaussFilterOwn(Mat img_input, Mat &img_output, int size);
};
#endif // WIDGET_H
