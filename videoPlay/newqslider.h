#ifndef NEWQSLIDER_H
#define NEWQSLIDER_H

#include <QWidget>
#include <QSlider>
#include <QMouseEvent>
class newqslider : public QSlider
{
    Q_OBJECT
public:
    explicit newqslider(QWidget *parent = 0);
    void mousePressEvent(QMouseEvent *ev);
signals:
    void costomSliderClicked();//自定义的鼠标单击信号，用于捕获并处理
public slots:
};

#endif // NEWQSLIDER_H
