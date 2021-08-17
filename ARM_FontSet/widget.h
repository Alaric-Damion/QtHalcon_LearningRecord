#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "Halcon.h"
#include "HalconCpp.h"
//#include <QThread>

using namespace HalconCpp;
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
    void on_btnShowText_clicked();

    void on_btnModifyFont_clicked();

protected:
    void resizeEvent (QResizeEvent*);
private:
    Ui::Widget *ui;
    void initForm();
    HTuple hv_WindowHandleCurrent;
    void dispOnWindow(const HObject &img,HTuple hv_WindowHandle);
};

#endif // WIDGET_H
