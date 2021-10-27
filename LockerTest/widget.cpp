#pragma execution_character_set("utf-8")
#include "widget.h"
#include "ui_widget.h"
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    initUI();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::initUI()
{
    this->resize(300, 600);
    m_PicList = 0;
    m_VideoList = 0;

    ui->btnPic->SetTextLabel("图像");
    ui->btnPic->SetImageLabel(QPixmap(":/image/Collapse.png"));
    ui->btnPic->setStyleSheet("#btnPic{background-color:transparent}"
                              "#btnPic:hover{background-color:rgba(195,195,195,0.4)}"
                              "#btnPic:pressed{background-color:rgba(127,127,127,0.4)}");
    ui->btnVideo->SetTextLabel("视频");
    ui->btnVideo->SetImageLabel(QPixmap(":/image/Collapse.png"));
    ui->btnVideo->setStyleSheet("#btnVideo{background-color:transparent}"
                                "#btnVideo:hover{background-color:rgba(195,195,195,0.4)}"
                                "#btnVideo:pressed{background-color:rgba(127,127,127,0.4)}");
    QLabel* PicLabel = ui->btnPic->GetTextHandle();
    PicLabel->setStyleSheet("QLabel{color:rgba(183,71,42,1)}");
    PicLabel->setFont(QFont("图像", 10, QFont::Black));
    QLabel* VideoLabel = ui->btnVideo->GetTextHandle();
    VideoLabel->setStyleSheet("QLabel{color:rgba(183,71,42,1)}");
    VideoLabel->setFont(QFont("视频", 10, QFont::Black));
    ui->widget_Pic->setVisible(false);
    ui->widget_Video->setVisible(false);
    ui->btnPic->setEnabled(false);
    ui->btnVideo->setEnabled(false);

    connect(ui->btnPic, &LockerButton::clicked, [this](bool) {
        if (m_PicList % 2)
        {
            ui->btnPic->SetImageLabel(QPixmap(":/image/Collapse.png"));
            //m_sizeList偶数屏蔽Size列表界面，奇数显示Size列表界面
            ui->widget_Pic->setVisible(false);
        }
        else
        {
            ui->btnPic->SetImageLabel(QPixmap(":/image/Expand.png"));
            ui->widget_Pic->setVisible(true);
        }
        m_PicList++; });

    connect(ui->btnVideo, &LockerButton::clicked, [this](bool) {
        if (m_VideoList % 2)
        {
            ui->btnVideo->SetImageLabel(QPixmap(":/image/Collapse.png"));
            ui->widget_Video->setVisible(false);
        }
        else
        {
            ui->btnVideo->SetImageLabel(QPixmap(":/image/Expand.png"));
            ui->widget_Video->setVisible(true);
        }
        m_VideoList++; });
}

void Widget::on_ckbPic_clicked(bool checked)
{
    if(checked)
    {
        qDebug()<<"复选框被选中";
        ui->btnPic->setEnabled(true);
        m_PicList++;
        ui->widget_Pic->setVisible(true);
        ui->btnPic->SetImageLabel(QPixmap(":/image/Expand.png"));
    }
    else
    {
        qDebug()<<"复选框被取消";
        ui->btnPic->setEnabled(false);
        m_PicList++;
                ui->widget_Pic->setVisible(false);
                ui->btnPic->SetImageLabel(QPixmap(":/image/Collapse.png"));
    }
}

void Widget::on_ckbVideo_clicked(bool checked)
{
    if(checked)
    {
        qDebug()<<"复选框被选中";
        ui->btnVideo->setEnabled(true);
        m_VideoList++;
        ui->widget_Video->setVisible(true);
        ui->btnVideo->SetImageLabel(QPixmap(":/image/Expand.png"));
    }
    else
    {
        qDebug()<<"复选框被取消";
        ui->btnVideo->setEnabled(false);
        m_VideoList++;
        ui->widget_Video->setVisible(false);
        ui->btnVideo->SetImageLabel(QPixmap(":/image/Collapse.png"));
    }
}
