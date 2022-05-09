#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Windows.h>
#include "settingdialog.h"
#include "aboutdialog.h"
#include "histogram.h"

#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
    #include <QtGui>
#else
    #include <QLabel>
    #include <QGraphicsScene>
    #include <QGraphicsPixmapItem>
    #include <QTimer>
    #include <QComboBox>
#endif

#define CAMN 2

class CaptureThread;

namespace Ui {
    class MainWindow;
}

struct CameraView {
    CaptureThread *m_thread;
    QGraphicsScene *m_scene;
    QGraphicsPixmapItem *m_image_item;
    int m_resoIndex;
    int m_totalFrame;
    int m_droppedFrame;
    float m_fps;
    int m_camera_id;
};

enum {LANG_EN=0x1, LANG_CN};  //lang
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void closeEvent(QCloseEvent *);

    bool nativeEvent(const QByteArray &eventType, void *message, long *result);
    bool winEvent(MSG *message, long *result);

private:
    Ui::MainWindow *ui;
    QMenu *fileMenu;
    QMenu *viewMenu;
    QLabel *msgLabel;
    QLabel *fpsLabel;
    QComboBox *m_resoCombo[CAMN];
    QComboBox *m_deviceCombo[CAMN];

    QTimer *m_timer;
    SettingDialog *m_setdlg;
    AboutDialog *m_aboutdlg;
    Histogram  *m_histogram;

    CameraView m_view[CAMN];
    int m_active_view;
    int m_language;

private slots:
    void process(int id, QImage img, unsigned char *buffer, qint64 timestamp);
    void openSetting();
    void saveImage();
    void pauseVideo();
    void streamVideo();
    void updateFps();
    void aboutMe();
    void zoomIn();
    void zoomOut();
    void resetView();
    void resetHardware();
    void switchlanguage();
    void resolutionChange(int index, int viewid);
    void deviceChange(int index, int viewid);
    void resolutionChange1(int index);
    void deviceChange1(int index);
    void resolutionChange2(int index);
    void deviceChange2(int index);
    void showHistogram();
    void on_actionWindow_Switch_triggered();
};

#endif // MAINWINDOW_H
