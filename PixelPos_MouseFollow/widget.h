#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include "opencv2/opencv.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

using namespace cv;
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

protected:
    void mouseMoveEvent(QMouseEvent *event);
private:
    Ui::Widget *ui;
    Mat img;
};
#endif // WIDGET_H
