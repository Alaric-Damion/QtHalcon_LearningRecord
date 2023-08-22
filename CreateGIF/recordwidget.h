#ifndef RecordWidget_H
#define RecordWidget_H

#include <QtWidgets>
#include <Gif.h>
#include <QImage>
#include <QTimer>
#include "FramelessHelper.h"


namespace Ui {
class RecordWidget;
}

class RecordWidget : public QWidget{
    Q_OBJECT

public:
    explicit RecordWidget(QWidget *parent = nullptr);
    ~RecordWidget();

private slots:
    void on_btnRecord_clicked();
    void on_btnStop_clicked();
    void on_btnClose_clicked();

    void slotSaveImage();       //保存一帧的图片

protected:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *event);

private:
    void UpdateSizeText();


private:
    Ui::RecordWidget *ui;
    FramelessHelper* mFrameHelper;  //实现标题拖动窗口、边框缩放窗口的功能类

    QTimer mTimer;
    QScreen *mScreen;       //当前屏幕，用于截取当前像素
    QString mFile;
    QRect mRectRecord;

    //GIF操作类
    Gif_H mGif;
    Gif_H::GifWriter *mGifWriter;
    uint mFps;


};

#endif
