#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSlider>
#include "videothread.h"
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

public slots:
    void receiveFrame(int currentFrame,Mat frame);
private slots:

    void sliderClickedSlot();//点击进度条信号槽

    void on_horizontalSlider_sliderReleased();

    void on_horizontalSlider_sliderMoved(int position);

    void on_btn_startPlay_clicked();

    void on_btn_backward_clicked();

    void on_btn_forward_clicked();

    void on_btn_stopPlay_clicked();

    void on_btn_replay_clicked();

    void on_btn_openFile_clicked();

private:
    Ui::Widget *ui;
    videothread *pthread = nullptr;
    //接收到视频帧数
    int receiveCurrentFramecount;
    //当前显示图片
    QImage img1;
    bool isend;
    //加载视频文件路径
    QString videoFilePath = "";
};

#endif // WIDGET_H
