#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "FramelessHelper.h"
#include "Gif.h"
#include <QMainWindow>
#include <QScreen>
#include <QTimer>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btnRecord_clicked();
    void on_btnStop_clicked();
    void on_btnClose_clicked();

    void slotSaveImage();       //保存一帧的图片

protected:
	void timerEvent(QTimerEvent *event);
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *event);

private:
    void UpdateSizeText();

private:
    Ui::MainWindow *ui;
    FramelessHelper* mFrameHelper;  //实现标题拖动窗口、边框缩放窗口的功能类

	int mTimerID;
    QTimer mTimer;
    QString mFile;

    //GIF操作类
    Gif_H mGif;
    Gif_H::GifWriter *mGifWriter;
    uint mFps;

};

#endif
