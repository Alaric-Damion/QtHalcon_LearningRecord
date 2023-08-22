#pragma execution_character_set("utf-8")
#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("VLC拉流测试");
    ui->btn_snap->setEnabled(false);
    inst = new VlcInstance(VlcCommon::args(), this);
    vlcPlayer = new VlcMediaPlayer(inst);
    vlcPlayer->setVideoWidget(ui->widget_video);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_btn_startPlay_clicked()
{
    QString rtspUrl = ui->edt_rtspUrl->text();
    if(!rtspUrl.isEmpty())
    {
        vlcMedia = new VlcMedia(rtspUrl, inst);
        vlcPlayer->open(vlcMedia);
        ui->btn_snap->setEnabled(true);
    }
}

void Widget::on_btn_pause_clicked(bool checked)
{
    if(checked)
    {
        ui->btn_pause->setText("继续");
    }
    else
    {
        ui->btn_pause->setText("暂停");
    }
    vlcPlayer->togglePause();
}


void Widget::on_btn_stop_clicked()
{
    vlcPlayer->stop();
    ui->btn_snap->setEnabled(false);
}


void Widget::on_btn_snap_clicked()
{
    emit vlcPlayer->snapshotTaken("E:/123.jpg");
}

