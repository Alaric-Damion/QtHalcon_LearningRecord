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
    void releaseCap();
    int getVideoAllFramecount();//获取视频总帧数
    void setCurrentFrame(int value);//设置当前进度条

    bool getStop() const;
    void setStop(bool value);//设置视频结束标识
    bool getIsrun() const;
    void setIsrun(bool value);

    void pauseThread();//暂停
    void resumeThread();//继续
    void stopThread();//停止

signals:
    void sendFrame(int currentFrame,Mat frame);//当前帧和 帧数
private:
    VideoCapture cap;//视频对象
    Mat frame;
    int currentFramecount;//视频当前帧数
    int allFramecount;//总帧数
    int fps;//视频帧率
    int videoWriterFrame;//录制视频帧
    bool stop;//线程结束标识位
    bool isrun;//视频暂停标识位

};

#endif // VIDEOTHREAD_H
