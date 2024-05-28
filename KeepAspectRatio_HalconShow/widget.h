#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "Halcon.h"
#include "HalconCpp.h"
#include <QMap>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

using namespace HalconCpp;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

protected:
    void resizeEvent(QResizeEvent *ev);

private slots:
    void on_btn_loadMultiPic_clicked();

    void on_btnPreviousPic_clicked();

    void on_btnNextPic_clicked();

    void on_btn_AdaptiveScreen_clicked();

    void on_btn_clearPic_clicked();

    void on_listWidget_doubleClicked(const QModelIndex &index);

private:
    //listwidget Item尺寸
    QSize ITEM_SIZE;
    //listwidget Img尺寸
    QSize IMG_SIZE;
    //文件夹当前加载图像下标
    int CurPicIndex=0;
    //当前图像变量
    HObject CurImg;
    //QLabel控件句柄
    HTuple m_hLabelID;
    //Halcon显示窗口句柄
    HTuple m_hHalconID = NULL;
    //批量图像路径
    QMap<int, QString> PicNameInfo;

private:
    Ui::Widget *ui;
    //初始化控件
    void init_Form();
    //显示图像
    void showImg();
    //检测文件夹图像是否第一/最后一张
    void checkCurPicIndex();
    //打开Halcon窗口
    void openWindowHandle();
    //加载Hobject图像
    void LoadHalconImg(QString imgPath);
    //QImage转HObjetc
    HObject QImage2HObject(QImage qimg);

};
#endif // WIDGET_H
