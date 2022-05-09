#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <QWidget>
#include<QDialog>
namespace Ui {
class Histogram;
}

class Histogram : public QDialog
{
    Q_OBJECT
    
public:
    explicit Histogram(QWidget *parent = 0);
    ~Histogram();
    void setImage(QImage image);
    void mousePressEvent(QMouseEvent  *event);
    void mouseMoveEvent(QMouseEvent  *event);
    void Show();
    void switchLanguage();

private:
    Ui::Histogram *ui;
    QImage img;

};

#endif // HISTOGRAM_H
