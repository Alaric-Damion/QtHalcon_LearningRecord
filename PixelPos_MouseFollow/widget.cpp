#pragma execution_character_set("utf-8")
#include "widget.h"
#include "ui_widget.h"
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("坐标像素实时监控");
    img = imread("lena.png");
    cvtColor(img, img, COLOR_BGR2RGB);
    QImage disImage = QImage((const unsigned char*)(img.data), img.cols, img.rows, QImage::Format_RGB888);
    QPixmap pix = QPixmap::fromImage(disImage);

    pix.scaled(ui->lbl_pic->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    // label 显示图像
    ui->lbl_pic->setPixmap(pix);
    /*激活控件鼠标跟随属性，激活后在点击鼠标后进入mouseMoveEvent函数*/
    /*如果不点击鼠标时想要在控件上触发mouseMoveEvent函数，就需要同时激活控件和窗口*/
    ui->lbl_pic->setMouseTracking(true);
    setMouseTracking(true);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    QPoint pt = event->pos();
    QRect rect = ui->lbl_pic->geometry();
    if(rect.contains(pt)){
        QPoint PicPoint = QPoint(pt.x()-rect.x(), pt.y()- rect.y());
        QString str = QString("(x:%1,y:%2)").arg(PicPoint.x()).arg(PicPoint.y());
        ui->lbl_pos->setText(str);
        if(img.channels() == 1){            //单通道图像
            int grayValue;
            switch (img.type())
            {
            case 0:
                grayValue = static_cast<int>(img.at<uchar>(Point(PicPoint.x(), PicPoint.y())));
                break;
            case 1:
                grayValue = static_cast<int>(img.at<char>(Point(PicPoint.x(), PicPoint.y())));
                break;
            case 2:
                grayValue = static_cast<int>(img.at<ushort>(Point(PicPoint.x(), PicPoint.y())));
                break;
            case 3:
                grayValue = static_cast<int>(img.at<short>(Point(PicPoint.x(), PicPoint.y())));
                break;
            case 4:
                grayValue = static_cast<int>(img.at<int>(Point(PicPoint.x(), PicPoint.y())));
                break;
            case 5:
                grayValue = static_cast<int>(img.at<float>(Point(PicPoint.x(), PicPoint.y())));
                break;
            case 6:
                grayValue = static_cast<int>(img.at<double>(Point(PicPoint.x(), PicPoint.y())));
                break;
            }
            QString str = QString("Gray Value：%1").arg(grayValue);
            ui->lbl_pix->setText(str);
        }
        else                                //多通道图像
        {
            int value_B = static_cast<int>(img.at<Vec3b>(Point(PicPoint.x(), PicPoint.y()))[0]);
            int value_G = static_cast<int>(img.at<Vec3b>(Point(PicPoint.x(), PicPoint.y()))[1]);
            int value_R = static_cast<int>(img.at<Vec3b>(Point(PicPoint.x(), PicPoint.y()))[2]);
            QString str = QString("B：%1, G：%2, R：%3").arg(value_B).arg(value_G).arg(value_R);
            ui->lbl_pix->setText(str);
        }
    }
}
