#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>

#include "cameractrl.h"
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QThread *liveThread;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    HTuple hv_WindowHandle;
    Hlong winID;

private slots:
    void on_btnAutoFocus_clicked();
    void AutoFocus(HObject);

    void on_btnMaxDev_clicked();
    void focusTimeout();
    void on_btnTimer_clicked();

    void on_lineEdit_textChanged(const QString &arg1);

    void on_editInter_returnPressed();
    void handleHObject(HObject);

private:
    Ui::MainWindow *ui;
    HTuple hv_PreDeviation;
    CameraCtrl *camera;

    QTimer *t_focus;

    HTuple  hv_I;


signals:

    void ContinuousGrab();

};
#endif // MAINWINDOW_H
