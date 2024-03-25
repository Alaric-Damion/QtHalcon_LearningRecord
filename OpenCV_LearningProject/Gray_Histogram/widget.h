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
    void on_btn_loadImg_clicked();

    void on_btn_histogram_clicked();

    void on_btn_equilibriumImg_clicked();

    void on_btn_matchImg_clicked();

    void on_btn_resetImg_clicked();

private:
    Ui::Widget *ui;

    Mat grayImg1,grayImg2;
    // Mat图像类型转换为QImage
    QImage cvMat2QImage(const cv::Mat &mat);
    // 获取直方图
    Mat getHistogramImg(Mat img);

};
#endif // WIDGET_H
