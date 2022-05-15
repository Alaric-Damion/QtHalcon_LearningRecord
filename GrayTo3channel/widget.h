#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSettings>
#include <QFileInfo>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include "opencv2/opencv.hpp"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:

    void on_btn_trans_clicked();

    void on_btn_close_clicked();

    void on_btn_OpenFile_clicked();

    void on_btn_NextPic_clicked();

    void on_btn_LoadPic_clicked();

    void on_btn_SaveImgPath_clicked();

private:
    Ui::Widget *ui;

    QImage curImg;
    //文件夹中图像数量
    int PicNum;
    //文件夹当前加载图像下标
    int CurPicIndex=0;
    //批量加载图像文件夹的路径
    QString file_path;
    //当前图像名称
    QString CurfileName;
    //图像名称缓存
    QMap<int,QString> PicNameInfo;
    //保存文件夹文件数量
    int saveFile_Num;
    //Mat转换QImage
    QImage cvMat2QImage(const cv::Mat& mat);
    //QImage转换Mat
    cv::Mat QImage2Mat(QImage image);
    //图像转换到三通道
    cv::Mat convertTo3Channels(const cv::Mat& binImg);
    void checkCurPicIndex();
};

#endif // WIDGET_H
