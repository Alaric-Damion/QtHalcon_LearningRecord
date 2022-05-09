#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "capturethread.h"
#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
    #include <QtGui>
#else
    #include <QLabel>
    #include <QGraphicsScene>
    #include <QGraphicsPixmapItem>
    #include <QTimer>
#endif
namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void closeEvent(QCloseEvent *);

private slots:
    void zoomIn();
    void zoomOut();
    void resetView();
    void updateFps();
    void initParam();
    void process(QImage img, unsigned char *buffer, qint64 timestamp);

    void on_actionExit_triggered();
    void on_btnZoomIn_clicked();
    void on_btnZoomOut_clicked();
    void on_btnResetView_clicked();



    void on_btnStart_clicked();
    void on_btnStop_clicked();

    void on_cmbDevice_currentIndexChanged(int index);
    void on_cmbResolution_currentIndexChanged(int index);

    void on_hsGamma_valueChanged(int value);
    void on_hsContrast_valueChanged(int value);
    void on_hsGain_valueChanged(int value);
    void on_hsExposure_valueChanged(int value);
    void on_btnROI_clicked();

    void on_cbGain_toggled(bool checked);

    void on_cbHFlip_clicked();

    void on_cbVFlip_clicked();

    void on_rbGroup0_clicked();

    void on_rbGroup1_clicked();

    void on_btnSaveParameter_clicked();



    void on_btnWB_clicked();

    void on_actionIO_Controller_triggered();

    void on_btnIOControl_clicked();

    void on_cbExposure_clicked(bool checked);

private:
    Ui::MainWindow *ui;

    QTimer *m_timer;
    int m_index;
    int m_totalFrame;
    int m_droppedFrame;
    int m_rotate;
    QLabel *m_LabelFps;

    QGraphicsScene *m_scene;
    QGraphicsPixmapItem *m_image_item;

    CaptureThread *m_thread;
    CaptureThread *m_thread2;
    CaptureThread *m_thread3;
};

#endif // MAINWINDOW_H
