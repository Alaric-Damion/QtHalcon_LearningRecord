#pragma once

#include <QLabel>
#include <QWheelEvent>
#include "HalconCpp.h"
#include "Halcon.h"


using namespace HalconCpp;

class CMyLabel : public QLabel
{
    Q_OBJECT

public:
    CMyLabel(QWidget *parent = Q_NULLPTR);
    ~CMyLabel();

    //设置Halcon图像和Halcon窗口句柄，用户响应鼠标事件后实时更新图像
    void setHalconWnd(HObject img, HTuple hHalconID, QLabel *label);

    //鼠标滚轮缩放事件
    void wheelEvent(QWheelEvent *ev);

    //鼠标按下事件
    void mousePressEvent(QMouseEvent *ev);

    //鼠标释放事件
    void mouseReleaseEvent(QMouseEvent *ev);

    //鼠标移动事件
    void mouseMoveEvent(QMouseEvent *ev);

public:

    //Halcon窗口句柄
    HTuple m_hHalconID;

    //当前的图像
    HObject m_currentImg;

    //主界面显示坐标的标签
    QLabel *m_label;

    //鼠标按下的位置
    HTuple m_tMouseDownRow;
    HTuple m_tMouseDownCol;

    //是否移动图像
    bool m_bIsMove;
};
