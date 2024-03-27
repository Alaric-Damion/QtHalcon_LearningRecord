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

    void on_btn_MedianOpenCV_clicked();

    void on_btn_MedianBasic_clicked();

    void on_btn_resetPic_clicked();

    void on_btn_saltPic_clicked();

private:
    Ui::Widget *ui;
    // 灰度图像
    Mat grayImg;
    // 椒盐噪声图
    Mat saltImg;
    // opencv中值图
    Mat medianImg_OpenCV;
    // 自计算中值图
    Mat medianImg_Own;
    // Mat图像类型转换为QImage
    QImage cvMat2QImage(const cv::Mat &mat);
    // 自计算中值滤波
    void MedianFilterOwn(Mat img_input, Mat &img_output, int size);
    // 获取中值
    int sortCompute(Mat& sortarray, int filterSize);
};
#endif // WIDGET_H
