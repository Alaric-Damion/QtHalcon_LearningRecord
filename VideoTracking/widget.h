#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "opencv2/opencv.hpp"
#include <QTimer>

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
    void on_btnOpenVedio_clicked();
    void on_btnQuit_clicked();
    void readFrame();

    void on_ckb_Track_clicked(bool checked);

private:
    Ui::Widget *ui;

    bool openCam;
    bool isTrack=false;
    bool isSaveFrame = false;
    QTimer *timer;
    VideoCapture *cap;
    Mat framePre;
    int fps,frameWidth,frameHeight;
    VideoWriter writer;

    int VideoNum = 0;


    //Mat转换QImage
    QImage cvMat2QImage(const cv::Mat& mat);
};

#endif // WIDGET_H
