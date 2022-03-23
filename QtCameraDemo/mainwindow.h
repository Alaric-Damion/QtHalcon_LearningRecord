#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCamera>
#include <QCameraImageCapture>
#include <QTimer>
#include <QCameraInfo>
#include <QLabel>
#include <QDateTime>

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
    //    void on_btnCameraCtrl_clicked();

    void on_btnSnap_clicked();

    void on_btnVedio_clicked();

    void processCapturedImage(int request_id, const QImage &img);
    void captureImage();

    void on_cmb_CameraName_activated(int index);

    void on_cmb_Resolution_activated(int index);

    void on_cmb_MaxFrameRate_activated(int index);

private:
    Ui::MainWindow *ui;
    QList<QCameraInfo> cameras;
    QCamera* m_camera;//摄像头设备
    QCameraImageCapture* image_capture;
    QTimer *timer_image_capture;//图像抓图

    QList<QSize> mResSize = {};//分辨率List
    QList<qreal> mMaxFrame = {};//最大帧率
    QTimer *myTimer;//状态栏时间显示
    //相机状态标志量参数
    QLabel * m_status1;
    QLabel * m_status2;
    QLabel * m_status3;

    void initCamera();
    void initForm();
    void SetFrameResolution();
    void UpdateCameraStatus();
};

#endif // MAINWINDOW_H
