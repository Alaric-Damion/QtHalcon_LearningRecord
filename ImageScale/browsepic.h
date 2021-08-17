#pragma once

#include <QtWidgets/QWidget>
#include "ui_BrowsePic.h"
#include <QToolBar>
#include "HalconCpp.h"
#include "Halcon.h"


using namespace HalconCpp;
#pragma execution_character_set("utf-8");

class BrowsePic : public QWidget
{
    Q_OBJECT

public:
    BrowsePic(QWidget *parent = Q_NULLPTR);
    ~BrowsePic();

    //初始化
    void init();

    //打开窗口
    void showImg();

    //窗体大小改变事件
    void resizeEvent(QResizeEvent *ev);
public:
    //显示图像的控件id
    HTuple m_hLabelID;			//QLabel控件句柄
    HTuple m_hHalconID;			//Halcon显示窗口句柄

    //原始图像的尺寸
    HTuple m_imgWidth, m_imgHeight;

    //图片路径列表
    HTuple m_imgFiles;

    //当前图像
    HObject m_hCurrentImg;

    //当前图像的下标
    HTuple index;

    //是否等比缩放
    bool m_bIsEqualScale;

    //是否居中显示
    bool m_bIsShowCenter;

    //缩放后的图像
    HObject m_hResizedImg;

    //缩放系数
    HTuple m_hvScaledRate;

    //缩放后图像的大小
    HTuple m_scaledHeight, m_scaledWidth;

    QToolBar *m_toolBar;
public slots:

    //打开图片
    void on_btn_openPic_clicked();

    //浏览前一张
    void on_btn_prePic_clicked();

    //浏览后一张
    void on_btn_nextPic_clicked();

    //恢复图片
    void on_btn_resetPic_clicked();

    //等比缩放复选框状态改变
    void on_checkBox_equalScale_stateChanged(int);

    //居中显示复选框状态改变
    void on_checkBox_showCenter_stateChanged(int);
private:
    Ui::BrowsePic ui;
};
