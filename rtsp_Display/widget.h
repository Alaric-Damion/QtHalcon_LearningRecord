#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "VLCQtCore/Common.h"
#include "VLCQtCore/Instance.h"
#include "VLCQtCore/Media.h"
#include "VLCQtCore/MediaPlayer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_btn_startPlay_clicked();


    void on_btn_pause_clicked(bool checked);

    void on_btn_stop_clicked();

    void on_btn_snap_clicked();

private:
    Ui::Widget *ui;

    VlcInstance * inst;
    VlcMediaPlayer *vlcPlayer;
    VlcMedia *vlcMedia;
};
#endif // WIDGET_H
