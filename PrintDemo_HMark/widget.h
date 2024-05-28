#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <Windows.h>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    HINSTANCE m_hInstancePrint;

private:
    Ui::Widget *ui;

    void* hprinter;

    void init_printer();
};
#endif // WIDGET_H
