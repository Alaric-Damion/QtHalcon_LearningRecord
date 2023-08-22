#include "newqslider.h"

newqslider::newqslider(QWidget *parent) : QSlider(parent)
{
}
/*****************************************************************
* 函数名称：mousePressEvent(QMouseEvent *ev)
* 功能描述：重写鼠标点击事件，实现进度条点击哪跳到哪
* 参数说明： 无
* 返回值：   无
******************************************************************/
void newqslider::mousePressEvent(QMouseEvent *ev)
{
    //先调用父类的鼠标点击处理事件，这样可以不影响拖动的情况
    QSlider::mousePressEvent(ev);
    //获取鼠标的位置，这里并不能直接从ev中取值（因为如果是拖动的话，鼠标开始点击的位置没有意义了）
    double pos = ev->pos().x() / (double)width();
    setValue(pos * (maximum() - minimum()) + minimum());
    //发送自定义的鼠标单击信号
    emit costomSliderClicked();
}
