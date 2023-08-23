#ifndef VIDEOTHREAD_H
#define VIDEOTHREAD_H

#include <QObject>
#include <QThread>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <QDebug>
#include <QDateTime>
using namespace  std;
using namespace  cv;
class videothread : public QThread
{
    Q_OBJECT
public:
    videothread(const char* filename);
    void run();
    //释放视频采集对象
    void releaseCap();
    //获取视频总帧数
    int getVideoAllFramecount();
    //设置当前进度条
    void setCurrentFrame(int value);

    bool getStop() const;
    //设置视频结束标识
    void setStop(bool value);
    bool getIsrun() const;
    void setIsrun(bool value);
    //暂停
    void pauseThread();
    //继续
    void resumeThread();
    //停止
    void stopThread();

signals:
    //发送当前帧和 帧数
    void sendFrame(int currentFrame,Mat frame);
private:
    //视频对象
    VideoCapture cap;
    Mat frame;
    //视频当前帧数
    int currentFramecount;
    //总帧数
    int allFramecount;
    //视频帧率
    int fps;
    //录制视频帧
    int videoWriterFrame;
    //线程结束标识位
    bool stop;
    //视频暂停标识位
    bool isrun;

};

#endif // VIDEOTHREAD_H
