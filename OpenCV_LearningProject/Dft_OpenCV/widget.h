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

    void on_btn_DftCV_clicked();

    void on_btn_resetPic_clicked();

private:
    Ui::Widget *ui;
    // 灰度图像
    Mat grayImg;
    // Mat图像类型转换为QImage
    QImage cvMat2QImage(const cv::Mat &mat);
    // QImage转换Mat图像
    cv::Mat QImage2cvMat(QImage& image);

    void DftOwn(Mat input_Img, Mat& output_Img, Mat& transform_Img);
};
#endif // WIDGET_H
